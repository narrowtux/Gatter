#ifndef CLOCK_H
#define CLOCK_H
#include "src/elements/element.h"


/*!
  \class Clock
  \brief Clock provides a constantly alternating input.
  
  The user can configure its hightime and lowtime (the duration how long the 
  output rests in the specific position).

  These durations can be configured by calling 
  Clock::setLowTime() or Clock::setHighTime()
*/
class Clock : public Element
{
    Q_OBJECT
public:
    explicit Clock(QGraphicsObject *parent = 0);
    void setPrivateXml(QXmlStreamWriter *xml);
    void readPrivateXml(QXmlStreamReader *xml);
signals:

private slots:
    void setLow();
    void setHigh();
public slots:
	/*! 
      Sets the duration how long the clock should rest in the low-position to \a value. 
	  
	  \a value: duration in milliseconds
    */
    void setLowTime(int value);
	/*! 
      Sets the duration how long the clock should rest in the high-position to \a value. 
	  
	  \a value: duration in milliseconds
    */
    void setHighTime(int value);
protected:
    QRectF boundingRect() const;
    QTimer* low;
    QTimer* high;
    bool createFormBefore();
};

#endif // CLOCK_H
