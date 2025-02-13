#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSystemTrayIcon>
#include <QMessageBox>
#include <QTcpSocket>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    static MainWindow* getInstance();

    void initTrayIcon();
    void moveToBottomRight();

    QString getUsername();
    QString getIpAddress();
    void initSocket();
    void receivedAlert(QString strIp, QString strName);

public slots:
    void slt_close();
    void slt_openSettings();
    void slt_readyRead();

    void on_lblLogo_clicked();

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void closeEvent(QCloseEvent *event);

private:
    Ui::MainWindow *ui;

    bool m_dragging = false;
    bool m_bStatus;
    QPoint m_dragPosition;

    QSystemTrayIcon *m_pTrayIcon;
    QTcpSocket* m_pSocket;
};
#endif // MAINWINDOW_H
