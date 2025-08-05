#pragma once
#include <boost/asio.hpp>
#include <iostream>
#include <array>
#include <winsock2.h>
#include <iphlpapi.h>
#include <ws2tcpip.h>
#include <thread>
#include <queue>
#include "async_udp_client.h"

#pragma comment(lib, "iphlpapi.lib")
#pragma comment(lib, "ws2_32.lib")

struct Packet {
    std::chrono::system_clock::time_point UtcArrival;
    // ??boost::asio::ip::udp::endpoint??????
    boost::asio::ip::udp::endpoint RemoteEndPoint;
    // ??std::vector<unsigned char>??????
    std::vector<unsigned char> Data;
};

class TrafficMonitor {
public:
    static bool IsWireless(const std::string& adapterName);
    static boost::asio::ip::address GetServerIPAddress();
    void makeUdpClient() {
        if (p_async_udp_client == nullptr) {
            p_async_udp_client = 
                std::make_shared<AsyncUdpClient>(io_context, "224.0.0.251", 5353);
        }
    }
    void start();
    void broadcast(std::string datagram);
    boost::asio::io_context io_context;
    std::shared_ptr<AsyncUdpClient> p_async_udp_client;
    std::queue<Packet> inQueue;
};