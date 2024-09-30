#ifndef IENTITY_H
#define IENTITY_H

#include <string>

class IEntity {
public:
    virtual ~IEntity() = default;

    virtual std::string getEntityName() const = 0;
    virtual std::string getId() const = 0;
    virtual void setId(const std::string& id) = 0;
    virtual std::string getFieldValue(const std::string& fieldName) const = 0;
    virtual void setFieldValue(const std::string& fieldName, const std::string& value) = 0;
};

#endif // IENTITY_H