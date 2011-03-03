#ifndef FLIPFLOP_H
#define FLIPFLOP_H
#include "src/elements/element.h"
#include "src/widgets/enumcombobox.h"


/*!
  \class FlipFlop
  \brief A FlipFlop is an Element that saves a one-bit state.
  
  The most common FlipFlop is the SR-FlipFlop, which has a Set and a Reset input.
  The inputs are used to set the save bit to 1 (Set) or 0 (Reset). This FlipFlop is
  implemented when myFlipFlopType is SetReset.
  
  The JK-FlipFlop is a more advanced FlipFlop. It has a Jump and a Kill input. 
  Like the SR-FlipFlop, the bit can be saved 1 with Jump and deleted to 0 with Kill.
  The difference is that you can give a 1 to both Jump and Kill at the same time 
  so the bit will toggle from 1 to 0 or from 0 to 1. JK-FlipFlops are implemented 
  when myFlipFlopType is JumpKill. They require a FlipFlopTrigger of OnValue or 
  OnSwitching.
  
  The Master-Slave FlipFlop is one of the most advanced FlipFlops. In an abstract 
  view, it has got 2 JK-FlipFlops which are connected together like this:
  
  \image html masterslave.png "Master-Slave FlipFlop in abstract view"
*/

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
