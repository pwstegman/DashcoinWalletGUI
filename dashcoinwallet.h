#ifndef DASHCOINWALLET_H
#define DASHCOINWALLET_H

#include <QMainWindow>
#include <QProcess>
#include <QLabel>
#include <QTimer>
#include <QtNetwork>

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
    void on_btn_open_clicked();
    void on_btn_generate_clicked();
    void on_btn_send_clicked();
    void on_btn_send_confirm_clicked();
    void on_btn_send_cancel_clicked();
    void done_generating();
    void daemon_started();
    void daemon_finished();
    void wallet_started();
    void wallet_finished();
    void parse_daemon_log();
    void load_wallet_data();
    void rpcReply(QNetworkReply *reply);
    void on_btn_close_wallet_clicked();

    void on_txt_password_open_returnPressed();

private:
    Ui::DashcoinWallet *ui;
    void init_ui();
    void init_wallet();
    void show_wallet(bool b);
    void load_wallets();
    void start_daemon();
    void load_balance();
    void load_address();
    void load_transactions();
    QString fix_amount(QString str);
    QString load_daemon_log();
    QString fix_balance(QString str);
    bool synced;
    bool daemon_is_running;
    bool wallet_is_running;
    bool opening_wallet;
    QLabel *syncLabel;
    QLabel *messageLabel;
    QString current_wallet;
    QProcess *daemon;
    QProcess *wallet_generate;
    QProcess *wallet;
    bool tryingToClose;
    QTimer *parse_daemon_timer;
    QNetworkAccessManager *loader_balance;
    QNetworkAccessManager *loader_transactions;
    QNetworkAccessManager *loader_send;
};

#endif // DASHCOINWALLET_H
