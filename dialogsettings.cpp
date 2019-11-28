#include "dialogsettings.h"
#include "ui_dialogsettings.h"
#include <QInputDialog>

DialogSettings::DialogSettings(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogSettings)
{
    ui->setupUi(this);
}

DialogSettings::~DialogSettings()
{
    delete ui;
}

void DialogSettings::setAddrList(const QStringList &list)
{
    ui->listWidgetAddr->clear();
    ui->listWidgetAddr->addItems(list);
}

void DialogSettings::setCmdList(const QStringList &list)
{
    ui->listWidgetCmd->clear();
    ui->listWidgetCmd->addItems(list);
}

QStringList DialogSettings::getAddrList() const
{
    QStringList sl;
    for (int row = 0; row < ui->listWidgetAddr->count(); row++){
        sl += ui->listWidgetAddr->item(row)->text();
    }
    return sl;
}

QStringList DialogSettings::getCmdList() const
{
    QStringList sl;
    for (int row = 0; row < ui->listWidgetCmd->count(); row++){
        sl += ui->listWidgetCmd->item(row)->text();
    }
    return sl;
}

void DialogSettings::on_pushButtonAddAddr_clicked()
{
    QInputDialog dialog;
    dialog.setLabelText("Введите ip адрес в десятичной нотации");
    if (dialog.exec()){
        ui->listWidgetAddr->addItem(dialog.textValue());
    }
}

void DialogSettings::on_pushButtonRemoveAddr_clicked()
{
   delete ui->listWidgetAddr->takeItem(ui->listWidgetAddr->currentRow());
}

void DialogSettings::on_pushButtonAddCmd_clicked()
{
    QInputDialog dialog;
    dialog.setLabelText("Введите команду");
    if (dialog.exec()){
        ui->listWidgetCmd->addItem(dialog.textValue());
    }
}

void DialogSettings::on_pushButtonRemoveCmd_clicked()
{
   delete ui->listWidgetCmd->takeItem(ui->listWidgetCmd->currentRow());
}
