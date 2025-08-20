#pragma once
#include <memory>
#include <thread>
#include <unordered_map>
#include <boost/asio/ip/tcp.hpp>
#include "traffic_monitor.h"
#include "service_browse_result.h"
#include "service.h"
#include "service_resolve_result.h"

class Browser {
public:
    Browser(std::shared_ptr<TrafficMonitor> monitor)
        : monitor_(std::move(monitor)) {
        if (!monitor_) {
            throw std::invalid_argument("TrafficMonitor cannot be null");
        }
		// TODO: 设置接收到数据包回调函数
        //expiration_thread_ = std::thread(&Browser::ExpirationTimerLoop, this);
    }
    // 禁用拷贝构造和赋值
    Browser(const Browser&) = delete;
    Browser& operator=(const Browser&) = delete;

    std::string addLocalSuffix(std::string& s) {
        const std::string suffix = ".local.";
        if (s.length() < suffix.length() || s.substr(s.length() - suffix.length()) != suffix) {
            return s + suffix;
        }
        return s;
    }

    /**
     * @brief 获取当前已发现的指定类型的服务列表。
     * @param serviceType 要浏览的服务类型。
     * @return 一个包含ServiceBrowseResult的向量，每个结果代表一个已发现的服务实例。
     */
    std::vector<ServiceBrowseResult> Browse(std::string& service_type) {
        std::unique_lock<std::mutex> lock(mutex_);
        std::vector<ServiceBrowseResult> results;
        const auto full_service_type = addLocalSuffix(service_type);
        auto it = service_root_.find(full_service_type);

        if (it != service_root_.end()) {
			std::unordered_map<std::string, std::shared_ptr<ServiceInfo>> temp_table = it->second->service_table;
            for (std::pair<std::string, std::shared_ptr<ServiceInfo>> t : temp_table) {
                std::string name = t.first;
                std::shared_ptr<ServiceInfo> info = t.second;
                if(info != nullptr && info->ptr != nullptr) {
                    ServiceBrowseResult sbr(name, full_service_type);
					results.push_back(sbr);
				}
            }
        }
        return results;
    }

    /**
     * @brief 解析一个已发现服务的详细信息。
     * @param browseResult 从Browse()方法获取的服务浏览结果。
     * @return 一个包含服务详细信息（主机名、IP地址、端口、TXT记录）的ServiceResolveResult指针，如果无法解析则返回nullptr。
     */
    std::unique_ptr<ServiceResolveResult> resolve(
        ServiceBrowseResult browse_result, int resolve_timeout_inseconds) {

        if (browse_result.name_.empty() || browse_result.service_type_.empty()) {
            return nullptr;
        }

        std::string name = browse_result.name_;
        std::string service_type = addLocalSuffix(browse_result.service_type_);

        // 加锁以保证线程安全
        std::unique_lock<std::mutex> lock(mutex_);

        // 查找服务类型
        auto p_service_collect = service_root_.find(service_type);
        if (p_service_collect == service_root_.end()) {
            return nullptr;
        }

        // 查找具体服务实例
        auto service_info = p_service_collect->second->service_table.find(name);
        if (service_info == p_service_collect->second->service_table.end()) {
            return nullptr;
        }

        auto info = service_info->second;
        // 必须同时有SRV（服务位置）和TXT（文本）记录才能成功解析
        if (!info || !info->srv || !info->txt) {
            return nullptr;
        }

		int port = info->srv->record->port_;
        std::string hostname = info->srv->record->target_;

        // 在主机表中查找主机信息
        auto p_host = host_table_.find(hostname);
        if (p_host == host_table_.end()) {
            return nullptr;
        }

        // 构建IP端点列表（IP地址 + 端口）
        std::vector<boost::asio::ip::tcp::endpoint> endpoint_list;
		std::vector<std::unique_ptr<ServiceFact<RecordA>>> address_list = p_host->second->address_list;
        for(int i = 0; i < address_list.size(); i++) {
            auto address = address_list[i]->record->address;
            boost::asio::ip::tcp::endpoint endpoint(address, port);
            endpoint_list.push_back(endpoint);
		}

        // 解析TXT记录
        std::unordered_map<std::string, std::string> txt_record;
        for (std::string item : info->txt->record->TXT) {
            size_t eq_pos = item.find('=');
            if (eq_pos == std::string::npos) {
                txt_record[item] = ""; // 没有'='，值为空字符串
            }
            else {
                txt_record[item.substr(0, eq_pos)] = item.substr(eq_pos + 1);
            }
        }

        return std::make_unique<ServiceResolveResult>(name, hostname, endpoint_list, txt_record);
    }

private:
    std::shared_ptr<TrafficMonitor> monitor_;
    std::thread expiration_thread_;
    std::mutex mutex_;
    std::unordered_map<std::string, std::shared_ptr<ServiceCollection>> service_root_;
    std::unordered_map<std::string, std::shared_ptr<HostInfo>> host_table_;
};