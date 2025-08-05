#pragma once
#include "record_reader.h"
#include "record_wirter.h"
#include "struct.h"
class Header {
public:
	unsigned short ID;
	unsigned short Flags;
	unsigned short QDCOUNT;
	unsigned short ANCOUNT;
	unsigned short NSCOUNT;
	unsigned short ARCOUNT;

	Header(RecordReader rr) {
		ID = rr.readUInt16();
		Flags = rr.readUInt16();
		QDCOUNT = rr.readUInt16();
		ANCOUNT = rr.readUInt16();
		NSCOUNT = rr.readUInt16();
		ARCOUNT = rr.readUInt16();
	}

	void write(RecordWriter rw)
	{
		rw.writeUInt16(ID);
		rw.writeUInt16(Flags);
		rw.writeUInt16(QDCOUNT);
		rw.writeUInt16(ANCOUNT);
		rw.writeUInt16(NSCOUNT);
		rw.writeUInt16(ARCOUNT);
	}

	unsigned short setBits(unsigned short oldValue, int position, int length, unsigned short newValue)
	{

		if (length <= 0 || position >= 16)
			return oldValue;

		int mask = (2 << length - 1) - 1;
		oldValue &= (unsigned short)~(mask << position);
		oldValue |= (unsigned short)((newValue & mask) << position);
		return oldValue;
	}

	unsigned short getBits(unsigned short oldValue, int position, int length)
	{

		if (length <= 0 || position >= 16)
			return 0;
		int mask = (2 << length - 1) - 1;
		return (unsigned short)(oldValue >> position & mask);
	}

	bool getQR() {
		return getBits(Flags, 15, 1) == 1;
	}

	void setQR(bool value) {
		setBits(Flags, 15, 1, value);
	}

	bool getAA() {
		return getBits(Flags, 10, 1) == 1;
	}

	void setAA(bool value) {
		setBits(Flags, 10, 1, value);
	}

	bool getTC() {
		return getBits(Flags, 9, 1) == 1;
	}

	void setTC(bool value) {
		setBits(Flags, 9, 1, value);
	}

	bool getRD() {
		return getBits(Flags, 8, 1) == 1;
	}

	void setRD(bool value) {
		setBits(Flags, 8, 1, value);
	}

	bool getRA() {
		return getBits(Flags, 7, 1) == 1;
	}

	void setRA(bool value) {
		setBits(Flags, 7, 1, value);
	}

	bool getZ() {
		return getBits(Flags, 4, 3);
	}
	// 这里Z是3个bit
	void setZ(uint16_t value) {
		setBits(Flags, 4, 3, value);
	}

	OPCode getOpcode() {
		return (OPCode)getBits(Flags, 11, 4);
	}
	// 这里Opcode是4个bit, 可能会有问题
	void setOpcode(uint16_t value) {
		Flags = setBits(Flags, 11, 4, value);
	}
	// 这里RCode是4个bit, 可能会有问题
	RCode getRCode() {
		return (RCode)getBits(Flags, 11, 4);
	}
	void setOpcode(uint16_t value) {
		Flags = setBits(Flags, 0, 4, value);
	}

};