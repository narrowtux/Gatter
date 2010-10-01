#include "clock.h"

Clock::Clock(QObject *parent) :
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
    Q_UNUSED(option)
    Q_UNUSED(widget)
    painter->setPen("black");
    painter->setBrush(QColor("white"));
    painter->drawRect(boundingRect().adjusted(2,2,-2,-2));    QTextOption o;
    o.setAlignment(Qt::AlignCenter);
    painter->drawText(boundingRect().adjusted(5,5,-5,-5),"C",o);
    if(isSelected()){
	painter->setPen(getSelectionPen());
	painter->setBrush(Qt::NoBrush);
	painter->drawRect(boundingRect().adjusted(1,1,-1,-1));
    }
}

void Clock::createForm()
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
    Element::createForm();
}

void Clock::deleteForm()
{
    
    Element::deleteForm();
}

void Clock::setLowTime(int value){
    low->setInterval(value);
}

void Clock::setHighTime(int value){
    high->setInterval(value);
}
