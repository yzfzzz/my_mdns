#pragma once
// RR.hpp
#include <string>
#include <cstdint>
#include "struct.h"      // Type, Class 枚举
//#include "Record.hpp"       // Record 抽象基类
#include "record_reader.h" // RecordReader
#include "record_writer.h" // RecordWriter

class RR {
public:
    std::string name_;
    Type type_;
    DnsClass rrclass_;
    uint32_t m_TTL_;
    uint16_t read_length_;
    Record* record_;
    int timelived_;
    uint16_t rd_length_;

    // TTL属性
    uint32_t getTTL() const {
        return m_TTL_ > static_cast<uint32_t>(timelived_) ? m_TTL_ - timelived_ : 0;
    }
    void setTTL(uint32_t value) {
        m_TTL_ = value;
    }

    // 通过RecordReader反序列化
    RR(RecordReader& rr) : timelived_(0), record_(nullptr) {
        name_ = rr.readDomainName();
        type_ = static_cast<Type>(rr.readUInt16());
        rrclass_ = static_cast<DnsClass>(rr.readUInt16());
        setTTL(rr.readUInt32());
        read_length_ = rr.readUInt16();
        record_ = rr.readRecord(type_, read_length_);
        if (record_) record_->rr = this;
    }


    // 写入到RecordWriter
    void Write(RecordWriter& rw) {
        rw.writeDomainNameCompressed(name_);
        rw.writeUInt16(static_cast<uint16_t>(type_));
        rw.writeUInt16(static_cast<uint16_t>(rrclass_));
        rw.writeUInt32(getTTL());
        rw.writeRecord(record_);
    }

    // 字符串表示
    std::string toString() const {
        char buf[256];
        snprintf(buf, sizeof(buf), "name=[%s] TTL=%u class=%d type=%d record=[%s]",
            name_.c_str(), m_TTL_, static_cast<int>(rrclass_), static_cast<int>(type_),
            record_ ? record_->toString().c_str() : "null");
        return std::string(buf);
    }

    // 析构函数
    ~RR() {
        delete record_;
    }
};