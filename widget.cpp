#include "widget.h"
#include "ui_widget.h"


Widget::Widget(QWidget *parent) :
    QWidget(parent),sortby(0),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint);//设置无边框
    this->setAttribute(Qt::WA_TranslucentBackground);//设置主窗口透明

    ui->widget_list->setStyleSheet("#widget_list{background-color: rgb(220, 238, 247);border-left:1px solid rgb(6, 163, 247)}");
    ui->widget_set->setStyleSheet("background-color: none");
    ui->label_list->setStyleSheet(QString("border-image:url(%1)").arg(":/通讯录.png"));
    ui->label_set->setStyleSheet(QString("border-image:url(%1)").arg(":/设置 (1).png"));
    ui->radioButton_name->setChecked(true);
    this->path=QFileDialog::getOpenFileName(this,tr("选择文件"),"./",tr("(*.txt)"));
    this->getInfoFromFile();
    this->addToListWidget();
}
Widget::~Widget()
{
    delete ui;
}
//从文件获取联系人信息
void Widget::getInfoFromFile(){
    QFile file(path);
    if(file.open(QIODevice::ReadOnly)){
        QTextStream in(&file);
        in.setCodec("utf-8");
        ui->lineEdit_path->setText(file.fileName());
        while(!in.atEnd())
        {
            Person p;
            in>>p.name>>p.number;
            personArray.push_back(p);
        }
        personArray.pop_back();
        file.close();
    }
}
//排序回调函数
bool cmpbyName(Person p1,Person p2){
        QTextCodec *utf8 = QTextCodec::codecForName("UTF-8");
        QTextCodec* gbk = QTextCodec::codecForName("gbk");
         QByteArray byte_p1,byte_p2;
            byte_p1= gbk->fromUnicode(utf8->toUnicode(p1.name.toStdString().c_str()));
            byte_p2= gbk->fromUnicode(utf8->toUnicode(p2.name.toStdString().c_str()));
        return byte_p1<byte_p2;
}
bool cmpbyNumber(Person p1,Person p2){
        QTextCodec *utf8 = QTextCodec::codecForName("UTF-8");
        QTextCodec* gbk = QTextCodec::codecForName("gbk");
         QByteArray byte_p1,byte_p2;
            byte_p1= gbk->fromUnicode(utf8->toUnicode(p1.number.toStdString().c_str()));
            byte_p2= gbk->fromUnicode(utf8->toUnicode(p2.number.toStdString().c_str()));
        return byte_p1<byte_p2;
}
//添加联系人信息到ListWidget中
void Widget::addToListWidget(){

    if(personArray.empty())return;
    //先排序
    if(this->sortby==0)sort(personArray.begin(),personArray.end(),cmpbyName);
    else if(this->sortby==1)sort(personArray.begin(),personArray.end(),cmpbyNumber);

    for(vector<Person>::iterator it=personArray.begin();it!=personArray.end();it++)
    {
        QString str=QString("%1      %2").arg((*it).name).arg((*it).number);
        QListWidgetItem* item =new QListWidgetItem(str);
        item->setTextAlignment(Qt::AlignVCenter);
        item->setFont(QFont("微软雅黑",10));
        ui->listWidget->addItem(item);
    }
}
void Widget::saveInfoToFile(){
    QFile file(path);
    if(file.open(QIODevice::WriteOnly)){
        QTextStream out(&file);
        out.setCodec("utf-8");
        for(vector<Person>::iterator it=personArray.begin();it!=personArray.end();it++)
        {
            out<<(*it).name<<"      "<<(*it).number<<endl;
        }
        file.close();
    }
}
//点击添加联系人按钮
void Widget::on_pushButton_add_clicked()
{
    ui->label_title->setText("添加联系人");
    ui->pushButton_commit->setText("添加");
    ui->stackedWidget->setCurrentIndex(2);

}
//删除联系人
void Widget::on_pushButton_delete_clicked()
{
    int index=ui->listWidget->currentIndex().row();
    if(index<0)return;
    ui->listWidget->clear();
    personArray.erase(personArray.begin()+index);
    this->addToListWidget();
}
//搜索
void Widget::on_lineEdit_editingFinished()
{
    QString text=ui->lineEdit->text();
    for(vector<Person>::iterator it=personArray.begin();it!=personArray.end();it++)
    {
        if((*it).name.contains(text)||(*it).number.contains(text)){
            ui->listWidget->setCurrentRow(it-personArray.begin());
            return;
        }
    }
}
//点击确定
void Widget::on_pushButton_commit_clicked()
{
    QString name=ui->lineEdit_name->text();
    QString number=ui->lineEdit_number->text();
    if(name.isEmpty()||number.isEmpty()) return;
    if(ui->label_title->text()=="添加联系人"){
        Person p;
        p.name=name;
        p.number=number;
        personArray.push_back(p);
        ui->listWidget->clear();
        this->addToListWidget();
    }
    else if(ui->label_title->text()=="编辑联系人"){
        int index=ui->listWidget->currentIndex().row();
        (*(personArray.begin()+index)).name=name;
        (*(personArray.begin()+index)).number=number;
        ui->listWidget->clear();
        this->addToListWidget();
    }
    ui->stackedWidget->setCurrentIndex(0);
    ui->label_title->setText("通讯录列表");
    ui->lineEdit_name->setText("");
    ui->lineEdit_number->setText("");
    return;
}
//点击修改联系人按钮
void Widget::on_pushButton_change_clicked()
{
    int index=ui->listWidget->currentIndex().row();
    if(index<0)return;
    ui->label_title->setText("编辑联系人");
    ui->lineEdit_name->setText((*(personArray.begin()+index)).name);
    ui->lineEdit_number->setText((*(personArray.begin()+index)).number);
    ui->pushButton_commit->setText("修改");
    ui->stackedWidget->setCurrentIndex(2);
}
//选择文件路径
void Widget::on_pushButton_file_clicked()
{
    QString filePath=QFileDialog::getOpenFileName(this,tr("选择文件"),"./",tr("(*.txt)"));
    if(filePath.isEmpty())return;
    this->saveInfoToFile();
    this->path=filePath;
    this->personArray.clear();
    this->getInfoFromFile();
    ui->listWidget->clear();
    this->addToListWidget();
}

//选择排序方式
void Widget::on_radioButton_name_clicked()
{
    this->sortby=0;
    ui->listWidget->clear();
    this->addToListWidget();
}
void Widget::on_radioButton_number_clicked()
{
    this->sortby=1;
    ui->listWidget->clear();
    this->addToListWidget();
}
//重写鼠标事件，使窗口可以自由拖动
void  Widget::mousePressEvent(QMouseEvent *event){
  QWidget::mousePressEvent(event);
  QPoint p1=event->globalPos();
  QPoint p2=this->geometry().topLeft();
  this->z=p1-p2;
}
void Widget::mouseMoveEvent(QMouseEvent *event)
{
    QWidget::mouseMoveEvent(event);
    QPoint p1=event->globalPos();
    QPoint p2=p1-this->z;
    this->move(p2);
}
//窗口关闭事件（保存信息到文件）
void Widget::closeEvent(QCloseEvent* event){
    qDebug()<<"关闭"<<endl;
    this->saveInfoToFile();
    QWidget::closeEvent(event);
}
//绘画阴影
void Widget::paintEvent(QPaintEvent *event)
{
    QPainterPath path;
    path.setFillRule(Qt::WindingFill);
    QRectF rect(10, 10, this->width()-20, this->height()-20);
    path.addRoundRect(rect, 8, 8);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.fillPath(path, QBrush(Qt::white));

    QColor color(0, 0, 0, 10);
    for(int i = 0; i < 5; i++) {
        QPainterPath path;
        path.setFillRule(Qt::WindingFill);
        path.addRect(10-i, 10-i, this->width()-(10-i)*2, this->height()-(10-i)*2);
        color.setAlpha(30 - qSqrt(i)*15);
        painter.setPen(color);
        painter.drawPath(path);
    }
}
//点击关闭按钮
void Widget::on_pushButton_close_clicked()
{
    this->close();
}
//点击最小化按钮
void Widget::on_pushButton_min_clicked()
{
    this->showMinimized();
}
//左边栏点击列表
void Widget::on_pushButton_list_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
    ui->widget_list->setStyleSheet("#widget_list{background-color: rgb(220, 238, 247);border-left:1px solid rgb(6, 163, 247)}");
    ui->widget_set->setStyleSheet("background-color: none");
    ui->label_title->setText("通讯录列表");

}
//左边栏点击设置
void Widget::on_pushButton_set_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
    ui->widget_set->setStyleSheet("#widget_set{background-color: rgb(220, 238, 247);border-left:1px solid rgb(6, 163, 247)}");
    ui->widget_list->setStyleSheet("background-color: none");
    ui->label_title->setText("设置");
}

