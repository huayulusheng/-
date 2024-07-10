#include "star.h"
#include "ui_star.h"
#include "mainwindow.h"
star::star(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::star)
{
    ui->setupUi(this);
}

star::~star()
{
    delete ui;
}


void star::on_start_clicked()
{
     this->close();
    qDebug("first");

}
