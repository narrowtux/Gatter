#ifndef CLOCK_H
#define CLOCK_H
#include "element.h"

class Clock : public Element
{
    Q_OBJECT
public:
    explicit Clock(QObject *parent = 0);

signals:

public slots:
    void setLow();
    void setHigh();
private slots:
    void setLowTime(int value);
    void setHighTime(int value);
protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QRectF boundingRect() const;
    QTimer* low;
    QTimer* high;
    void createForm();
    void deleteForm();
};

#endif // CLOCK_H
