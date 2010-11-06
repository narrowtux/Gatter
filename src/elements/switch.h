#ifndef SWITCH_H
#define SWITCH_H

#include "src/defines.h"
#include "src/elements/element.h"

class Switch : public Element
{
    Q_OBJECT
public:
    explicit Switch(QGraphicsObject *parent = 0);
	~Switch();
    void setPrivateXml(QXmlStreamWriter *xml);
    void readPrivateXml(QXmlStreamReader *xml);
    bool isInput();
    void setInput(bool value);
public slots:
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
