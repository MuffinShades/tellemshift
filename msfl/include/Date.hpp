#pragma once
#include <iostream>
#include "balloon.hpp"
#include "msutil.hpp"

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

class Date {
private:
    void _i(std::string date = "", std::string time = "");
public:
    i16 year = 0, ms = 0;
    byte day = 0, month = 0, hour = 0, minute = 0, second = 0;
    MSFL_EXP Date(std::string date, std::string time = "") {
        this->_i(date, time);
    } 
    MSFL_EXP Date(u64 iVal);
    MSFL_EXP Date(time_t t);
    MSFL_EXP Date(){};
    MSFL_EXP u64 getLong();
    MSFL_EXP std::string getString();
};

#ifdef __cplusplus
}
#endif