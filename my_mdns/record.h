#pragma once
#include "struct.h"      // Type 枚举
#include "record_writer.h" // RecordWriter 前置声明
#include "record_reader.h"
#include "rr.h"

class Record {
public:
    RR* rr = nullptr;

    virtual ~Record() = default;

    // 写入自身到 RecordWriter
    virtual void write(RecordWriter& rw)= 0;

    // 返回记录类型
    virtual Type recordType() = 0;

    virtual std::string toString() = 0;
};
