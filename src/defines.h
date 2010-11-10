#ifndef DEFINES_H
#define DEFINES_H
#include <QGraphicsItem>
#define ElementRecognition QGraphicsItem::UserType+128
#define ElementName QGraphicsItem::UserType+400
#define ElementRotation QGraphicsItem::UserType+1411
enum ConnectionType{Input,Output};
enum Value{Low=0,High=1};

//Element Type defines
#define TunnelID 1


#endif // DEFINES_H
