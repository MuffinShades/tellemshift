#include <iostream>
//TODO: add logging

enum ErrorLevel {
    EL_minor,
    EL_major
};

class ErrorHandler {
private:
    static bool printError, printWarning, logInfo;
public:
    static void ThrowError(int code, std::string additionalInfo = "", std::string file = "", enum ErrorLevel level = EL_minor);
    static void ThrowWarning(int code, std::string additionalInfo = "", std::string file = "");
    static void EnableErrPrinting();
    static void DisableErrPrinting();
};