#ifndef MYWINDOW_H
#define MYWINDOW_H

#include <QMainWindow>
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <QtSerialPort/QSerialPort>

namespace Ui {
class MyWindow;
}

class MyWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MyWindow(QWidget *parent = 0);
    ~MyWindow();


private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_7_clicked();

public slots:
void capt();




private:
    Ui::MyWindow *ui;
    cv::VideoCapture capWebcam;
    cv::Mat frame;
    QSerialPort *arduino;
    QImage qimg;

    QTimer* tmrTimer;

    static const quint16 arduino_uno_vendor_id = 9025;
    static const quint16 arduino_uno_product_id = 66;
    QString arduino_port_name;
    bool arduino_is_available;
};


#endif // MYWINDOW_H
