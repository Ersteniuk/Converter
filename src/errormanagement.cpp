#include "errormanagement.h"
#include "qnetworkreply.h"
#include "qnetworkaccessmanager.h"
#include <QDebug>

void ErrorManagement::errorInNetworkReply(const QNetworkReply* r,const QString& text,
                                    UpdateError::Errors errorType)
{
    error(r->url().host(),text,errorType);
}

void ErrorManagement::checkFile(const QFile& testFile, bool atEndError, UpdateError::Errors errorType)
{
    if (testFile.atEnd() && atEndError)
            errorInFile(testFile,"Unexpected end of file.");
    switch (testFile.error())
    {
    case QFileDevice::NoError:
        break;
    case QFileDevice::ReadError:
        errorInFile(testFile,"An error occurred when reading from the file.",errorType);
    case QFileDevice::WriteError:
        errorInFile(testFile,"An error occurred when writing to the file.",errorType);
    case QFileDevice::FatalError:
        errorInFile(testFile,"A fatal error occurred.",errorType);
    case QFileDevice::ResourceError:
        errorInFile(testFile,"Out of resources (e.g., too many open files, out of memory, etc.)",errorType);
    case QFileDevice::OpenError:
        errorInFile(testFile,"The file could not be opened.",errorType);
    case QFileDevice::AbortError:
        errorInFile(testFile,"The operation was aborted.",errorType);
    case QFileDevice::TimeOutError:
        errorInFile(testFile,"A timeout occurred.",errorType);
    case QFileDevice::UnspecifiedError:
        errorInFile(testFile,"An unspecified error occurred.",errorType);
    case QFileDevice::RemoveError:
        errorInFile(testFile,"The file could not be removed.",errorType);
    case QFileDevice::RenameError:
        errorInFile(testFile,"The file could not be renamed.",errorType);
    case QFileDevice::PositionError:
        errorInFile(testFile,"The position in the file could not be changed.",errorType);
    case QFileDevice::ResizeError:
        errorInFile(testFile,"The file could not be resized.",errorType);
    case QFileDevice::PermissionsError	:
        errorInFile(testFile,"The file could not be accessed.",errorType);
    case QFileDevice::CopyError:
        errorInFile(testFile,"The file could not be copied.",errorType);
    default:
        errorInFile(testFile,"An unknown error occured.",errorType);
    }
}

void ErrorManagement::checkStream(const QTextStream& is,const QString& title ,bool atEndError,
                                  UpdateError::Errors errorType)
{
    switch (is.status())
    {
    case QTextStream::Ok:
        break;
    case QTextStream::ReadPastEnd:
        if (atEndError)
            error(title,"Unexpected end of file.",errorType);
        break;
    case QTextStream::ReadCorruptData:
        error(title,"The data is corrupted.",errorType);
    case QTextStream::WriteFailed:
        error(title,"Can't write the data.",errorType);
    default:
        error(title,"Unknown state.",errorType);
    }
}

void ErrorManagement::checkNetworkReply(const QNetworkReply *r, const QString& title, UpdateError::Errors errorType)
{
    const QString userFault="Будь-ласка, перевірте справність підключення до інтернету і налаштування фаєрволу.";
    const QString proxyFault="Будь-ласка, перевірте налаштування проксі.";
    const QString myFault="Для вирішення цієї проблеми зверніться до постачальник ПО.";
    const QString maibeMyFault="Якщо причина помилки після цього не встановлена зверніться до постачальника ПО.";
    switch (r->error())
    {
    case QNetworkReply::NoError:
        break;
    case QNetworkReply::ConnectionRefusedError:
        error(title,"the remote server refused the connection (the server is not accepting requests)"+QString("\n")+
              userFault+"\n"+proxyFault+"\n"+maibeMyFault,errorType);
    case QNetworkReply::RemoteHostClosedError:
        error(title,"the remote server closed the connection prematurely, before the entire reply was received"
              " and processed"+QString("\n")+userFault+"\n"+proxyFault,errorType);
    case QNetworkReply::HostNotFoundError:
        error(title,"the remote host name was not found (invalid hostname)"+QString("\n")+
              userFault+"\n"+proxyFault+"\n"+maibeMyFault,errorType);
    case QNetworkReply::TimeoutError:
        error(title,"the connection to the remote server timed out"+QString("\n")+userFault,errorType);
    case QNetworkReply::OperationCanceledError:
        error(title,"the operation was canceled via calls to some functions before it was finished."+
              QString("\n")+myFault,errorType);
    case QNetworkReply::SslHandshakeFailedError:
        error(title,"the SSL/TLS handshake failed and the encrypted channel could not be established."+
              QString("\n")+myFault,errorType);
    case QNetworkReply::TemporaryNetworkFailureError:
        error(title,"the connection was broken due to disconnection from the network, however the system has "
              "initiated roaming to another access point. The request should be resubmitted and will be "
              "processed as soon as the connection is re-established."
              "Будь-ласка, зачейка декілька хвилин",errorType);
    case QNetworkReply::NetworkSessionFailedError:
        error(title,"the connection was broken due to disconnection from the network or failure to start the network."+
              QString("\n")+userFault+"\n"+proxyFault,errorType);
    case QNetworkReply::ProxyConnectionRefusedError:
        error(title,"the connection to the proxy server was refused (the proxy server is not accepting requests)"+
              QString("\n")+userFault+"\n"+proxyFault,errorType);
    case QNetworkReply::ProxyConnectionClosedError:
        error(title,"the proxy server closed the connection prematurely, before the entire reply was received "
              "and processed"+QString("\n")+userFault+"\n"+proxyFault,errorType);
    case QNetworkReply::ProxyNotFoundError:
        error(title,"the proxy host name was not found (invalid proxy hostname)"+QString("\n")+
              userFault+"\n"+proxyFault,errorType);
    case QNetworkReply::ProxyTimeoutError:
        error(title,"the connection to the proxy timed out or the proxy did not reply in time to the request sent"+
              QString("\n")+userFault+"\n"+proxyFault,errorType);
    case QNetworkReply::ProxyAuthenticationRequiredError:
        error(title,"the proxy requires authentication in order to honour the request but did not accept any "
              "credentials offered (if any)"+QString("\n")+userFault+"\n"+proxyFault,errorType);
    case QNetworkReply::ContentAccessDenied:
        error(title,"the access to the remote content was denied (HTTP error 401)"+QString("\n")+
              userFault+"\n"+maibeMyFault,errorType);
    case QNetworkReply::ContentOperationNotPermittedError:
        error(title,"the operation requested on the remote content is not permitted"+QString("\n")+
              userFault+"\n"+maibeMyFault,errorType);
    case QNetworkReply::ContentNotFoundError:
        error(title,"the remote content was not found at the server (HTTP error 404)"+QString("\n")+
              userFault+"\n"+maibeMyFault,errorType);
    case QNetworkReply::AuthenticationRequiredError:
        error(title,"the remote server requires authentication to serve the content but the "
              "credentials provided were not accepted (if any)"+QString("\n")+myFault,errorType);
    case QNetworkReply::ContentReSendError:
        error(title,"the request needed to be sent again, but this failed for example because the upload data "
              "could not be read a second time"+QString("\n")+userFault,errorType);
    case QNetworkReply::ProtocolUnknownError:
        error(title,"the Network Access API cannot honor the request because the protocol is not known"+QString("\n")+
              userFault+"\n"+maibeMyFault,errorType);
    case QNetworkReply::ProtocolInvalidOperationError:
        error(title,"the requested operation is invalid for this protocol"+QString("\n")+
              userFault+"\n"+maibeMyFault,errorType);
    case QNetworkReply::UnknownNetworkError:
        error(title,"an unknown network-related error was detected"+QString("\n")+
              userFault+"\n"+proxyFault,errorType);
    case QNetworkReply::UnknownProxyError:
        error(title,"an unknown proxy-related error was detected"+QString("\n")+proxyFault,errorType);
    case QNetworkReply::UnknownContentError:
        error(title,"an unknown error related to the remote content was detected"+QString("\n")+
              userFault+"\n"+maibeMyFault,errorType);
    case QNetworkReply::ProtocolFailure:
        error(title,"a breakdown in protocol was detected (parsing error, invalid "
              "or unexpected responses, etc.)"+QString("\n")+userFault+"\n"+maibeMyFault,errorType);
    case QNetworkReply::BackgroundRequestNotAllowedError:
        error(title,"background network requests are not allowed according to the current "
              "policy of the bearer plugin.)."+QString("\n")+userFault,errorType);
    default:
        error(title,"unknown error)"+QString("\n")+userFault+"\n"+proxyFault+"\n"+maibeMyFault,errorType);

    }
}

void ErrorManagement::checkNetworkReply(const QNetworkReply* r, UpdateError::Errors errorType)
{
    checkNetworkReply(r,r->url().host(),errorType);
}

void ErrorManagement::checkNetworkAccess(const QNetworkAccessManager* manager,const QString& title,
                                   UpdateError::Errors errorType)
{
    const QString userFault="Будь-ласка, перевірте справність підключення до інтернету і налаштування фаєрволу.";
    const QString maibeMyFault="Якщо причина помилки після цього не встановлена зверніться до постачальника ПО.";
    switch (manager->networkAccessible()) {
    case QNetworkAccessManager::Accessible:
        break;
    case QNetworkAccessManager::UnknownAccessibility:
        error(title,"The network accessibility cannot be determined."+QString("\n")+userFault,errorType);
    case QNetworkAccessManager::NotAccessible:
        error(title,"The network is not currently accessible."+QString("\n")+userFault,errorType);
    default:
        error(title,"The network accessibility cannot be determined."+QString("\n")+userFault+
              "\n"+maibeMyFault,errorType);
    }
}
