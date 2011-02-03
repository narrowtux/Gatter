#include "src/elements/clock.h"

Clock::Clock(QGraphicsObject *parent) :
    Element(parent)
{
    height=50;
    width=50;
    setMinMaxInputsOutputs(0,0,1,1);
    low=new QTimer;
    high=new QTimer;
    low->setSingleShot(true);
    high->setSingleShot(true);
    low->setInterval(100);
    high->setInterval(100);
    connect(low,SIGNAL(timeout()),this,SLOT(setLow()));
    connect(high,SIGNAL(timeout()),this,SLOT(setHigh()));
    high->start();
    myType="clock";
    tr("Clock");
}

void Clock::setLow(){
    myOutputs[0]->setValue(0);
    high->start();
}

void Clock::setHigh(){
    myOutputs[0]->setValue(1);
    low->start();
}

QRectF Clock::boundingRect() const{
    return QRectF(-25,-25,width,height);
}

void Clock::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget){
    Element::paint(painter,option,widget);
    QTextOption o;
    o.setAlignment(Qt::AlignCenter);
    painter->drawText(boundingRect().adjusted(5,5,-5,-5),"C",o);
}

bool Clock::createFormBefore()
{
    QLabel* l=new QLabel(tr("Low-Time"));
    QSpinBox* s=new QSpinBox();
    s->setRange(10,1000);
    s->setValue(low->interval());
    additionalWidgets<<l<<s;
    layout->addRow(l,s);
    connect(s,SIGNAL(valueChanged(int)),this,SLOT(setLowTime(int)));
    l=new QLabel(tr("High-Time"));
    s=new QSpinBox();
    s->setRange(10,1000);
    s->setValue(high->interval());
    additionalWidgets<<l<<s;
    layout->addRow(l,s);
    connect(s,SIGNAL(valueChanged(int)),this,SLOT(setHighTime(int)));
    return true;
}


void Clock::setLowTime(int value){
    high->setInterval(value);
}

void Clock::setHighTime(int value){
    low->setInterval(value);
}

void Clock::setPrivateXml(QXmlStreamWriter *xml)
{
    xml->writeAttribute("lowTime",QString().setNum(low->interval()));
    xml->writeAttribute("highTime",QString().setNum(high->interval()));
}

void Clock::readPrivateXml(QXmlStreamReader *xml){
    low->setInterval(xml->attributes().value("lowTime").toString().toInt());
    high->setInterval(xml->attributes().value("highTime").toString().toInt());
}
