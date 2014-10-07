#ifndef DASHCOINWALLET_H
#define DASHCOINWALLET_H

#include <QMainWindow>

namespace Ui {
class DashcoinWallet;
}

class DashcoinWallet : public QMainWindow
{
    Q_OBJECT

public:
    explicit DashcoinWallet(QWidget *parent = 0);
    ~DashcoinWallet();

private:
    Ui::DashcoinWallet *ui;
};

#endif // DASHCOINWALLET_H
