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
#include <QJsonDocument>
#include <QJsonObject>
#include <QCloseEvent>

DashcoinWallet::DashcoinWallet(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::DashcoinWallet)
{
    ui->setupUi(this);
    tryingToClose = false;
    daemonRunning = false;
    walletRunning = false;
    synced = false;
    ui->balance->hide();
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
    connect(daemon, SIGNAL(finished(int , QProcess::ExitStatus)),this, SLOT(daemonFinished()));
    daemon->start(QDir::currentPath ()+"/dashcoind", QStringList() << "");
}

void DashcoinWallet::daemonStarted(){
    daemonRunning = true;
    syncLabel->setText("Starting daemon");
    QTimer::singleShot(3000, this, SLOT(loadBlockHeight()));
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(loadBlockHeight()));
    timer->start(10000);
}

void DashcoinWallet::daemonFinished()
{
    daemonRunning = false;
    qDebug() << "daemon finished";
    if(tryingToClose == true){
        qDebug() << "quitting";
        qApp->quit();
    }
}

void DashcoinWallet::loadBlockHeight(){
    if(daemonRunning){
        QNetworkAccessManager *manager = new QNetworkAccessManager(this);
        connect(manager, SIGNAL(finished(QNetworkReply*)),this, SLOT(replyFinished(QNetworkReply*)));
        QString dataStr = "{\"jsonrpc\": \"2.0\", \"method\":\"getblockcount\", \"id\": \"test\"}";
        QJsonDocument jsonData = QJsonDocument::fromJson(dataStr.toUtf8());
        QByteArray data = jsonData.toJson();
        QNetworkRequest request = QNetworkRequest(QUrl("http://127.0.0.1:29081/json_rpc"));
        request.setHeader(QNetworkRequest::ContentTypeHeader, "application/octet-stream");
        manager->post(request, data);
    }

}

void DashcoinWallet::replyFinished(QNetworkReply *reply)
{
    if(tryingToClose == false){
        QByteArray bytes = reply->readAll();
        QString str = QString::fromUtf8(bytes.data(), bytes.size());
        QJsonDocument jsonResponse = QJsonDocument::fromJson(str.toUtf8());
        QJsonObject jsonObj = jsonResponse.object()["result"].toObject();
        QString status = jsonObj["status"].toString();
        QString height = QString::number(jsonObj["count"].toInt());
        if(status == "OK"){
            if(synced == false){
                messageLabel->setText("");
                synced = true;
            }
            syncLabel->setText("Synced with network. Height: "+height);
        }else{
            syncLabel->setText("Syncing with network");
        }
    }
}

void DashcoinWallet::on_openWallet_btn_clicked()
{
    //they clicked the open wallet button
    if(synced == true){
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
    }else{
        messageLabel->setText("Please wait for the sync to complete");
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
    walletRunning = true;
    showWallet();
}

void DashcoinWallet::walletFinished()
{
    walletRunning = false;
    hideWallet();
    messageLabel->setText("Wallet disconnected");
}

void DashcoinWallet::closeEvent(QCloseEvent *event)
 {
    qDebug() << "exiting now event";
    if(walletRunning == true){
        wallet->kill();
    }
    if(daemonRunning == true){
        if(tryingToClose == false){
            daemon->write("exit\n");
            tryingToClose = true;
            syncLabel->setText("Saving blockchain...");
        }
        event->ignore();
    }else{
        event->accept();
    }
 }

void DashcoinWallet::loadBalance()
{
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    connect(manager, SIGNAL(finished(QNetworkReply*)),this, SLOT(balanceReply(QNetworkReply*)));
    QString dataStr = "{\"jsonrpc\": \"2.0\", \"method\":\"getbalance\", \"id\": \"test\"}";
    QJsonDocument jsonData = QJsonDocument::fromJson(dataStr.toUtf8());
    QByteArray data = jsonData.toJson();
    QNetworkRequest request = QNetworkRequest(QUrl("http://127.0.0.1:49253/json_rpc"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/octet-stream");
    manager->post(request, data);

}

void DashcoinWallet::balanceReply(QNetworkReply *reply)
{
    QByteArray bytes = reply->readAll();
    QString str = QString::fromUtf8(bytes.data(), bytes.size());
    QJsonDocument jsonResponse = QJsonDocument::fromJson(str.toUtf8());
    QJsonObject jsonObj = jsonResponse.object();
    QString balance = QString::number(jsonObj["balance"].toInt());
    QString unlocked_balance = QString::number(jsonObj["unlocked_balance"].toInt());
    balance = fixBalance(balance);
    unlocked_balance = fixBalance(unlocked_balance);
    ui->balance_txt->setText(balance+" DSH");
    ui->balance_unlocked_txt->setText(unlocked_balance+" DSH");
    showAllWallet();
}

QString DashcoinWallet::fixBalance(QString str)
{
    if(str == "0"){
        return "0";
    }
    QString left = "";
    QString right = "";
    if(str.length() <= 8){
        str = QString((8-str.length()),'0')+str;
        left = "0";
    }else{
        left = str.left(str.length()-8);
    }
    right = str.right(8);
    right.remove(QRegExp("0+$"));
    QString result = left+"."+right;
    return result;
}

void DashcoinWallet::hideWallet()
{
    ui->passwordBox->show();
    ui->balance->hide();
}

void DashcoinWallet::showWallet()
{
    ui->passwordBox->hide();
    loadWalletData();
}

void DashcoinWallet::loadWalletData(){
    qDebug() << "Loaded wallet data";
    if(walletRunning == true){
        loadBalance();
    }
}

void DashcoinWallet::showAllWallet()
{
    messageLabel->setText("Wallet connected");
    ui->balance->show();
    QTimer *walletTimer = new QTimer(this);
    connect(walletTimer, SIGNAL(timeout()), this, SLOT(loadWalletData()));
    walletTimer->start(10000);
}
