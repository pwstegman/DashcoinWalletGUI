#ifndef DASHCOINWALLET_H
#define DASHCOINWALLET_H

#include <QMainWindow>
#include <QProcess>
#include <QNetworkReply>
#include <QLabel>

namespace Ui {
class DashcoinWallet;
}

class DashcoinWallet : public QMainWindow
{
    Q_OBJECT

public:
    explicit DashcoinWallet(QWidget *parent = 0);
    ~DashcoinWallet();

protected:
    void closeEvent(QCloseEvent *event);

private slots:
    void replyFinished(QNetworkReply *reply);
    void daemonStarted();
    void daemonFinished();
    void loadBlockHeight();
    void killWalletGenerate();
    void on_openWallet_btn_clicked();
    void walletStarted();
    void walletFinished();
    //void closing();

    void on_closeDaemon_btn_clicked();

private:
    Ui::DashcoinWallet *ui;
    void loadFile();
    QProcess *daemon;
    QProcess *wallet;
    QProcess *walletGenerate;
    QString pass;
    QLabel *syncLabel;
    QLabel *messageLabel;
    bool tryingToClose;
    bool daemonRunning;
};

#endif // DASHCOINWALLET_H
