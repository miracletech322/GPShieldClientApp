#ifndef DLGSETTINGS_H
#define DLGSETTINGS_H

#include <QDialog>

namespace Ui {
class DlgSettings;
}

class DlgSettings : public QDialog
{
    Q_OBJECT

public:
    explicit DlgSettings(QWidget *parent = nullptr);
    ~DlgSettings();

public slots:
    void on_btnSave_clicked();
    void on_btnClose_clicked();

private:
    Ui::DlgSettings *ui;
};

#endif // DLGSETTINGS_H
