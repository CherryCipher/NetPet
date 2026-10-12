#include "ethernetmanager.h"
#include <utility/w5100.h>

void EthernetManager::generateMAC() {
    uint64_t chipMac = ESP.getEfuseMac();

    mac[0] = 0x02;

    for (uint8_t i = 1; i < 6; i++) {
        mac[i] = static_cast<uint8_t>(chipMac >> ((5 - i) * 8));
    }
}

bool EthernetManager::begin() {
    generateMAC();

    pinMode(PIN_CS, OUTPUT);
    digitalWrite(PIN_CS, HIGH);

    SPI.begin(PIN_SCK, PIN_MISO, PIN_MOSI, PIN_CS);
    Ethernet.init(PIN_CS);

    // Initialize the controller without requesting DHCP.
    W5100.init();

    available = Ethernet.hardwareStatus() == EthernetW5500;

    if (!available) {
        state = State::UNAVAILABLE;
        Serial.println("[ETH] W5500 not found.");
        return false;
    }

    Serial.println("[ETH] W5500 detected.");

    Serial.print("[ETH] Device MAC: ");
    Serial.println(formatMAC(mac));

    linkUp = Ethernet.linkStatus() == LinkON;
    state = State::DISCONNECTED;

    if (linkUp) handleLinkUp();
    else Serial.println("[ETH] Waiting for cable.");

    return true;
}

void EthernetManager::update() {
    if (!available) return;

    const unsigned long now = millis();

    if (connected && linkUp) {
        const int result = Ethernet.maintain();

        if (result == 1 || result == 3) {
            Serial.println("[ETH] DHCP renewal failed.");

            connected = false;
            gatewayEvent = false;
            gatewayMAC = "";

            state = State::NO_DHCP;
            lastDhcpAttempt = now;
        }
    }

    if (now - lastLinkCheck < LINK_INTERVAL) return;
    lastLinkCheck = now;

    const bool currentLink = Ethernet.linkStatus() == LinkON;

    if (currentLink != linkUp) {
        linkUp = currentLink;

        if (linkUp) handleLinkUp();
        else handleLinkDown();

        return;
    }

    if (!linkUp || connected) return;

    if (now - lastDhcpAttempt >= DHCP_RETRY_INTERVAL) requestDHCP();
}

bool EthernetManager::isAvailable() const {
    return available;
}

bool EthernetManager::isLinkUp() const {
    return linkUp;
}

bool EthernetManager::isConnected() const {
    return connected;
}

bool EthernetManager::hasLinkUpEvent() {
    if (!linkUpEvent) return false;

    linkUpEvent = false;
    return true;
}

bool EthernetManager::hasNewGateway() {
    if (!gatewayEvent) return false;

    gatewayEvent = false;
    return true;
}

void EthernetManager::clearGatewayEvent() {
    gatewayEvent = false;
}

EthernetManager::State EthernetManager::getState() const {
    return state;
}

IPAddress EthernetManager::getLocalIP() const {
    return connected ? Ethernet.localIP() : IPAddress(0, 0, 0, 0);
}

IPAddress EthernetManager::getGatewayIP() const {
    return connected ? Ethernet.gatewayIP() : IPAddress(0, 0, 0, 0);
}

IPAddress EthernetManager::getSubnetMask() const {
    return connected ? Ethernet.subnetMask() : IPAddress(0, 0, 0, 0);
}

IPAddress EthernetManager::getDnsServer() const {
    return connected ? Ethernet.dnsServerIP() : IPAddress(0, 0, 0, 0);
}

String EthernetManager::getGatewayMAC() const {
    return gatewayMAC;
}

void EthernetManager::handleLinkUp() {
    Serial.println("[ETH] Cable connected.");

    connected = false;
    linkUpEvent = true;
    gatewayEvent = false;
    gatewayMAC = "";

    requestDHCP();
}

void EthernetManager::handleLinkDown() {
    Serial.println("[ETH] Cable disconnected.");

    connected = false;
    linkUpEvent = false;
    gatewayEvent = false;
    gatewayMAC = "";

    state = State::DISCONNECTED;
}

bool EthernetManager::requestDHCP() {
    lastDhcpAttempt = millis();
    state = State::DHCP;

    Serial.println("[ETH] Requesting DHCP...");

    if (Ethernet.begin(mac, DHCP_TIMEOUT, DHCP_RESPONSE_TIMEOUT) == 0) {
        state = State::NO_DHCP;
        connected = false;

        Serial.println("[ETH] DHCP failed.");
        return false;
    }

    connected = true;

    Serial.println("[ETH] DHCP successful.");
    printNetworkInfo();

    state = State::RESOLVING;

    if (!resolveGatewayMAC()) {
        state = State::NO_GATEWAY;

        Serial.println("[ETH] Gateway MAC resolution failed.");
        return true;
    }

    state = State::READY;
    gatewayEvent = true;

    Serial.print("[ETH] Gateway MAC: ");
    Serial.println(gatewayMAC);

    return true;
}

bool EthernetManager::resolveGatewayMAC() {
    const IPAddress gateway = Ethernet.gatewayIP();

    if (gateway == IPAddress(0, 0, 0, 0)) return false;

    /*
     * Sending a UDP packet to the gateway causes the W5500
     * to resolve the destination MAC address through ARP.
     */
    class GatewayUDP : public EthernetUDP {
    public:
        uint8_t socketNumber() const { return sockindex; }
    };

    GatewayUDP udp;

    if (!udp.begin(49153)) return false;

    const uint8_t socket = udp.socketNumber();

    if (socket >= MAX_SOCK_NUM) {
        udp.stop();
        return false;
    }

    if (!udp.beginPacket(gateway, 9)) {
        udp.stop();
        return false;
    }

    udp.write(static_cast<uint8_t>(0x00));

    const bool sent = udp.endPacket() == 1;

    if (!sent) {
        udp.stop();
        return false;
    }

    uint8_t resolvedMAC[6] = {};

    SPI.beginTransaction(SPI_ETHERNET_SETTINGS);
    W5100.readSnDHAR(socket, resolvedMAC);
    SPI.endTransaction();

    udp.stop();

    bool valid = false;
    bool broadcast = true;

    for (uint8_t i = 0; i < 6; i++) {
        if (resolvedMAC[i] != 0x00) valid = true;
        if (resolvedMAC[i] != 0xFF) broadcast = false;
    }

    if (!valid || broadcast || (resolvedMAC[0] & 0x01)) return false;

    gatewayMAC = formatMAC(resolvedMAC);

    return true;
}

String EthernetManager::formatMAC(const uint8_t* address) const {
    char buffer[18];

    snprintf(
        buffer,
        sizeof(buffer),
        "%02X:%02X:%02X:%02X:%02X:%02X",
        address[0],
        address[1],
        address[2],
        address[3],
        address[4],
        address[5]
    );

    return String(buffer);
}

void EthernetManager::printNetworkInfo() const {
    Serial.print("[ETH] IP: ");
    Serial.println(Ethernet.localIP());

    Serial.print("[ETH] Subnet: ");
    Serial.println(Ethernet.subnetMask());

    Serial.print("[ETH] Gateway: ");
    Serial.println(Ethernet.gatewayIP());

    Serial.print("[ETH] DNS: ");
    Serial.println(Ethernet.dnsServerIP());
}