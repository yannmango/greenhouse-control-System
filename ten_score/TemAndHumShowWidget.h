#ifndef TEMANDHUMSHOWWIDGET_H
#define TEMANDHUMSHOWWIDGET_H

#include <QWidget>
#include <QLCDNumber>
#include <QLabel>
#include <QPushButton>

/**
 * @brief The TemAndHumShowWidget class
 * 温湿度显示面板
 */
class TemAndHumShowWidget:public QWidget{
    Q_OBJECT

public :
    TemAndHumShowWidget(QWidget *parent =0);
    ~TemAndHumShowWidget();
private:
    QLabel *label_temperature;//温度标签
    QLCDNumber *num_temperature;//温度

    QLabel *label_humidity;//湿度标签
    QLCDNumber *num_humidity;//湿度
public:
    QPushButton *btn_go_line;//显示温湿度曲线
    QPushButton *btn_close;//关闭

public:
    void show_label_button();//显示标签
    void show_temperature(float temperature);//初始化温度数据显示
    void show_humidity(int humidity);//初始化湿度数据显示
    void set_temperature_and_humidity(float temperature,int humidity);//根据温度和湿度数据显示
    void set_position();//界面控件重定位
    void reset_positon();//界面控件恢复定位
};

#endif // TEMANDHUMSHOWWIDGET_H
