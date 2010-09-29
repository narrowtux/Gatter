#include "element.h"

Element::Element(QObject *parent, QGraphicsItem *gparent) :
    QObject(parent), QGraphicsItem(gparent)
{
    setFlag(ItemIsSelectable,true);
    setFlag(ItemIsMovable,true);
    setData(ElementName,"Element");
    
    setFlag(ItemSendsGeometryChanges,true);
    setFlag(ItemSendsScenePositionChanges,true);
    pressed=false;
    qDebug()<<(QGraphicsItem*)this<<"flags:"<<flags();
}

void Element::addInput(Connection *i){
    i->setParentItem(this);
    myInputs<<i;
    i->myConnectionType=Input;
    relayoutConnections();
    connect(i,SIGNAL(changed(bool)),this,SLOT(inputChanged()));
}

void Element::addOutput(Connection *o){
    o->setParentItem(this);
    myOutputs<<o;
    o->myConnectionType=Output;
    relayoutConnections();
}

void Element::removeInput(Connection *i){
    i->setParentItem(0);
    myInputs.removeAll(i);
    relayoutConnections();
    disconnect(i,SIGNAL(changed(bool)),this,SLOT(inputChanged()));
}

void Element::removeOutput(Connection *o){
    o->setParentItem(0);
    myOutputs.removeAll(o);
    relayoutConnections();
}

void Element::relayoutConnections(){
    //Precalculate Optimal Height
    if(myInputs.count()*10>height||myOutputs.count()*10>height){
	height=qMax(myInputs.count()*10,myOutputs.count()*10);
    }

    //Inputs
    qreal leftSide=boundingRect().left();
    qreal rightSide=boundingRect().right();
    qreal topSide=boundingRect().top();
    int count=myInputs.count();
    for(int i=0;i<count;i++){
	myInputs[i]->setPos(leftSide-20+2,(height/(qreal)(count+1))*(1+i)+topSide);
    }

    //Outputs
    count=myOutputs.count();
    for(int i=0;i<count;i++){
	myOutputs[i]->setPos(rightSide-2,(height/(qreal)(count+1))*(1+i)+topSide);
    }

}

QPen Element::getSelectionPen(){
    QColor hi=QApplication::palette().highlight().color();
    int h,s,v;
    hi.getHsv(&h,&s,&v);
    hi.setHsv(h,50,v);
    QPen p(h);
    p.setWidth(2);
    return p;
}

void Element::inputChanged(){
    recalculate();
}

void Element::recalculate(){
    //DUMMY-Method
}

void Element::mouseMoveEvent(QGraphicsSceneMouseEvent *event){
    QGraphicsItem::mouseMoveEvent(event);
}

void Element::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mousePressEvent(event);
}

void Element::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mouseReleaseEvent(event);
}
