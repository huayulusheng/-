#include "page1.h"
#include "ui_page1.h"
#include "mainwindow.h"

/*参考https://blog.csdn.net/she666666/article/details/128177953?ops_request_misc=%257B%2522request%255Fid%2522%253A%2522172049250616800226529489%2522%252C%2522scm%2522%253A%252220140713.130102334..%2522%257D&request_id=172049250616800226529489&biz_id=0&utm_medium=distribute.pc_search_result.none-task-blog-2~all~sobaiduend~default-1-128177953-null-null.142^v100^pc_search_result_base7&utm_term=qt%E8%8E%B7%E5%8F%96%E5%BF%83%E7%9F%A5%E5%A4%A9%E6%B0%94&spm=1018.2226.3001.4187
*/


page1::page1(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::page1)
{
    ui->setupUi(this);
    QString styleSheet = "QLabel {"
                         "  transform: rotate(90deg);"
                         "}";

    ui->label->setStyleSheet(styleSheet);

    http = new QNetworkAccessManager(this);//建立对象
        connect(ui->pushButton,&QPushButton::clicked,[=](){
           QString location = "tianjin";
           QString ask = QString("https://api.seniverse.com/v3/weather/now.json?key=SoWpgC4nTU_56wkBV&location=%1&language=zh-Hans&unit=c").arg(location);
           qDebug()<<"请求网址: "<<ask;
           http->get(QNetworkRequest(QUrl(ask)));
           ui->listWidget->clear();//清除
        });

        connect(http,&QNetworkAccessManager::finished,[=](QNetworkReply * reply)
        {
            QByteArray text = reply->readAll();//读取所有的数据
            QJsonObject json = QJsonDocument::fromJson(text).object();//获取json 源码
            // qDebug()<<json<<endl;
            if(json.find("results")!=json.end())
            {
                qDebug("获取天气成功: ");
                QString name = json["results"].toArray()[0].toObject()["location"].toObject()["path"].toString();//解析 Json 的代码 读取温度
                QString temp = json["results"].toArray()[0].toObject()["now"].toObject()["temperature"].toString();//解析 Json 的代码 获取温度
                QString weather = json["results"].toArray()[0].toObject()["now"].toObject()["text"].toString();//解析 Json 的代码 获取天气
                QString time =json["results"].toArray()[0].toObject()["last_update"].toString();//解析 Json 的代码  获取现在的时间
                QString code =json["results"].toArray()[0].toObject()["now"].toObject()["code"].toString();//解析 Json 的代码 获取现在天气的图标信号
                int code_1=code.toInt();//代表现在天气的 数字


                //bb.mid(bb.lastIndexOf("/")+1);
                time=time.left(time.indexOf("T"));
                QString text = QString("更新时间: %1 \n地点: %2\n 温度: %3\n天气: %4 \n天气的标号: %5").arg(time,name,temp,weather,code);//转成字符串
                ui->listWidget->addItem(text);
                // set_label(code_1);
            }

        });



}

page1::~page1()
{
    delete ui;






}
void page1::on_page0_clicked()

{
         // 隐藏当前界面
         this->hide();
    qDebug("page0");

    // Page0->show(); // 显示下一个界面

}










