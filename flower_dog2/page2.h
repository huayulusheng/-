#ifndef PAGE2_H
#define PAGE2_H

#include <QMainWindow>

namespace Ui {
class page2;
}

class page2 : public QMainWindow
{
    Q_OBJECT

public:
    explicit page2(QWidget *parent = nullptr);
    ~page2();




private slots:
    void on_page2_1_clicked();
    void on_light_on_clicked(); // 按键点击事件槽函数
    void on_light_off_clicked(); // 按键点击事件槽函数
    void on_water_on_clicked(); // 按键点击事件槽函数
    void on_water_off_clicked(); // 按键点击事件槽函数

signals:
    void sendData(const QString& data); // 定义信号，用于发送数据

private:
    Ui::page2 *ui;
};

#endif // PAGE2_H
