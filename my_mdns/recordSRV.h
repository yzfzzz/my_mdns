#pragma once
// RecordSRV.hpp
#pragma once
#include <string>
#include <sstream>
#include "record.h"
#include "record_reader.h"
#include "record_writer.h"
#include "struct.h" // Type 枚举

class RecordSRV : public Record {
public:
    uint16_t priority_;
    uint16_t weight_;
    uint16_t port_;
    std::string target_;

    // 通过 RecordReader 解析
    RecordSRV(RecordReader& rr) {
        priority_ = rr.readUInt16();
        weight_ = rr.readUInt16();
        port_ = rr.readUInt16();
        target_ = rr.readDomainName();
    }

    // 通过参数构造
    RecordSRV(uint16_t priority, uint16_t weight, uint16_t port, const std::string& target)
        : priority_(priority), weight_(weight), port_(port), target_(target) {
    }

    // 返回类型
    Type recordType() override {
        return Type::SRV;
    }

    // 写入到 RecordWriter
    void write(RecordWriter& rw) override {
        rw.writeUInt16(priority_);
        rw.writeUInt16(weight_);
        rw.writeUInt16(port_);
        rw.writeDomainNameUncompressed(target_);
    }

    // 字符串表示
    std::string toString() override {
        std::ostringstream oss;
        oss << priority_ << " " << weight_ << " " << port_ << " " << target_;
        return oss.str();
    }
};
