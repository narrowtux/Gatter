#include "lamp.h"
#include "colorbutton.h"
Lamp::Lamp(QObject *parent) :
    Element(parent)
{
    value=false;
    height=44;
    width=44;
    addInput(new Connection);
    setData(ElementName,"Lamp");
    setMinMaxInputsOutputs(1,1,0,0);
    myType="lamp";
    color=Scene::highValueColor;
    tr("Lamp");
}

QRectF Lamp::boundingRect() const {
    return QRectF(-22,-22,width,height);
}

void Lamp::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget){
    Q_UNUSED(option)
    Q_UNUSED(widget)
    // Draw border;
    if(!isSelected()){
	painter->setPen(Qt::NoPen);
    } else {
	painter->setPen(getSelectionPen());
    }
    QRadialGradient g;
    g.setRadius(40);
    g.setCenter(-20,-20);
    g.setColorAt(1,QColor(50,50,50));
    g.setColorAt(0,QColor(150,150,150));
    QBrush b(g);
    painter->setBrush(b);
    painter->drawEllipse(QPointF(0,0),20,20);
    painter->setPen(Qt::NoPen);
    if(value){
	painter->setBrush(color);
    }else{
	painter->setBrush(QColor("black"));
    }
    painter->drawEllipse(QPointF(0,0),15,15);
    QRadialGradient g2;
    g2.setRadius(30);
    g2.setCenter(-7,-7);
    g2.setColorAt(0,QColor(Qt::transparent));
    QColor color(150,150,150);
    color.setAlpha(150);
    g2.setColorAt(1,color);
    painter->setBrush(QBrush(g2));
    painter->drawEllipse(QPointF(0,0),15,15);
}

void Lamp::recalculate(){
    value=myInputs[0]->value();
    qDebug()<<"Recalculated";
    update();
}

void Lamp::setColor(QColor c){
    color=c;
    update();
}

void Lamp::createFormBefore(){
    ColorButton* c=new ColorButton;
    c->setColor(color);
    connect(c,SIGNAL(colorChanged(QColor)),this,SLOT(setColor(QColor)));
    QLabel*l=new QLabel(tr("Color"));
    additionalWidgets<<c<<l;
    layout->addRow(l,c);
}
