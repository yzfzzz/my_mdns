#pragma once

// RecordUnknown.hpp
#pragma once
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>
#include "record.h"
#include "record_reader.h"
#include "record_writer.h"
#include "struct.h" // Type 枚举

class RecordUnknown : public Record {
public:
    Type type_;
    std::vector<uint8_t> rdata_;

    // 返回类型
    Type recordType() override {
        return Type::UINFO;
    }

    // 通过 RecordReader 解析
    RecordUnknown(RecordReader& rr, Type type, int record_length)
        : type_(type) {
        rdata_ = rr.readBytes(record_length);
    }

    // 写入到 RecordWriter
    void write(RecordWriter& rw)override {
        for (size_t i = 0; i < rdata_.size(); ++i)
            rw.writeByte(rdata_[i]);
    }

    // 字符串表示
    std::string toString()override {
        if (rdata_.empty())
            return "RDATA = null";
        std::ostringstream oss;
        oss << "RDATA = [";
        for (size_t i = 0; i < rdata_.size(); ++i) {
            if (i > 0) oss << " ";
            oss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(rdata_[i]);
        }
        oss << "]";
        return oss.str();
    }
};
