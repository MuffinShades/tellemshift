#pragma once
#include <iostream>
#include <cstring>
#include <cmath>
#include "balloon.hpp"
#include "msutil.hpp"

#ifndef min
#define min(a, b) ((a) < (b) ? (a) : (b))
#endif

#ifdef __cplusplus
extern "C" {
#endif

#define MSFL_COMPILE_DLL
#define MSFL_EXPORTS

#ifdef WIN32
	#ifdef MSFL_COMPILE_DLL
		#ifdef MSFL_EXPORTS
			#define MSFL_EXP __declspec(dllexport)
		#else
			#define MSFL_EXP __declspec(dllimport)
		#endif
	#else
		#define MSFL_EXP
	#endif
#else
	#define MSFL_EXP
#endif

enum ByteStreamMode {
	bmode_LittleEndian,
	bmode_BigEndian
};

class ByteStream {
private:
	byte* bytes = nullptr;
	size_t len = 0;
	size_t allocSz = 0;
	size_t readPos = 0;
	size_t chunkSz = 0xfff;
public:
	int mode = bmode_BigEndian;
	MSFL_EXP void allocNewChunk();
	MSFL_EXP void allocBytes(size_t sz);
	MSFL_EXP ByteStream(byte* dat, size_t sz);
	MSFL_EXP ByteStream();
	MSFL_EXP byte readByte();
	MSFL_EXP size_t seek(size_t pos);
	MSFL_EXP unsigned long long readBytesAsVal(size_t nBytes);
	MSFL_EXP short readInt16();
	MSFL_EXP unsigned short readUInt16();
	MSFL_EXP int readInt32();
	MSFL_EXP unsigned int readUInt32();
	MSFL_EXP int64_t readInt64();
	MSFL_EXP uint64_t readUInt64();
	MSFL_EXP void writeByte(byte b);
	MSFL_EXP void writeNBytesAsVal(unsigned long long v, size_t nBytes);
	MSFL_EXP void writeInt16(short v);
	MSFL_EXP void writeUInt16(unsigned short v);
	MSFL_EXP void writeInt32(int v);
	MSFL_EXP void writeUInt32(unsigned int v);
	MSFL_EXP void writeInt64(int64_t v);
	MSFL_EXP void writeUInt64(uint64_t v);
	MSFL_EXP void clip();
	MSFL_EXP void writeBytes(byte *dat, size_t sz);
	MSFL_EXP void free() {
		if (this->bytes != nullptr) delete[] this->bytes;
		this->bytes = nullptr;
	}
	MSFL_EXP ~ByteStream();
	MSFL_EXP size_t getSize() {
		return this->len;
	};
	MSFL_EXP size_t getAllocSize() {
		return this->allocSz;
	};
	MSFL_EXP byte *getBytePtr() {
		return this->bytes;
	}
	MSFL_EXP size_t tell() {
		return this->readPos;
	}
	MSFL_EXP size_t skipBytes(size_t nBytes);
	MSFL_EXP char *readCStr(size_t len);
	MSFL_EXP std::string readStr(size_t len);
};

#ifdef __cplusplus
}
#endif