#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QRegExpValidator>

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
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButtonConnect_clicked()
{
    socket.connectToHost(ui->lineEditIp->text(), 54321);
    if (!socket.waitForConnected(3000)){
        ui->textBrowserOutput->append("Не могу соединиться!");
    }
}

void MainWindow::connectedToHost()
{
    ui->textBrowserOutput->append("Соединено!");
    ui->pushButtonSend->setEnabled(true);
}

void MainWindow::disconnectedFromHost()
{

    ui->textBrowserOutput->append("Разъединено!");
    ui->pushButtonSend->setEnabled(false);
}

void MainWindow::readFromHost()
{
    ui->textBrowserOutput->setText(socket.readAll());
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
