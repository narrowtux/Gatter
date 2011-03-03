#ifndef APPLICATION_H
#define APPLICATION_H

#include <QApplication>

/*!
  \class Application
  \brief this class extends QApplication to dispatch QFileOpenEvent
  */

class Application : public QApplication
{
    Q_OBJECT
public:
    explicit Application(int & argc, char ** argv);

signals:

public slots:
protected:
    bool event(QEvent *);

};

#endif // APPLICATION_H
