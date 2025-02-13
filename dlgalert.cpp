#include "dlgalert.h"
#include "ui_dlgalert.h"
#include "mainwindow.h"
#include <QTimer>

DlgAlert::DlgAlert(QString strIp, QString strName, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::DlgAlert)
{
    ui->setupUi(this);

    m_bSendBack = false;
    ui->lblUser->setText(strName);
    m_strIp = strIp;
    m_strName = strName;

    QScreen *screen = QApplication::primaryScreen();
    if (screen)
    {
        QRect screenGeometry = screen->geometry();
        setGeometry(screenGeometry);
    }

    sound = new QSoundEffect(this);  
    sound->setSource(QUrl::fromLocalFile(QCoreApplication::applicationDirPath() + "/assets/alarm-sound.wav"));
    sound->setLoopCount(QSoundEffect::Infinite);
    sound->play();
}

DlgAlert::~DlgAlert()
{
    delete ui;

    sound->stop();
    sound = nullptr;
    delete sound;
}

void DlgAlert::on_btnAcknowledge_clicked()
{
    close();
}

void DlgAlert::on_btnSend_clicked()
{
    m_bSendBack = true;
    close();
}

void DlgAlert::closeEvent(QCloseEvent *e)
{
    MainWindow::getInstance()->receivedAlert(m_strIp, m_strName);

    if(m_bSendBack) {
        QTimer::singleShot(500, [](){
            MainWindow::getInstance()->on_lblLogo_clicked();
        });
    }

    sound->stop();
    sound = nullptr;
    delete sound;

    e->accept();
}
