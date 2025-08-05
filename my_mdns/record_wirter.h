#pragma once
#include <vector>
#include <string>
#include <unordered_map>
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

    // 写入域名（不含压缩，压缩可扩展）
    void writeDomainName(const std::string& domain) {
    }

    // 获取最终字节数组
    std::vector<uint8_t> toArray() const {
        return buffer;
    }

    // 获取当前写入位置
    size_t position() const {
        return buffer.size();
    }

    // 域名压缩支持（可选扩展）
    // void WriteDomainNameCompressed(const std::string& domain, std::unordered_map<std::string, size_t>& domainOffsets);

private:
    std::vector<uint8_t> buffer;
};