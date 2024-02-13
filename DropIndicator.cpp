// DropIndicator.cpp
#include "DropIndicator.h"
#include <QPainter>

DropIndicator::DropIndicator(QWidget *parent) : QWidget(parent) {
    // Optionally set a fixed size for the indicator
    setFixedSize(100, 100); // Adjust the size as needed
}

void DropIndicator::paintEvent(QPaintEvent *event) {
    QWidget::paintEvent(event);
    QPainter painter(this);
    // Draw the square
    painter.setPen(Qt::DashLine);
    painter.drawRect(rect().adjusted(1, 1, -2, -2));
    // Draw the "+"
    int centerX = width() / 2;
    int centerY = height() / 2;
    painter.drawLine(centerX - 10, centerY, centerX + 10, centerY); // Horizontal line
    painter.drawLine(centerX, centerY - 10, centerX, centerY + 10); // Vertical line
}
