#include "LbsTrack.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    CLbsTrack w;
    w.show();
    
    return a.exec();
}
