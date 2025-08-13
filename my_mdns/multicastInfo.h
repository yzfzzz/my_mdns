#pragma once

// MulticastInfo.hpp
#pragma once
#include <string>
#include <chrono>

class MulticastInfo {
public:
    std::string service_name_;
    std::string service_type_;
    std::string host_;
    std::string dev_id_;
    std::string dev_name_;
    std::string dev_type_;
    std::string dev_mac_;
    std::string port_;
    std::string version_;
    std::chrono::system_clock::time_point ttl_;

    MulticastInfo() = default;
};
