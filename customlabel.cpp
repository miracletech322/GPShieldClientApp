#include "customlabel.h"

CustomLabel::CustomLabel(QWidget *parent)
    : QLabel{parent}
{}

void CustomLabel::mouseDoubleClickEvent(QMouseEvent *event)
{
    emit clicked();
    event->accept();
}
