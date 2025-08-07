#pragma once
#include <array>
#include <string>
#include <stdexcept>
#include <sstream>
#include <boost/asio/ip/address_v4.hpp>
#include "record.h"
#include "record_reader.h"
#include "record_writer.h"
#include "struct.h" // Type 枚举

class RecordA : public Record {
public:
    std::array<uint8_t, 4> data;
    boost::asio::ip::address_v4 address;

    // 通过 RecordReader 解析
    RecordA(RecordReader& rr) {
        for (int i = 0; i < 4; ++i)
            data[i] = rr.readByte();
        address = boost::asio::ip::address_v4(data);
    }

    // 通过字节数组构造
    RecordA(const std::array<uint8_t, 4>& addr) : data(addr), address(boost::asio::ip::address_v4(addr)) {}

    // 返回类型
    Type recordType() override{
        return Type::A;
    }

    // 写入到 RecordWriter
    void write(RecordWriter& rw) override{
        for (int i = 0; i < 4; ++i)
            rw.writeByte(data[i]);
    }

    // 字符串表示
    std::string toString() override  {
        return address.to_string();
    }
};
