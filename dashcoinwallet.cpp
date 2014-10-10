#include "dashcoinwallet.h"
#include "ui_dashcoinwallet.h"
#include <QDir>
#include <QDebug>
#include <QNetworkRequest>
#include <QUrl>
#include <QNetworkReply>
#include <QUrlQuery>
#include <QTimer>
#include <QPushButton>
#include <QVboxLayout>

DashcoinWallet::DashcoinWallet(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::DashcoinWallet)
{
    ui->setupUi(this);
    syncLabel = new QLabel(this);
    messageLabel = new QLabel(this);
    syncLabel->setContentsMargins(9,0,9,0);
    messageLabel->setContentsMargins(9,0,9,0);
    ui->statusBar->addPermanentWidget(syncLabel);
    ui->statusBar->addPermanentWidget(messageLabel,1);
    loadFile();
}

DashcoinWallet::~DashcoinWallet()
{
    delete ui;
}

void DashcoinWallet::loadFile()
{
    daemon = new QProcess(this);
    connect(daemon, SIGNAL(started()),this, SLOT(daemonStarted()));
    daemon->start(QDir::currentPath ()+"/dashcoind", QStringList() << "--no-console");
}

void DashcoinWallet::daemonStarted(){
    syncLabel->setText("Starting sync...");
    QTimer::singleShot(2000, this, SLOT(loadBlockHeight()));
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(loadBlockHeight()));
    timer->start(15000);
}

void DashcoinWallet::loadBlockHeight(){
    /*
     * POST Request not working yet
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    QUrl url("http://127.0.0.1:29081/json_rpc");
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    QUrlQuery params;
    params.addQueryItem("jsonrpc", "2.0");
    params.addQueryItem("id", "dashcoinguiwallet");
    params.addQueryItem("method","getblockcount");
    connect(manager, SIGNAL(finished(QNetworkReply *)), this, SLOT(replyFinished(QNetworkReply *)));
    manager->post(request, params.query(QUrl::FullyEncoded).toUtf8());2
    */

    QNetworkAccessManager *manager;
    manager = new QNetworkAccessManager(this);
    connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)));
    manager->get(QNetworkRequest(QUrl("http://127.0.0.1:29081/getheight")));

}

void DashcoinWallet::replyFinished(QNetworkReply *reply)
{
    QByteArray bytes = reply->readAll();
    QString str = QString::fromUtf8(bytes.data(), bytes.size());
    int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    qDebug() << "Status code: " << statusCode;
    qDebug() << "Reply: " << str;
}

void DashcoinWallet::on_openWallet_btn_clicked()
{
    //they clicked the open wallet button
    pass = ui->password_txt->text();
    ui->password_txt->setText("");
    wallet = new QProcess(this);
    connect(wallet, SIGNAL(started()),this, SLOT(walletStarted()));
    connect(wallet, SIGNAL(finished(int , QProcess::ExitStatus)),this, SLOT(walletFinished()));
    QFile walletFile(QDir::currentPath ()+"/wallet.bin");
    if(!walletFile.exists()){
        messageLabel->setText("No wallet found. Generating new wallet...");
        walletGenerate = new QProcess(this);
        walletGenerate->start(QDir::currentPath ()+"/simplewallet", QStringList() << "--generate-new-wallet" << "wallet.bin" << "--password" << pass);
        QTimer::singleShot(2000, this, SLOT(killWalletGenerate()));
    }else{
        messageLabel->setText("Opening wallet...");
        wallet->start(QDir::currentPath ()+"/simplewallet", QStringList() << "--wallet-file=wallet.bin" << "--pass="+pass << "--rpc-bind-port=49253");
    }
}

void DashcoinWallet::killWalletGenerate()
{
    messageLabel->setText("Generated wallet file wallet.bin. Now starting wallet server on port 49253.");
    walletGenerate->kill();
    wallet->start(QDir::currentPath ()+"/simplewallet", QStringList() << "--wallet-file=wallet.bin" << "--pass="+pass << "--rpc-bind-port=49253");
    pass = "";
}

void DashcoinWallet::walletStarted()
{
    messageLabel->setText("Wallet connected");
}

void DashcoinWallet::walletFinished()
{
    messageLabel->setText("Wallet disconnected. Please enter password to reconnect.");
}
