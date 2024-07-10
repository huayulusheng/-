#ifndef PAGE1_H
#define PAGE1_H

#include <QMainWindow>
#include "mainwindow.h"



#include <QWidget>
#include <QtNetwork> //将所有的网络头文件 直接加入
#include <QNetworkAccessManager>

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>

namespace Ui {
class page1;
}

class page1 : public QMainWindow
{
    Q_OBJECT

public:
    explicit page1(QWidget *parent = nullptr);
    ~page1();


private slots:
    void on_page0_clicked();


private:
    Ui::page1 *ui;
    QNetworkAccessManager * http;


};

#endif // PAGE1_H
