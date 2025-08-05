#include "traffic_monitor.h"
#include <iostream>
#include <thread>

int main() {
	boost::asio::ip::address addr = TrafficMonitor::GetServerIPAddress();
	if (addr.is_v4() && !addr.is_loopback()) {
		printf("get server ip address: %s\r\n", addr.to_string());
	}
	TrafficMonitor tm;
	while (true) {
		tm.broadcast("I am mdns test");
		std::this_thread::sleep_for(std::chrono::seconds(2));
	}
	return 0;
}