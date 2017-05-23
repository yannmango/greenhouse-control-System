#ifndef MainWidget_H
#define MainWidget_H

#include <QWidget>
#include <QImage>
#include <QPushButton>
#include <QRect>
#include <QPainter>
#include <QPixmap>
#include <QIcon>
#include <QTimer>
#include <PortSettingWidget.h>
#include <SettingWidget.h>
#include <QMouseEvent>
#include <AboutUsWidget.h>
#include <SerialPort.h>
#include <TemAndHumWidget.h>
#include <TemAndHumShowWidget.h>
#include <PasswordWidget.h>

/**
 * @brief The MainWidget class
 * 主界面
 */
class MainWidget : public QWidget
{
    Q_OBJECT

public:
    MainWidget(QWidget *parent = 0);
    ~MainWidget();

private:
    bool is_big_light_on;//大灯状态
    bool is_alarm_light_on;//台灯状态
    bool is_night_light_on;//晚灯状态
    int curtain_state;//窗帘状态
    bool is_airconditioner_on;//空调状态
    int wind_state;//风
    bool is_add_wet_on;//加湿器状态
    int fog_state;
    bool is_port_opening;//串口是否打开
    bool is_has_port;//是否有串口

    float temperature;//温度
    int humidity;//湿度
    bool shake;//震动
    bool infrared;//红外
    bool light;//光敏
    QString RFID;

private:
    QPainter *painter;

    //背景
    QRect rect_background;
    QImage img_background;

    QRect rect_doorground;
    QImage img_doorground;

    //大灯
    QRect rect_big_light;//位置矩形
    QImage img_big_light;
    //台灯
    QRect rect_alarm_light;
    QImage img_alarm_light;
    //晚灯
    QRect rect_night_light;
    QImage img_night_light;


    //窗帘
    QRect rect_curtain;
    QImage img_curtain_0;
    QImage img_curtain_1;
    QImage img_curtain_2;
    QImage img_curtain_3;
    QImage img_curtain_4;

    //灰度图
    QImage img_gray1;
    QImage img_gray2;
    QImage img_gray3;
    QImage img_gray4;

    //logo
    QPixmap *pixmap_logo;
    QIcon *icon_logo;

    //空调效果雪花
    QPixmap * pixmap_wind_1;
    QPixmap * pixmap_wind_2;
    QPixmap * pixmap_wind_3;

    QIcon *icon_wind_1;
    QIcon *icon_wind_2;
    QIcon *icon_wind_3;

    QPushButton *btn_wind_1;
    QPushButton *btn_wind_2;
    QPushButton *btn_wind_3;

    QTimer *air_timer;//空调定时器

    //加湿器效果
    QPixmap *pixmap_add_wet_1;
    QPixmap *pixmap_add_wet_2;
    QPixmap *pixmap_add_wet_3;
    QIcon *icon_add_wet_1;
    QIcon *icon_add_wet_2;
    QIcon *icon_add_wet_3;

    QPushButton *btn_add_wet_1;
    QPushButton *btn_add_wet_2;
    QPushButton *btn_add_wet_3;
    QPushButton *btn_add_wet;//供点击，透明

    QTimer *wet_timer;//加湿器定时器

    QPushButton *btn_all;
    QPushButton *btn_big_light;
    QPushButton *btn_alarm_light;
    QPushButton *btn_night_light;
    QPushButton *btn_curtain_open;
    QPushButton *btn_curtain_close;
    QPushButton *btn_airconditioner;

    SettingWidget * setting_widget;//设置对话框
    bool is_setting_weiget_show;
    PortSettingWidget *port_setting_widget;//串口设置对话框
    bool is_port_setting_widget_show;
    AboutUsWidget *about_us_widget;
    bool is_about_us_widget;

    SerialPort * port;//串口
    QStringList list_temperature;//温度数据列表
    QStringList list_humidity;//湿度数据列表

    QPushButton *btn_temAndHum;//打开温湿度计开关
    TemAndHumShowWidget *temAndHum_show_widget;//温湿度显示面板
    TemAndHumWidget *temAndHum_widget;//温湿度曲线面板
    bool is_temAndHum;//温湿度计面板是否显示
    bool is_temAndHum_line;//温湿度曲线面板是否显示

    PasswordWidget *password_widget;//密码框
    bool is_password_widget;//密码面板是否显示

    QTimer *timer_alarm_light;

    bool is_timer_alarm_on;


public:
    void load_resource();
    void paint_background(QPainter *painter);
    void paint_doorground(QPainter *painter);
    void paint_big_light(QPainter *painter,bool is_big_light_on);
    void paint_alarm_light(QPainter *painter,bool is_alarm_light_on);
    void paint_night_light(QPainter *painter,bool is_night_light_on);
    void paint_curtain(QPainter *painter,int curtain_state);
    void paint_gray(QPainter *painter);//画灰度

    void show_setting_weiget(int y);//弹出设置对话框
    void hide_setting_weiget();//隐藏设置对话框

    void show_port_setting_weiget();//弹出串口设置对话框
    void hide_port_setting_weiget();//隐藏串口设置对话框


    void hide_about_us_widget();//隐藏关于我们对话框

    void check_port();//检测串口
    void open_port();//根据检测到的串口开启
    void start_port();//开始串口
    void stop_port();//停止串口

    QTimer *timer_port_data;//定时获取串口数据
    bool is_timer_port_data;//定时器是否为开

    void write_temperature_data();//写温度数据
    void write_humidity_data();//写湿度数据
    void data_control_big_light_and_curtain();//光敏数据控制大灯和窗帘
    void data_control_airconditioner();//温度数据控制空调
    void data_control_add_wet();//湿度数据控制加湿器



private slots:
    void close_all();//关闭所有的电器
    void control_big_light();
    void control_alarm_light();
    void control_night_light();
    void close_curtain();
    void open_curtain();
    //空调
    void control_airconditioner();
    void open_airconditioner();
    //加湿器
    void control_add_wet();
    void open_add_wet();
    //设置串口面板
    void open_setting_port_widget();
    void show_about_us_widget();//弹出关于我们对话框

    void port_setting_Ok();//设置串口面板确定按钮槽函数
    void port_setting_Cancel();//设置串口面板取消按钮槽函数

    void get_port_data();//获取串口数据

    void show_temAndHum_widget();//显示温湿度计面板
    void hide_temAndHum_widget();//隐藏温湿度计面板

    void show_temAndHum_line_widget();//显示温湿度曲线面板
    void hide_temAndHum_line_widget();//隐藏温湿度曲线面板

    void hide_temAndHum_and_show_line();//隐藏温湿度面板，显示温湿度曲线
    void hide_line_and_show_temAndHum();//隐藏温湿度曲线，显示温湿度计面板

    void show_password_widget();//弹出密码面板
    void hide_password_widget();//隐藏密码面板

    void check_password();//验证密码

    void shake_window();//窗口抖动

    void switch_model();//切换模式

protected:
    void paintEvent(QPaintEvent *event);
    void mouseMoveEvent(QMouseEvent*event);
    void mousePressEvent(QMouseEvent *event);
    void closeEvent(QCloseEvent *event);
};

#endif // MainWidget_H
