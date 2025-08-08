#pragma once
// Question.hpp
#pragma once
#include <string>
#include "struct.h"      // QType, QClass 枚举定义
#include "record_reader.h" // 需实现
#include "record_writer.h" // 需实现

class Question {
public:
    std::string QName;
    QType Qtype;
    QClass Qclass;

    // 构造函数
    Question(const std::string& qname, QType qtype, QClass qclass)
        : QName(qname), Qtype(qtype), Qclass(qclass) {
        if (!QName.empty() && QName.back() != '.')
            QName += '.';
    }

    // 从RecordReader反序列化
    Question(RecordReader& rr) {
        QName = rr.readDomainName();
        Qtype = static_cast<QType>(rr.readUInt16());
        Qclass = static_cast<QClass>(rr.readUInt16());
    }

    // 写入到RecordWriter
    void write(RecordWriter& writer) const {
        writer.writeDomainNameUncompressed(QName);
        writer.writeUInt16(static_cast<uint16_t>(Qtype));
        writer.writeUInt16(static_cast<uint16_t>(Qclass));
    }

    // ToString
    std::string toString() const {
        char buf[128];
        snprintf(buf, sizeof(buf), "%-32s %d %d", QName.c_str(), static_cast<int>(Qclass), static_cast<int>(Qtype));
        return std::string(buf);
    }
};
