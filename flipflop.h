#ifndef FLIPFLOP_H
#define FLIPFLOP_H
#include "element.h"

class FlipFlop : public Element
{
    Q_OBJECT
    
public:
    enum FlipFlopType  {SetReset, JumpKill, MasterSlave, Delay, Toggle};
    enum FlipFlopTrigger {NotApplicable, None, OnValue, OnSwitching};
    explicit FlipFlop(QObject *parent = 0);
    FlipFlop(const FlipFlop &copy);
    ~FlipFlop();
signals:

public slots:
private:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QRectF boundingRect() const;
    void recalculate();
    FlipFlopType myFlipFlopType;
    FlipFlopTrigger myFlipFlopTrigger;
    bool myOnWhichValue;
    void setFlipFlopType(FlipFlopType type);
    void setFlipFlopTrigger(FlipFlopTrigger clock);
    void setOnWhichValue(bool owv);
};

#endif // FLIPFLOP_H
