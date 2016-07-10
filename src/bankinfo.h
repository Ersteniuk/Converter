#ifndef BANKINFO_H
#define BANKINFO_H
#include "valutetype.h"
#include <QFile>
#include <QTextStream>
#include <QUrl>
class  AbstractExtractor;

struct BankInfo
{
public:
    enum ExtractorType { unknown=-1, regex, privat };
private:
    QString name;
    QUrl site;
    QString file;
    ExtractorType extractor;
public:
    BankInfo():extractor(unknown)
        {}
    BankInfo(QString vName,QUrl vSite, QString vFile, ExtractorType vExtractor):name(vName),site(vSite),
        file(vFile),extractor(vExtractor)
            {}
    ExtractorType toExtractorType(const QString &s) const;
    AbstractExtractor* useExtractor(ValuteType base) const;
    AbstractExtractor* operator()(ValuteType base) const
        { return useExtractor(base); }
    QUrl siteUrl() const
        { return site; }
    QString bankName() const
        { return name; }

    QTextStream& read(QFile& f);
    QTextStream& read(QTextStream& stream);
};

inline QTextStream& operator>>(QTextStream& is, BankInfo& info)
{
    return info.read(is);
}

#endif // BANKINFO_H
