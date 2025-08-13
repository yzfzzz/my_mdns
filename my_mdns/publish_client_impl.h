#pragma once
#include <string> 

class IPublishClient {
public:
    virtual ~IPublishClient() = default;
    virtual void OnPublish(const std::string& requested_name, const std::string& actual_name) = 0;
};
