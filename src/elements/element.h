#ifndef ELEMENT_H
#define ELEMENT_H
#include <QGraphicsObject>
#include <QObject>
#include "src/scene/scene.h"
#include "src/defines.h"
#include "src/connection.h"
#include <QFormLayout>
#include <QLineEdit>
#include <QCheckBox>
#include <QLabel>
#include <QSignalMapper>
#include <qxmlstream.h>
#include <QtConcurrentRun>
#include "src/widgets/colorbutton.h"
#include <QPropertyAnimation>

class Element : public QGraphicsObject
{
	Q_PROPERTY(qreal selectionOpacity READ selectionOpacity WRITE setSelectionOpacity)
    Q_OBJECT
    friend class Scene;
    friend class Connection;
public:
	static qreal rotationSteps;
	
    explicit Element(QGraphicsObject *parent = 0);
    ~Element();
    static QPen getSelectionPen();
    QMap<QString,QVariant> settings();
    void setSettings(QMap<QString, QVariant> s);
    void setInputs(int c);
    void setOutputs(int c);
    void setFormLayout(QFormLayout* layout);
    virtual void setPrivateXml(QXmlStreamWriter *xml);
    virtual void readPrivateXml(QXmlStreamReader *xml);
    void setPos(const QPointF &pos);
    void setPos(qreal x, qreal y);
    virtual bool isInput();
    virtual bool isOutput();
    virtual void setInput(bool value);
    bool value();
    QString name();
    int count(const char* propName);
    QColor elementColor();
    void releaseConnections();
	QPainterPath shape() const;
	QList<Connection*> inputs();
	QList<Connection*> outputs();
	qreal selectionOpacity();
	void setSelectionOpacity(qreal op);
	QString title();
public slots:
	void setTitle(QString title);
    void setElementColor(QColor c);
signals:
    void outputChanged(bool);
    void moved();
protected slots:
    virtual void recalculate();
protected:
    QFormLayout*layout;
    bool myValue;
    QColor myElementColor;
    int minInputs,minOutputs,maxInputs,maxOutputs;
    bool pressed;
    QList<Connection*> myInputs;
    QList<Connection*> myOutputs;
    void addInput(Connection* i=0);
    void addInput(int c);
    void addOutput(Connection* o=0);
    void addOutput(int c);
    void removeInput(Connection* i=0);
    void removeInput(int c);
    void removeOutput(Connection* o=0);
    void removeOutput(int c);
    void setMinMaxInputsOutputs(int minIn, int maxIn, int minOut, int maxOut);
    qreal height, width, minHeight, minWidth;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    QList<QWidget*> additionalWidgets;
    QMap<Connection*,QLineEdit*> lineEdits;
    QMap<Connection*,QCheckBox*> checkBoxes;
    QSignalMapper lineMapper, checkMapper;
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);
    QString myType;
    virtual bool createFormBefore();
    virtual void createFormAfter();
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    virtual void relayoutConnections();
	virtual void connectionsChanged();
	qreal mySelectionOpacity;
	QString myTitle;
	virtual void loadEvent();
private:
    int uniqueId;
    void createForm();
    void deleteForm();
    bool isMoving;
	QPropertyAnimation *mySelectionOpacityAnimation;
	bool myIsSelected;
private slots:
    void inputChanged();
    void updateName(QWidget* lineEdit);
    void updateNegation(QWidget* checkBox);
    void updateInputs(int);
    void updateOutputs(int);
	void selectionUpdated();
};

#endif // ELEMENT_H
