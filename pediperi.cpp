#include "pediperi.h"
#include "ui_pediperi.h"
#include "QFile"
#include "QString"
#include "QTextStream"
#include "mywindow.h"


PediPeri::PediPeri(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::PediPeri)
{
    ui->setupUi(this);
}

PediPeri::~PediPeri()
{
    delete ui;
}

void PediPeri::on_pushButton_4_clicked()
{
    writefile();
    mw = new MyWindow(this);
    mw->show();


}
void PediPeri::writefile()
{
    QString Name = ui->lineEdit->text();
    QString Age = ui->lineEdit_2->text();
    QString EMR = ui->lineEdit_3->text();
    QString Description = ui->textEdit->toPlainText();
    //QString Gender = ui->comboBox->
    QString filename = "data.txt";
    QFile file(filename);
    file.open(QIODevice::WriteOnly|QIODevice::Text);
    QTextStream out(&file);
    out<<"Name: "<<Name<<endl;
    out<<"Age: "<<Age<<endl;
    out<<"EMR No.: "<<EMR<<endl;
    //out<<"Gender: "<<Gender<<endl;
    out<<"Description of Patient: "<<Description<<endl;
    file.close();
    PediPeri::close();
}

void PediPeri::on_pushButton_2_clicked()
{
    cleartext();
}

void PediPeri::cleartext()
{
    this->ui->lineEdit->clear();
    this->ui->lineEdit_2->clear();
    this->ui->lineEdit_3->clear();
    this->ui->textEdit->clear();
    this->ui->comboBox->clear();
}

