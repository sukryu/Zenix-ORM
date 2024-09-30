#ifndef CONFIGURATION_EXCEPTION_H
#define CONFIGURATION_EXCEPTION_H

#include "ORMException.h"

class ConfigurationException : public ORMException {
public:
    explicit ConfigurationException(const std::string& message);
    virtual ~ConfigurationException() noexcept = default;
};

#endif // CONFIGURATION_EXCEPTION_H
