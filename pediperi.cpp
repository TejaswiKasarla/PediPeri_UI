#include "pediperi.h"
#include "ui_pediperi.h"
#include "QFile"
#include "QString"
#include "QTextStream"
#include "mywindow.h"
#include "QDir"


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
    QString Gender = ui->comboBox->currentText();
    if(QDir("Patient Data").exists())
    {
        QDir().setCurrent("Patient Data");
    }
    else
    {
        QDir().mkdir("Patient Data");
        QDir().setCurrent("Patient Data");
    }
    if (QDir(ui->lineEdit_3->text()).exists())
    {
        QDir().setCurrent(ui->lineEdit_3->text());
    }
    else
    {
        QDir().mkdir(ui->lineEdit_3->text());
        QDir().setCurrent(ui->lineEdit_3->text());
    }
    QString filename = ui->lineEdit_3->text()+".txt";
    QFile file(filename);
    file.open(QIODevice::WriteOnly|QIODevice::Text);
    QTextStream out(&file);
    out<<"Name: "<<Name<<endl;
    out<<"Age: "<<Age<<endl;
    out<<"EMR No.: "<<EMR<<endl;
    out<<"Gender: "<<Gender<<endl;
    out<<"Description of Patient: "<<Description<<endl;
    QDir().setCurrent(ui->lineEdit_3->text());
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

