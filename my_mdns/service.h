#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <memory>
#include <functional>
#include <mutex>
#include <thread>
#include <atomic>
#include <chrono>
#include <algorithm>

// 假设这些依赖类型也已被转换为C++
#include "traffic_monitor.h" 
#include "struct.h"   
#include "record.h"
#include "recordSRV.h"
#include "recordPTR.h"
#include "recordTXT.h"
#include "recordA.h"

// DNS记录的时间包装器
template<class RecordType>
class ServiceFact {
public:
    std::chrono::steady_clock::time_point creation_time;
    std::shared_ptr<RecordType> record;

    explicit ServiceFact(std::shared_ptr<RecordType> rec)
        : creation_time(std::chrono::steady_clock::now()), record(rec) {
        if (!record) {
            throw std::invalid_argument("Record cannot be null");
        }
    }

    double remainingLifeInSeconds() const {
        auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(
            std::chrono::steady_clock::now() - creation_time).count();
        return static_cast<double>(record->rr->getTTL()) - elapsed;
    }

    bool isExpired() const {
        return remainingLifeInSeconds() < 0.0;
    }

};

// 单个服务的完整信息
class ServiceInfo {
public:
    std::unique_ptr<ServiceFact<RecordSRV>> srv;
    std::unique_ptr<ServiceFact<RecordPTR>> ptr;
    std::unique_ptr<ServiceFact<RecordTXT>> txt;

    bool isExpired() const {
        return ptr && ptr->isExpired();
    }

    void updateSrv(std::shared_ptr<RecordSRV> record) {
        if (!srv || record->priority_ < srv->record->priority_) {
            srv = std::make_unique<ServiceFact<RecordSRV>>(record);
        }
    }

    void updatePtr(std::shared_ptr<RecordPTR> record) {
        ptr = std::make_unique<ServiceFact<RecordPTR>>(record);
    }

    void updateTxt(std::shared_ptr<RecordTXT> record) {
        txt = std::make_unique<ServiceFact<RecordTXT>>(record);
    }
};

// 主机地址信息
class HostInfo {
public:
    std::vector<std::unique_ptr<ServiceFact<RecordA>>> address_list;

    void updateAddress(std::shared_ptr<RecordA> record) {
        auto fact = std::make_unique<ServiceFact<RecordA>>(record);
        for (auto& existing : address_list) {
            if (existing->record->address == record->address) {
                existing = std::move(fact); // 替换
                return;
            }
        }
        address_list.push_back(fact); // 新增
    }
};


// 服务集合
class ServiceCollection {
public:
    std::unordered_map<std::string, std::shared_ptr<ServiceInfo>> service_table;

    std::shared_ptr<ServiceInfo> lazyCreate(const std::string& name) {
        auto it = service_table.find(name);
        if (it != service_table.end()) {
            return it->second;
        }
        auto info = std::make_shared<ServiceInfo>();
        service_table[name] = info;
        return info;
    }
};



