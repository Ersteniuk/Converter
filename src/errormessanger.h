#ifndef ERRORMESSANGER_H
#define ERRORMESSANGER_H
#include "updateerror.h"

class ErrorMessanger
{
public:
    virtual ~ErrorMessanger()
    {}

    void check(const UpdateError& e);
protected:
    virtual void preCheckCalculation(const UpdateError& e)
        {}
    virtual void postCheckCalculation(const UpdateError& e)
        {}
    virtual void unknownErrorType(const UpdateError& e)
        { criticalError(e); }

    virtual void smallError(const UpdateError& e);
    virtual void mediumError(const UpdateError& e);
    virtual void hugeError(const UpdateError& e);
    virtual void criticalError(const UpdateError& e);
};

#endif // ERRORMESSANGER_H
