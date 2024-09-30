#include "include/utils/ORMException/MappingException/EntityNotFoundException/EntityNotFoundException.h"

EntityNotFoundException::EntityNotFoundException(const std::string& message)
    : MappingException("EntityNotFoundException: " + message) {
}
