#pragma once
#include <vector>
#include <chrono>
#include <functional>
#include <boost/asio.hpp>
#include "packet.h"

class Packet {
public:
    std::chrono::system_clock::time_point UtcArrival;
    boost::asio::ip::udp::endpoint RemoteEndPoint;
    std::vector<uint8_t> Data;

    Packet() = default;
    Packet(const boost::asio::ip::udp::endpoint& ep, const std::vector<uint8_t>& data)
        : UtcArrival(std::chrono::system_clock::now()), RemoteEndPoint(ep), Data(data) {
    }

    using TrafficEventHandler = std::function<void(void* sender, const Packet& packet)>;
};
