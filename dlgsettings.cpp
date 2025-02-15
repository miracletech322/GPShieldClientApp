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

    QSettings settings(APP_REG_COMPANY, APP_REG_NAME);

    QString strIp = settings.value(REG_KEY_IPADDRESS).toString();
    QString strName = settings.value(REG_KEY_USERNAME).toString();
    QString strDepart = settings.value(REG_KEY_DEPART).toString();

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
        settings.setValue(REG_KEY_IPADDRESS, strIp);
    }

    if(strName == "") {
        QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
        strName = env.value("USERNAME", "Unknown User");
        settings.setValue(REG_KEY_USERNAME, strName);
    }

    if(strDepart == "") {
        ui->edtDepart->setText(strDepart);
    }

    ui->edtServerIp->setText(strIp);
    ui->edtUser->setText(strName);
}

DlgSettings::~DlgSettings()
{
    delete ui;
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

    QSettings settings(APP_REG_COMPANY, APP_REG_NAME);
    settings.setValue(REG_KEY_IPADDRESS, strIp);
    settings.setValue(REG_KEY_USERNAME, strName);
    settings.setValue(REG_KEY_DEPART, strDepart);
    accept();
}

void DlgSettings::on_btnClose_clicked()
{
    reject();
}
