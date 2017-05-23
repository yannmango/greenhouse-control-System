#ifndef SETTINGWIDGET_H
#define SETTINGWIDGET_H

#include <QPushButton>
#include <QVBoxLayout>

/**
 * @brief The SettingWidget class
 * 自定义边栏设置对话框样式
 */
class SettingWidget: public QWidget{

    Q_OBJECT
public :
    SettingWidget(QWidget *parent =0);
    ~SettingWidget();
public:
    QPushButton *btn_set_port;//串口设置
    QPushButton *btn_model;//模式设置
    QPushButton *btn_about;//关于
    QPushButton *btn_exit;//退出

    QVBoxLayout *layout_v;

};

#endif // SETTINGWIDGET_H
