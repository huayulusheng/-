#ifndef STAR_H
#define STAR_H

#include <QMainWindow>

namespace Ui {
class star;
}

class star : public QMainWindow
{
    Q_OBJECT

public:
    explicit star(QWidget *parent = nullptr);
    ~star();

private slots:
    void on_start_clicked();

private:
    Ui::star *ui;
};

#endif // STAR_H
