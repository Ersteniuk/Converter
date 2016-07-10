#include "bankinfo.h"
#include "errormanagement.h"
#include "extractor.h"
#include "defening.h"

BankInfo::ExtractorType BankInfo::toExtractorType(const QString& s) const
{
    const QString strRegex="regex",strPrivat="privat";
    if (strRegex==s)
        return regex;
    if (strPrivat==s)
        return privat;
    return unknown;
}

AbstractExtractor* BankInfo::useExtractor(ValuteType base) const
{
    switch (extractor)
    {
    case regex:
        return new RegexExtractor(file,base);
    case privat:
        return new PrivatApiExtractor(file,base);
    default:
        return nullptr;
    }
}

/*namespace {
    bool checkStatus(QTextStream& is,bool atEndError,const QString& title)
    {
        switch (is.status()) {
        case QTextStream::Ok:
            return true;
        case QTextStream::ReadPastEnd:
            if (atEndError)
                QMessageBox::warning(0,title,"Unexpected end of file.");
            return false;\
        case QTextStream::ReadCorruptData:
            QMessageBox::warning(0,title,"The data is corrupted.");
            return false;
        case QTextStream::WriteFailed:
            QMessageBox::warning(0,title,"Can't write the data.");
            return false;
        default:
            QMessageBox::warning(0,title,"Unknown state.");
            return false;
        }
    }
}*/

QTextStream& BankInfo::read(QTextStream &is)
{
    const QString errorTitle="General Bank information";
    const QChar beginPart='{', endPart='}';
    const QString extention=".ext";
    QChar ch=0;
    QString line,bank,site,file;
    ExtractorType ex;
    qint64 text_pos;
    ErrorManagement::StreamChecker sCheck(is,errorTitle,"Unexpected character in the file",UpdateError::critical);
    is.skipWhiteSpace();
    text_pos = is.pos();
    is>>ch;
    if (is.atEnd())
        return is;
    sCheck.check();
    //if (!checkStatus(is,false,errorTitle))
    //    return is;
    if (beginPart!=ch)
    {
        is.seek(text_pos);
        is.setStatus(QTextStream::ReadCorruptData);
        sCheck.error();
        //QMessageBox::warning(0,errorTitle,);
        //return is;
    }
    is.skipWhiteSpace();
    bank=is.readLine();
    line=is.readLine();
    sCheck.check();
    //if (!checkStatus(is,true,errorTitle))
    //    return is;
    ex=toExtractorType(line);
    site=is.readLine();
    sCheck.check();
    //if (!checkStatus(is,true,errorTitle))
    //    return is;
    QUrl siteUrl(site);
    text_pos = is.pos();
    is>>ch;
    if (endPart!=ch)
    {
        is.seek(text_pos);
        file=is.readLine();
        file.prepend(addressExe());
        is>>ch;
    }
    else
        file=addressExe()+bank+extention;
//#ifdef Q_OS_WIN
//    if (!file.startsWith('\\'))
//        file.prepend('\\');
//#endif
    if (endPart!=ch || !siteUrl.isValid())
    {
        is.setStatus(QTextStream::ReadCorruptData);
        sCheck.error();
        //QMessageBox::warning(0,errorTitle,"Unexpected character in the file");
        //return is;
    }
    *this=BankInfo(bank,siteUrl,file,ex);
    return is;
}
