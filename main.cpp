#include "pediperi.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    PediPeri w;
    w.show();
    return a.exec();
}
