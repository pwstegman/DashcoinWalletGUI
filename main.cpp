#include "dashcoinwallet.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    DashcoinWallet w;
    w.show();

    return a.exec();
}
