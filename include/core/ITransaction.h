#ifndef ITRANSACTION_H
#define ITRANSACTION_H

class ITransaction {
public:
    virtual ~ITransaction() = default;

    virtual void commit() = 0;
    virtual void rollback() = 0;
};

#endif // ITRANSACTION_H