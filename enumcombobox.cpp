#include "enumcombobox.h"

EnumComboBox::EnumComboBox(QWidget *parent) :
    QComboBox(parent)
{
    connect(this,SIGNAL(currentIndexChanged(int)),this,SLOT(onIndexChanged(int)));
}

void EnumComboBox::addItems(QMap<int, QString> enumStrings){
    clear();
    myEnumString=enumStrings;
    foreach(int type,myEnumString.keys()){
	myIntEnum<<type;
	myIntString<<myEnumString.value(type);
    }
    QComboBox::addItems(myIntString);
    setCurrentIndex(0);
}

void EnumComboBox::onIndexChanged(int index){
    if(index+1<=myIntEnum.count()&&index>=0){
	emit(currentEnumIndexChanged(myIntEnum.at(index)));
    }
}

void EnumComboBox::setCurrentIndex(int type){
    int index=myIntEnum.indexOf(type);
    QComboBox::setCurrentIndex(index);
}

void EnumComboBox::clear(){
    QComboBox::clear();
    myIntEnum.clear();
    myEnumString.clear();
    myIntString.clear();
}
