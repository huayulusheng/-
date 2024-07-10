#ifndef PAGE3_H
#define PAGE3_H

#include <QMainWindow>
#include <QNetworkReply> // 包含 QNetworkReply 的头文件
#include <QLabel>
namespace Ui {
class page3;
}

class page3 : public QMainWindow
{
    Q_OBJECT

public:
    explicit page3(QWidget *parent = nullptr);
    ~page3();

private slots:
    void on_page3_1_clicked();
    void dataReceived();


private:
    Ui::page3 *ui;
    QLabel *label_;
};

#endif // PAGE3_H
