#pragma once
// RecordPTR.hpp
#pragma once
#include <string>
#include "record.h"
#include "record_reader.h"
#include "record_writer.h"
#include "struct.h" // Type 枚举

class RecordPTR : public Record {
public:
    std::string ptrd_name;

    // 通过 RecordReader 解析
    RecordPTR(RecordReader& rr) {
        ptrd_name = rr.readDomainName();
    }

    // 通过字符串构造
    RecordPTR(const std::string& dname) : ptrd_name(dname) {}

    // 返回类型
    Type recordType() override {
        return Type::PTR;
    }

    // 写入到 RecordWriter
    void write(RecordWriter& rw) override {
        rw.writeDomainNameCompressed(ptrd_name);
    }

    // 字符串表示
    std::string toString() override {
        return ptrd_name;
    }
};