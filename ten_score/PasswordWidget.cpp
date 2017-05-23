#include <PasswordWidget.h>
#include <QFont>
#include <QPixmap>
#include <QPalette>
#include <QBitmap>

PasswordWidget::PasswordWidget(QWidget *parent)
    : QWidget(parent){

    QPixmap pixmap(":/images/password.png");
    QBitmap bitmap(pixmap);
    QPalette   palette;
    palette.setBrush(this->backgroundRole(),QColor(255,255,255));
    palette.setBrush(QPalette::Window,QColor(255,0,0,0));
    palette.setBrush(this->backgroundRole(),QBrush( pixmap ));
    palette.setBrush(QPalette::Window,QBrush( pixmap ));
    this->setPalette(palette);
    this->setMask(bitmap.mask());//可以将图片中透明部分显示为透明的
    this-> setAutoFillBackground( true );

    QFont font;
    font.setFamily("楷体");
    font.setPixelSize(14);
    font.setBold(true);

    label_option = new QLabel(this);
    label_option->setText("请刷卡或输入密码:");
    label_option->setFont(font);
    label_option->move(30,30);


    line_password = new QLineEdit(this);
    line_password->setEchoMode(QLineEdit::Password);//字符模式为密码
    line_password->setFont(font);
    line_password->move(30,60);

    btn_Ok = new QPushButton(this);
    btn_Ok->setText("确定");
    btn_Ok->setFont(font);
    btn_Ok->move(35,100);

    btn_Cancel = new QPushButton(this);
    btn_Cancel->setText("取消");
    btn_Cancel->setFont(font);
    btn_Cancel->move(118,100);


    this->resize(220,150);
    this->show();
}

PasswordWidget::~PasswordWidget(){

}
