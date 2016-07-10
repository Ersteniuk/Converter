#ifndef CONVERTERGUI_H
#define CONVERTERGUI_H
#include <qwidget.h>
#include "updateerror.h"
#include "errormessanger.h"
class UpdateManager;
class MainWindow;
class SplashScreen;
class TrayIcon;
class BankDialog;
class BankSettings;

class QEventLoop;

class ConverterGui:public QWidget, private ErrorMessanger
{
    Q_OBJECT
private:
    const QString programName;
    const QString banksFile;
    MainWindow* window;
    BankDialog* dialog;
    BankSettings* settings;

    QEventLoop* loop;
    UpdateManager* manager;

    UpdateManager* addValutes();
    void makeConnection(UpdateManager* manager, SplashScreen* screen, TrayIcon* tray);

    QStringList banks();

public:

    explicit ConverterGui(const QString& bankFile);
public slots:
    void createMainWindow();
    void endCreating();
    void destroyMainWindow()
        { window=nullptr; }
    void errorHandle(UpdateError e)
        { check(e); }
    void settingWindow();
    void newBanks(const QStringList& sl,bool shouldBeUpdated);
    void destroyDialog()
        { dialog=nullptr; }
    void find(int);
signals:
    void banksMayChange();
    void updateSignal();
};

#endif // CONVERTERGUI_H
