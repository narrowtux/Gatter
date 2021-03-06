#ifndef COLORBUTTON_H
#define COLORBUTTON_H

#include <QToolButton>
#include <QColor>
#include <QMouseEvent>
#include <QPoint>
#include <QColorDialog>
class ColorButton : public QToolButton
{
    Q_OBJECT
public:
    explicit ColorButton(QWidget *parent = 0);
	QColor color();
	void setColor(QColor);
signals:

public slots:
	void cl();
	/*Diese Methode wird ausgeführt, wenn der Button geklickt wurde*/
	void onColorChanged(QColor c);
signals:
	void colorChanged(QColor);
protected:
	QColor m_color;
	void paintEvent(QPaintEvent *e);
	void mousePressEvent(QMouseEvent *e);
	QPoint dragStartPos;
	void mouseMoveEvent(QMouseEvent *e);
	void dragEnterEvent(QDragEnterEvent *e);
	void dropEvent(QDropEvent *e);
	QColorDialog* dialog;
};

#endif // COLORBUTTON_H
