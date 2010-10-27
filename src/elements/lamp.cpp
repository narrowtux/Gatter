#include "src/elements/lamp.h"
#include "src/widgets/colorbutton.h"
#include <QRegExp>
Lamp::Lamp(QGraphicsObject *parent) :
    Element(parent)
{
    myValue=false;
    height=44;
    width=44;
    minHeight=44;
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
    g.setRadius(44);
    g.setCenter(-22,-22);
    g.setColorAt(1,QColor(50,50,50));
    g.setColorAt(0,QColor(150,150,150));
    QBrush b(g);
    painter->setBrush(b);
    painter->drawEllipse(QPointF(0,0),22,22);
    painter->setPen(Qt::NoPen);
    if(myValue){
	painter->setBrush(color);
    }else{
	painter->setBrush(QColor("black"));
    }
    painter->drawEllipse(QPointF(0,0),17,17);
    QRadialGradient g2;
    g2.setRadius(35);
    g2.setCenter(-9,-9);
    g2.setColorAt(0,QColor(Qt::transparent));
    QColor color(150,150,150);
    color.setAlpha(150);
    g2.setColorAt(1,color);
    painter->setBrush(QBrush(g2));
    painter->drawEllipse(QPointF(0,0),17,17);
    painter->setPen(QColor(220,220,220));
    painter->setBrush(Qt::NoBrush);
    painter->drawPoint(0,boundingRect().top()+3);
    painter->drawPoint(boundingRect().left()+3,0);
    painter->drawPoint(boundingRect().right()-3,0);
    painter->drawPoint(0,boundingRect().bottom()-3);
}

void Lamp::recalculate(){
    myValue=myInputs[0]->value();
    //qDebug()<<"Recalculated";
    emit(outputChanged(myValue));
    update();
}

void Lamp::setColor(QColor c){
    color=c;
    update();
}

bool Lamp::createFormBefore(){
    ColorButton* c=new ColorButton;
    c->setColor(color);
    connect(c,SIGNAL(colorChanged(QColor)),this,SLOT(setColor(QColor)));
    QLabel*l=new QLabel(tr("Color"));
    additionalWidgets<<c<<l;
    layout->addRow(l,c);
    return true;
}

void Lamp::setPrivateXml(QXmlStreamWriter *xml){
    xml->writeAttribute("color",QString("rgb(%0,%1,%2)").arg(color.red()).arg(color.green()).arg(color.blue()));
}

void Lamp::readPrivateXml(QXmlStreamReader *xml){
    QString colorString=xml->attributes().value("color").toString();
    QRegExp exp("rgb\\(([0-9]{1,3}),([0-9]{1,3}),([0-9]{1,3})\\)");
    exp.indexIn(colorString);
    color.setRed(exp.cap(1).toInt());
    qDebug()<<exp.capturedTexts();
    color.setGreen(exp.cap(2).toInt());
    color.setBlue(exp.cap(3).toInt());
}

bool Lamp::isOutput(){
    return true;
}
