#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QCloseEvent>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButtonConnect_clicked();
    void connectedToHost();
    void disconnectedFromHost();
    void readFromHost();

    void on_pushButtonSend_clicked();

    void on_pushButtonClear_clicked();

    void on_comboBoxAddrs_activated(const QString &arg1);

    void on_comboBoxCmds_activated(const QString &arg1);

    void on_pushButtonSettings_clicked();

    void saveToJson();
    void loadFromJson();

    void on_pushButtonAddIp_clicked();

    void on_pushButtonAddCmd_clicked();

protected:
    void closeEvent(QCloseEvent *event) override;

private:
    Ui::MainWindow *ui;
    QTcpSocket socket;
};
#endif // MAINWINDOW_H
