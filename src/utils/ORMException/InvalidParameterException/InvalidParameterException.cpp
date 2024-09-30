#include "include/utils/ORMException/MappingException/InvalidParameterException/InvalidParameterException.h"

InvalidParameterException::InvalidParameterException(const std::string& message)
    : ORMException("InvalidParameterException: " + message) {
}
