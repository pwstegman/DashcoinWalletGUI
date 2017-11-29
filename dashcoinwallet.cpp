#include "dashcoinwallet.h"
#include "ui_dashcoinwallet.h"
#include <QDebug>
#include <QProcess>
#include <QTimer>

DashcoinWallet::DashcoinWallet(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::DashcoinWallet){
    ui->setupUi(this);

    init();
}

DashcoinWallet::~DashcoinWallet(){
    delete ui;
}

void DashcoinWallet::init(){
    ui->balance_section->hide();

    wallet_rpc_port = new QString("49253");

    initUI();
    // initDaemon();
    // generateWallet("test.bin", "nope");
    openWallet("test.bin", "test");
}

/* =================================
               UI Setup
   ================================= */

void DashcoinWallet::initUI(){
    statusbar_message = new QLabel("Status bar messages to go here");
    statusbar_message->setContentsMargins(10, 0, 0, 10);
    ui->statusbar->addPermanentWidget(statusbar_message, 1);
}


/* =================================
                Daemon
   ================================= */

void DashcoinWallet::initDaemon(){
    QString program = "cmd.exe";

    daemon = new QProcess();

    daemon->setProcessChannelMode(QProcess::MergedChannels);

    connect(daemon, SIGNAL(errorOccurred(QProcess::ProcessError)), this, SLOT(daemonError(QProcess::ProcessError)));
    connect(daemon, SIGNAL(readyRead()), this, SLOT(daemonRead()));

    daemon->start(program);

    daemon->write("dashcoind.exe \n");
    daemon->closeWriteChannel();
}

void DashcoinWallet::daemonRead(){
    while(daemon->canReadLine()){
        qDebug() << "Daemon> " << daemon->readLine();
    }
}


void DashcoinWallet::daemonError(QProcess::ProcessError error){
    qDebug() << "Daemon error: " << error;
    // TODO: Show errors in GUI
}


/* =================================
            Wallet Generate
   ================================= */

void DashcoinWallet::generateWallet(QString name, QString pass){
    QString program = "cmd.exe";

    walletgen = new QProcess();

    walletgen->setProcessChannelMode(QProcess::MergedChannels);

    connect(walletgen, SIGNAL(errorOccurred(QProcess::ProcessError)), this, SLOT(walletGenError(QProcess::ProcessError)));
    connect(walletgen, SIGNAL(readyRead()), this, SLOT(walletGenRead()));

    walletgen->start(program);

    QString command = "simplewallet.exe --generate-new-wallet \"" + name + "\" --password \"" + pass + "\" \n";
    walletgen->write(command.toUtf8());
    walletgen->closeWriteChannel();
}

void DashcoinWallet::walletGenRead(){
    while(walletgen->canReadLine()){
        QString line = walletgen->readLine();
        qDebug() << "Simplewallet (generate wallet)> " << line;

        if(line.contains("already exists")){
            statusbar_message->setText("Error: Wallet file already exists");
        }
    }
}


void DashcoinWallet::walletGenError(QProcess::ProcessError error){
    qDebug() << "Wallet generation error: " << error;
    // TODO: Show errors in GUI
}


/* =================================
             Wallet Open
   ================================= */

void DashcoinWallet::openWallet(QString name, QString pass){
    QString program = "cmd.exe";

    walletopen = new QProcess();

    walletopen->setProcessChannelMode(QProcess::MergedChannels);

    connect(walletopen, SIGNAL(errorOccurred(QProcess::ProcessError)), this, SLOT(walletOpenError(QProcess::ProcessError)));
    connect(walletopen, SIGNAL(readyRead()), this, SLOT(walletOpenRead()));

    walletopen->start(program);

    // TODO: Use rpc api
    QString command = "simplewallet.exe --wallet-file \"" + name + "\" --password \"" + pass + "\" \n";
    walletopen->write(command.toUtf8());
    walletopen->closeWriteChannel();

    // Notes: Current sync height eventually shows up on stdout. Rpc call for get height seems to return incorrect height
}

void DashcoinWallet::walletOpenRead(){
    while(walletopen->canReadLine()){
        QString line = walletopen->readLine();
        qDebug() << "Simplewallet (open wallet)> " << line;
    }
}


void DashcoinWallet::walletOpenError(QProcess::ProcessError error){
    qDebug() << "Wallet open error: " << error;
    // TODO: Show errors in GUI
}
