#include "include/utils/ORMException/MappingException/MappingException.h"

MappingException::MappingException(const std::string& message)
    : ORMException("MappingException: " + message) {
}
