#include "src/elements/element.h"

qreal Element::rotationSteps=90;

Element::Element(QGraphicsObject* parent) :
		QGraphicsObject(parent)
{
    minHeight=50;
    setFlag(ItemIsSelectable,true);
    setFlag(ItemIsMovable,true);
    setData(ElementName,"Element");
    
    setFlag(ItemSendsGeometryChanges,true);
    setFlags(flags()|ItemSendsScenePositionChanges);
    pressed=false;
    //qDebug()<<(QGraphicsItem*)this<<"flags:"<<flags();
    minInputs=2;
    maxInputs=-1;
    minOutputs=1;
    maxOutputs=1;
    myType="generic";
    recalculate();
    layout=0;
    isMoving=false;
    myElementColor=QColor(255,255,255);
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
		Connection*c=0;
		for(int x=myInputs.count()-1;x>=0;x--){
			c=myInputs[x];
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
		Connection*c=0;
		for(int x=myOutputs.count()-1;x>=0;x--){
			c=myOutputs[x];
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
    //qDebug()<<"height before"<<height;
    qreal minimumHeight=qMax((myInputs.count())*20,(myOutputs.count())*20);
    if(minimumHeight>height){
		prepareGeometryChange();
		height=qMax(myInputs.count()*20,myOutputs.count()*20);
    } else if(minimumHeight<=height){
		if(minimumHeight>minHeight){
			prepareGeometryChange();
			height=minimumHeight;
		} else {
			prepareGeometryChange();
			height=minHeight;
		}
    }
    /*
    qDebug()<<"Minimum needed Height:"<<minimumHeight;
    qDebug()<<"minHeight"<<minHeight;
    qDebug()<<"height after"<<height;
*/
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
    QtConcurrent::run(this,&Element::recalculate);
}

void Element::recalculate(){
    if(Scene::debugMethods)qDebug()<<(void*)this<<","<<count("recalcs")<<", Element::recalculate()";
    //DUMMY-Method
}

void Element::mouseMoveEvent(QGraphicsSceneMouseEvent *event){
    QGraphicsItem::mouseMoveEvent(event);
}

void Element::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	if(event->button()==Qt::RightButton)
		setSelected(true);
    QGraphicsItem::mousePressEvent(event);
}

void Element::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if(isMoving){
		emit(moved());
		isMoving=false;
    }
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
    foreach(Connection*c, QList<Connection*>()<<myInputs<<myOutputs){
		c->setClock(false);
		c->setName("");
    }
}

void Element::createForm()
{
    if(layout==0){
		return;
    }
    QFrame* frame=new QFrame;
    frame->setFrameStyle(QFrame::HLine|QFrame::Sunken);
    additionalWidgets<<frame;
    layout->addRow(frame);
    QLabel* label;
    label=new QLabel("<h2>"+tr(metaObject()->className())+"</h2>");
    additionalWidgets<<label;
    layout->addRow(label);
    ColorButton*cb=new ColorButton;
    cb->setColor(myElementColor);
    connect(cb,SIGNAL(colorChanged(QColor)),this,SLOT(setElementColor(QColor)));
    QLabel *cl=new QLabel(tr("Element Color"));
    additionalWidgets<<cb<<cl;
    layout->addRow(cl,cb);
    if(createFormBefore()){
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
				ch->setChecked(c->isNegated());
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
    }
    createFormAfter();
}

void Element::deleteForm()
{
    if(layout==0){
		return;
    }
    foreach(Connection*c, lineEdits.keys()){
		lineEdits.value(c)->deleteLater();
		lineEdits.remove(c);
    }
    foreach(Connection*c, checkBoxes.keys()){
		checkBoxes.value(c)->deleteLater();
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
    if(change==ItemScenePositionHasChanged){
		isMoving=true;
    }
	if(change==ItemRotationHasChanged){
		foreach(Connection*c, myInputs+myOutputs){
			c->itemChange(change,value);
		}
	}
    return value;
}

void Element::setPrivateXml(QXmlStreamWriter *xml){
    xml->writeAttribute("rotation",QString().setNum(rotation()));
}

void Element::readPrivateXml(QXmlStreamReader *xml){
    qreal rot=xml->attributes().value("rotation").toString().toDouble();
	setRotation(rot);
	setData(UserType+1411,rot);
}

bool Element::createFormBefore(){
    return true;
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
    int h,s,v;
    myElementColor.getHsv(&h,&s,&v);
    QColor gradientLight, gradientDark;
    gradientLight.setHsv(h,qMin(s,50),255);
    gradientDark.setHsv(h,qMin(s,50),220);
    gradient.setColorAt(0,gradientDark);
    gradient.setColorAt(1,gradientLight);
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
	
	//Draw labels
	QFont font=painter->font();
	
	foreach(Connection*c, myInputs){
		QString name=c->name();
		QPointF pos=c->pos();
		QRectF rect(boundingRect().left()+3,pos.y()-10,20,20);
		if(name.contains("*")){
			name.remove("*");
			font.setOverline(true);
		}else{
			font.setOverline(false);
		}
		painter->setFont(font);
		if(name!="C")
			painter->drawText(rect, name, QTextOption(Qt::AlignLeft|Qt::AlignVCenter));
		else {
			rect.setLeft(boundingRect().left());
			QPolygonF poly;
			poly<<rect.topLeft()+QPointF(1,5)
			    <<QPointF(rect.right()-10,(rect.top()+rect.bottom())/2)
				<<rect.bottomLeft()-QPointF(-1,5);
			painter->drawPolyline(poly);
		}
	}
	
	foreach(Connection*c, myOutputs){
		QString name=c->name();
		QPointF pos=c->pos();
		QRectF rect(boundingRect().right()-23,pos.y()-10,20,20);
		if(name.contains("*")){
			name.remove("*");
			font.setOverline(true);
		}else{
			font.setOverline(false);
		}
		painter->setFont(font);
		painter->drawText(rect, name, QTextOption(Qt::AlignRight|Qt::AlignVCenter));
	}
}

void Element::setPos(const QPointF &pos){
    qreal x=qRound(pos.x())+0.5;
    qreal y=qRound(pos.y())+0.5;
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
    myValue=value;
}

bool Element::value(){
    return myValue;
}

QString Element::name(){
    QString ret="";
    foreach(Connection* c, QList<Connection*>()<<myInputs<<myOutputs){
		if(c->name()!=""){
			ret=c->name();
			break;
		}
    }
    return ret;
}

int Element::count(const char *propName){
    int ret=property(propName).toInt()+1;
    setProperty(propName,ret);
    return ret;
}

void Element::setElementColor(QColor c){
    myElementColor=c;
    update();
}

QColor Element::elementColor(){
    return myElementColor;
}

void Element::releaseConnections()
{
    foreach(Connection* c, QList<Connection*>()<<myInputs<<myOutputs){
		c->connectWith(0);
    }
}
