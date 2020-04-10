#include <string>

#ifndef SCANNING_EXCEPTIONS_H
#define SCANNING_EXCEPTIONS_H

class TokenError {
    private:
        std::string errorMsg;

    public:
        TokenError(std::string msg);
        const std::string &what();
};
#endif
