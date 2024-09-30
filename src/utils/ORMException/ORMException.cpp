#include "include/utils/ORMException/ORMException.h"

ORMException::ORMException(const std::string& message)
    : message("ORMException: " + message) {
}

const char* ORMException::what() const noexcept {
    return message.c_str();
}
