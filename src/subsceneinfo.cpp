#include "subsceneinfo.h"
#include "scene.h"
#include "subscene.h"

SubSceneInfo::SubSceneInfo(QObject *parent) :
    QObject(parent)
{
    myName=tr("untitled");
    myFileName="";
}

void SubSceneInfo::setFileName(QString fileName){
    myFileName=fileName;
}

void SubSceneInfo::setName(QString name){
    myName=name;
}

QString SubSceneInfo::name(){
    return myName;
}

QString SubSceneInfo::fileName(){
    return myFileName;
}

QIcon SubSceneInfo::icon(){
    if(!myIcon.isNull()){
	return myIcon;
    } else {
	// Create and draw the Icon
	QPixmap icon(64,64);
	QPainter painter(&icon);
	painter.setBrush(QColor(255,255,255));
	painter.setPen(QColor(255,255,255));
	painter.drawRect(icon.rect());
	painter.setPen(QColor(0,0,0));
	painter.drawRect(icon.rect().adjusted(10,0,-10,-1));
	QFont font;
	font.setPixelSize(12);
	font.setFamily("sans-serif");
	painter.setFont(font);
	if(myFileName!=""){
	    SubScene* scene=new SubScene(0, false);
	    scene->loadFromFile(myFileName,false);
	    int leftC=scene->sceneInputs.count(), rightC=scene->sceneOutputs.count();
	    qreal spaceLeft=64/(leftC+1), spaceRight=64/(rightC+1);
	    for(int i=1;i<=leftC;i++){
		qreal y=i*spaceLeft;
		painter.drawLine(0,y,10,y);
		QString text=scene->sceneInputs.at(i-1)->name();
		if(text.contains('*')){
		    text.replace('*',"");
		    font.setOverline(true);
		    painter.setFont(font);
		}
		painter.drawText(12,y+6,text);
		font.setOverline(false);
		painter.setFont(font);
	    }
	    for(int i=1;i<=rightC;i++){
		qreal y=i*spaceRight;
		painter.drawLine(64-10,y,64,y);
		QString text=scene->sceneOutputs.at(i-1)->name();
		if(text.contains('*')){
		    text.replace('*',"");
		    font.setOverline(true);
		    painter.setFont(font);
		}
		painter.drawText(44,y+6,text);
		font.setOverline(false);
		painter.setFont(font);
	    }
	    delete scene;
	}
	myIcon=QIcon(icon);
	return myIcon;
    }
}

void SubSceneInfo::setProtected(bool p){
    myProtected=p;
}

bool SubSceneInfo::isProtected(){
    return myProtected;
}

void SubSceneInfo::setIcon(QIcon icon){
    myIcon=icon;
}
