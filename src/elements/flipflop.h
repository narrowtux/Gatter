#ifndef FLIPFLOP_H
#define FLIPFLOP_H
#include "src/elements/element.h"
#include "src/widgets/enumcombobox.h"

class FlipFlop : public Element
{
    Q_OBJECT
    
public:
    enum FlipFlopType  {SetReset, JumpKill, MasterSlave, Delay, Toggle};
    enum FlipFlopTrigger {NotApplicable, None, OnValue, OnSwitching};
    explicit FlipFlop(QGraphicsObject *parent = 0);
    FlipFlop(const FlipFlop &copy);
    ~FlipFlop();
    void setFlipFlopType(FlipFlopType type);
    void setFlipFlopTrigger(FlipFlopTrigger clock, bool spontaneous=true);
    void setPrivateXml(QXmlStreamWriter *xml);
    void readPrivateXml(QXmlStreamReader *xml);
public slots:
    void setClockNegated(bool v);
signals:

private slots:
    void set(bool v);
    void reset(bool v);
    void clock(bool v);
    void jump(bool v);
    void kill(bool v);
    void other(bool v);
    void onTypeBoxChanged(int t);
    void onTriggerBoxChanged(int t);
private:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QRectF boundingRect() const;
    void recalculate();
    FlipFlopType myFlipFlopType;
    FlipFlopTrigger myFlipFlopTrigger;
    bool myOnWhichValue;
    bool createFormBefore();
    EnumComboBox *typeBox;
    EnumComboBox *triggerBox;
    QCheckBox *negateBox;
    bool myBetweenValue;
	bool toggled;
};

#endif // FLIPFLOP_H
