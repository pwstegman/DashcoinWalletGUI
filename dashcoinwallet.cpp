#include "dashcoinwallet.h"
#include "ui_dashcoinwallet.h"

DashcoinWallet::DashcoinWallet(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::DashcoinWallet)
{
    ui->setupUi(this);
}

DashcoinWallet::~DashcoinWallet()
{
    delete ui;
}
