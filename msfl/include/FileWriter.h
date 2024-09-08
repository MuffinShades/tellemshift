#pragma once
#include <iostream>
#include <fstream>
#include "balloon.hpp"

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

class FileWriter {
public:
	MSFL_EXP static bool WriteToBinFile(std::string path, unsigned char* bytes, int len);
	MSFL_EXP static arr_container<byte> ReadBytesFromBinFile(std::string path);

#ifdef WIN32__a
	MSFL_EXP static bool WriteToBinFileLPCWSTR(LPCWSTR path, unsigned char* bytes, int len);
	MSFL_EXP static arr_container<byte> ReadBytesFromBinFileLPCWSTR(LPCWSTR path);
#endif
};

#ifdef __cplusplus
}
#endif