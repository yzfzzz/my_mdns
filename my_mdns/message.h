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
    std::vector<std::shared_ptr<Question>> Questions;
    std::vector<std::shared_ptr<RR>> Answers;
    std::vector<std::shared_ptr<RR>> Authorities;
    std::vector<std::shared_ptr<RR>> Additionals;
    std::chrono::system_clock::time_point TimeStamp;

    // 默认构造函数
    Message() : TimeStamp(std::chrono::system_clock::now()) {
        // 初始化各部分容器
    }

    // 从字节数组构造函数
    Message(const std::vector<uint8_t>& data, bool enableSecurityExtensions)
        : TimeStamp(std::chrono::system_clock::now()) {

        RecordReader rr(data, enableSecurityExtensions);

        // 解析头部
        header = Header(rr);

        // 解析问题部分
        for (int i = 0; i < header.QDCOUNT; i++) {
            Questions.push_back(std::make_shared<Question>(rr));
        }

        // 解析回答部分
        for (int i = 0; i < header.ANCOUNT; i++) {
            Answers.push_back(std::make_shared<RR>(rr));
        }

        // 解析授权部分
        for (int i = 0; i < header.NSCOUNT; i++) {
            Authorities.push_back(std::make_shared<RR>(rr));
        }

        // 解析附加部分
        for (int i = 0; i < header.ARCOUNT; i++) {
            Additionals.push_back(std::make_shared<RR>(rr));
        }
    }

    // 写入函数
    void Write(RecordWriter& rw) {
        // 更新头部计数
        header.QDCOUNT = CheckUInt16(static_cast<int>(Questions.size()));
        header.ANCOUNT = CheckUInt16(static_cast<int>(Answers.size()));
        header.NSCOUNT = CheckUInt16(static_cast<int>(Authorities.size()));
        header.ARCOUNT = CheckUInt16(static_cast<int>(Additionals.size()));

        // 写入头部
        header.Write(rw);

        // 写入问题部分
        for (const auto& q : Questions) {
            q->Write(rw);
        }

        // 写入回答部分
        for (const auto& a : Answers) {
            a->Write(rw);
        }

        // 写入授权部分
        for (const auto& a : Authorities) {
            a->Write(rw);
        }

        // 写入附加部分
        for (const auto& a : Additionals) {
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
