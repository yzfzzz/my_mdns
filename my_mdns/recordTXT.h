#pragma once

// RecordTXT.hpp
#pragma once
#include <vector>
#include <string>
#include <sstream>
#include <map>
#include <algorithm>
#include "record.h"
#include "record_reader.h"
#include "record_writer.h"
#include "struct.h" // Type 枚举

class RecordTXT : public Record {
public:
    std::vector<std::string> TXT;

    // 通过 RecordReader 解析
    RecordTXT(RecordReader& rr, int length) {
        int pos = rr.getPosition();
        while (rr.getPosition() - pos < length && rr.getPosition() < rr.getLength()) {
            TXT.push_back(rr.readString());
        }
    }

    // 通过字典构造
    RecordTXT(const std::map<std::string, std::string>& dict) {
        for (const auto& kv : dict) {
            TXT.push_back(kv.first + "=" + kv.second);
        }
    }

    // 通过字符串序列构造
    RecordTXT(const std::vector<std::string>& sequence) : TXT(sequence) {}

    // 返回类型
    Type recordType() override {
        return Type::TXT;
    }

    // 写入到 RecordWriter
    void write(RecordWriter& rw)  override {
        for (const auto& s : TXT) {
            rw.writeString(s);
        }
    }

    // 字符串表示
    std::string toString() override {
        std::ostringstream oss;
        for (const auto& txt : TXT) {
            oss << "TXT \"" << txt << "\"\n";
        }
        return oss.str();
    }
};
