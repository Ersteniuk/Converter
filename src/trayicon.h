#ifndef TRAYICON_H
#define TRAYICON_H

#include <QSystemTrayIcon>
#include <QEvent>
class QMenu;

class TrayIcon : public QSystemTrayIcon
{
    Q_OBJECT
private:
    int updated;
    QMenu* pMenu;

    using QSystemTrayIcon::setContextMenu;
public:
    TrayIcon(const QString& trayIcon,const QString& programName, QObject *parent = 0);
    ~TrayIcon();

signals:
    void updateSignal();
    void mainWindowSignal();
    void settingsSignal();
private slots:
    void activatedSlot(QSystemTrayIcon::ActivationReason reason)
    {
        if (DoubleClick==reason)
            emit mainWindowSignal();
    }

public slots:
    void showMessage();
    void zeroedUpdated()
        { updated=0; }
};

#endif // TRAYICON_H
