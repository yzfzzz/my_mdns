#pragma once

#include <cstdint>

enum class Type : uint16_t {
    A = 1,
    NS = 2,
    MD = 3,
    MF = 4,
    CNAME = 5,
    SOA = 6,
    MB = 7,
    MG = 8,
    MR = 9,
    NULL_T = 10, // C++ 关键字，避免冲突
    WKS = 11,
    PTR = 12,
    HINFO = 13,
    MINFO = 14,
    MX = 15,
    TXT = 16,
    RP = 17,
    AFSDB = 18,
    X25 = 19,
    ISDN = 20,
    RT = 21,
    NSAP = 22,
    NSAPPTR = 23,
    SIG = 24,
    KEY = 25,
    PX = 26,
    GPOS = 27,
    AAAA = 28,
    LOC = 29,
    NXT = 30,
    EID = 31,
    NIMLOC = 32,
    SRV = 33,
    ATMA = 34,
    NAPTR = 35,
    KX = 36,
    CERT = 37,
    A6 = 38,
    DNAME = 39,
    SINK = 40,
    OPT = 41,
    APL = 42,
    DS = 43,
    SSHFP = 44,
    IPSECKEY = 45,
    RRSIG = 46,
    NSEC = 47,
    DNSKEY = 48,
    DHCID = 49,
    NSEC3 = 50,
    NSEC3PARAM = 51,
    HIP = 55,
    SPF = 99,
    UINFO = 100,
    UID = 101,
    GID = 102,
    UNSPEC = 103,
    TKEY = 249,
    TSIG = 250,
    TA = 32768,
    DLV = 32769
};

enum class QType : uint16_t {
    // 与 Type 保持一致
    A = static_cast<uint16_t>(Type::A),
    NS = static_cast<uint16_t>(Type::NS),
    MD = static_cast<uint16_t>(Type::MD),
    MF = static_cast<uint16_t>(Type::MF),
    CNAME = static_cast<uint16_t>(Type::CNAME),
    SOA = static_cast<uint16_t>(Type::SOA),
    MB = static_cast<uint16_t>(Type::MB),
    MG = static_cast<uint16_t>(Type::MG),
    MR = static_cast<uint16_t>(Type::MR),
    NULL_T = static_cast<uint16_t>(Type::NULL_T),
    WKS = static_cast<uint16_t>(Type::WKS),
    PTR = static_cast<uint16_t>(Type::PTR),
    HINFO = static_cast<uint16_t>(Type::HINFO),
    MINFO = static_cast<uint16_t>(Type::MINFO),
    MX = static_cast<uint16_t>(Type::MX),
    TXT = static_cast<uint16_t>(Type::TXT),
    RP = static_cast<uint16_t>(Type::RP),
    AFSDB = static_cast<uint16_t>(Type::AFSDB),
    X25 = static_cast<uint16_t>(Type::X25),
    ISDN = static_cast<uint16_t>(Type::ISDN),
    RT = static_cast<uint16_t>(Type::RT),
    NSAP = static_cast<uint16_t>(Type::NSAP),
    NSAP_PTR = static_cast<uint16_t>(Type::NSAPPTR),
    SIG = static_cast<uint16_t>(Type::SIG),
    KEY = static_cast<uint16_t>(Type::KEY),
    PX = static_cast<uint16_t>(Type::PX),
    GPOS = static_cast<uint16_t>(Type::GPOS),
    AAAA = static_cast<uint16_t>(Type::AAAA),
    LOC = static_cast<uint16_t>(Type::LOC),
    NXT = static_cast<uint16_t>(Type::NXT),
    EID = static_cast<uint16_t>(Type::EID),
    NIMLOC = static_cast<uint16_t>(Type::NIMLOC),
    SRV = static_cast<uint16_t>(Type::SRV),
    ATMA = static_cast<uint16_t>(Type::ATMA),
    NAPTR = static_cast<uint16_t>(Type::NAPTR),
    KX = static_cast<uint16_t>(Type::KX),
    CERT = static_cast<uint16_t>(Type::CERT),
    A6 = static_cast<uint16_t>(Type::A6),
    DNAME = static_cast<uint16_t>(Type::DNAME),
    SINK = static_cast<uint16_t>(Type::SINK),
    OPT = static_cast<uint16_t>(Type::OPT),
    APL = static_cast<uint16_t>(Type::APL),
    DS = static_cast<uint16_t>(Type::DS),
    SSHFP = static_cast<uint16_t>(Type::SSHFP),
    IPSECKEY = static_cast<uint16_t>(Type::IPSECKEY),
    RRSIG = static_cast<uint16_t>(Type::RRSIG),
    NSEC = static_cast<uint16_t>(Type::NSEC),
    DNSKEY = static_cast<uint16_t>(Type::DNSKEY),
    DHCID = static_cast<uint16_t>(Type::DHCID),
    NSEC3 = static_cast<uint16_t>(Type::NSEC3),
    NSEC3PARAM = static_cast<uint16_t>(Type::NSEC3PARAM),
    HIP = static_cast<uint16_t>(Type::HIP),
    SPF = static_cast<uint16_t>(Type::SPF),
    UINFO = static_cast<uint16_t>(Type::UINFO),
    UID = static_cast<uint16_t>(Type::UID),
    GID = static_cast<uint16_t>(Type::GID),
    UNSPEC = static_cast<uint16_t>(Type::UNSPEC),
    TKEY = static_cast<uint16_t>(Type::TKEY),
    TSIG = static_cast<uint16_t>(Type::TSIG),
    IXFR = 251,
    AXFR = 252,
    MAILB = 253,
    MAILA = 254,
    ANY = 255,
    TA = static_cast<uint16_t>(Type::TA),
    DLV = static_cast<uint16_t>(Type::DLV)
};

enum class DnsClass : uint16_t {
    IN = 1,   // Internet
    CS = 2,   // CSNET
    CH = 3,   // CHAOS
    HS = 4    // Hesiod
};


enum class QClass : uint16_t {
    IN = static_cast<uint16_t>(DnsClass::IN),
    CS = static_cast<uint16_t>(DnsClass::CS),
    CH = static_cast<uint16_t>(DnsClass::CH),
    HS = static_cast<uint16_t>(DnsClass::HS),
    ANY = 255
};

enum OPCode
{
    Query = 0,
    IQUERY = 1,

    Status = 2,
    RESERVED3 = 3,

    Notify = 4,
    Update = 5,

    RESERVED6 = 6,
    RESERVED7 = 7,
    RESERVED8 = 8,
    RESERVED9 = 9,
    RESERVED10 = 10,
    RESERVED11 = 11,
    RESERVED12 = 12,
    RESERVED13 = 13,
    RESERVED14 = 14,
    RESERVED15 = 15,
};


enum RCode
{
    NoError = 0,
    FormErr = 1,
    ServFail = 2,
    NXDomain = 3,
    NotImp = 4,
    Refused = 5,
    YXDomain = 6,
    YXRRSet = 7,
    NXRRSet = 8,
    NotAuth = 9,
    NotZone = 10,

    RESERVED11 = 11,
    RESERVED12 = 12,
    RESERVED13 = 13,
    RESERVED14 = 14,
    RESERVED15 = 15,

    BADVERSSIG = 16,

    BADKEY = 17,
    BADTIME = 18,
    BADMODE = 19,
    BADNAME = 20,
    BADALG = 21,
    BADTRUNC = 22,
};

