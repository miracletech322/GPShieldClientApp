#include "mainwindow.h"
#include "dlgsettings.h"
#include "global_vars.h"

#include <QApplication>
#include <QSettings>
#include <QProcessEnvironment>
#include <QStandardPaths>

QString getUsername()
{
    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    QString username = env.value("USERNAME", "Unknown User");
    return username;
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QString iniPath = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + "/config.ini";
    QSettings settings(iniPath, QSettings::IniFormat);
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
