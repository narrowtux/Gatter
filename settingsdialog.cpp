#include "settingsdialog.h"
#include "ui_settingsdialog.h"
#include <QSettings>
#include "scene.h"
SettingsDialog::SettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsDialog)
{
    ui->setupUi(this);
    QSettings settings;
    ui->highValueColor->setColor(settings.value("highValueColor",QColor("red")).value<QColor>());
}

SettingsDialog::~SettingsDialog()
{
    delete ui;
}

void SettingsDialog::on_buttonBox_accepted()
{
    QSettings settings;
    Scene::highValueColor=ui->highValueColor->color();
    settings.setValue("highValueColor",ui->highValueColor->color());
}
