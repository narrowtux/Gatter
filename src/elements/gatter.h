#ifndef GATTER_H
#define GATTER_H
#include "src/elements/element.h"
#include "src/defines.h"
#include <QTimer>
/*!
  \class Gatter
  \brief Gatter contains the simulation for all basic gatters such as AND, OR, XOR and NOT.
  */
class Gatter : public Element
{
    Q_OBJECT
public:
	/*!
	  Creates a new Gatter as a child of \a parent
	  */
    explicit Gatter(QGraphicsObject *parent = 0);
	
	/*!
	  Describes the type of the Gatter.
	  
	  \see Gatter::setType()
	  \see Gatter::type()
	  */
    enum Type
	{
		/*!
		  AND will only result true if all inputs are true
		  */
		AND,
		/*! 
		  NOT will result true if the input is false
		  */
		NOT,
		/*! 
		  OR will result true when one or more of the inputs are true
		  */
		OR,
		/*!
		  XOR will result true when only one of the inputs is true
		  */
		XOR,
		/*!
		  DUPLICATOR will give its input value to all of its outputs
		  \deprecated The DUPLICATOR has been replaced with an own Element, 
          the Distributor. It provides a more standard look. DUPLICATOR will
		  only be used when an old file references it.
		  */
		DUPLICATOR,
		/*!
		  ICON will display the Gatter-application icon.
		  */
		ICON
	};
    QSize sizeHint() const;
    QRectF boundingRect() const;
	/*!
	  Sets the Type of the Gatter to \a t.
	  
	  \see Gatter::type()
	  \see Gatter::Type
	  */
    void setType(Type t);
	/*!
	  \returns the Type of the Gatter.
	  
	  \see Gatter::setType()
      \see Gatter::Type
	  */
    Type type();
    void recalculate();
    static int delayMS;
    void setPrivateXml(QXmlStreamWriter *xml);
    void readPrivateXml(QXmlStreamReader *xml);
signals:
    void changed();
public slots:
    void sendChanges();
protected:
    Type myGatterType;
    bool beforeValue;
    bool beforeUndefined;
    QTimer* delay;
};

#endif // GATTER_H
