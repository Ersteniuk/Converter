#ifndef UPDATEERROR_H
#define UPDATEERROR_H
#include <QMetaType>
#include <QString>

class StopKran
{};

struct UpdateError
{
    QString title;
    QString text;
    enum Errors {small, medium,huge, critical} errorType;

    UpdateError(QString title_,QString text_,Errors errorType_):title(title_),text(text_), errorType(errorType_)
        {}
    UpdateError(QString title_,QString text_):title(title_),text(text_), errorType(medium)
        {}
    UpdateError(): errorType(medium)
        {}
};

Q_DECLARE_METATYPE(UpdateError)

#endif // UPDATEERROR_H
