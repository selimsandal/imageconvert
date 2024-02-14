#include "DropIndicator.h"
#include <QPainter>
#include <QFontMetrics>
#include <QFont>

DropIndicator::DropIndicator(QWidget *parent) : QWidget(parent) {
    // Optionally set a fixed size for the indicator
    setFixedSize(100, 100); // Adjust the size as needed
    // Set the background color or leave it transparent
    // setStyleSheet("background-color: #color;");
}

void DropIndicator::paintEvent(QPaintEvent *event) {
    QWidget::paintEvent(event);
    QPainter painter(this);

    // Set the painter to use a white pen for drawing
    QPen pen(Qt::lightGray);
    pen.setStyle(Qt::DashLine);
    painter.setPen(pen);

    // Draw the dashed square border in white
    painter.drawRect(rect().adjusted(1, 1, -2, -2));

    // Draw the drag and drop instruction text in white
    QString text = tr("Drag & Drop\nFiles Here");
    QFont font = painter.font();
    font.setPointSize(10); // Set the desired point size for the font
    painter.setFont(font);

    // Use QFontMetrics to calculate the size of the text bounding box
    QFontMetrics fontMetrics(font);
    QRect textRect = fontMetrics.boundingRect(rect(), Qt::AlignCenter, text);

    // Set the brush to white for filling text
    painter.setBrush(Qt::white);
    painter.setPen(Qt::white); // Ensure that the text outline is also white

    // Draw the text aligned to the center of the widget
    painter.drawText(textRect, Qt::AlignCenter, text);
}
