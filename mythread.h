#ifndef MYTHREAD_H
#define MYTHREAD_H
#include <QtCore>
#include <QThread>

class Mythread : public QThread
{
public:
    Mythread();
    void run();

signals:

public slots:
};

#endif // MYTHREAD_H
