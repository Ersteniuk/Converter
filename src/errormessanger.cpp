#include "errormessanger.h"
#include <qmessagebox.h>
#include <QDebug>
#include <QApplication>

void ErrorMessanger::check(const UpdateError& e)
{
    preCheckCalculation(e);
    switch (e.errorType)
    {
    case UpdateError::small:
        smallError(e);
        break;
    case UpdateError::medium:
        mediumError(e);
        break;
    case UpdateError::huge:
        hugeError(e);
        break;
    case UpdateError::critical:
        criticalError(e);
        break;
    default:
        unknownErrorType(e);
    }
    postCheckCalculation(e);
}

void ErrorMessanger::smallError(const UpdateError& e)
{
    qDebug()<<e.title<<':'<<e.text<<endl;
}

void ErrorMessanger::mediumError(const UpdateError& e)
{
    QMessageBox* msg=new QMessageBox(QMessageBox::Warning,e.title,e.text);
    msg->setModal(false);
    msg->setAttribute(Qt::WA_DeleteOnClose);
    msg->show();
}

void ErrorMessanger::hugeError(const UpdateError& e)
{
    QMessageBox::warning(0,e.title,e.text);
}

void ErrorMessanger::criticalError(const UpdateError& e)
{
    QMessageBox::critical(0,e.title,e.text);
    throw StopKran(); //to close the application
}
