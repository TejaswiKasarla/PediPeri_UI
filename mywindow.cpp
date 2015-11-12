#include "mywindow.h"
#include "ui_mywindow.h"


#include <QtCore>
#include <QtSerialPort>
#include <QtWidgets>
#include <QDebug>
#include <QtMath>

#include <opencv/cv.h>
#include <opencv/highgui.h>

#include <boost/thread.hpp>
#include "boost/date_time/posix_time/posix_time.hpp"


using namespace std;
using namespace boost::posix_time;
using namespace cv;



MyWindow::MyWindow(QWidget *parent):
    QMainWindow(parent),
    ui(new Ui::MyWindow)
{
    ui->setupUi(this);

    ui->label_6->hide();

    capWebcam.open(0);
    if(capWebcam.isOpened() == false)
    {
        return;
    }

tmrTimer = new QTimer(this);
connect(tmrTimer, SIGNAL(timeout()),this,SLOT(capt()));
tmrTimer->start(1);

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
        arduino->open(QSerialPort::ReadWrite);
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


    connect(arduino,SIGNAL(readyRead()),this,SLOT(serialReceived()));

}

MyWindow::~MyWindow()
{
    if(arduino->isOpen())
    {
        arduino->close();
    }
    delete ui;
    capWebcam.release();

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
        tmrTimer->start(1);
        ui->pushButton->setText("pause");
    }
    capt();
}

void MyWindow::capt()
{ 
    bool bSuccess = capWebcam.read(frame); // get a new frame from camera

    if(frame.empty()==true) return;

    i++;
    std::ostringstream strs;
    strs << i;
    std::string str = strs.str();
    putText(frame, strs.str() , cvPoint(15,20),FONT_HERSHEY_COMPLEX_SMALL, 0.8, cvScalar(200,255,250), 1, CV_AA);
    std::string stringshow = updatestr.toStdString();
    putText(frame,stringshow, cvPoint(300,450),FONT_HERSHEY_COMPLEX_SMALL,0.8,cvScalar(200,255,200),1,CV_AA);

    if (!bSuccess)
    {

        ui->label_5->setText("Not Recording!");
    }

    cv::cvtColor(frame,frame,CV_BGR2RGB);

    QImage qimg((uchar*)frame.data, frame.cols, frame.rows, frame.step, QImage::Format_RGB888);

    ui->label->setPixmap(QPixmap::fromImage(qimg));

}

void captureFunc(Mat *frame, VideoCapture *capture){
  //loop infinitely
  for(;;){
        //capture from webcame to Mat frame
        (*capture) >> (*frame);

    }
}

void capture_image(VideoCapture capWebcam)
{
    time_duration td, td1;
    ptime nextFrameTimestamp, currentFrameTimestamp, initialLoopTimestamp, finalLoopTimestamp;
    int delayFound;

    // set framerate to record and capture at
    int framerate = 120;

    // Get the properties from the camera
    double width = capWebcam.get(CV_CAP_PROP_FRAME_WIDTH);
    double height = capWebcam.get(CV_CAP_PROP_FRAME_HEIGHT);

    // Create a matrix to keep the retrieved frame
    Mat frame;

    // Create the video writer
    VideoWriter video("capture.avi",CV_FOURCC('D','I','V','X'), framerate, cvSize(640,480) );

    // initialize initial timestamps
    nextFrameTimestamp = microsec_clock::local_time();
    currentFrameTimestamp = nextFrameTimestamp;
    td = (currentFrameTimestamp - nextFrameTimestamp);

    // start thread to begin capture and populate Mat frame
    boost::thread captureThread(captureFunc, &frame, &capWebcam);

    int i=0;
    for(;;)
    {

        // wait for X microseconds until 1second/framerate time has passed after previous frame write
        while(td.total_microseconds() < 1000000/framerate)
        {
        //determine current elapsed time
            currentFrameTimestamp = microsec_clock::local_time();
            td = (currentFrameTimestamp - nextFrameTimestamp);
        }

        //determine time at start of write
        initialLoopTimestamp = microsec_clock::local_time();

        if((frame.rows && frame.cols))
        {
            std::ostringstream strs;
            strs << i;
            std::string str = strs.str();
            putText(frame, strs.str() , cvPoint(100,100),FONT_HERSHEY_COMPLEX_SMALL, 0.8, cvScalar(200,255,250), 1, CV_AA);
            waitKey(1) && 0xFF;
            i++;
        }

        // Save frame to video
        video << frame;
        //MyWindow obj;
        //obj.ui->label_5->setText("recording");

        // add 1second/framerate time for next loop pause
        nextFrameTimestamp = nextFrameTimestamp + microsec(1000000/framerate);

        // reset time_duration so while loop engages
        td = (currentFrameTimestamp - nextFrameTimestamp);

        //determine and print out delay in ms, should be less than 1000/FPS
        //occasionally, if delay is larger than said value, correction will occur
        //if delay is consistently larger than said value, then CPU is not powerful
        // enough to capture/decompress/record/compress that fast.
        finalLoopTimestamp = microsec_clock::local_time();
        td1 = (finalLoopTimestamp - initialLoopTimestamp);
        delayFound = td1.total_milliseconds();
        //cout << delayFound << endl;

        //output will be in following format
        //[TIMESTAMP OF PREVIOUS FRAME] [TIMESTAMP OF NEW FRAME] [TIME DELAY OF WRITING]

    }

    // Exit
}


void MyWindow::on_pushButton_2_clicked()
{
    if(arduino->isWritable())
    {
        updatestr = "Hemisphere, right";
        arduino->write("h");
        arduino->write(",");
        arduino->write("r");
        arduino->write("\n");
    }
    else
    {
        ui->label_5->setText("Couldn't write to arduino! Please reconnect.");
    }

}


void MyWindow::on_pushButton_3_clicked()
{
    if(arduino->isWritable())
    {
        updatestr = "Hemisphere, left";
        arduino->write("h");
        arduino->write(",");
        arduino->write("l");
        arduino->write("\n");
    }
    else
    {
        ui->label_5->setText("Couldn't write to arduino! Please reconnect.");
    }
}



void MyWindow::on_pushButton_4_clicked()
{
    if(arduino->isWritable())
    {
        updatestr = "Quadrant, bottom right";
        arduino->write("q");
        arduino->write(",");
        arduino->write("1");
        arduino->write("\n");
    }
    else
    {
        ui->label_5->setText("Couldn't write to arduino! Please reconnect.");
    }
}



void MyWindow::on_pushButton_5_clicked()
{
    if(arduino->isWritable())
    {
        updatestr = "Quadrant, top right";
        arduino->write("q");
        arduino->write(",");
        arduino->write("2");
        arduino->write("\n");
    }
    else
    {
        ui->label_5->setText("Couldn't write to arduino! Please reconnect.");
    }
}



void MyWindow::on_pushButton_6_clicked()
{
    if(arduino->isWritable())
    {
        updatestr = "Quadrant, top left";
        arduino->write("q");
        arduino->write(",");
        arduino->write("3");
        arduino->write("\n");
    }
    else
    {
        ui->label_5->setText("Couldn't write to arduino! Please reconnect.");
    }
}

void MyWindow::on_pushButton_7_clicked()
{
    if(arduino->isWritable())
    {
        updatestr = "Quadrant, bottom left";
        arduino->write("q");
        arduino->write(",");
        arduino->write("4");
        arduino->write("\n");
    }
    else
    {
        ui->label_5->setText("Couldn't write to arduino! Please reconnect.");
    }
}

void MyWindow::hemirightlower()
{
    if(arduino->isWritable())
    {
        updatestr = "Hemisphere, right inner";
        arduino->write("h");
        arduino->write(",");
        arduino->write("b");
        arduino->write("\n");
    }
    else
    {
        ui->label_5->setText("Couldn't write to arduino! Please reconnect.");
    }
}

void MyWindow::hemileftlower()
{
    if(arduino->isWritable())
    {
        updatestr = "Hemisphere, left inner";
        arduino->write("h");
        arduino->write(",");
        arduino->write("a");
        arduino->write("\n");
    }
    else
    {
        ui->label_5->setText("Couldn't write to arduino! Please reconnect.");
    }
}

void MyWindow::quad1()
{
    if(arduino->isWritable())
    {
        updatestr = "Quadrant, bottom right inner";
        arduino->write("q");
        arduino->write(",");
        arduino->write("5");
        arduino->write("\n");
    }
    else
    {
        ui->label_5->setText("Couldn't write to arduino! Please reconnect.");
    }
}

void MyWindow::quad2()
{
    if(arduino->isWritable())
    {
        updatestr = "Quadrant, top right inner";
        arduino->write("q");
        arduino->write(",");
        arduino->write("6");
        arduino->write("\n");
    }
    else
    {
        ui->label_5->setText("Couldn't write to arduino! Please reconnect.");
    }
}

void MyWindow::quad3()
{
    if(arduino->isWritable())
    {
        updatestr = "Quadrant, top left inner";
        arduino->write("q");
        arduino->write(",");
        arduino->write("7");
        arduino->write("\n");
    }
    else
    {
        ui->label_5->setText("Couldn't write to arduino! Please reconnect.");
    }
}
void MyWindow::quad4()
{
    if(arduino->isWritable())
    {
        updatestr = "Quadrant, bottom left inner";
        arduino->write("q");
        arduino->write(",");
        arduino->write("8");
        arduino->write("\n");
    }
    else
    {
        ui->label_5->setText("Couldn't write to arduino! Please reconnect.");
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


void MyWindow::on_pushButton_25_clicked()
{

    if(arduino->isWritable())
    {
        degree = 23;
        arduino->write("s");
        arduino->write(",");
        arduino->write("50");
        arduino->write("\n");

    }
    else
    {
        ui->label_5->setText("Couldn't write to arduino! Please reconnect.");
    }
}

void MyWindow::on_pushButton_26_clicked()
{
    degree = 13;
    if(arduino->isWritable())
    {
        arduino->write("s");
        arduino->write(",");
        arduino->write("22");
        arduino->write("\n");

    }
    else
    {
        ui->label_5->setText("Couldn't write to arduino! Please reconnect.");
    }
}

void MyWindow::serialReceived()
{
    //qDebug()<<"received";
    inByteArray = arduino->read(3);
    QString temp = QString::fromStdString(inByteArray.toStdString());
    int qarray = temp.toInt();
    qDebug()<<qarray;
    if(qarray>1)
    {
        ang = ((degree + 1 )*15);    // we will be re-drawing each time for all meridians. This is due to the refresh-methodology on which processing operates.
        float rad = qDegreesToRadians(ang);
        float multipler = 25 + (qarray - 2)*22 ;
        int x = int(1090.00 + qCos(rad)* multipler);
        int y = int(190.00  - qSin(rad)* multipler);
        ui->label_6->show();
        ui->label_6->move(x,y);
    }
    else
    {
        ui->label_6->hide();
    }

}

void MyWindow::on_pushButton_39_clicked()
{
    degree = 0;
    if(arduino->isWritable())
    {
        arduino->write("s");
        arduino->write(",");
        arduino->write("48");
        arduino->write("\n");
    }
    else
    {
        ui->label_5->setText("Couldn't write to arduino! Please reconnect.");
    }
}

void MyWindow::on_pushButton_38_clicked()
{
    degree = 1;
    if(arduino->isWritable())
    {
        arduino->write("s");
        arduino->write(",");
        arduino->write("46");
        arduino->write("\n");
    }
    else
    {
        ui->label_5->setText("Couldn't write to arduino! Please reconnect.");
    }
}

void MyWindow::on_pushButton_37_clicked()
{
    degree = 2;
    if(arduino->isWritable())
    {
        arduino->write("s");
        arduino->write(",");
        arduino->write("44");
        arduino->write("\n");
    }
    else
    {
        ui->label_5->setText("Couldn't write to arduino! Please reconnect.");
    }
}

void MyWindow::on_pushButton_36_clicked()
{
    degree = 3;
    if(arduino->isWritable())
    {
        arduino->write("s");
        arduino->write(",");
        arduino->write("42");
        arduino->write("\n");
    }
    else
    {
        ui->label_5->setText("Couldn't write to arduino! Please reconnect.");
    }
}

void MyWindow::on_pushButton_35_clicked()
{
    degree = 4;
    if(arduino->isWritable())
    {
        arduino->write("s");
        arduino->write(",");
        arduino->write("40");
        arduino->write("\n");
    }
    else
    {
        ui->label_5->setText("Couldn't write to arduino! Please reconnect.");
    }
}

void MyWindow::on_pushButton_34_clicked()
{
    degree = 5;
    if(arduino->isWritable())
    {
        arduino->write("s");
        arduino->write(",");
        arduino->write("38");
        arduino->write("\n");
    }
    else
    {
        ui->label_5->setText("Couldn't write to arduino! Please reconnect.");
    }
}

void MyWindow::on_pushButton_33_clicked()
{
    degree = 6;
    if(arduino->isWritable())
    {
        arduino->write("s");
        arduino->write(",");
        arduino->write("36");
        arduino->write("\n");
    }
    else
    {
        ui->label_5->setText("Couldn't write to arduino! Please reconnect.");
    }
}

void MyWindow::on_pushButton_32_clicked()
{
    degree = 7;
    if(arduino->isWritable())
    {
        arduino->write("s");
        arduino->write(",");
        arduino->write("34");
        arduino->write("\n");

    }
    else
    {
        ui->label_5->setText("Couldn't write to arduino! Please reconnect.");

    }
}

void MyWindow::on_pushButton_31_clicked()
{
    degree = 8;
    if(arduino->isWritable())
    {
        arduino->write("s");
        arduino->write(",");
        arduino->write("32");
        arduino->write("\n");
    }
    else
    {
        ui->label_5->setText("Couldn't write to arduino! Please reconnect.");
    }
}

void MyWindow::on_pushButton_30_clicked()
{
    degree = 9;
    if(arduino->isWritable())
    {
        arduino->write("s");
        arduino->write(",");
        arduino->write("30");
        arduino->write("\n");
    }
    else
    {
        ui->label_5->setText("Couldn't write to arduino! Please reconnect.");
    }
}

void MyWindow::on_pushButton_29_clicked()
{
    degree = 10;
    if(arduino->isWritable())
    {
        arduino->write("s");
        arduino->write(",");
        arduino->write("28");
        arduino->write("\n");
    }
    else
    {
        ui->label_5->setText("Couldn't write to arduino! Please reconnect.");
    }
}

void MyWindow::on_pushButton_28_clicked()
{
    degree = 11;
    if(arduino->isWritable())
    {
        arduino->write("s");
        arduino->write(",");
        arduino->write("26");
        arduino->write("\n");
    }
    else
    {
        ui->label_5->setText("Couldn't write to arduino! Please reconnect.");
    }
}

void MyWindow::on_pushButton_27_clicked()
{
    degree = 12;
    if(arduino->isWritable())
    {
        arduino->write("s");
        arduino->write(",");
        arduino->write("24");
        arduino->write("\n");
    }
    else
    {
        ui->label_5->setText("Couldn't write to arduino! Please reconnect.");
    }
}

void MyWindow::on_pushButton_48_clicked()
{
    degree = 14;
    if(arduino->isWritable())
    {
        arduino->write("s");
        arduino->write(",");
        arduino->write("37");
        arduino->write("\n");
    }
    else
    {
        ui->label_5->setText("Couldn't write to arduino! Please reconnect.");
    }
}

void MyWindow::on_pushButton_47_clicked()
{
    degree = 15;
    if(arduino->isWritable())
    {
        arduino->write("s");
        arduino->write(",");
        arduino->write("35");
        arduino->write("\n");
    }
    else
    {
        ui->label_5->setText("Couldn't write to arduino! Please reconnect.");
    }
}

void MyWindow::on_pushButton_46_clicked()
{
    degree = 16;
    if(arduino->isWritable())
    {
        arduino->write("s");
        arduino->write(",");
        arduino->write("33");
        arduino->write("\n");
    }
    else
    {
        ui->label_5->setText("Couldn't write to arduino! Please reconnect.");
    }
}

void MyWindow::on_pushButton_45_clicked()
{
    degree = 17;
    if(arduino->isWritable())
    {
        arduino->write("s");
        arduino->write(",");
        arduino->write("31");
        arduino->write("\n");
    }
    else
    {
        ui->label_5->setText("Couldn't write to arduino! Please reconnect.");
    }
}

void MyWindow::on_pushButton_44_clicked()
{
      degree = 18;
    if(arduino->isWritable())
    {

        arduino->write("s");
        arduino->write(",");
        arduino->write("29");
        arduino->write("\n");
    }
    else
    {
        ui->label_5->setText("Couldn't write to arduino! Please reconnect.");
    }
}

void MyWindow::on_pushButton_43_clicked()
{
    degree = 19;
    if(arduino->isWritable())
    {
        arduino->write("s");
        arduino->write(",");
        arduino->write("27");
        arduino->write("\n");
    }
    else
    {
        ui->label_5->setText("Couldn't write to arduino! Please reconnect.");
    }
}

void MyWindow::on_pushButton_42_clicked()
{
    degree = 20;
    if(arduino->isWritable())
    {
        arduino->write("s");
        arduino->write(",");
        arduino->write("25");
        arduino->write("\n");
    }
    else
    {
        ui->label_5->setText("Couldn't write to arduino! Please reconnect.");
    }
}

void MyWindow::on_pushButton_41_clicked()
{
    degree = 21;
    if(arduino->isWritable())
    {
        arduino->write("s");
        arduino->write(",");
        arduino->write("23");
        arduino->write("\n");
    }
    else
    {
        ui->label_5->setText("Couldn't write to arduino! Please reconnect.");
    }
}

void MyWindow::on_pushButton_40_clicked()
{
    degree = 22;
    if(arduino->isWritable())
    {
        arduino->write("s");
        arduino->write(",");
        arduino->write("52");
        arduino->write("\n");
    }
    else
    {
        ui->label_5->setText("Couldn't write to arduino! Please reconnect.");
    }
}

void MyWindow::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Space)
    {
       on_pushButton_49_clicked();
    }
}

void MyWindow::on_pushButton_49_clicked()
{
    if(arduino->isWritable())
    {
        arduino->write("x");
        arduino->write("\n");
    }
    else
    {
        ui->label_5->setText("Couldn't write to arduino! Please reconnect.");
    }
}

void MyWindow::on_pushButton_50_clicked()
{
   connect(ui->horizontalSlider,SIGNAL(valueChanged(int)),this, SLOT(sendvalues(int)));
   QString text = ui->lineEdit->text();
   if(arduino->isWritable())
   {
       arduino->write("t");
       arduino->write(",");
       arduino->write(text.toUtf8());
       arduino->write("\n");
   }
   else
   {
       ui->label_5->setText("Couldn't write to arduino! Please reconnect.");
   }

}

void MyWindow::sendvalues(int value)
{
    if(arduino->isWritable())
    {
        arduino->write("m");
        arduino->write(",");
        arduino->write((char*)value);
        arduino->write("\n");
    }
    else
    {
        ui->label_5->setText("Couldn't write to arduino! Please reconnect.");
    }

}

void MyWindow::on_pushButton_24_clicked()
{
    boost::thread thr(capture_image,capWebcam);
}
