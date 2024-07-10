#include "page2.h"
#include "ui_page2.h"
#include "mainwindow.h"
page2::page2(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::page2)
{
    ui->setupUi(this);

}

page2::~page2()
{
    delete ui;



}
void page2::on_page2_1_clicked()

{
         // 隐藏当前界面
         this->hide();
    qDebug("page2_0");
    // Page0->show(); // 显示下一个界面

}

void page2::on_light_on_clicked()

{

    qDebug("send");
    QString data = "light:1"; // 准备要发送的数据
    MainWindow* mainWindow = qobject_cast<MainWindow*>(parent()); // 获取父对象的指针
    if (mainWindow)
    {
        QMetaObject::invokeMethod(mainWindow, "receiveData", Q_ARG(QString, data)); // 调用父对象的槽函数，并传递数据
    }


}


void page2::on_light_off_clicked()

{

    qDebug("send2");
    QString data = "light:0"; // 准备要发送的数据
    MainWindow* mainWindow = qobject_cast<MainWindow*>(parent()); // 获取父对象的指针
    if (mainWindow)
    {
        QMetaObject::invokeMethod(mainWindow, "receiveData", Q_ARG(QString, data)); // 调用父对象的槽函数，并传递数据
    }


}



void page2::on_water_on_clicked()

{

    qDebug("send2");
    QString data = "water:1"; // 准备要发送的数据
    MainWindow* mainWindow = qobject_cast<MainWindow*>(parent()); // 获取父对象的指针
    if (mainWindow)
    {
        QMetaObject::invokeMethod(mainWindow, "receiveData", Q_ARG(QString, data)); // 调用父对象的槽函数，并传递数据
    }


}

void page2::on_water_off_clicked()

{

    qDebug("send2");
    QString data = "water:0"; // 准备要发送的数据
    MainWindow* mainWindow = qobject_cast<MainWindow*>(parent()); // 获取父对象的指针
    if (mainWindow)
    {
        QMetaObject::invokeMethod(mainWindow, "receiveData", Q_ARG(QString, data)); // 调用父对象的槽函数，并传递数据
    }


}
