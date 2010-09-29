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

class Element : public QObject, public QGraphicsItem
{
    Q_OBJECT
    friend class Scene;
public:
    explicit Element(QObject *parent = 0, QGraphicsItem *gparent=0);
    ~Element();
    static QPen getSelectionPen();
    QMap<QString,QVariant> settings();
    void setSettings(QMap<QString, QVariant> s);
    void setInputs(int c);
    void setOutputs(int c);
    void setFormLayout(QFormLayout* layout);
signals:

private slots:
    void inputChanged();
    void updateName(QWidget* lineEdit);
    void updateNegation(QWidget* checkBox);
    void updateInputs(int);
    void updateOutputs(int);
protected:
    QFormLayout*layout;
    virtual void createForm();
    virtual void deleteForm();
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
    qreal height, width;
    virtual void recalculate();
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    QList<QWidget*> additionalWidgets;
    QMap<Connection*,QLineEdit*> lineEdits;
    QMap<Connection*,QCheckBox*> checkBoxes;
    QSignalMapper lineMapper, checkMapper;
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);
private:
    int uniqueId;
    void relayoutConnections();
};

#endif // ELEMENT_H
