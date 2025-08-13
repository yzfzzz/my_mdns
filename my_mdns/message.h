#pragma once
#include <vector>
#include <memory>
#include <chrono>
#include <stdexcept>
#include "header.h"
#include "question.h"
#include "rr.h"
#include "record_writer.h"
#include "record_reader.h"

// 使用智能指针的版本
class Message {
public:
    Header header;
    std::vector<std::shared_ptr<Question>> questions_;
    std::vector<std::shared_ptr<RR>> answers_;
    std::vector<std::shared_ptr<RR>> authorities_;
    std::vector<std::shared_ptr<RR>> additionals_;
    std::chrono::system_clock::time_point TimeStamp;

    // 默认构造函数
    Message() : TimeStamp(std::chrono::system_clock::now()) {
        // 初始化各部分容器
    }

    // 从字节数组构造函数
    Message(const std::vector<uint8_t>& data, bool enableSecurityExtensions)
        : TimeStamp(std::chrono::system_clock::now()) {

        RecordReader rr(data);

        // 解析头部
        header = Header(rr);

        // 解析问题部分
        for (int i = 0; i < header.QDCOUNT; i++) {
            questions_.push_back(std::make_shared<Question>(rr));
        }

        // 解析回答部分
        for (int i = 0; i < header.ANCOUNT; i++) {
            answers_.push_back(std::make_shared<RR>(rr));
        }

        // 解析授权部分
        for (int i = 0; i < header.NSCOUNT; i++) {
            authorities_.push_back(std::make_shared<RR>(rr));
        }

        // 解析附加部分
        for (int i = 0; i < header.ARCOUNT; i++) {
            additionals_.push_back(std::make_shared<RR>(rr));
        }
    }

    // 写入函数
    void Write(RecordWriter& rw) {
        // 更新头部计数
        header.QDCOUNT = CheckUInt16(static_cast<int>(questions_.size()));
        header.ANCOUNT = CheckUInt16(static_cast<int>(answers_.size()));
        header.NSCOUNT = CheckUInt16(static_cast<int>(authorities_.size()));
        header.ARCOUNT = CheckUInt16(static_cast<int>(additionals_.size()));

        // 写入头部
        header.write(rw);

        // 写入问题部分
        for (const auto& q : questions_) {
            q->write(rw);
        }

        // 写入回答部分
        for (const auto& a : answers_) {
            a->Write(rw);
        }

        // 写入授权部分
        for (const auto& a : authorities_) {
            a->Write(rw);
        }

        // 写入附加部分
        for (const auto& a : additionals_) {
            a->Write(rw);
        }
    }

private:
    // 检查值是否在uint16范围内
    static uint16_t CheckUInt16(int value) {
        if (value < 0 || value > 0xFFFF) {
            throw std::invalid_argument("Value is outside the range allowed for uint16: " + std::to_string(value));
        }
        return static_cast<uint16_t>(value);
    }
};
