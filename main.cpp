#include "mainwindow.h"
#include "dlgsettings.h"
#include "global_vars.h"

#include <QApplication>
#include <QSettings>
#include <QProcessEnvironment>

QString getUsername()
{
    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    QString username = env.value("USERNAME", "Unknown User");
    return username;
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QSettings settings("config.ini", QSettings::IniFormat);
    QString strIp = settings.value(getUsername() + "/Server").toString();

    if(strIp == "")
    {
        DlgSettings dlg;

        if(dlg.exec() == QDialog::Rejected)
            return 0;
    }

    MainWindow::getInstance()->show();

    return a.exec();
}
