#include <AboutUsWidget.h>
#include <QFont>
#include <QPixmap>
#include <QPalette>
#include <QBitmap>
#include <QSize>

AboutUsWidget::AboutUsWidget(QWidget *parent)
    : QWidget(parent){

    QFont font;
    font.setFamily("楷体");
    font.setPointSize(12);

    //字体设置颜色
    QPalette palette_text;
    palette_text.setColor(QPalette::WindowText,QColor(0,0,255));

    btn_logo = new QPushButton(this);
    btn_logo->setIcon(QIcon(":/images/logo1.jpg"));
    btn_logo->setStyleSheet("QPushButton{border-radius:5px;border-width:0px;}");//透明化处理
    btn_logo->resize(170,105);
    btn_logo->setIconSize(QSize(170,105));
    btn_logo->move(10,10);

    label_text = new QLabel(this);

    label_text->setText("GreenHouse\n\n    版本 1.0(内部版本1200)\n    版权所有 2014 十分队\n    Ten_Score智能温室控制系统及其\n    用户界面受中国和其他国家/地\n    区的商标法和其他待颁布或已\n    颁布的知识产权法保护。\n    ");
    label_text->setFont(font);
    label_text->resize(300,200);
    label_text->move(165,60);

    /**
     * @brief pixmap
     * 加载背景图
     */
    QPixmap pixmap(":images/about_us_bg_2.png");
    QBitmap bitmap(pixmap);
    QPalette   palette;
    palette.setBrush(this->backgroundRole(),QColor(255,255,255));
    palette.setBrush(this->backgroundRole(),QBrush( pixmap ));
    palette.setBrush(QPalette::Window,QBrush( pixmap ));
    this->setPalette(palette);
    this->setMask(bitmap.mask());//可以将图片中透明部分显示为透明的
    this-> setAutoFillBackground( true );

    this->resize(500,300);
    this->show();
}

AboutUsWidget::~AboutUsWidget(){

}

