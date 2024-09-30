#include "include/utils/ORMException/LazyInitializationException/LazyInitializationException.h"

LazyInitializationException::LazyInitializationException(const std::string& message)
    : ORMException("LazyInitializationException: " + message) {
}
