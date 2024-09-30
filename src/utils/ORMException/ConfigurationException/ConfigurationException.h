#include "include/utils/ORMException/ConfigurationException/ConfigurationException.h"

ConfigurationException::ConfigurationException(const std::string& message)
    : ORMException("ConfigurationException: " + message) {
}
