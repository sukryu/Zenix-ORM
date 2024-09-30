#ifndef ORM_EXCEPTION_H
#define ORM_EXCEPTION_H

#include <exception>
#include <string>

class ORMException : public std::exception {
public:
    explicit ORMException(const std::string& message);
    virtual ~ORMException() noexcept = default;
    virtual const char* what() const noexcept override;

protected:
    std::string message;
};

#endif // ORM_EXCEPTION_H
