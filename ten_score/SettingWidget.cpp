#include <SettingWidget.h>
#include <QIcon>

SettingWidget::SettingWidget(QWidget *parent)
    : QWidget(parent){

    QFont font;
    font.setPointSize(12);
    font.setFamily("楷体");

    btn_set_port = new QPushButton("设置串口");
    btn_set_port->setIcon(QIcon(":/images/settings.png"));
    btn_set_port->setFont(font);   

    btn_model = new QPushButton("有人模式");
    btn_model->setIcon(QIcon(":/images/sign_out_in.png"));
    btn_model->setFont(font);

    btn_about = new QPushButton("关于我们");
    btn_about->setIcon(QIcon(":/images/aboutus.png"));
    btn_about->setFont(font);

    btn_exit = new QPushButton("退出程序");
    btn_exit->setIcon(QIcon(":/images/exit1.png"));
    btn_exit->setFont(font);

    layout_v = new QVBoxLayout;
    layout_v->addWidget(btn_set_port);
    layout_v->addWidget(btn_model);
    layout_v->addWidget(btn_about);
    layout_v->addWidget(btn_exit);
    this->setLayout(layout_v);
    this->resize(120,180);
    this->show();
}

SettingWidget::~SettingWidget(){

}

