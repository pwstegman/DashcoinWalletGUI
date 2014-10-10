#ifndef DASHCOINWALLET_H
#define DASHCOINWALLET_H

#include <QMainWindow>
#include <QProcess>
#include <QNetworkReply>

namespace Ui {
class DashcoinWallet;
}

class DashcoinWallet : public QMainWindow
{
    Q_OBJECT

public:
    explicit DashcoinWallet(QWidget *parent = 0);
    ~DashcoinWallet();

private slots:
    void replyFinished(QNetworkReply *reply);
    void daemonStarted();
    void loadBlockHeight();
    void killWalletGenerate();
    void on_openWallet_btn_clicked();
    void walletStarted();
    void walletFinished();

private:
    Ui::DashcoinWallet *ui;
    void loadFile();
    QProcess *daemon;
    QProcess *wallet;
    QProcess *walletGenerate;
    QString pass;
};

#endif // DASHCOINWALLET_H
