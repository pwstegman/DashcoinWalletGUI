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

private:
    Ui::DashcoinWallet *ui;
    void loadFile();
    void showPasswordPrompt();
    QProcess *daemon;
};

#endif // DASHCOINWALLET_H
