#pragma once
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

