#pragma once
#include <vector>
#include <string>
#include <sstream>
#include <stdexcept>
#include <cstdint>



class RecordWriter {
public:
    RecordWriter() {}

    // 写入单字节
    void writeByte(uint8_t value) {
        buffer.push_back(value);
    }

    // 写入16位无符号整数（大端序）
    void writeUInt16(uint16_t value) {
        buffer.push_back((value >> 8) & 0xFF);
        buffer.push_back(value & 0xFF);
    }

    // 写入32位无符号整数（大端序）
    void writeUInt32(uint32_t value) {
        buffer.push_back((value >> 24) & 0xFF);
        buffer.push_back((value >> 16) & 0xFF);
        buffer.push_back((value >> 8) & 0xFF);
        buffer.push_back(value & 0xFF);
    }

    // 写入字节数组
    void writeBytes(const std::vector<uint8_t>& data) {
        buffer.insert(buffer.end(), data.begin(), data.end());
    }

    // 获取最终字节数组
    std::vector<uint8_t> toArray() const {
        return buffer;
    }

    // 获取当前写入位置
    size_t position() const {
        return buffer.size();
    }

    void writeDomainNameUncompressed(std::string s)
    {
        writeDomainNameUncompressed(buffer, s);
    }

    // 写入未压缩的域名到 buffer
    // "www.example.com" 会被编码为：[3]www[7]example[3]com[0]。
    std::vector<uint8_t>& writeDomainNameUncompressed(std::vector<uint8_t> buffer, std::string s) {
        std::istringstream iss(s);
        std::string label;
        while (std::getline(iss, label, '.')) {
            if (label.empty()) continue; // 跳过空标签
            if (label.size() > 63)
                throw std::runtime_error("Label [" + label + "] is longer than 63 bytes.");
            buffer.push_back(static_cast<uint8_t>(label.size()));
            buffer.insert(buffer.end(), label.begin(), label.end());
        }
        buffer.push_back(0); // 域名结尾
        return buffer;
    }

    // 写入已压缩的域名到 buffer
    void writeDomainNameCompressed(const std::string& s) {
        std::vector<uint8_t> buffer;
        std::vector<uint8_t> serial = writeDomainNameUncompressed(buffer, s);
        int index = 0;
        while (serial[index] != 0) {
            int position = -1;
            if (findTail(serial, index, position)) {
                buffer.push_back(static_cast<uint8_t>(0xc0 | (position >> 8)));
                buffer.push_back(static_cast<uint8_t>(position));
                return;
            }
            buffer.push_back(serial[index]);
            for (int k = 1; k <= serial[index]; ++k)
                buffer.push_back(serial[index + k]);
            index += 1 + serial[index];
        }
        buffer.push_back(0);
    }

    // 查找尾部是否可压缩
    bool findTail(const std::vector<uint8_t>& serial, int index, int& position) {
        position = -1;
        int tailLength = static_cast<int>(serial.size()) - index;
        if (tailLength < 3)
            return false;
        int limit = std::min(0x3fff, 1 + static_cast<int>(buffer.size()) - tailLength);
        for (position = 0; position < limit; ++position) {
            bool match = true;
            for (int offset = 0; match && offset < tailLength; ++offset) {
                if (buffer[position + offset] != serial[index + offset])
                    match = false;
            }
            if (match)
                return true;
        }
        return false;
    }

    // 写入一个长度前缀的字符串（UTF-8编码）
    void writeString(const std::string& s) {
        size_t len = s.size();
        if (len > 255) throw std::runtime_error("String too long for DNS TXT record");
        buffer.push_back(static_cast<uint8_t>(len));
        buffer.insert(buffer.end(), s.begin(), s.end());
    }

    void writeRecord(Record* rec) {
        size_t front = buffer.size();

        // 预留2个字节用于长度字段
        buffer.push_back(0);
        buffer.push_back(0);

        // 写入记录数据
        rec->write(*this);

        // 计算记录长度
        size_t length = buffer.size() - front - 2;
        if (length > 0xffff) {
            throw std::invalid_argument("Record is too long: " + std::to_string(length) + " bytes");
        }

        // 填充长度字段（大端序）
        buffer[front] = static_cast<uint8_t>(length >> 8);
        buffer[front + 1] = static_cast<uint8_t>(length);

    }
private:
    std::vector<uint8_t> buffer;
};