#pragma once

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
		//byte pointer with length
#ifndef RawBytes
class RawBytes {
public:
	unsigned char* bytes = nullptr;
	int len = 0;
	MSFL_EXP void CopyToBuffer(RawBytes* buffer);
	MSFL_EXP void deleteMemory();
	MSFL_EXP RawBytes() {};
};
#endif

#ifdef __cplusplus
}
#endif