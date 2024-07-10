#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "page1.h"
#include "page2.h"
#include "page3.h"
#include <QDateTime>
#include <QPushButton>
#include <QHostAddress>
#include "star.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    TcpServer = new QTcpServer(this);
    if (!TcpServer->listen(QHostAddress::AnyIPv4, 8090)) {
        qDebug() << "连搬失败，请重试" << TcpServer->errorString();
        close();
    }
    connect(TcpServer, &QTcpServer::newConnection, this, &MainWindow::onNewConnect);
    connect(TcpServer, &QTcpServer::newConnection, this, &MainWindow::onSendBackMsg);
    ;
    star* firstPage = new star(this);
    firstPage->show();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onNewConnect()
{
    QTcpSocket* clientSocket = TcpServer->nextPendingConnection();
    connect(clientSocket, &QTcpSocket::readyRead, this, &MainWindow::onReadMsg);
    connect(clientSocket, &QTcpSocket::disconnected, clientSocket, &QTcpSocket::deleteLater);

    clientSockets.append(clientSocket);
    qDebug() << "New client connected.";
}

void MainWindow::onSendBackMsg()
{
    QString text = "Hello!";
    for (QTcpSocket* clientSocket : clientSockets) {
        clientSocket->write(text.toUtf8());
    }
    qDebug("发送成功");
}

void MainWindow::onReadMsg()
{
    for (QTcpSocket* clientSocket : clientSockets) {
        QByteArray data = clientSocket->readAll();
        // ui->ReCvText->setText(data);

        // 广播消息给所有客户端
        for (QTcpSocket* socket : clientSockets) {
            socket->write(data);
        }
    }
}

void MainWindow::on_page1_clicked()
{
    qDebug("pushButtonConnect");
    page1* nextPage = new page1(this);
    nextPage->show();
}

void MainWindow::on_page1_2_clicked()
{
    qDebug("pushButtonConnect");
    page2* next2Page = new page2(this);
    next2Page->show();
}

void MainWindow::on_page3_clicked()
{
    qDebug("pushButtonConnect");
    page3* next3Page = new page3(this);
    next3Page->show();
}



void MainWindow::receiveData(const QString& data)
{
    qDebug("收到");
    qDebug() << "Received data: " << data;
    for (QTcpSocket* clientSocket : clientSockets) {
        clientSocket->write(data.toUtf8());
    }
}
