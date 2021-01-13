#ifndef WIDGET_H
#define WIDGET_H
using namespace std;
#include <QWidget>
#include<QPoint>
#include<QtMath>
#include<QString>
#include<QMouseEvent>
#include<QCloseEvent>
#include<QGraphicsDropShadowEffect>
#include<QPainter>
#include<QFileDialog>
#include<QFile>
#include<QDebug>
#include<QFont>
#include<algorithm>
#include<vector>
#include<QTextCodec>
#include<QFileDialog>
#include<QDir>
namespace Ui {
class Widget;
}

typedef struct PERSON{
    QString name;
    QString number;
}Person;

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();
    void  mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *event);
    void closeEvent(QCloseEvent* event);
    void getInfoFromFile();
    void saveInfoToFile();
    void addToListWidget();
private slots:
    void on_pushButton_close_clicked();
    void on_pushButton_min_clicked();
    void on_pushButton_list_clicked();
    void on_pushButton_set_clicked();

    void on_pushButton_delete_clicked();

    void on_lineEdit_editingFinished();

    void on_pushButton_add_clicked();

    void on_pushButton_commit_clicked();

    void on_pushButton_change_clicked();

    void on_pushButton_file_clicked();

    void on_radioButton_name_clicked();

    void on_radioButton_number_clicked();

private:
    Ui::Widget *ui;
    QPoint z;
    QString path;
    int sortby;
    vector<Person> personArray;
};

#endif // WIDGET_H
