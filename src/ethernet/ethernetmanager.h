#pragma once

#include <Arduino.h>
#include <SPI.h>
#include <Ethernet.h>

/**
 * @brief Manages the W5500 Ethernet connection.
 *
 * Handles hardware initialization, link detection, DHCP,
 * gateway discovery and gateway MAC address resolution.
 */
class EthernetManager {
public:
    enum class State : uint8_t {
        UNAVAILABLE,
        DISCONNECTED,
        DHCP,
        NO_DHCP,
        RESOLVING,
        READY,
        NO_GATEWAY
    };

    bool begin();
    void update();

    bool isAvailable() const;
    bool isLinkUp() const;
    bool isConnected() const;

    bool hasNewGateway();
    void clearGatewayEvent();

    State getState() const;

    IPAddress getLocalIP() const;
    IPAddress getGatewayIP() const;
    IPAddress getSubnetMask() const;
    IPAddress getDnsServer() const;

    String getGatewayMAC() const;

private:
    static constexpr uint8_t PIN_MOSI = 11;
    static constexpr uint8_t PIN_SCK = 12;
    static constexpr uint8_t PIN_MISO = 13;
    static constexpr uint8_t PIN_CS = 14;

    static constexpr unsigned long LINK_INTERVAL = 500;
    static constexpr unsigned long DHCP_RETRY_INTERVAL = 10000;
    static constexpr unsigned long DHCP_TIMEOUT = 5000;
    static constexpr unsigned long DHCP_RESPONSE_TIMEOUT = 2000;

    uint8_t mac[6] = {};

    State state = State::UNAVAILABLE;

    bool available = false;
    bool linkUp = false;
    bool connected = false;
    bool gatewayEvent = false;

    unsigned long lastLinkCheck = 0;
    unsigned long lastDhcpAttempt = 0;

    String gatewayMAC;

    void generateMAC();
    void handleLinkUp();
    void handleLinkDown();

    bool requestDHCP();
    bool resolveGatewayMAC();

    void printNetworkInfo() const;
    String formatMAC(const uint8_t* address) const;
};