#pragma once
#include <vector>
#include <string>
#include <cstdint>

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

    // 域名读取（简化版，不含压缩指针处理）
    // 可能暂时没有用
    std::string ReadDomainName() {
        std::string name;
        while (true) {
            uint8_t len = readByte();
            if (len == 0) break;
            if (!name.empty()) name += ".";
            for (int i = 0; i < len; ++i) {
                name += static_cast<char>(readByte());
            }
        }
        return name;
    }

    size_t position() const { return pos_; }
    void setPosition(size_t pos) { pos_ = pos; }

private:
    const std::vector<uint8_t>& data_;
    size_t pos_;
};
