#include "page3.h"
#include "ui_page3.h"
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QLabel>
#include <QPixmap>

page3::page3(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::page3),
    label_(new QLabel(this))  // 添加对 label_ 的定义和初始化
{
    ui->setupUi(this);

    // 发送请求获取视频流
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    QNetworkRequest request(QUrl("http://192.168.137.117:81/stream"));

    QNetworkReply *reply = manager->get(request);

    connect(reply, &QNetworkReply::readyRead, this, &page3::dataReceived);
    connect(reply, &QNetworkReply::finished, reply, &QNetworkReply::deleteLater);

    // 在界面上添加 label_
    ui->verticalLayout->addWidget(label_);
}

void page3::dataReceived()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
    if (reply)
    {
        QByteArray data = reply->readAll();

        // 根据视频流数据更新图像
        QPixmap pixmap;
        bool loadStatus = pixmap.loadFromData(data);
        if (loadStatus)
        {
            // 在标签中显示图像
            label_->setPixmap(pixmap);
            label_->setScaledContents(true); // 将图像自适应 label_ 的大小
        }
    }
}
page3::~page3()
{
    delete ui;
}
void page3::on_page3_1_clicked()

{
         // 隐藏当前界面
         this->hide();
    qDebug("page3_0");
    // Page0->show(); // 显示下一个界面

}
