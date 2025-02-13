#include "mainwindow.h"
#include "dlgsettings.h"
#include "./ui_mainwindow.h"
#include "global_vars.h"
#include <QSettings>
#include <QProcessEnvironment>
#include <QMenu>
#include <QNetworkInterface>
#include "dlgalert.h"
#include <QTimer>
static MainWindow* instance = nullptr;

MainWindow *MainWindow::getInstance()
{
    if(instance == nullptr)
    {
        instance = new MainWindow(0);
    }

    return instance;
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setWindowFlags(Qt::Window | Qt::CustomizeWindowHint | Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);

    moveToBottomRight();

    initTrayIcon();
    initSocket();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        m_dragging = true;
        m_dragPosition = event->globalPosition().toPoint() - frameGeometry().topLeft();
        event->accept();
    }
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    if (m_dragging)
    {
        move(event->globalPosition().toPoint() - m_dragPosition);
        event->accept();
    }
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        m_dragging = false;
        event->accept();
    }
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if(QMessageBox::question(this, APP_NAME, "Do you want to exit application?") == QMessageBox::Yes)
    {
        event->accept();
    }
    else
    {
        event->ignore();
    }
}

void MainWindow::moveToBottomRight()
{
    QScreen *screen = QApplication::primaryScreen();
    QRect availableGeometry = screen->availableGeometry();

    int x = availableGeometry.right() - width();
    int y = availableGeometry.bottom() - height();

    move(x, y);
}

QString MainWindow::getUsername()
{
    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    QString username = env.value("USERNAME", "Unknown User");
    return username;
}

QString MainWindow::getIpAddress()
{
    QList<QNetworkInterface> interfaces = QNetworkInterface::allInterfaces();
    for (const QNetworkInterface &interface : interfaces) {
        if (interface.flags() & QNetworkInterface::IsUp && !(interface.flags() & QNetworkInterface::IsLoopBack)) {
            QList<QNetworkAddressEntry> entries = interface.addressEntries();
            for (const QNetworkAddressEntry &entry : entries) {
                if (entry.ip().toIPv4Address()) {
                    return entry.ip().toString();
                }
            }
        }
    }
    return "";
}

void MainWindow::initSocket()
{
    QSettings settings(APP_REG_COMPANY, APP_REG_NAME);
    QString strIp = settings.value(REG_KEY_IPADDRESS).toString();
    QString strName = settings.value(REG_KEY_USERNAME).toString();

    m_pSocket = new QTcpSocket(this);
    bool b = connect(m_pSocket, SIGNAL(readyRead()), this, SLOT(slt_readyRead()));

    m_pSocket->connectToHost(strIp, 3022);

    if (!m_pSocket->waitForConnected(5000)) {
        QMessageBox::critical(this, APP_NAME, "Unable to connect to the server.");

        QTimer::singleShot(1000, [](){
            exit(EXIT_SUCCESS);
        });
    } else {
        QByteArray block;
        QDataStream out(&block, QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_6_5);

        int nHeader = RequestHeader::RH_SET_NAME;

        out << nHeader;
        out << strIp;
        out << strName;

        m_pSocket->write(block);
    }
}

void MainWindow::receivedAlert(QString strIp, QString strName)
{
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_6_5);

    int nHeader = RequestHeader::RH_RECEIVED_ALERT;

    out << nHeader;
    out << strIp;
    out << strName;

    m_pSocket->write(block);
}

void MainWindow::slt_close()
{
    close();
}

void MainWindow::slt_openSettings()
{
    DlgSettings *pDlg = new DlgSettings(this);
    pDlg->show();
}

void MainWindow::slt_readyRead()
{
    QDataStream in;
    in.setDevice(m_pSocket);
    in.setVersion(QDataStream::Qt_6_5);
    in.startTransaction();

    int nHeader;
    in >> nHeader;

    if(nHeader == RequestHeader::RH_SEND_ALERT)
    {
        QString strIp;
        QString strName;

        in >> strIp;
        in >> strName;

        DlgAlert *pDialog = new DlgAlert(strIp, strName, this);
        pDialog->showFullScreen();
    }

    if(nHeader == RequestHeader::RH_RECEIVED_ALERT)
    {
        QString strIp;
        QString strName;

        in >> strIp;
        in >> strName;

        if(m_bStatus) {
            QMessageBox::information(this, APP_NAME, tr("Received: %1(%2)").arg(strName).arg(strIp));
        }
        m_bStatus = false;
        ui->centralwidget->setStyleSheet("QLabel#lblLogo { border-image: url(:/Resource/assets/logo.png); }");
    }

    in.commitTransaction();
}

void MainWindow::on_lblLogo_clicked()
{
    QSettings settings(APP_REG_COMPANY, APP_REG_NAME);
    QString strIp = settings.value(REG_KEY_IPADDRESS).toString();
    QString strName = settings.value(REG_KEY_USERNAME).toString();

    ui->centralwidget->setStyleSheet("QLabel#lblLogo { border-image: url(:/Resource/assets/logo-red.png); }");

    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_6_5);

    int nReqHeader = RequestHeader::RH_SEND_ALERT;

    out << nReqHeader;
    out << strIp;
    out << strName;

    m_pSocket->write(block);

    m_bStatus = true;
}

void MainWindow::initTrayIcon()
{
    QIcon icon(":/Resource/assets/logo.png");

    m_pTrayIcon = new QSystemTrayIcon();
    m_pTrayIcon->setIcon(icon);

    QMenu *pTrayMenu = new QMenu();

    QAction *pSettingsAction = new QAction("Settings", pTrayMenu);
    connect(pSettingsAction, SIGNAL(triggered()), this, SLOT(slt_openSettings()));
    QAction *pExitAction = new QAction("Exit", pTrayMenu);
    connect(pExitAction, SIGNAL(triggered()), this, SLOT(slt_close()));

    pTrayMenu->addAction(pSettingsAction);
    pTrayMenu->addSeparator();
    pTrayMenu->addAction(pExitAction);
    m_pTrayIcon->setContextMenu(pTrayMenu);
    m_pTrayIcon->show();
}
