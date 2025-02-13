#ifndef CUSTOMLABEL_H
#define CUSTOMLABEL_H

#include <QLabel>
#include <QWidget>
#include <QMouseEvent>

class CustomLabel : public QLabel
{
    Q_OBJECT
public:
    explicit CustomLabel(QWidget *parent = nullptr);

    void mouseDoubleClickEvent(QMouseEvent *event);

signals:
    void clicked();
};

#endif // CUSTOMLABEL_H
