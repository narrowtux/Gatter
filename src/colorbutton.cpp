#include "colorbutton.h"
#include <QPainter>
#include <QApplication>
#include <QColorDialog>

ColorButton::ColorButton(QWidget *parent) :
    QToolButton(parent)
{
	m_color=QColor("black");
	connect(this,SIGNAL(clicked()),this,SLOT(cl()));
	setAcceptDrops(true);
	dialog=new QColorDialog(this);
	dialog->setOption(QColorDialog::NoButtons);
	connect(dialog,SIGNAL(currentColorChanged(QColor)),this,SLOT(onColorChanged(QColor)));
}

void ColorButton::paintEvent(QPaintEvent *e){
	QToolButton::paintEvent(e);
	QPainter p(this);
	p.setPen(QColor("black"));
	p.setBrush(m_color);
	p.drawRect(5,5,width()-11,height()-11);
}

void ColorButton::setColor(QColor c){
	m_color=c;
	update();
}

QColor ColorButton::color(){
	return m_color;
}

void ColorButton::cl(){
    dialog->setCurrentColor(m_color);
    dialog->show();
}

void ColorButton::mousePressEvent(QMouseEvent *event)
{
	QToolButton::mousePressEvent(event);
	if (event->button() == Qt::LeftButton)
		dragStartPos = event->pos();
}

void ColorButton::mouseMoveEvent(QMouseEvent *e){
	if (!(e->buttons() & Qt::LeftButton))
			 return;
		 if ((e->pos() - dragStartPos).manhattanLength()
			  < QApplication::startDragDistance())
			 return;
		 
		 mouseReleaseEvent(new QMouseEvent(QEvent::KeyRelease,QPoint(-1,-1),Qt::LeftButton,Qt::LeftButton,Qt::NoModifier));
		 QDrag *drag = new QDrag(this);
		 QMimeData *mimeData = new QMimeData;
	
		 mimeData->setColorData(QVariant(m_color));
		 drag->setMimeData(mimeData);
		 drag->exec(Qt::CopyAction);
}

void ColorButton::dragEnterEvent(QDragEnterEvent *e){
	if(e->mimeData()->hasColor()){
		e->acceptProposedAction();
	} else {
		e->ignore();
	}
}

void ColorButton::dropEvent(QDropEvent *e){
	setColor(qvariant_cast<QColor>(e->mimeData()->colorData()));
	emit colorChanged(m_color);
	e->acceptProposedAction();
}

void ColorButton::onColorChanged(QColor c){
    m_color=c;
    update();
    emit(colorChanged(c));
}
