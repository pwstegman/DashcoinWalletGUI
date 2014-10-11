#include "dashcoinwallet.h"
#include <QApplication>
#include <QObject>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    DashcoinWallet w;
    w.show();
    QObject::connect(&a, SIGNAL(aboutToQuit()), &w, SLOT(closing()));

    return a.exec();
}
