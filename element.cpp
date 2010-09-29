#include "element.h"

Element::Element(QObject *parent, QGraphicsItem *gparent) :
    QObject(parent), QGraphicsItem(gparent)
{
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
}

Element::~Element()
{
    if(hasFocus()){
	deleteForm();
    } else {
	lineEdits.clear();
	checkBoxes.clear();
	additionalWidgets.clear();
    }
}

void Element::addInput(Connection *i){
    if(i==0){
	i=new Connection;
    }
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
    QLabel* label=new QLabel(tr("<b>Inputs</b>"));
    additionalWidgets<<label;
    layout->addRow(label);
    label=new QLabel(tr("Count"));
    QSpinBox* widget=new QSpinBox;
    widget->setMinimum(minInputs);
    if(maxInputs==-1)
	widget->setMaximum(100);
    else
	widget->setMaximum(maxInputs);
    widget->setValue(myInputs.count());
    connect(widget,SIGNAL(valueChanged(int)),this,SLOT(updateInputs(int)));
    layout->addRow(label, widget);
    additionalWidgets<<label<<widget;
    foreach(Connection* c, myInputs)
    {
	QLineEdit*l=new QLineEdit(c->myName);
	QCheckBox*ch=new QCheckBox("");
	ch->setChecked(c->isNegated());
	layout->addRow(l,ch);
	lineEdits.insert(c,l);
	lineMapper.setMapping(l,l);
	checkBoxes.insert(c,ch);
	checkMapper.setMapping(ch,ch);
	connect(l,SIGNAL(textChanged(QString)),&lineMapper,SLOT(map()));
	connect(ch,SIGNAL(clicked()),&checkMapper,SLOT(map()));
	
    }
    label= new QLabel(tr("<b>Outputs</b>"));
    additionalWidgets<<label;
    layout->addRow(label);
    label=new QLabel(tr("Count"));
    widget=new QSpinBox;
    widget->setMinimum(minOutputs);
    if(maxOutputs==-1)
	widget->setMaximum(100);
    else
	widget->setMaximum(maxOutputs);
    widget->setValue(myOutputs.count());
    connect(widget,SIGNAL(valueChanged(int)),this,SLOT(updateOutputs(int)));
    layout->addRow(label, widget);
    additionalWidgets<<label<<widget;
    foreach(Connection* c, myOutputs)
    {
	QLineEdit*l=new QLineEdit(c->myName);
	QCheckBox*ch=new QCheckBox("");
	layout->addRow(l,ch);
	lineEdits.insert(c,l);
	lineMapper.setMapping(l,l);
	checkBoxes.insert(c,ch);
	checkMapper.setMapping(ch,ch);
	connect(l,SIGNAL(textChanged(QString)),&lineMapper,SLOT(map()));
	connect(ch,SIGNAL(clicked()),&checkMapper,SLOT(map()));
    }
    connect(&checkMapper,SIGNAL(mapped(QWidget*)),this,SLOT(updateNegation(QWidget*)));
    connect(&lineMapper,SIGNAL(mapped(QWidget*)),this,SLOT(updateName(QWidget*)));
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
