#include "dlgsettings.h"
#include "ui_dlgsettings.h"
#include <QSettings>
#include "mainwindow.h"
#include <QMessageBox>
#include "global_vars.h"

DlgSettings::DlgSettings(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::DlgSettings)
{
    ui->setupUi(this);

    QSettings settings(APP_REG_COMPANY, APP_REG_NAME);

    QString strIp = settings.value(REG_KEY_IPADDRESS).toString();
    QString strName = settings.value(REG_KEY_USERNAME).toString();

    if(strIp == "") {
        strIp = MainWindow::getInstance()->getIpAddress();
        settings.setValue(REG_KEY_IPADDRESS, strIp);
    }

    if(strName == "") {
        strName = MainWindow::getInstance()->getUsername();
        settings.setValue(REG_KEY_USERNAME, strName);
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

    QSettings settings(APP_REG_COMPANY, APP_REG_NAME);
    settings.setValue(REG_KEY_IPADDRESS, strIp);
    settings.setValue(REG_KEY_USERNAME, strName);
    accept();
}

void DlgSettings::on_btnClose_clicked()
{
    reject();
}
