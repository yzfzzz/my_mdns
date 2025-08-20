#pragma once

#include <string>
#include <vector>
#include <string_view>
#include <unordered_map>
// 包含所需的Boost库头文件
#include <boost/asio/ip/tcp.hpp>


struct ServiceResolveResult {
public:
    /**
        * @brief 类型别名，使用 boost::asio::ip::tcp::endpoint 代表IP端点。
        * 这是Boost.Asio中用于网络编程的标准类型。
        */
    using IPEndPoint = boost::asio::ip::tcp::endpoint;

    /**
        * @brief 类型别名，使用 boost::unordered_map 代表键值对字典。
        * 这是Boost提供的高性能哈希表。
        */
    using TxtRecordMap = std::unordered_map<std::string, std::string>;

    // 成员变量声明为const，以确保在对象构造后其值不可更改。
    const std::string name_;
    const std::string host_name_;
    const std::vector<IPEndPoint> ip_endpoint_list_;
    const TxtRecordMap txt_record_;

    /**
        * @brief 构造函数
        * @param name 服务的实例名称 (例如 "My Computer")。
        * @param hostName 提供服务的主机名 (例如 "my-computer.local.")。
        * @param endpointList 服务的IP端点列表 (IP地址和端口)。
        * @param txtRecord 从服务的TXT记录中解析出的键值对字典。
        */
    ServiceResolveResult(
        std::string name,
        std::string host_name,
        std::vector<IPEndPoint> end_point_list, // 按值传递以支持移动或复制
        TxtRecordMap txt_record)               // 按值传递
        : name_(name)
        , host_name_(host_name)
        , ip_endpoint_list_(end_point_list) // 从参数移动，高效初始化
        , txt_record_(txt_record)         // 从参数移动，高效初始化
    {
    }
};

