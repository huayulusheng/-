#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "page1.h"
#include<QTcpSocket>
#include<QTcpServer>
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private slots:
    void onNewConnect();
    void onSendBackMsg();
    void onReadMsg();
    void on_page1_clicked();
    void on_page1_2_clicked();
    void on_page3_clicked();
    void receiveData(const QString& data); // 槽函数，接收数据

signals:
    void dataReceived(const QString& data); // 定义信号，用于返回接收到的数据

private:
    Ui::MainWindow *ui;
    QTcpServer * TcpServer;
    QTcpSocket * TcpSocket;
    QList<QTcpSocket*> clientSockets;
    // MainWindow* Page0 =new MainWindow(this); // 创建下一个界面的实例
};
#endif // MAINWINDOW_H
