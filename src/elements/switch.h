#ifndef SWITCH_H
#define SWITCH_H

#include "src/defines.h"
#include "src/elements/element.h"

/*!
  \class Switch
  \brief Switch is a toggleable input.
  
  When the user clicks on it, the Switch will toggle its value between true and false
  */
class Switch : public Element
{
    Q_OBJECT
public:
	/*!
	  Creates a new Switch as a child of \a parent
	  */
    explicit Switch(QGraphicsObject *parent = 0);
	~Switch();
    void setPrivateXml(QXmlStreamWriter *xml);
    void readPrivateXml(QXmlStreamReader *xml);
    bool isInput();
    void setInput(bool value);
public slots:
	/*!
	  This signal is emitted whenever the Shortcut of the Switch has been triggered.
	  */
	void keyTriggered();
protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    QRectF boundingRect() const;
    QPointF mouseDownPos;
	bool createFormBefore();
	QShortcut *shortcut;
	QVariant itemChange(GraphicsItemChange change, const QVariant &value);
signals:

private slots:
	void changeKeySequence(const QKeySequence &seq);
};

#endif // SWITCH_H
