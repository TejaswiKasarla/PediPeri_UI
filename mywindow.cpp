#include "mywindow.h"
#include "ui_mywindow.h"
#include "QString"
#include "QLabel"
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <QTimer>
#include <QDebug>
#include <QtCore>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QtWidgets>


using namespace cv;


MyWindow::MyWindow(QWidget *parent):
    QMainWindow(parent),
    ui(new Ui::MyWindow)
{
    ui->setupUi(this);
    capWebcam.open(0);
    if(capWebcam.isOpened() == false)
    {
        qDebug() <<"webcam is not opened";
        return;
    }
capWebcam.set(CV_CAP_PROP_FPS,120);
tmrTimer = new QTimer(this);
connect(tmrTimer, SIGNAL(timeout()),this,SLOT(capt()));
tmrTimer->start(8);

recTimer = new QTimer(this);



arduino_is_available = false;
arduino_port_name = "";
arduino = new QSerialPort;

    /*qDebug()<< "Number of Available Ports: " <<QSerialPortInfo::availablePorts().length();
    foreach(const QSerialPortInfo &serialPortInfo, QSerialPortInfo::availablePorts())
    {
        qDebug()<< "Has Vendor ID:" <<serialPortInfo.hasVendorIdentifier();
        if(serialPortInfo.hasVendorIdentifier())
        {
        qDebug()<< "Vendor ID:" << serialPortInfo.vendorIdentifier();
        }
        qDebug()<< "Has Product ID:" <<serialPortInfo.hasProductIdentifier();
        if(serialPortInfo.hasProductIdentifier())
        {
        qDebug()<<"Product ID:" <<serialPortInfo.productIdentifier();
        }
    }*/
    foreach(const QSerialPortInfo &serialPortInfo, QSerialPortInfo::availablePorts())
    {
        if(serialPortInfo.hasVendorIdentifier() && serialPortInfo.hasProductIdentifier())
        {
            if(serialPortInfo.vendorIdentifier() == arduino_uno_vendor_id)
            {
                if(serialPortInfo.productIdentifier() == arduino_uno_product_id)
                {
                    arduino_port_name = serialPortInfo.portName();
                    arduino_is_available = true;
                }
            }
        }
    }
    if(arduino_is_available)
    {
        //open and configure the serial port
        arduino->setPortName(arduino_port_name);
        arduino->open(QSerialPort::WriteOnly);
        arduino->setBaudRate(QSerialPort::Baud9600);
        arduino->setDataBits(QSerialPort::Data8);
        arduino->setParity(QSerialPort::NoParity);
        arduino->setStopBits(QSerialPort::OneStop);
        arduino->setFlowControl(QSerialPort::NoFlowControl);
    }
    else
    {
        //give error message
        QMessageBox::warning(this,"Port error","Couldn't find the Arduino!");
    }

}

MyWindow::~MyWindow()
{
        if(arduino->isOpen())
        {
            arduino->close();
        }
    delete ui;
    capWebcam.release();
    video1.release();
}





void MyWindow::on_pushButton_clicked()
{
   if(tmrTimer->isActive()==true)
    {
        tmrTimer->stop();
        ui->pushButton->setText("resume");
    }
    else
    {
        tmrTimer->start(8);
        ui->pushButton->setText("pause");
    }
 capt();

}
void MyWindow::capt()
{ 
        capWebcam.read(frame);
        if(frame.empty()==true) return;

        cv::cvtColor(frame,frame,CV_BGR2RGB);

        QImage qimg((uchar*)frame.data, frame.cols, frame.rows, frame.step, QImage::Format_RGB888);

        ui->label->setPixmap(QPixmap::fromImage(qimg));
}


void MyWindow::on_pushButton_2_clicked()
{
    if(arduino->isWritable())
    {
        arduino->write("h");
        arduino->write(",");
        arduino->write("r");
        arduino->write("\n");
    }
    else
    {
    qDebug() << "Couldn't Write to Serial!" ;
    }

}


void MyWindow::on_pushButton_3_clicked()
{
    if(arduino->isWritable())
    {
        arduino->write("h");
        arduino->write(",");
        arduino->write("l");
        arduino->write("\n");
    }
    else
    {
    qDebug() << "Couldn't Write to Serial!" ;
    }
}



void MyWindow::on_pushButton_4_clicked()
{
    if(arduino->isWritable())
    {
        arduino->write("q");
        arduino->write(",");
        arduino->write("1");
        arduino->write("\n");
    }
    else
    {
    qDebug() << "Couldn't Write to Serial!" ;
    }
}



void MyWindow::on_pushButton_5_clicked()
{
    if(arduino->isWritable())
    {
        arduino->write("q");
        arduino->write(",");
        arduino->write("2");
        arduino->write("\n");
    }
    else
    {
    qDebug() << "Couldn't Write to Serial!" ;
    }
}



void MyWindow::on_pushButton_6_clicked()
{
    if(arduino->isWritable())
    {
        arduino->write("q");
        arduino->write(",");
        arduino->write("3");
        arduino->write("\n");
    }
    else
    {
    qDebug() << "Couldn't Write to Serial!" ;
    }
}

void MyWindow::on_pushButton_7_clicked()
{
    if(arduino->isWritable())
    {
        arduino->write("q");
        arduino->write(",");
        arduino->write("4");
        arduino->write("\n");
    }
    else
    {
    qDebug() << "Couldn't Write to Serial!" ;
    }
}

void MyWindow::hemirightlower()
{
    if(arduino->isWritable())
    {
        arduino->write("h");
        arduino->write(",");
        arduino->write("b");
        arduino->write("\n");
    }
    else
    {
    qDebug() << "Couldn't Write to Serial!" ;
    }
}

void MyWindow::hemileftlower()
{
    if(arduino->isWritable())
    {
        arduino->write("h");
        arduino->write(",");
        arduino->write("a");
        arduino->write("\n");
    }
    else
    {
    qDebug() << "Couldn't Write to Serial!" ;
    }
}

void MyWindow::quad1()
{
    if(arduino->isWritable())
    {
        arduino->write("q");
        arduino->write(",");
        arduino->write("5");
        arduino->write("\n");
    }
    else
    {
    qDebug() << "Couldn't Write to Serial!" ;
    }
}

void MyWindow::quad2()
{
    if(arduino->isWritable())
    {
        arduino->write("q");
        arduino->write(",");
        arduino->write("6");
        arduino->write("\n");
    }
    else
    {
        qDebug() << "Couldn't Write to Serial!" ;
    }
}

void MyWindow::quad3()
{
    if(arduino->isWritable())
    {
        arduino->write("q");
        arduino->write(",");
        arduino->write("7");
        arduino->write("\n");
    }
    else
    {
        qDebug() << "Couldn't Write to Serial!" ;
    }
}
void MyWindow::quad4()
{
    if(arduino->isWritable())
    {
        arduino->write("q");
        arduino->write(",");
        arduino->write("8");
        arduino->write("\n");
    }
    else
    {
        qDebug() << "Couldn't Write to Serial!" ;
    }
}
void MyWindow::on_pushButton_11_clicked()
{
    quad2();
}

void MyWindow::on_pushButton_8_clicked()
{
    quad2();
}

void MyWindow::on_pushButton_9_clicked()
{
    quad2();
}

void MyWindow::on_pushButton_10_clicked()
{
    quad2();
}

void MyWindow::on_pushButton_13_clicked()
{
    quad1();
}

void MyWindow::on_pushButton_15_clicked()
{
    quad1();
}

void MyWindow::on_pushButton_14_clicked()
{
    quad1();
}

void MyWindow::on_pushButton_12_clicked()
{
    quad1();
}

void MyWindow::on_pushButton_16_clicked()
{
    quad4();
}

void MyWindow::on_pushButton_23_clicked()
{
    quad4();
}

void MyWindow::on_pushButton_17_clicked()
{
    quad4();
}

void MyWindow::on_pushButton_20_clicked()
{
    quad3();
}

void MyWindow::on_pushButton_19_clicked()
{
    quad3();
}

void MyWindow::on_pushButton_18_clicked()
{
    quad3();
}

void MyWindow::on_pushButton_21_clicked()
{
    hemirightlower();
}

void MyWindow::on_pushButton_22_clicked()
{
    hemileftlower();
}

void MyWindow::on_pushButton_24_clicked()
{

    char fname[100];
    strcpy(fname, "video1.avi");
    qDebug()<<"created video file";
    VideoWriter video1;
    video1.open(fname,CV_FOURCC('M','J','P','G'),120,cvSize(frame.cols,frame.rows),true);
    connect(recTimer,SIGNAL(timeout()),this, SLOT(recordVideo()));
    qDebug() << "Recording started";
    recTimer->start(100);

}
void MyWindow::recordVideo()
{
    qDebug()<<"Recording phase one";
    bool bSuccess = capWebcam.read(frame); // get a new frame from camera

    qDebug()<< frame.rows<<"";
    if (!bSuccess) {

    qDebug() << "not recording";
    }

    video1.write(frame);
    qDebug()<<"successfully started recording";

}

