#ifndef ENTITY_NOT_FOUND_EXCEPTION_H
#define ENTITY_NOT_FOUND_EXCEPTION_H

#include "MappingException.h"

class EntityNotFoundException : public MappingException {
public:
    explicit EntityNotFoundException(const std::string& message);
    virtual ~EntityNotFoundException() noexcept = default;
};

#endif // ENTITY_NOT_FOUND_EXCEPTION_H
