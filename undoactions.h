#ifndef UNDOACTIONS_H
#define UNDOACTIONS_H
#include <QUndoCommand>
#include "scene.h"
#include "gatter.h"
#include "switch.h"
#include "lamp.h"
#include "button.h"
#include "clock.h"
#include "subscene.h"
#include "delay.h"
#include "flipflop.h"
#include "hexoutput.h"
#include "mainwindow.h"


class AddElement : public QUndoCommand
{
public:
    AddElement(Element* e, QPointF pos, MainWindow*m){
	setText(QApplication::tr("Add Element"));
	myGenericElement=e;
	myAddPos=pos;
	myMainWindow=m;
    }
    
    virtual void undo(){
	myMainWindow->scene()->removeElement(myGenericElement);
    }

    virtual void redo(){
	myMainWindow->scene()->addElement(myGenericElement);
	myGenericElement->setPos(myAddPos);
    }
protected:

    QPointF myAddPos;
    Element* myGenericElement;
    MainWindow* myMainWindow;
};

class RemoveElement : public QUndoCommand
{
public:
    RemoveElement(Element* e, MainWindow* m){
	myGenericElement=e;
	myMainWindow=m;
	setText(QApplication::tr("Remove Element"));
    }
    
    virtual void undo(){
	myMainWindow->scene()->addElement(myGenericElement);
	myGenericElement->setPos(pos);
    }
    
    virtual void redo(){
	pos=myGenericElement->pos();
	myMainWindow->scene()->removeElement(myGenericElement);
    }
private:
    MainWindow *myMainWindow;
    Element* myGenericElement;
    QPointF pos;
};

class MoveElement : public QUndoCommand
{
public:
    MoveElement(Element*e, QPointF old, MainWindow* m){
	myGenericElement=e;
	myMainWindow=m;
	oldPos=old;
	newPos=e->pos();
	setText(QApplication::tr("Move Element"));
    }
    
    virtual void undo(){
	myGenericElement->setPos(oldPos);
    }
    
    virtual void redo(){
	myGenericElement->setPos(newPos);
    }
    
protected:
    QPointF oldPos, newPos;
    Element* myGenericElement;
    MainWindow* myMainWindow;
};

#endif // UNDOACTIONS_H
