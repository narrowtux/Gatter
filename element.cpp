#include "element.h"

Element::Element(QObject *parent, QGraphicsItem *gparent) :
    QObject(parent), QGraphicsItem(gparent)
{
    minHeight=height;
    setFlag(ItemIsSelectable,true);
    setFlag(ItemIsMovable,true);
    setData(ElementName,"Element");
    
    setFlag(ItemSendsGeometryChanges,true);
    pressed=false;
    qDebug()<<(QGraphicsItem*)this<<"flags:"<<flags();
    minInputs=2;
    maxInputs=-1;
    minOutputs=1;
    maxOutputs=1;
    myType="generic";
}

Element::~Element()
{
    lineEdits.clear();
    checkBoxes.clear();
    additionalWidgets.clear();
}

void Element::addInput(Connection *i){
    if(i==0){
	i=new Connection;
    }
    i->setElement(this);
    connect(i,SIGNAL(recalculate()),this,SLOT(recalculate()));
    i->setParentItem(this);
    myInputs<<i;
    i->myConnectionType=Input;
    relayoutConnections();
    connect(i,SIGNAL(changed(bool)),this,SLOT(inputChanged()));
}

void Element::addOutput(Connection *o){
    if(o==0){
	o=new Connection;
    }
    o->setElement(this);
    connect(o,SIGNAL(recalculate()),this,SLOT(recalculate()));
    o->setParentItem(this);
    myOutputs<<o;
    o->myConnectionType=Output;
    relayoutConnections();
}

void Element::removeInput(Connection *i){
    if(i==0){
	foreach(Connection*c, myInputs){
	    i=c;
	    if(!c->isConnected()){
		break;
	    }
	}
    }
    i->setParentItem(0);
    myInputs.removeAll(i);
    relayoutConnections();
    delete i;
}

void Element::removeOutput(Connection *o){
    if(o==0){
	foreach(Connection*c, myOutputs){
	    o=c;
	    if(!c->isConnected()){
		break;
	    }
	}
    }
    o->setParentItem(0);
    myOutputs.removeAll(o);
    delete o;
    relayoutConnections();
}

void Element::addInput(int c){
    for(int i=0;i<c;i++){
	addInput();
    }
}

void Element::addOutput(int c){
    for(int i=0;i<c;i++){
	addOutput();
    }
}

void Element::removeInput(int c){
    for(int i=0;i<c;i++){
	removeInput();
    }
}

void Element::removeOutput(int c){
    for(int i=0;i<c;i++){
	removeOutput();
    }
}

void Element::setInputs(int c){
    if(c==myInputs.count()){
	return;
    }
    if(c<myInputs.count()){
	removeInput(myInputs.count()-c);
    }
    if(c>myInputs.count()){
	addInput(c-myInputs.count());
    }
}

void Element::setOutputs(int c){
    if(c==myOutputs.count()){
	return;
    }
    if(c<myOutputs.count()){
	removeOutput(myOutputs.count()-c);
    }
    if(c>myOutputs.count()){
	addOutput(c-myOutputs.count());
    }
}

void Element::relayoutConnections(){
    //Precalculate Optimal Height
    qDebug()<<"height before"<<height;
    qreal minimumHeight=qMax((myInputs.count())*20,(myOutputs.count())*20);
    if(minimumHeight>height){
	height=qMax(myInputs.count()*20,myOutputs.count()*20);
    } else if(minimumHeight<=height){
	if(minimumHeight>minHeight){
	    height=minimumHeight;
	} else {
	    height=minHeight;
	}
    }
    qDebug()<<"Minimum needed Height:"<<minimumHeight;
    qDebug()<<"minHeight"<<minHeight;
    qDebug()<<"height after"<<height;

    //Inputs
    qreal leftSide=boundingRect().left();
    qreal rightSide=boundingRect().right();
    qreal topSide=boundingRect().top();
    int count=myInputs.count();
    for(int i=0;i<count;i++){
	myInputs[i]->setPos(leftSide-20,(height/(qreal)(count+1))*(1+i)+topSide);
    }

    //Outputs
    count=myOutputs.count();
    for(int i=0;i<count;i++){
	myOutputs[i]->setPos(rightSide,(height/(qreal)(count+1))*(1+i)+topSide);
    }
    update();

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

void Element::setMinMaxInputsOutputs(int minIn, int maxIn, int minOut, int maxOut)
{
    minInputs=minIn;
    maxInputs=maxIn;
    minOutputs=minOut;
    maxOutputs=maxOut;
    if(minInputs>myInputs.count()){
	setInputs(minInputs);
    }
    if(minOutputs>myOutputs.count()){
	setOutputs(minOutputs);
    }
    if(maxInputs<myInputs.count()&&maxInputs!=-1){
	setInputs(maxInputs);
    }
    if(maxOutputs<myOutputs.count()&&maxOutputs!=-1){
	setOutputs(maxOutputs);
    }
}

void Element::createForm()
{
    QFrame* frame=new QFrame;
    frame->setFrameStyle(QFrame::HLine|QFrame::Sunken);
    additionalWidgets<<frame;
    layout->addRow(frame);
    QLabel* label;
    label=new QLabel("<h2>"+tr(metaObject()->className())+"</h2>");
    additionalWidgets<<label;
    layout->addRow(label);
    createFormBefore();
    QSpinBox* widget;
    if(myInputs.count()!=0){
	label=new QLabel(tr("<b>Inputs</b>"));
	additionalWidgets<<label;
	if(minInputs!=maxInputs){
	    widget=new QSpinBox;
	    layout->addRow(label,widget);
	    widget->setMinimum(minInputs);
	    if(maxInputs==-1)
		widget->setMaximum(100);
	    else
		widget->setMaximum(maxInputs);
	    widget->setValue(myInputs.count());
	    connect(widget,SIGNAL(valueChanged(int)),this,SLOT(updateInputs(int)));
	    additionalWidgets<<widget;
	}else{
	    layout->addRow(label);
	}
	foreach(Connection* c, myInputs)
	{
	    QLineEdit*l=new QLineEdit(c->myName);
	    QCheckBox*ch=new QCheckBox("");
	    ch->setChecked(c->isNegated());
	    ch->setToolTip(tr("Negated"));
	    layout->addRow(l,ch);
	    lineEdits.insert(c,l);
	    lineMapper.setMapping(l,l);
	    checkBoxes.insert(c,ch);
	    checkMapper.setMapping(ch,ch);
	    connect(l,SIGNAL(textChanged(QString)),&lineMapper,SLOT(map()));
	    connect(ch,SIGNAL(clicked()),&checkMapper,SLOT(map()));
	    
	}
    }
    if(myOutputs.count()!=0){
	label= new QLabel(tr("<b>Outputs</b>"));
	additionalWidgets<<label;
	    if(minOutputs!=maxOutputs){
	    widget=new QSpinBox;
	    layout->addRow(label,widget);
	    widget->setMinimum(minOutputs);
	    if(maxOutputs==-1)
		widget->setMaximum(100);
	    else
		widget->setMaximum(maxOutputs);
	    widget->setValue(myOutputs.count());
	    connect(widget,SIGNAL(valueChanged(int)),this,SLOT(updateOutputs(int)));
	    additionalWidgets<<widget;
	}else{
	    layout->addRow(label);
	}
	foreach(Connection* c, myOutputs)
	{
	    QLineEdit*l=new QLineEdit(c->myName);
	    QCheckBox*ch=new QCheckBox("");
	    ch->setToolTip(tr("Negated"));
	    layout->addRow(l,ch);
	    lineEdits.insert(c,l);
	    lineMapper.setMapping(l,l);
	    checkBoxes.insert(c,ch);
	    checkMapper.setMapping(ch,ch);
	    connect(l,SIGNAL(textChanged(QString)),&lineMapper,SLOT(map()));
	    connect(ch,SIGNAL(clicked()),&checkMapper,SLOT(map()));
	}
    }
    connect(&checkMapper,SIGNAL(mapped(QWidget*)),this,SLOT(updateNegation(QWidget*)));
    connect(&lineMapper,SIGNAL(mapped(QWidget*)),this,SLOT(updateName(QWidget*)));
    foreach(QLineEdit*l,lineEdits){
	l->setMinimumWidth(50);
	l->setMaximumWidth(50);
    }

    createFormAfter();
}

void Element::deleteForm()
{
    foreach(Connection*c, lineEdits.keys()){
	delete lineEdits.value(c);
	lineEdits.remove(c);
    }
    foreach(Connection*c, checkBoxes.keys()){
        delete checkBoxes.value(c);
	checkBoxes.remove(c);
    }
    foreach(QWidget*w, additionalWidgets){
	additionalWidgets.removeAll(w);
	w->deleteLater();
    }
}

void Element::updateName(QWidget *lineEdit)
{
    QLineEdit*l=static_cast<QLineEdit*>(lineEdit);
    Connection*c=lineEdits.key(l);
    c->setName(l->text());
}

void Element::updateNegation(QWidget *checkBox)
{
    QCheckBox*l=static_cast<QCheckBox*>(checkBox);
    Connection*c=checkBoxes.key(l);
    c->setNegated(l->isChecked());
}

void Element::updateInputs(int c)
{
    setInputs(c);
    deleteForm();
    createForm();
}

void Element::updateOutputs(int c)
{
    setOutputs(c);
    deleteForm();
    createForm();
}

void Element::setFormLayout(QFormLayout *flayout)
{
    layout=flayout;
}

QVariant Element::itemChange(GraphicsItemChange change, const QVariant &value)
{
    if(change==ItemSelectedHasChanged)
    {
	if(value.toBool())
	{
	    createForm();
	} else {
	    deleteForm();
	}
    }
    return value;
}

void Element::setPrivateXml(QXmlStreamWriter *xml){
    Q_UNUSED(xml)
}

void Element::readPrivateXml(QCoreXmlStreamReader *xml){
    Q_UNUSED(xml)
}

void Element::createFormBefore(){
    //Dummy
}

void Element::createFormAfter(){
    //Dummy
}

void Element::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget){
    Q_UNUSED(option)
    Q_UNUSED(widget)
    painter->setRenderHint(QPainter::HighQualityAntialiasing,true);
    painter->setPen(QColor("gray"));
    if(isSelected()){
	painter->setPen(getSelectionPen());
    }
    QLinearGradient gradient;
    gradient.setColorAt(0,QColor(220,220,220));
    gradient.setColorAt(1,QColor(255,255,255));
    gradient.setStart(boundingRect().topLeft());
    gradient.setFinalStop(boundingRect().bottomLeft());
    painter->setBrush(QBrush(gradient));
    painter->drawRoundedRect(boundingRect().adjusted(1,1,-1,-1),3,3);
    QPen pointPen;
    pointPen.setColor(QColor(100,100,100));
    painter->setPen(pointPen);
    painter->setBrush(pointPen.color());
    painter->drawEllipse(boundingRect().topLeft()+QPointF(5,5),1,1);
    painter->drawEllipse(boundingRect().topRight()+QPointF(-5,5),1,1);
    painter->drawEllipse(boundingRect().bottomLeft()+QPointF(5,-5),1,1);
    painter->drawEllipse(boundingRect().bottomRight()+QPointF(-5,-5),1,1);
    painter->setPen(QColor("black"));
    painter->setBrush(Qt::NoBrush);
}

void Element::setPos(const QPointF &pos){
    qreal x=(int)pos.x()+0.5;
    qreal y=(int)pos.y()+0.5;
    QGraphicsItem::setPos(x,y);
}

void Element::setPos(qreal x, qreal y){
    setPos(QPointF(x,y));
}

bool Element::isInput(){
    return false;
}

bool Element::isOutput(){
    return false;
}

void Element::setInput(bool value){
    
}

bool Element::value(){
    return myValue;
}
