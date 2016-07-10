#ifndef ERRORMANAGEMENT_H
#define ERRORMANAGEMENT_H
#include "QString"
#include "QFile"
#include "QTextStream"
#include "updateerror.h"
class QNetworkReply;
class QNetworkAccessManager;

class ErrorManagement
{
public:
    static void error(const QString& title,const QString& text,UpdateError::Errors errorType=UpdateError::medium)
    {
        throw UpdateError(title,text,errorType);
    }
    static void errorInFile(const QFile& file,const QString& text,UpdateError::Errors errorType=UpdateError::medium)
    {
        error(file.fileName(),text,errorType);
    }
    static void errorInNetworkReply(const QNetworkReply* r,const QString& text,
                                    UpdateError::Errors errorType=UpdateError::medium);
    static void checkFile(const QFile& testFile,bool atEndError,UpdateError::Errors errorType=UpdateError::medium);
    static void checkStream(const QTextStream& is,const QString& title ,bool atEndError,
                            UpdateError::Errors errorType=UpdateError::medium);
    static void checkNetworkReply(const QNetworkReply* r,const QString& title,
                            UpdateError::Errors errorType=UpdateError::medium);
    static void checkNetworkReply(const QNetworkReply* r,
                            UpdateError::Errors errorType=UpdateError::medium);
    static void checkNetworkAccess(const QNetworkAccessManager* manager,const QString& title,
                                   UpdateError::Errors errorType=UpdateError::medium );

    class Checker
    {
    public:
        typedef UpdateError::Errors Errors;
    protected:
       QString defaultErrorText;
       QString defaultErrorTitle;
       Errors errorType;
    public:
       Checker(const QString& defaultText="",const QString& defaultTitle="",Errors type=UpdateError::medium):
           defaultErrorText(defaultText), defaultErrorTitle(defaultTitle), errorType(type)
                {}
       Checker(QStringRef defaultText,QStringRef defaultTitle,Errors type=UpdateError::medium):
            defaultErrorText(defaultText.toString()), defaultErrorTitle(defaultTitle.toString()), errorType(type)
                {}
       Checker(QStringRef defaultText,const QString& defaultTitle="",Errors type=UpdateError::medium):
            defaultErrorText(defaultText.toString()), defaultErrorTitle(defaultTitle), errorType(type)
                {}
       Checker(const QString& defaultText,QStringRef defaultTitle,Errors type=UpdateError::medium):
            defaultErrorText(defaultText), defaultErrorTitle(defaultTitle.toString()), errorType(type)
                {}
       Checker(QStringRef defaultText,Errors type):
            defaultErrorText(defaultText.toString()), errorType(type)
                {}
       Checker(const QString& defaultText, Errors type):
            defaultErrorText(defaultText), errorType(type)
                {}
       Checker(Errors type=UpdateError::medium): errorType(type)
            {}
       virtual ~Checker()
            {}
       virtual void error(const QString& errorTitle, const QString& errorText,Errors type)
            { ErrorManagement::error(errorTitle,errorText,type); }
       virtual void error(const QString& errorTitle, const QString& errorText)
            { ErrorManagement::error(errorTitle,errorText,errorType); }
       virtual void error(const QString& errorText, Errors type)
            { ErrorManagement::error(defaultErrorTitle,errorText,type); }
       virtual void error(const QString& errorText)
            { ErrorManagement::error(defaultErrorTitle,errorText,errorType); }
       virtual void error(Errors type)
            { ErrorManagement::error(defaultErrorTitle,defaultErrorText,type); }
       virtual void error()
            { ErrorManagement::error(defaultErrorTitle,defaultErrorText,errorType); }

       virtual void check(Errors type)
            { check(true,type); }
       virtual void check(bool atEndError=true)
            { check(atEndError,UpdateError::medium); }
       virtual void check(bool atEndError, Errors type)
                   {}
    };

    class FileChecker:public Checker
    {
        const QFile& file;
    public:
        FileChecker(const QFile& f,const QString& defaultText="",Errors type=UpdateError::medium):
            Checker(defaultText,type),file(f)
                {}
        FileChecker(const QFile& f,Errors type):
            Checker("",type),file(f)
                {}
        FileChecker(const QFile& f,QStringRef defaultText,Errors type=UpdateError::medium):
            Checker(defaultText,type),file(f)
            {}
        void check(Errors type)
             { check(true,type); }
        void check(bool atEndError=true)
             { check(atEndError,UpdateError::medium); }
        void check(bool atEndError, Errors type)
            { ErrorManagement::checkFile(file,atEndError,type); }
        void error(const QString& errorText,Errors type)
            { ErrorManagement::errorInFile(file,errorText,type); }
        void error(const QString& errorText)
            { ErrorManagement::errorInFile(file,errorText,errorType); }
        void error(Errors type)
            { ErrorManagement::errorInFile(file,defaultErrorText,type); }
        void error()
            { ErrorManagement::errorInFile(file,defaultErrorText,errorType); }
    };

    class StreamChecker:public Checker
    {
        const QTextStream& stream;
    public:
        StreamChecker(const QTextStream& s,const QString& streamName,const QString& defaultText="",
                      Errors type=UpdateError::medium):Checker(defaultText,streamName,type),stream(s)
                {}
        StreamChecker(const QTextStream& s,const QStringRef& streamName,const QString& defaultText="",
                      Errors type=UpdateError::medium):Checker(defaultText,streamName,type),stream(s)
                {}
        StreamChecker(const QTextStream& s,const QString& streamName,QStringRef defaultText,
                      Errors type=UpdateError::medium):Checker(defaultText,streamName,type),stream(s)
                {}
        StreamChecker(const QTextStream& s,const QStringRef& streamName,QStringRef defaultText,
                      Errors type=UpdateError::medium):Checker(defaultText,streamName,type),stream(s)
                {}
        StreamChecker(const QTextStream& s,const QString& streamName,
                      Errors type=UpdateError::medium):Checker("",streamName,type),stream(s)
                {}
        StreamChecker(const QTextStream& s,const QStringRef& streamName,
                      Errors type=UpdateError::medium):Checker("",streamName,type),stream(s)
                {}
        void check(Errors type)
             { check(true,type); }
        void check(bool atEndError=true)
             { check(atEndError,UpdateError::medium); }
        void check(bool atEndError, Errors type)
            { ErrorManagement::checkStream(stream, defaultErrorTitle,atEndError,type); }
    };
};

#endif // ERRORMANAGEMENT_H
