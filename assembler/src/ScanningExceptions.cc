#include <string>
#include "ScanningExceptions.h"

TokenError::TokenError(std::string msg) : errorMsg(std::move(msg)){};
const std::string &TokenError::what() {
    return errorMsg;
}