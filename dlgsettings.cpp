#include "dlgsettings.h"
#include "ui_dlgsettings.h"
#include <QSettings>
#include "mainwindow.h"
#include <QMessageBox>
#include "global_vars.h"
#include <QNetworkInterface>
#include <QProcessEnvironment>

DlgSettings::DlgSettings(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::DlgSettings)
{
    ui->setupUi(this);

    QSettings settings("config.ini", QSettings::IniFormat);

    QString strKey = getUsername();
    QString strIp = settings.value(strKey + "/Server").toString();
    QString strName = settings.value(strKey + "/Name").toString();
    QString strDepart = settings.value(strKey + "/Depart").toString();

    if(strIp == "") {
        QList<QNetworkInterface> interfaces = QNetworkInterface::allInterfaces();
        for (const QNetworkInterface &interface : interfaces) {
            if (interface.flags() & QNetworkInterface::IsUp && !(interface.flags() & QNetworkInterface::IsLoopBack)) {
                QList<QNetworkAddressEntry> entries = interface.addressEntries();
                for (const QNetworkAddressEntry &entry : entries) {
                    if (entry.ip().toIPv4Address()) {
                        strIp = entry.ip().toString();
                        break;
                    }
                }
            }
        }
    }

    if(strName == "") {
        QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
        strName = env.value("USERNAME", "Unknown User");
    }

    ui->edtServerIp->setText(strIp);
    ui->edtUser->setText(strName);
    ui->edtDepart->setText(strDepart);
}

DlgSettings::~DlgSettings()
{
    delete ui;
}

QString DlgSettings::getUsername()
{
    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    QString username = env.value("USERNAME", "Unknown User");
    return username;
}

void DlgSettings::on_btnSave_clicked()
{
    QString strIp = ui->edtServerIp->text();
    if(strIp == "") {
        QMessageBox::warning(this, APP_NAME, "Please input valid IP address");
        return;
    }

    QString strName = ui->edtUser->text();
    if(strName == "") {
        QMessageBox::warning(this, APP_NAME, "Please input valid username");
        return;
    }

    QString strDepart = ui->edtDepart->text();
    if(strDepart == "") {
        QMessageBox::warning(this, APP_NAME, "Please input valid department");
        return;
    }

    QSettings settings("config.ini", QSettings::IniFormat);
    QString strKey = getUsername();

    settings.setValue(strKey + "/Server", strIp);
    settings.setValue(strKey + "/Name", strName);
    settings.setValue(strKey + "/Depart", strDepart);
    accept();
}

void DlgSettings::on_btnClose_clicked()
{
    reject();
}
