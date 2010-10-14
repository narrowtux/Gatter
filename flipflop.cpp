#include "flipflop.h"
#include <QInputDialog>
FlipFlop::FlipFlop(QObject *parent) :
    Element(parent)
{
    height=50;
    minHeight=50;
    width=50;
    qRegisterMetaType<FlipFlop>("FlipFlop");
    setOnWhichValue(true);
    setFlipFlopType(SetReset);
    setFlipFlopTrigger(None);
    myValue=false;
}

FlipFlop::FlipFlop(const FlipFlop &copy){
    FlipFlop(copy.parent());
    setOnWhichValue(copy.myOnWhichValue);
    setFlipFlopType(copy.myFlipFlopType);
    setFlipFlopTrigger(copy.myFlipFlopTrigger);
    myValue=copy.myValue;
    update();
}

FlipFlop::~FlipFlop(){
    
}

void FlipFlop::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget){
    Element::paint(painter, option, widget);
}

void FlipFlop::setOnWhichValue(bool owv){
    myOnWhichValue=owv;
    update();
}

void FlipFlop::setFlipFlopType(FlipFlopType type){
    myFlipFlopType=type;
    switch(type){
    case SetReset:
	if(myFlipFlopTrigger==None){
	    setMinMaxInputsOutputs(2,2,2,2);
	    myInputs.at(0)->setName("S");
	    myInputs.at(1)->setName("R");
	} else {
	    setMinMaxInputsOutputs(3,3,2,2);
	    myInputs.at(0)->setName("S");
	    myInputs.at(1)->setName("C");
	    myInputs.at(1)->setClock(true);
	    myInputs.at(2)->setName("R");
	}
	myOutputs.at(0)->setName("Q");
	myOutputs.at(1)->setName("Q*");
	break;
    case JumpKill:
	setMinMaxInputsOutputs(3,3,2,2);
	myInputs.at(0)->setName("J");
	myInputs.at(1)->setName("C");
	myInputs.at(1)->setClock(true);
	myInputs.at(2)->setName("K");
	myOutputs.at(0)->setName("Q");
	myOutputs.at(1)->setName("Q*");
	break;
    case MasterSlave:
	setMinMaxInputsOutputs(3,3,2,2);
	myInputs.at(0)->setName("J");
	myInputs.at(1)->setName("C");
	myInputs.at(1)->setClock(true);
	myInputs.at(2)->setName("K");
	myOutputs.at(0)->setName("Q");
	myOutputs.at(1)->setName("Q*");
	break;
    case Delay:
	setMinMaxInputsOutputs(2,2,2,2);
	myInputs.at(0)->setName("D");
	myInputs.at(1)->setName("C");
	myInputs.at(1)->setClock(true);
	myOutputs.at(0)->setName("Q");
	myOutputs.at(1)->setName("Q*");
	break;
    case Toggle:
	setMinMaxInputsOutputs(2,2,2,2);
	myInputs.at(0)->setName("1");
	myInputs.at(1)->setName("C");
	myInputs.at(1)->setClock(true);
	myOutputs.at(0)->setName("Q");
	myOutputs.at(1)->setName("Q*");
	break;
    }
    update();
}

void FlipFlop::setFlipFlopTrigger(FlipFlopTrigger clock){
    myFlipFlopTrigger=clock;
    switch(clock){
    case None:
	if(myFlipFlopType==MasterSlave||myFlipFlopType==Delay||myFlipFlopType==Toggle){
	    myFlipFlopTrigger=OnSwitching;
	}
	break;
    case OnValue:
	if(myFlipFlopType==MasterSlave||myFlipFlopType==Delay||myFlipFlopType==Toggle){
	    myFlipFlopTrigger=OnSwitching;
	}
	if(myFlipFlopType==SetReset){
	    myFlipFlopTrigger=None;
	}
	break;
    case OnSwitching:
	if(myFlipFlopType==SetReset){
	    myFlipFlopTrigger=None;
	}
	break;
    }
    setFlipFlopType(myFlipFlopType);
    update();
}

QRectF FlipFlop::boundingRect() const {
    return QRectF(0,0,height,width);
}

void FlipFlop::recalculate(){
    myOutputs.at(0)->setValue(myValue);
    myOutputs.at(1)->setValue(!myValue);
}
