#include "traffic_monitor.h"

bool TrafficMonitor::isWireless(const std::string& adapterName) {
    // 获取网络适配器信息
    ULONG buffer_size = 0;
    DWORD result = GetAdaptersAddresses(AF_UNSPEC, GAA_FLAG_INCLUDE_PREFIX, NULL, NULL, &buffer_size);

    if (result != ERROR_BUFFER_OVERFLOW) {
        return false;
    }

    std::vector<BYTE> buffer(buffer_size);
    PIP_ADAPTER_ADDRESSES adapter_addresses = reinterpret_cast<PIP_ADAPTER_ADDRESSES>(buffer.data());

    result = GetAdaptersAddresses(AF_UNSPEC, GAA_FLAG_INCLUDE_PREFIX, NULL, adapter_addresses, &buffer_size);

    if (result != NO_ERROR) {
        return false;
    }

    // 遍历所有网络适配器
    for (PIP_ADAPTER_ADDRESSES adapter = adapter_addresses; adapter != NULL; adapter = adapter->Next) {
        // 检查适配器名称是否匹配
        if (adapterName == std::string(adapter->AdapterName)) {
            // 检查适配器类型是否为无线802.11
            return (adapter->IfType == IF_TYPE_IEEE80211);
        }
    }

    return false;
}

boost::asio::ip::address TrafficMonitor::getServerIPAddress() {
    static boost::asio::ip::address sticky_ethernet_ip_address;
    static boost::asio::ip::address sticky_wifi_ip_address;

    unsigned long buffer_size = 0;
    unsigned long  result = GetAdaptersAddresses(AF_INET, GAA_FLAG_INCLUDE_PREFIX, NULL, NULL, &buffer_size);
    std::vector<unsigned char> buffer(buffer_size);
    // reinterpret_cast 强转
    PIP_ADAPTER_ADDRESSES adapter_addresses = reinterpret_cast<PIP_ADAPTER_ADDRESSES>(buffer.data());

    result = GetAdaptersAddresses(AF_INET, GAA_FLAG_INCLUDE_PREFIX, NULL, adapter_addresses, &buffer_size);

    if (result != NO_ERROR) {
        return boost::asio::ip::address();
    }

    std::vector<boost::asio::ip::address> ethernetList;
    std::vector<boost::asio::ip::address> wifiList;

    // 遍历所有网络适配器
    for (PIP_ADAPTER_ADDRESSES adapter = adapter_addresses; adapter != NULL; adapter = adapter->Next) {
        // 检查适配器是否处于活动状态
        if (adapter->OperStatus != IfOperStatusUp) {
            continue;
        }

        // 跳过回环接口
        if (adapter->IfType == IF_TYPE_SOFTWARE_LOOPBACK) {
            continue;
        }

        // 遍历适配器的单播地址, 一个联网设备可能有很多个ip地址
        for (PIP_ADAPTER_UNICAST_ADDRESS unicast = adapter->FirstUnicastAddress; unicast != NULL; unicast = unicast->Next) {
            char ip_str[INET_ADDRSTRLEN];

            // 将sockaddr转换为可读的IP地址字符串
            if (getnameinfo(unicast->Address.lpSockaddr,
                unicast->Address.iSockaddrLength,
                ip_str, sizeof(ip_str),
                NULL, 0, NI_NUMERICHOST) == 0) {

                boost::asio::ip::address ip = boost::asio::ip::make_address(ip_str);

                // 分类处理IP地址
                if (isWireless(adapter->AdapterName)) {
                    wifiList.push_back(ip);
                }
                else {
                    ethernetList.push_back(ip);
                }
            }
        }
    }

    // 优先选择有线网络
    if (!ethernetList.empty()) {
        // 实现粘性机制
        
        if (!sticky_ethernet_ip_address.is_unspecified()) {
            auto it = std::find(ethernetList.begin(), ethernetList.end(), sticky_ethernet_ip_address);
            if (it != ethernetList.end()) {
                return *it;
            }
        }

        sticky_ethernet_ip_address = ethernetList[0];
        return sticky_ethernet_ip_address;
    }

    // 备选无线网络
    if (!wifiList.empty()) {
        // 实现粘性机制
        if (!sticky_wifi_ip_address.is_unspecified()) {
            auto it = std::find(wifiList.begin(), wifiList.end(), sticky_wifi_ip_address);
            if (it != wifiList.end()) {
                return *it;
            }
        }

        sticky_wifi_ip_address = wifiList[0];
        return sticky_wifi_ip_address;
    }

    return boost::asio::ip::address();
}

void TrafficMonitor::start() {
    std::thread queue_worker_thread([]() {

        });
    queue_worker_thread.detach();
    p_async_udp_client->asyncReceive();
}

void TrafficMonitor::broadcast(std::string datagram) {
    makeUdpClient();
    p_async_udp_client->asyncSend(datagram);
}