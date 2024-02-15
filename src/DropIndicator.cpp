#include "DropIndicator.h"
#include <QPainter>
#include <QFontMetrics>
#include <QFont>
#include <QPalette>

DropIndicator::DropIndicator(QWidget *parent) : QWidget(parent) {
    // Optionally set a fixed size for the indicator
    setFixedSize(100, 100); // Adjust the size as needed
    // Set the background color or leave it transparent
    // setStyleSheet("background-color: #color;");
}

bool DropIndicator::isDarkMode() const {
    // Use the window text color to determine if the system is in dark mode
    QColor windowTextColor = palette().color(QPalette::WindowText);
    // Dark mode is assumed if the brightness is less than 128
    return windowTextColor.lightness() < 128;
}

void DropIndicator::paintEvent(QPaintEvent *event) {
    QWidget::paintEvent(event);
    QPainter painter(this);

    // Determine if the system is in dark mode
    bool darkMode = isDarkMode();

    // Set the pen color based on the mode
    QPen pen(darkMode ? Qt::black : Qt::lightGray);
    pen.setStyle(Qt::DashLine);
    painter.setPen(pen);

    // Draw the dashed square border
    painter.drawRect(rect().adjusted(1, 1, -2, -2));

    // Draw the drag and drop instruction text
    QString text = tr("Drag & Drop\nFiles Here");
    QFont font = painter.font();
    font.setPointSize(10); // Set the desired point size for the font
    painter.setFont(font);

    // Use QFontMetrics to calculate the size of the text bounding box
    QFontMetrics fontMetrics(font);
    QRect textRect = fontMetrics.boundingRect(rect(), Qt::AlignCenter, text);

    // Set the brush and pen for the text based on the mode
    QColor textColor = darkMode ? Qt::black : Qt::white;
    painter.setBrush(textColor);
    painter.setPen(textColor); // Ensure that the text outline is consistent with the fill

    // Draw the text aligned to the center of the widget
    painter.drawText(textRect, Qt::AlignCenter, text);
}
