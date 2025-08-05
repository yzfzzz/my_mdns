#pragma once
#include <chrono>
#include <vector>
#include <WinSock2.h>
#include <functional>

using TrafficEventHandler = std::function<void(void* sender, Packet* packet)>;
struct Packet
{ 
	std::chrono::system_clock::time_point utc_arrival_;
	std::vector<char> data_;
	sockaddr_in remote_endpoint_;
};
