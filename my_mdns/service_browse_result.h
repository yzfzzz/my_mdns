#pragma once

#pragma once

#include <string>
#include <string_view>
#include <sstream>

struct ServiceBrowseResult {
public:
    
    std::string name_;
    std::string service_type_;

    /**
        * @brief 构造函数
        * @param name 服务实例的名称。
        * @param serviceType 服务的类型，会自动移除".local."后缀。
        */
    ServiceBrowseResult(std::string name, std::string service_type)
        : name_(name), service_type_(abbreviate(service_type)) {
    }

    /**
        * @brief 生成对象的字符串表示形式，类似于C#的ToString()。
        * @return 格式为 "[Name] ServiceType" 的字符串。
        */
    std::string ToString()  {
        std::stringstream ss;
        ss << "[" << name_ << "] " << service_type_;
        return ss.str();
    }

private:
    /**
        * @brief 移除服务类型名称末尾的".local."后缀。
        * @param name 完整的服务类型名称。
        * @return 缩写后的服务类型名称。
        */
    std::string abbreviate(std::string name) {
        std::string suffix = ".local.";
         if (name.length() >= suffix.length() && 
             name.substr(name.length() - suffix.length()) == suffix) {
             return std::string(name.substr(0, name.length() - suffix.length()));
         }

        return std::string(name);
    }
};


