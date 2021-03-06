#ifndef MYWINDOW_H
#define MYWINDOW_H

#include <QMainWindow>
#include <QtSerialPort>

#include <opencv/cv.h>
#include <opencv/highgui.h>

namespace Ui {
class MyWindow;
}

class MyWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MyWindow(QWidget *parent = 0);
    ~MyWindow();
    Ui::MyWindow *ui;


private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_7_clicked();

    void on_pushButton_11_clicked();

    void on_pushButton_8_clicked();

    void on_pushButton_9_clicked();

    void on_pushButton_10_clicked();

    void on_pushButton_13_clicked();

    void on_pushButton_15_clicked();

    void on_pushButton_14_clicked();

    void on_pushButton_12_clicked();

    void on_pushButton_16_clicked();

    void on_pushButton_23_clicked();

    void on_pushButton_17_clicked();

    void on_pushButton_20_clicked();

    void on_pushButton_19_clicked();

    void on_pushButton_18_clicked();

    void on_pushButton_21_clicked();

    void on_pushButton_22_clicked();

    void on_pushButton_25_clicked();

    void on_pushButton_26_clicked();

    void on_pushButton_39_clicked();

    void on_pushButton_38_clicked();

    void on_pushButton_37_clicked();

    void on_pushButton_36_clicked();

    void on_pushButton_35_clicked();

    void on_pushButton_34_clicked();

    void on_pushButton_33_clicked();

    void on_pushButton_32_clicked();

    void on_pushButton_31_clicked();

    void on_pushButton_30_clicked();

    void on_pushButton_29_clicked();

    void on_pushButton_28_clicked();

    void on_pushButton_27_clicked();

    void on_pushButton_48_clicked();

    void on_pushButton_47_clicked();

    void on_pushButton_46_clicked();

    void on_pushButton_45_clicked();

    void on_pushButton_44_clicked();

    void on_pushButton_43_clicked();

    void on_pushButton_42_clicked();

    void on_pushButton_41_clicked();

    void on_pushButton_40_clicked();

    void on_pushButton_49_clicked();

    void on_pushButton_50_clicked();



    void on_pushButton_24_clicked();

public slots:
void capt();
void hemirightlower();
void hemileftlower();
void quad1();
void quad2();
void quad3();
void quad4();
void sendvalues(int value);
void serialReceived();
void keyPressEvent(QKeyEvent *event);


private:
    QByteArray inByteArray;
    cv::VideoCapture capWebcam;
    cv::Mat frame;
    QSerialPort *arduino;
    QImage qimg;
    QTimer* tmrTimer;
    static const quint16 arduino_uno_vendor_id = 9025;
    static const quint16 arduino_uno_product_id = 66;
    QString arduino_port_name;
    bool arduino_is_available;
    int i=0;
    QString updatestr;
    int qarray;
    float ang;
    float degree;
};


#endif // MYWINDOW_H
