#ifndef PEDIPERI_H
#define PEDIPERI_H

#include <QMainWindow>
#include "mywindow.h"

namespace Ui {
class PediPeri;
class Interface;
}

class PediPeri : public QMainWindow
{
    Q_OBJECT

public:
    explicit PediPeri(QWidget *parent = 0);
    ~PediPeri();

private slots:
    void on_pushButton_4_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::PediPeri *ui;
    MyWindow *mw;
    void writefile();
    void cleartext();

};

#endif // PEDIPERI_H
