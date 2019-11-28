#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QRegExpValidator>
#include "dialogsettings.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>
#include <QDateTime>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QString ipRange = "(?:[0-1]?[0-9]?[0-9]|2[0-4][0-9]|25[0-5])";
    QRegExp ipRegex ("^" + ipRange
                     + "\\." + ipRange
                     + "\\." + ipRange
                     + "\\." + ipRange + "$");
    QRegExpValidator *ipValidator = new QRegExpValidator(ipRegex, this);
    ui->lineEditIp->setValidator(ipValidator);

    connect(&socket, SIGNAL(connected()), this, SLOT(connectedToHost()));
    connect(&socket, SIGNAL(disconnected()), this, SLOT(disconnectedFromHost()));
    connect(&socket, SIGNAL(readyRead()), this, SLOT(readFromHost()));
    ui->textBrowserOutput->setReadOnly(true);

    loadFromJson();
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButtonConnect_clicked()
{
    socket.connectToHost(ui->lineEditIp->text(), 54321);
    if (!socket.waitForConnected(3000)){
        ui->textBrowserOutput->append(QDateTime::currentDateTime().toString() + " - Не могу соединиться!");
    }
}

void MainWindow::connectedToHost()
{
    ui->textBrowserOutput->append(QDateTime::currentDateTime().toString() + " - Соединено!");
    ui->pushButtonSend->setEnabled(true);
}

void MainWindow::disconnectedFromHost()
{

    ui->textBrowserOutput->append(QDateTime::currentDateTime().toString() + " - Разъединено!");
    ui->pushButtonSend->setEnabled(false);
}

void MainWindow::readFromHost()
{
    ui->textBrowserOutput->append(QDateTime::currentDateTime().toString() + " - " + socket.readAll());
}

void MainWindow::on_pushButtonSend_clicked()
{
    QByteArray cmd;
    cmd.append(ui->lineEditCmd->text());
    socket.write(cmd);
}

void MainWindow::on_pushButtonClear_clicked()
{
   ui->textBrowserOutput->clear();
}

void MainWindow::on_comboBoxAddrs_activated(const QString &arg1)
{
   ui->lineEditIp->setText(arg1);
}

void MainWindow::on_comboBoxCmds_activated(const QString &arg1)
{
   ui->lineEditCmd->setText(arg1);
}

void MainWindow::on_pushButtonSettings_clicked()
{
    DialogSettings dialog;
    QStringList listOfCmds;
    for (int row = 0; row < ui->comboBoxCmds->count(); row++){
       listOfCmds.append(ui->comboBoxCmds->itemText(row)) ;
    }
    dialog.setCmdList(listOfCmds);
    QStringList listOfAddrs;
    for (int row = 0; row < ui->comboBoxAddrs->count(); row++){
       listOfAddrs.append(ui->comboBoxAddrs->itemText(row)) ;
    }
    dialog.setAddrList(listOfAddrs);
    if (dialog.exec()){
       ui->comboBoxCmds->clear();
       ui->comboBoxAddrs->clear();
       ui->comboBoxCmds->addItems(dialog.getCmdList());
       ui->comboBoxAddrs->addItems(dialog.getAddrList());
    }
}

void MainWindow::saveToJson()
{
    QJsonObject object;
    QJsonArray addrs;
    for (int row = 0; row < ui->comboBoxAddrs->count(); row++){
       addrs.append(QJsonValue(ui->comboBoxAddrs->itemText(row)));
    }
    object["addrs"] = addrs;
    QJsonArray cmds;
    for (int row = 0; row < ui->comboBoxCmds->count(); row++){
       cmds.append(QJsonValue(ui->comboBoxCmds->itemText(row)));
    }
    object["cmds"] = cmds;

    QString val = QJsonDocument( object).toJson();

    QFile file;
    file.setFileName("settings.ini");
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)){
        file.write(val.toStdString().c_str());
        file.close();
    }
}

void MainWindow::loadFromJson()
{
    QString val;
    QFile file;
    file.setFileName("settings.ini");
    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        val = file.readAll();
        file.close();
        QJsonDocument d = QJsonDocument::fromJson(val.toUtf8());
        QJsonObject sett2 = d.object();
        QJsonValue addrValue = sett2.value(QString("addrs"));
        QJsonArray addrs = addrValue.toArray();
        for (auto a: addrs){
            ui->comboBoxAddrs->addItem(a.toString());
        }
        QJsonValue cmdValue = sett2.value(QString("cmds"));
        QJsonArray cmds = cmdValue.toArray();
        for (auto c: cmds){
            ui->comboBoxCmds->addItem(c.toString());
        }
    }
}

void MainWindow::on_pushButtonAddIp_clicked()
{
   ui->comboBoxAddrs->addItem(ui->lineEditIp->text());
}

void MainWindow::on_pushButtonAddCmd_clicked()
{
   ui->comboBoxCmds->addItem(ui->lineEditCmd->text());
}

void MainWindow::closeEvent(QCloseEvent *event)
{
   saveToJson();
}
