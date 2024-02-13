// DropIndicator.h
#ifndef DROPINDICATOR_H
#define DROPINDICATOR_H

#include <QWidget>

class DropIndicator : public QWidget {
Q_OBJECT

public:
    explicit DropIndicator(QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *event) override;
};

#endif // DROPINDICATOR_H
