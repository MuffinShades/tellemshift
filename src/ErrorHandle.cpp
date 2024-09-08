#include "ErrorHandle.hpp"
#include <string>

#define ERROR_HANDLE_PRINT_ERRORS
#ifdef ERROR_HANDLE_PRINT_ERRORS
bool ErrorHandler::printError = true;
#else
bool ErrorHandler::printError = false;   
#endif /*ERROR_HANDLE_PRINT_ERRORS*/

#ifdef ERROR_HANDLE_PRINT_WARNINGS
bool ErrorHandler::printWarning = true;
#else
bool ErrorHandler::printWarning = false;   
#endif /*ERROR_HANDLE_PRINT_ERRORS*/

bool ErrorHandler::logInfo = true;

void ErrorHandler::ThrowError(int code, std::string additionalInfo, std::string file, enum ErrorLevel level) {
    std::string errorLabel;

    if (file.length() > 0) {
        errorLabel = "[ERROR " + std::to_string(code) + "] in file " + file + ": " + additionalInfo;
    } else {
        errorLabel = "[ERROR " + std::to_string(code) + "]: " + additionalInfo;
    }

    if (printError)
        std::cout << errorLabel << std::endl;

    if (logInfo) {} // TODO: add logging here

    if (static_cast<int>(level) > 0) //throw proper error ya know
        throw std::invalid_argument(errorLabel);
}