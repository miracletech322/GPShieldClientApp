#include "mainwindow.h"
#include "dlgsettings.h"
#include "global_vars.h"

#include <QApplication>
#include <QSettings>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QSettings settings(APP_REG_COMPANY, APP_REG_NAME);
    QString strIp = settings.value(REG_KEY_IPADDRESS).toString();

    if(strIp == "")
    {
        DlgSettings dlg;

        if(dlg.exec() == QDialog::Rejected)
            return 0;
    }

    MainWindow::getInstance()->show();

    return a.exec();
}
