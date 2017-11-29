#ifndef DASHCOINWALLET_H
#define DASHCOINWALLET_H

#include <QMainWindow>
#include <QProcess>
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

private slots:
    void daemonRead();
    void daemonError(QProcess::ProcessError error);
    void walletGenRead();
    void walletGenError(QProcess::ProcessError error);
    void walletOpenRead();
    void walletOpenError(QProcess::ProcessError error);

private:
    Ui::DashcoinWallet *ui;
    void init();
    void initUI();
    void initDaemon();
    void generateWallet(QString name, QString pass);
    void openWallet(QString name, QString pass);
    QProcess *daemon;
    QProcess *walletgen;
    QProcess *walletopen;
    QLabel *statusbar_message;
    QString *wallet_rpc_port;
};

#endif // DASHCOINWALLET_H
