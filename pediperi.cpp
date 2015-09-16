#include "pediperi.h"
#include "ui_pediperi.h"
#include "QFile"
#include "QString"
#include "QTextStream"

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
    system ("start C:/Users/TSMS/Documents/PediPeri_Test/bin/Debug/PediPeri_Test.exe");
    PediPeri::close();


}

void PediPeri::on_pushButton_2_clicked()
{
    cleartext();
}

void PediPeri::cleartext()
{

}

