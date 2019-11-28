#ifndef DIALOGSETTINGS_H
#define DIALOGSETTINGS_H

#include <QDialog>

namespace Ui {
class DialogSettings;
}

class DialogSettings : public QDialog
{
    Q_OBJECT

public:
    explicit DialogSettings(QWidget *parent = nullptr);
    ~DialogSettings();
    void setAddrList(const QStringList& list);
    void setCmdList(const QStringList& list);
    QStringList getAddrList() const;
    QStringList getCmdList() const;

private slots:
    void on_pushButtonAddAddr_clicked();

    void on_pushButtonRemoveAddr_clicked();

    void on_pushButtonAddCmd_clicked();

    void on_pushButtonRemoveCmd_clicked();

private:
    Ui::DialogSettings *ui;
};

#endif // DIALOGSETTINGS_H
