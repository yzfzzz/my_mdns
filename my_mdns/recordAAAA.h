#pragma once
// RecordAAAA.hpp
#pragma once
#include <array>
#include <string>
#include <sstream>
#include <iomanip>
#include "record.h"
#include "record_reader.h"
#include "record_writer.h"
#include "struct.h" // Type 枚举

class RecordAAAA : public Record {
public:
    std::array<uint16_t, 8> data;

    // 通过 RecordReader 解析
    RecordAAAA(RecordReader& rr) {
        for (int i = 0; i < 8; ++i)
            data[i] = rr.readUInt16();
    }

    // 通过数组构造
    RecordAAAA(const std::array<uint16_t, 8>& addr) : data(addr) {}

    // 返回类型
    Type recordType() override {
        return Type::AAAA;
    }

    // 写入到 RecordWriter
    void write(RecordWriter& rw) override {
        for (int i = 0; i < 8; ++i)
            rw.writeUInt16(data[i]);
    }

    // 字符串表示
    std::string toString() override {
        std::ostringstream oss;
        for (int i = 0; i < 8; ++i) {
            if (i > 0) oss << ":";
            oss << std::hex << data[i];
        }
        return oss.str();
    }
};
