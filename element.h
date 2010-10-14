#ifndef ELEMENT_H
#define ELEMENT_H
#include <QGraphicsItem>
#include <QObject>
#include "scene.h"
#include "defines.h"
#include "connection.h"
#include <QFormLayout>
#include <QLineEdit>
#include <QCheckBox>
#include <QLabel>
#include <QSignalMapper>
#include <qxmlstream.h>
#include <QtConcurrentRun>

class Element : public QObject, public QGraphicsItem
{
    Q_OBJECT
    friend class Scene;
    friend class Connection;
public:
    explicit Element(QObject *parent = 0, QGraphicsItem *gparent=0);
    ~Element();
    static QPen getSelectionPen();
    QMap<QString,QVariant> settings();
    void setSettings(QMap<QString, QVariant> s);
    void setInputs(int c);
    void setOutputs(int c);
    void setFormLayout(QFormLayout* layout);
    virtual void setPrivateXml(QXmlStreamWriter* xml);
    virtual void readPrivateXml(QXmlStreamReader *xml);
    void setPos(const QPointF &pos);
    void setPos(qreal x, qreal y);
    virtual bool isInput();
    virtual bool isOutput();
    virtual void setInput(bool value);
    bool value();
    QString name();
signals:
    void outputChanged(bool);
    void moved();
protected slots:
    virtual void recalculate();
private slots:
    void inputChanged();
    void updateName(QWidget* lineEdit);
    void updateNegation(QWidget* checkBox);
    void updateInputs(int);
    void updateOutputs(int);
protected:
    QFormLayout*layout;
    bool myValue;
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
    qreal height, width, minHeight;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    QList<QWidget*> additionalWidgets;
    QMap<Connection*,QLineEdit*> lineEdits;
    QMap<Connection*,QCheckBox*> checkBoxes;
    QSignalMapper lineMapper, checkMapper;
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);
    QString myType;
    virtual void createFormBefore();
    virtual void createFormAfter();
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
private:
    int uniqueId;
    void relayoutConnections();
    void createForm();
    void deleteForm();
    bool isMoving;
};

#endif // ELEMENT_H
