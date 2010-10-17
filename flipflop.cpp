#include "flipflop.h"
#include <QInputDialog>
#include <QtGui>
FlipFlop::FlipFlop(QGraphicsObject *parent) :
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
    recalculate();
}

FlipFlop::FlipFlop(const FlipFlop &copy){
    FlipFlop(copy.parentObject());
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
    foreach(Connection*c, myInputs){
	//Clear Slot connections
	disconnect(c,SIGNAL(changed(bool)),this,SLOT(set(bool)));
	disconnect(c,SIGNAL(changed(bool)),this,SLOT(reset(bool)));
	disconnect(c,SIGNAL(changed(bool)),this,SLOT(clock(bool)));
	disconnect(c,SIGNAL(changed(bool)),this,SLOT(jump(bool)));
	disconnect(c,SIGNAL(changed(bool)),this,SLOT(kill(bool)));
	disconnect(c,SIGNAL(changed(bool)),this,SLOT(other(bool)));
    }

    myFlipFlopType=type;
    switch(type){
    case SetReset:
	if(myFlipFlopTrigger==None){
	    setMinMaxInputsOutputs(2,2,2,2);
	    myInputs.at(0)->setName("S");
	    connect(myInputs.at(0),SIGNAL(changed(bool)),this,SLOT(set(bool)));
	    myInputs.at(1)->setName("R");
	    connect(myInputs.at(1),SIGNAL(changed(bool)),this,SLOT(reset(bool)));
	} else {
	    setMinMaxInputsOutputs(3,3,2,2);
	    myInputs.at(0)->setName("S");
	    connect(myInputs.at(0),SIGNAL(changed(bool)),this,SLOT(set(bool)));
	    myInputs.at(1)->setName("C");
	    connect(myInputs.at(1),SIGNAL(changed(bool)),this,SLOT(clock(bool)));
	    myInputs.at(1)->setClock(true);
	    myInputs.at(2)->setName("R");
	    connect(myInputs.at(2),SIGNAL(changed(bool)),this,SLOT(reset(bool)));
	}
	myOutputs.at(0)->setName("Q");
	myOutputs.at(1)->setName("Q*");
	break;
    case JumpKill:
	if(myFlipFlopTrigger==None){
	    setFlipFlopTrigger(OnSwitching,false);
	}
	setMinMaxInputsOutputs(3,3,2,2);
	myInputs.at(0)->setName("J");
	connect(myInputs.at(0),SIGNAL(changed(bool)),this,SLOT(jump(bool)));
	myInputs.at(1)->setName("C");
	connect(myInputs.at(1),SIGNAL(changed(bool)),this,SLOT(clock(bool)));
	myInputs.at(1)->setClock(true);
	myInputs.at(2)->setName("K");
	connect(myInputs.at(2),SIGNAL(changed(bool)),this,SLOT(kill(bool)));
	myOutputs.at(0)->setName("Q");
	myOutputs.at(1)->setName("Q*");
	break;
    case MasterSlave:
	setFlipFlopTrigger(OnSwitching, false);
	setMinMaxInputsOutputs(3,3,2,2);
	myInputs.at(0)->setName("J");
	connect(myInputs.at(0),SIGNAL(changed(bool)),this,SLOT(jump(bool)));
	myInputs.at(1)->setName("C");
	connect(myInputs.at(1),SIGNAL(changed(bool)),this,SLOT(clock(bool)));
	myInputs.at(1)->setClock(true);
	myInputs.at(2)->setName("K");
	connect(myInputs.at(2),SIGNAL(changed(bool)),this,SLOT(kill(bool)));
	myOutputs.at(0)->setName("Q");
	myOutputs.at(1)->setName("Q*");
	break;
    case Delay:
	setFlipFlopTrigger(OnSwitching, false);
	setMinMaxInputsOutputs(2,2,2,2);
	myInputs.at(0)->setName("D");
	connect(myInputs.at(0),SIGNAL(changed(bool)),this,SLOT(other(bool)));
	myInputs.at(1)->setName("C");
	connect(myInputs.at(1),SIGNAL(changed(bool)),this,SLOT(clock(bool)));
	myInputs.at(1)->setClock(true);
	myOutputs.at(0)->setName("Q");
	myOutputs.at(1)->setName("Q*");
	break;
    case Toggle:
	setFlipFlopTrigger(OnSwitching, false);
	setMinMaxInputsOutputs(2,2,2,2);
	myInputs.at(0)->setName("1");
	connect(myInputs.at(0),SIGNAL(changed(bool)),this,SLOT(other(bool)));
	myInputs.at(1)->setName("C");
	connect(myInputs.at(1),SIGNAL(changed(bool)),this,SLOT(clock(bool)));
	myInputs.at(1)->setClock(true);
	myOutputs.at(0)->setName("Q");
	myOutputs.at(1)->setName("Q*");
	break;
    }
    update();
}

void FlipFlop::setFlipFlopTrigger(FlipFlopTrigger clock, bool spontaneous){
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
	break;
    case OnSwitching:
	break;
    }
    if(spontaneous) setFlipFlopType(myFlipFlopType);
    update();
}

QRectF FlipFlop::boundingRect() const {
    return QRectF(0,0,width,height);
}

void FlipFlop::recalculate(){
    myOutputs.at(0)->setValue(myValue);
    myOutputs.at(1)->setValue(!myValue);
}

void FlipFlop::set(bool v){
    if(v==1){
	switch(myFlipFlopTrigger){
	case None:
	    myValue=1;
	    recalculate();
	    break;
	case OnValue:
	    if(myInputs[1]->value()==myOnWhichValue){
		myValue=1;
		recalculate();
	    }
	    break;
	case OnSwitching:
	    break;
	}
    }
}

void FlipFlop::reset(bool v){
    if(v==1){
	switch(myFlipFlopTrigger){
	case None:
	    myValue=0;
	    recalculate();
	    break;
	case OnValue:
	    if(myInputs[1]->value()==myOnWhichValue){
		myValue=0;
		recalculate();
	    }
	    break;
	case OnSwitching:
	    break;
	}
    }
}

void FlipFlop::clock(bool v){
    switch(myFlipFlopType){
    case SetReset:
	if(v==myOnWhichValue){
	    if(myInputs[0]->value()){
		myValue=1;
	    }
	    if(myInputs[2]->value()){
		myValue=0;
	    }
	    recalculate();
	}
	break;
    case JumpKill:
	if(v==myOnWhichValue){
	    if(myInputs[0]->value()){
		myValue=1;
	    }
	    if(myInputs[2]->value()){
		myValue=0;
	    }
	    if(myInputs[0]->value()&&myInputs[2]->value()){
		myValue=!myValue;
	    }
	    recalculate();
	}
	break;
    case MasterSlave:
	
	break;
    case Delay:
	
	break;
    case Toggle:
	
	break;
    }
}

void FlipFlop::jump(bool v){
    switch(myFlipFlopType){
    default:
	break;
    case JumpKill:
	if(v&&myFlipFlopTrigger==OnValue&&myInputs[1]->value()==myOnWhichValue){
	    if(myInputs[2]->value()){
		myValue=!myValue;
	    } else {
		myValue=1;
	    }
	}
	recalculate();
	break;
    }
}

void FlipFlop::kill(bool v){
    switch(myFlipFlopType){
    default:
	break;
    case JumpKill:
	if(v&&myFlipFlopTrigger==OnValue&&myInputs[1]->value()==myOnWhichValue){
	    if(myInputs[0]->value()){
		myValue=!myValue;
	    } else {
		myValue=0;
	    }
	}
	recalculate();
	break;
    }
}

void FlipFlop::other(bool v){
    
}

bool FlipFlop::createFormBefore(){
    typeBox=new EnumComboBox;
    triggerBox=new EnumComboBox;
    QMap<int, QString> types;
    QMap<int, QString> triggers;
    types.insert(SetReset,tr("SR-Flipflop"));
    types.insert(JumpKill,tr("JK-Flipflop"));
    types.insert(MasterSlave,tr("Master-Slave-Flipflop"));
    types.insert(Toggle,tr("Toggle Flipflop"));
    types.insert(Delay,tr("Delay Flipflop"));
    typeBox->addItems(types);
    typeBox->setCurrentIndex(myFlipFlopType);
    triggers.insert(None,tr("None"));
    triggers.insert(OnValue,tr("On Value"));
    triggers.insert(OnSwitching,tr("On Switching"));
    triggerBox->addItems(triggers);
    triggerBox->setCurrentIndex(myFlipFlopTrigger);
    connect(typeBox,SIGNAL(currentEnumIndexChanged(int)),this,SLOT(onTypeBoxChanged(int)));
    connect(triggerBox,SIGNAL(currentEnumIndexChanged(int)),this,SLOT(onTriggerBoxChanged(int)));
    QLabel *typeLabel=new QLabel(tr("Type")), *triggerLabel=new QLabel(tr("Trigger")), *onWhichValueLabel=new QLabel(tr("Triggered on"));
    QCheckBox* onWhichValue=new QCheckBox;
    onWhichValue->setChecked(myOnWhichValue);
    connect(onWhichValue,SIGNAL(clicked(bool)),this,SLOT(setOnWhichValue(bool)));
    additionalWidgets<<typeBox<<triggerBox<<typeLabel<<triggerLabel<<onWhichValueLabel<<onWhichValue;
    
    layout->addRow(typeLabel,typeBox);
    layout->addRow(triggerLabel,triggerBox);
    layout->addRow(onWhichValueLabel,onWhichValue);
    return false;
}

void FlipFlop::onTypeBoxChanged(int t){
    setFlipFlopType(static_cast<FlipFlopType>(t));
    typeBox->setCurrentIndex(myFlipFlopType);
    triggerBox->setCurrentIndex(myFlipFlopTrigger);
}

void FlipFlop::onTriggerBoxChanged(int t){
    setFlipFlopTrigger(static_cast<FlipFlopTrigger>(t));
    triggerBox->setCurrentIndex(myFlipFlopTrigger);
}

void FlipFlop::setPrivateXml(QXmlStreamWriter *xml){
    xml->writeAttribute("flipfloptype",QString("%0").arg((int)myFlipFlopType));
    xml->writeAttribute("flipfloptrigger",QString("%0").arg((int)myFlipFlopTrigger));
    xml->writeAttribute("onwhichvalue",myOnWhichValue?"true":"false");
}

void FlipFlop::readPrivateXml(QXmlStreamReader *xml){
    setFlipFlopType(static_cast<FlipFlopType>(xml->attributes().value("flipfloptype").toString().toInt()));
    setFlipFlopTrigger(static_cast<FlipFlopTrigger>(xml->attributes().value("flipfloptrigger").toString().toInt()));
    setOnWhichValue(xml->attributes().value("onwhichvalue")=="true"?1:0);
}
