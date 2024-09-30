#ifndef MAPPING_EXCEPTION_H
#define MAPPING_EXCEPTION_H

#include "ORMException.h"

class MappingException : public ORMException {
public:
    explicit MappingException(const std::string& message);
    virtual ~MappingException() noexcept = default;
};

#endif // MAPPING_EXCEPTION_H
