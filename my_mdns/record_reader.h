#pragma once
#include <vector>
#include <string>
#include <cstdint>
#include <stdexcept>
#include "record.h"
#include "recordA.h"
#include "recordAAAA.h"
#include "recordSRV.h"
#include "recordTXT.h"
#include "recordPTR.h"
#include "struct.h"
#include "recordUNKNOWN.h"

class RecordReader {
public:
    RecordReader(const std::vector<uint8_t>& data)
        : data_(data), pos_(0) {
    }

    uint8_t readByte() {
        return data_[pos_++];
    }

    uint16_t readUInt16() {
        uint8_t hi = readByte();
        uint8_t lo = readByte();
        return (hi << 8) | lo;
    }

    uint32_t readUInt32() {
        uint32_t b1 = readByte();
        uint32_t b2 = readByte();
        uint32_t b3 = readByte();
        uint32_t b4 = readByte();
        return (b1 << 24) | (b2 << 16) | (b3 << 8) | b4;
    }

    std::vector<uint8_t> readBytes(size_t count) {
        std::vector<uint8_t> result(data_.begin() + pos_, data_.begin() + pos_ + count);
        pos_ += count;
        return result;
    }

    // 域名读取（含压缩指针处理）
    std::string readDomainName() {
        std::vector<uint8_t> bytes;
        int length = 0;
        std::string name;

        while ((length = readByte()) != 0) {
            if ((length & 0xc0) == 0xc0) {
                int position = ((length & 0x3f) << 8) | readByte();
                if (position < 0 || position >= pos_ - 2)
                    throw std::runtime_error("Invalid domain name compression reference");

                int save_position = pos_;
                pos_ = position;
                std::string tail = readDomainName();
                pos_ = save_position;

                std::string head = bytes.empty() ? "" : std::string(bytes.begin(), bytes.end());
                name = head + tail;
                return name;
            }

            if ((length & 0xc0) != 0) {
                throw std::runtime_error("Unsupported domain name compression type");
            }

            while (length > 0) {
                bytes.push_back(readByte());
                --length;
            }
            bytes.push_back('.');
        }

        name = bytes.empty() ? "." : std::string(bytes.begin(), bytes.end());
        return name;
    }

    size_t getPosition() const { return pos_; }
    void setPosition(size_t pos) { pos_ = pos; }

    int getLength() {
        return data_.size();
    }

    std::string readString() {
        uint8_t length = readByte();
        std::vector<uint8_t> bytes;
        for (int i = 0; i < length; ++i)
            bytes.push_back(readByte());
        return std::string(bytes.begin(), bytes.end()); // 假定内容为UTF-8编码
    }

    Record* readRecord(Type type, int length) {
        int pos_before = pos_;
        Record* rec = nullptr;
        switch (type) {
        case Type::A:
            rec = new RecordA(*this);
            break;
        case Type::PTR:
            rec = new RecordPTR(*this);
            break;
        case Type::TXT:
            rec = new RecordTXT(*this, length);
            break;
        case Type::AAAA:
            rec = new RecordAAAA(*this);
            break;
        case Type::SRV:
            rec = new RecordSRV(*this);
            break;
        default:
            rec = new RecordUnknown(*this, type, length);
            break;
        }
        int bytes_read = pos_ - pos_before;
        if (bytes_read != length) {
            delete rec;
            throw std::runtime_error("Number of bytes read != declared length");
        }
        return rec;
    }

private:
    const std::vector<uint8_t> data_;
    size_t pos_;
};
