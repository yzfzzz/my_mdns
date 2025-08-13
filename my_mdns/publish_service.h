#pragma once
#include <string>
#include <map>
#include <memory>
#include <boost/asio.hpp>
#include "publish_client_impl.h"


class PublishedService {
public:
    std::shared_ptr<IPublishClient> client_;
    std::string long_name_;
    std::string short_name_;
    std::string service_type_;
    uint16_t port_;
    std::map<std::string, std::string> txt_record_;

    PublishedService() = default;
};