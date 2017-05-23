#include "MainWidget.h"
#include <QtDebug>
#include <QSize>
#include <QMainWindow>
#include <QPropertyAnimation>
#include <QSequentialAnimationGroup>
#include <QMessageBox>
#include <QThread>
#include <QFile>
#include <QTextStream>
#include <QTime>

MainWidget::MainWidget(QWidget *parent)
    : QWidget(parent)
{

    this->resize(1023,607);
    this->setWindowIcon(QIcon(":/images/logo.jpg"));
    this->setWindowTitle(tr("IOTer"));
    //设置不可放大缩小
    this->setWindowFlags(Qt::FramelessWindowHint);
    //初始化资源
    load_resource();
    this->setMouseTracking(true);//设置可追踪鼠标
}

/**
 * @brief MainWidget::load_resource
 * 初始化资源及属性
 */
void MainWidget::load_resource(){
    //背景图
    img_doorground.load(":/images/door.jpg");
    rect_doorground.setRect(0,0,1023,607);


    //状态
    curtain_state = 0;//开始窗帘为开
    is_big_light_on = false;//补光灯为关
    is_alarm_light_on = false;//报警灯开始为关
    is_night_light_on = false;//晚灯开始为关
    is_airconditioner_on = false;//开始时空调为关
    wind_state = 1;
    is_add_wet_on = false;//开始时加湿器为关
    fog_state = 1;//喷雾状态
    is_setting_weiget_show = false;//开始时设置面板隐藏
    is_port_setting_widget_show = false;//开始时串口设置面板隐藏
    is_about_us_widget = false; //开始时关于我们面板隐藏
    is_port_opening = false;//初始串口是否打开
    is_has_port = false ;//是否有串口
    is_timer_port_data = false;//开始时获取数据定时器为关
    is_temAndHum = false;//初始温度计面板没有显示
    is_temAndHum_line = false;//初始时温湿度曲线面板没有显示
    is_password_widget = false;

    //窗帘

    img_curtain_1.load(":/images/2.jpg");
    img_curtain_2.load(":/images/3.jpg");
    img_curtain_3.load(":/images/4.jpg");
    img_curtain_4.load(":/images/5.jpg");
    rect_curtain.setRect(0,0,1023,607);


    //总开关
    btn_all = new QPushButton(this);
    btn_all->resize(50,50);
    btn_all->move(0,557);
    btn_all->setCursor(Qt::PointingHandCursor);
    btn_all->setStyleSheet("QPushButton{border-radius:5px;border-width:0px;}");//透明化处理
    connect(btn_all,SIGNAL(clicked()),this,SLOT(close_all()));

    //补光灯
    img_big_light.load(":/images/big_light_on.jpg");
    rect_big_light.setRect(0,0,1023,607);

    //晚灯
    img_night_light.load(":/images/night.jpg");
    rect_night_light.setRect(0,0,1023,607);

    //报警灯
    img_alarm_light.load(":/images/alarm.jpg");
    rect_alarm_light.setRect(0,0,1023,607);



    //按钮
    //补光灯按钮
    btn_big_light = new QPushButton(this);
    btn_big_light->resize(160,80);
    btn_big_light->move(450,70);
    btn_big_light->setCursor(Qt::PointingHandCursor);
    btn_big_light->setStyleSheet("QPushButton{border-radius:5px;border-width:0px;}");//透明化处理
    connect(btn_big_light,SIGNAL(clicked()),this,SLOT(control_big_light()));
    //报警灯按钮
    btn_alarm_light = new QPushButton(this);
    btn_alarm_light->resize(25,25);
    btn_alarm_light->move(1,1);
    btn_alarm_light->setCursor(Qt::PointingHandCursor);
    btn_alarm_light->setStyleSheet("QPushButton{border-radius:5px;border-width:0px;}");//透明化处理
    connect(btn_alarm_light,SIGNAL(clicked()),this,SLOT(control_alarm_light()));

    //晚灯按钮
    btn_night_light = new QPushButton(this);
    btn_night_light->resize(25,25);
    btn_night_light->move(75,75);
    btn_night_light->setCursor(Qt::PointingHandCursor);
    btn_night_light->setStyleSheet("QPushButton{border-radius:5px;border-width:0px;}");//透明化处理
    connect(btn_night_light,SIGNAL(clicked()),this,SLOT(control_night_light()));

    //关窗帘按钮
    btn_curtain_close = new QPushButton(this);
    //btn_curtain_open->setToolTip(tr("关闭窗帘"));
    btn_curtain_close->resize(50,50);
    btn_curtain_close->move(40,200);
    btn_curtain_close->setCursor(Qt::PointingHandCursor);
    btn_curtain_close->setStyleSheet("QPushButton{border-radius:5px;border-width:0px;}");//透明化处理
    connect(btn_curtain_close,SIGNAL(clicked()),this,SLOT(close_curtain()));

    //开窗帘按钮
    btn_curtain_open = new QPushButton(this);
    btn_curtain_open->resize(50,50);
    btn_curtain_open->move(40,300);
    btn_curtain_open->setCursor(Qt::PointingHandCursor);
    btn_curtain_open->setStyleSheet("QPushButton{border-radius:5px;border-width:0px;}");//透明化处理
    connect(btn_curtain_open,SIGNAL(clicked()),this,SLOT(open_curtain()));

    //风1
    pixmap_wind_1 = new QPixmap(50,30);
    pixmap_wind_1->load(tr(":/images/w1.png"));
    btn_wind_1 = new QPushButton(this);
    icon_wind_1 = new QIcon(*pixmap_wind_1);
    btn_wind_1->setIcon(*icon_wind_1);
    btn_wind_1->setIconSize(QSize(50,30));
    btn_wind_1->setStyleSheet("QPushButton{border-radius:5px;border-width:0px;}");//透明化处理
    btn_wind_1->resize(50,30);
    btn_wind_1->move(680,220);
    btn_wind_1->hide();//先隐藏

     //风2
    pixmap_wind_2 = new QPixmap(60,40);
    pixmap_wind_2->load(tr(":/images/w2.png"));
    btn_wind_2 = new QPushButton(this);
    icon_wind_2 = new QIcon(*pixmap_wind_2);
    btn_wind_2->setIcon(*icon_wind_2);
    btn_wind_2->setIconSize(QSize(60,40));
    btn_wind_2->setStyleSheet("QPushButton{border-radius:5px;border-width:0px;}");//透明化处理
    btn_wind_2->resize(60,40);
    btn_wind_2->move(670,230);
    btn_wind_2->hide();

    //风3
    pixmap_wind_3 = new QPixmap(80,50);
    pixmap_wind_3->load(tr(":/images/w3.png"));
    btn_wind_3 = new QPushButton(this);
    icon_wind_3 = new QIcon(*pixmap_wind_3);
    btn_wind_3->setIcon(*icon_wind_3);
    btn_wind_3->setIconSize(QSize(80,50));
    btn_wind_3->setStyleSheet("QPushButton{border-radius:5px;border-width:0px;}");//透明化处理
    btn_wind_3->resize(80,50);
    btn_wind_3->move(660,240);
    btn_wind_3->hide();

    //空调开关
    btn_airconditioner = new QPushButton(this);
    btn_airconditioner->resize(70,100);
    btn_airconditioner->move(680,200);
    btn_airconditioner->setCursor(Qt::PointingHandCursor);
    btn_airconditioner->setStyleSheet("QPushButton{border-radius:5px;border-width:0px;}");//透明化处理
    connect(btn_airconditioner,SIGNAL(clicked()),this,SLOT(control_airconditioner()));

    //加湿器1
    pixmap_add_wet_1 = new QPixmap(200,350);
    pixmap_add_wet_1->load(tr(":/images/fog_1.png"));
    btn_add_wet_1 = new QPushButton(this);
    icon_add_wet_1 = new QIcon(*pixmap_add_wet_1);
    btn_add_wet_1->setIcon(*icon_add_wet_1);
    btn_add_wet_1->setIconSize(QSize(200,350));
    btn_add_wet_1->setStyleSheet("QPushButton{border-radius:5px;border-width:0px;}");//透明化处理
    btn_add_wet_1->resize(200,350);
    btn_add_wet_1->move(130,207);
    btn_add_wet_1->hide();//隐藏

    //加湿器2
    pixmap_add_wet_2 = new QPixmap(200,350);
    pixmap_add_wet_2->load(tr(":/images/fog_2.png"));
    btn_add_wet_2 = new QPushButton(this);
    icon_add_wet_2 = new QIcon(*pixmap_add_wet_2);
    btn_add_wet_2->setIcon(*icon_add_wet_2);
    btn_add_wet_2->setIconSize(QSize(200,350));
    btn_add_wet_2->setStyleSheet("QPushButton{border-radius:5px;border-width:0px;}");//透明化处理
    btn_add_wet_2->resize(200,350);
    btn_add_wet_2->move(130,207);
    btn_add_wet_2->hide();//隐藏

    //加湿器3
    pixmap_add_wet_3 = new QPixmap(200,350);
    pixmap_add_wet_3->load(tr(":/images/fog_3.png"));
    btn_add_wet_3 = new QPushButton(this);
    icon_add_wet_3 = new QIcon(*pixmap_add_wet_3);
    btn_add_wet_3->setIcon(*icon_add_wet_3);
    btn_add_wet_3->setIconSize(QSize(200,350));
    btn_add_wet_3->setStyleSheet("QPushButton{border-radius:5px;border-width:0px;}");//透明化处理
    btn_add_wet_3->resize(200,350);
    btn_add_wet_3->move(130,207);
    btn_add_wet_3->hide();//隐藏

    btn_add_wet = new QPushButton(this);
    btn_add_wet->resize(200,200);
    btn_add_wet->move(130,357);
    btn_add_wet->setCursor(Qt::PointingHandCursor);
    btn_add_wet->setStyleSheet("QPushButton{border-radius:5px;border-width:0px;}");//透明化处理
    connect(btn_add_wet,SIGNAL(clicked()),this,SLOT(control_add_wet()));

    //设置对话框
    setting_widget = new SettingWidget(this);
    setting_widget->move(1023,350);
    setting_widget->show();

    //设置串口按钮事件
    connect(setting_widget->btn_set_port,SIGNAL(clicked()),this,SLOT(open_setting_port_widget()));
    //切换模式
    connect(setting_widget->btn_model,SIGNAL(clicked()),this,SLOT(show_password_widget()));
    connect(setting_widget->btn_about,SIGNAL(clicked()),this,SLOT(show_about_us_widget()));
    connect(setting_widget->btn_exit, SIGNAL(clicked()),this, SLOT(close()));
    //串口设置对话框
    port_setting_widget = new PortSettingWidget(this);
    port_setting_widget->move(1023,300);
    port_setting_widget->show();
    connect(port_setting_widget->btn_ok,SIGNAL(clicked()),this,SLOT(port_setting_Ok()));
    connect(port_setting_widget->btn_cancel,SIGNAL(clicked()),this,SLOT(port_setting_Cancel()));

    btn_temAndHum = new QPushButton(this);
    btn_temAndHum->resize(80,80);
    btn_temAndHum->move(870,500);
    btn_temAndHum->setCursor(Qt::PointingHandCursor);
    btn_temAndHum->setStyleSheet("QPushButton{border-radius:5px;border-width:0px;}");
    connect(btn_temAndHum,SIGNAL(clicked()),this,SLOT(show_temAndHum_widget()));

    //关于我们对话框
    about_us_widget = new AboutUsWidget(this);
    about_us_widget->move(267,-300);
    about_us_widget->show();

    this->check_port();//打开串口
    this->open_port();//根据检测到的串口开启


    //温湿度显示面板
    temAndHum_show_widget = new TemAndHumShowWidget(this);
    temAndHum_show_widget->move(-1023,200);
    temAndHum_show_widget->show();
    connect(temAndHum_show_widget->btn_close,SIGNAL(clicked()),this,SLOT(hide_temAndHum_widget()));
    connect(temAndHum_show_widget->btn_go_line,SIGNAL(clicked()),this,SLOT(hide_temAndHum_and_show_line()));

    //温度曲线面板
    temAndHum_widget = new TemAndHumWidget(this);
    temAndHum_widget->move(-1023,200);
    temAndHum_widget->show();
    connect(temAndHum_widget->btn_close,SIGNAL(clicked()),this,SLOT(hide_temAndHum_line_widget()));
    connect(temAndHum_widget->btn_go,SIGNAL(clicked()),this,SLOT(hide_line_and_show_temAndHum()));
    //密码显示面板
    password_widget = new PasswordWidget(this);
    password_widget->move(1023,200);
    password_widget->show();
    connect(password_widget->btn_Cancel,SIGNAL(clicked()),this,SLOT(hide_password_widget()));
    connect(password_widget->btn_Ok,SIGNAL(clicked()),this,SLOT(check_password()));

    timer_alarm_light = new QTimer(this);
    timer_alarm_light->setInterval(180);
    connect(timer_alarm_light,SIGNAL(timeout()),this,SLOT(control_alarm_light()));
    is_timer_alarm_on = false;


}

/**
 * @brief MainWidget::check_port
 * 打开串口函数
 */
void MainWidget::check_port(){
    //检测当前主机所有的串口
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
    {
        qDebug() << "Name        : " << info.portName();
        qDebug() << "Description : " << info.description();
        qDebug() << "Manufacturer: " << info.manufacturer();

        QSerialPort serial;
        serial.setPort(info);
        if (serial.open(QIODevice::ReadWrite))
        {

           //将串口名称加入串口设置面板
           port_setting_widget->combobox_port_name->clear();
           port_setting_widget->combobox_port_name->addItem(info.portName());
           serial.close();
        }
    }
    //如果有串口的话    
    if(port_setting_widget->combobox_port_name->currentText()!=""){
        is_has_port = true;
    }else{
        is_has_port = false;
    }

}

/**
 * @brief MainWidget::open_port
 * 根据检测到的串口开启
 */
void MainWidget::open_port(){
    if(is_has_port){
        start_port();
    }else if(!is_has_port){
        is_port_opening = false;
        QMessageBox::information(this,"提示","当前主机没有检测到串口，请检查！");
    }
}

/**
 * @brief MainWidget::start_port
 * 开启串口
 */
void MainWidget::start_port(){
    port = new SerialPort();//串口实例化
    //设置串口名称
    qDebug()<<"串口名称："<<port_setting_widget->combobox_port_name->currentText()<<endl;
    port->port_set_name(port_setting_widget->combobox_port_name->currentText());
    port->port_open();
    is_port_opening = true;
    //设置参数
    port->port_set(port_setting_widget->combobox_baud_rate->currentText(),
                   port_setting_widget->combobox_data_bits->currentText(),
                   port_setting_widget->combobox_stop_bits->currentText(),
                   port_setting_widget->combobox_parity->currentText());

    port->port_start();

    timer_port_data = new QTimer(this);
    timer_port_data->setInterval(200);
    connect(timer_port_data,SIGNAL(timeout()),this,SLOT(get_port_data()));
    timer_port_data->start();
    is_timer_port_data = true;
}

/**
 * @brief MainWidget::get_port_data
 * 读取串口数据
 */
void MainWidget::get_port_data(){
    this->temperature = port->get_temperature();
    this->humidity = port->get_humidity();
    this->shake = port->get_shake();
    this->light = port->get_light();
    this->infrared = port->get_infrared();
    this->RFID = port->get_RFID();//获取RFID
    this->port->reset_RFID();
    //实时更新温湿度面板
    temAndHum_show_widget->set_temperature_and_humidity(this->temperature,this->humidity);

    //温度数据入列表
    if(list_temperature.length()==20){
        list_temperature.removeFirst();//去掉第一个
        list_temperature.append(QString::number(this->temperature));
    }else if(list_temperature.length()<20){
        list_temperature.append(QString::number(this->temperature));
    }
    //写入文件
    this->write_temperature_data();

    //湿度数据入列表
    if(list_humidity.length() == 20){
         list_humidity.removeFirst();
         list_humidity.append(QString::number(this->humidity));
    }else if(list_humidity.length()<20){
         list_humidity.append(QString::number(this->humidity));
    }
    //写入文件
    this->write_humidity_data();

    /*验证RFID卡号*/

    if(this->is_password_widget&&RFID!=""){
        password_widget->line_password->setText(this->RFID);
        check_password();
    }


    //数据控制
    if(setting_widget->btn_model->text()=="无人模式"){
        //当有人模式时才用数据控制
        data_control_big_light_and_curtain();
        data_control_airconditioner();
        data_control_add_wet();
    }

    else if(setting_widget->btn_model->text()=="有人模式"){


    }
}

/**
 * @brief MainWidget::write_temperature_data
 * 温度数据写入文件
 */
void MainWidget::write_temperature_data(){
    //QFile f("E:/data/briup/QT/IOTer/temperature.txt");
    QFile f(".temperature.txt");
    if(!f.open(QIODevice::ReadWrite | QIODevice::Text))
    {
        qDebug() << "Open failed." << endl;
    }
    QTextStream txtOutput(&f);

    for (QStringList::Iterator it=list_temperature.begin();it!= list_temperature.end();it++){
        txtOutput << *it << endl;
    }
    f.close();
}

/**
 * @brief MainWidget::write_humidity_data
 * 湿度数据写入文件
 */
void MainWidget::write_humidity_data(){
    //QFile f("E:/data/briup/QT/IOTer/humidity.txt");
    QFile f("./humidity.txt");
    if(!f.open(QIODevice::ReadWrite | QIODevice::Text))
    {
        qDebug() << "Open failed." << endl;
    }
    QTextStream txtOutput(&f);

    for (QStringList::Iterator it=list_humidity.begin();it!= list_humidity.end();it++){
        txtOutput << *it << endl;
    }
    f.close();
}

/**
 * @brief MainWidget::stop_port
 * 停止串口
 */
void MainWidget::stop_port(){
    if(is_port_opening){
        port->port_stop();
        port->port_close();
    }
}

/**
 * @brief MainWidget::port_setting_Ok
 * 串口设置面板确定按钮槽函数
 */
void MainWidget::port_setting_Ok(){
    if(is_port_opening){
        stop_port();
        start_port();
        hide_port_setting_weiget();
        return;
    }else{
         start_port();
         hide_port_setting_weiget();
         return;
    }
}

/**
 * @brief MainWidget::port_setting_Cancel
 *  串口设置面板取消按钮槽函数
 */
void MainWidget::port_setting_Cancel(){
    hide_port_setting_weiget();//隐藏串口设置面板即可
}

/**
 * @brief MainWidget::data_control_big_light
 * 串口光敏数据控制补光灯和窗帘
 */
void MainWidget::data_control_big_light_and_curtain(){
    QTime  time;
    time = QTime::currentTime();

    if(time.hour()<=8&&time.hour()>=6){
    if(this->light){
        close_curtain();
        if(is_big_light_on){
            is_big_light_on = false;
            port->control_electrical_machine_left();//控制电机反转
        }

    }else if(!this->light){
        open_curtain();

        if(!is_big_light_on){
            is_big_light_on = true;
            this->port->control_led();//控制流水灯
            port->control_electrical_machine_right();//控制电机正转
        }
    }
    }
    else{
        if(this->shake&&infrared)
        {
            is_night_light_on = true;
        }
    }
    update();    
}

/**
 * @brief MainWidget::data_control_airconditioner
 * 串口温度数据控制空调
 */
void MainWidget::data_control_airconditioner(){
    if(this->temperature>=24.0||this->temperature<=18.0){//温度大于等于24度或小于18度开空调
        if(!is_airconditioner_on){
            is_airconditioner_on = false;//因为control_airconditioner函数会反转这个状态
            control_airconditioner();
        }
    }else{
        if(is_airconditioner_on){
            is_airconditioner_on = true;//因为control_airconditioner函数会反转这个状态
            control_airconditioner();
        }
    }
}

/**
 * @brief MainWidget::data_control_add_wet
 * 串口湿度数据控制加湿器
 */
void MainWidget::data_control_add_wet(){
    if(this->humidity < 62){
        if(!is_add_wet_on){
            is_add_wet_on = false;
            control_add_wet();
        }
    }else{
        if(is_add_wet_on){
            is_add_wet_on = true;
            control_add_wet();
        }
    }
}

/**
 * @brief MainWidget::paint_background
 * @param painter
 * 绘制背景函数
 */
void MainWidget::paint_background(QPainter *painter){
    painter->drawImage(rect_background,img_background);
}

void MainWidget::paint_doorground(QPainter *painter){
    painter->drawImage(rect_doorground,img_doorground);
}


/**
 * @brief MainWidget::control_big_light
 *  控制补光灯状态槽函数
 */
void MainWidget::control_big_light(){
    is_big_light_on = !is_big_light_on;
    update();
}

/**
 * @brief MainWidget::paint_big_light
 * @param painter
 * @param is_big_light_on
 *  绘制补光灯函数
 */
void MainWidget::paint_big_light(QPainter *painter,bool is_big_light_on){
    if(is_big_light_on){
        painter->drawImage(rect_big_light,img_big_light);
    }else{
        painter->drawImage(rect_background,img_gray1);
    }
}

/**
 * @brief MainWidget::control_alarm_light
 * 控制报警灯槽函数
 */
void MainWidget::control_alarm_light(){
    is_alarm_light_on = !is_alarm_light_on;
    update();
}

/**
 * @brief MainWidget::control_night_light
 * 控制晚灯槽函数
 */
void MainWidget::control_night_light(){
    is_night_light_on = !is_night_light_on;
    update();
}


/**
 * @brief MainWidget::paint_night_light
 * @param painter
 * @param is_night_light_on
 * 绘制晚灯函数
 */
void MainWidget::paint_night_light(QPainter *painter, bool is_night_light_on){
    if(is_night_light_on){
        painter->drawImage(rect_night_light,img_night_light);
    }else{
        painter->drawImage(rect_background,img_gray1);
    }
}

/**
 * @brief MainWidget::paint_alarm_light
 * @param painter
 * @param is_alarm_light_on
 * 绘制报警灯函数
 */
void MainWidget::paint_alarm_light(QPainter *painter, bool is_alarm_light_on){
    if(is_alarm_light_on){
        painter->drawImage(rect_alarm_light,img_alarm_light);
    }else{
        painter->drawImage(rect_background,img_gray1);
    }
}

/**
 * @brief MainWidget::close_curtain
 * 打开窗帘槽函数
 */
void MainWidget::close_curtain(){
    curtain_state++;
    if(curtain_state > 4){
        curtain_state = 4;
    }
    update();
}

/**
 * @brief MainWidget::open_curtain
 * 关闭窗帘槽函数
 */
void MainWidget::open_curtain(){

    curtain_state--;
    if(curtain_state < 0){
        curtain_state = 0;
    }
    update();
}

/**
 * @brief MainWidget::paint_curtain
 * @param painter
 * @param curtain_state
 *  绘制窗帘函数
 */
void MainWidget::paint_curtain(QPainter *painter,int curtain_state){
    switch(curtain_state){
        case 0 :
            painter->drawImage(rect_curtain,img_curtain_0);
            break;
        case 1 :
            painter->drawImage(rect_curtain,img_curtain_1);
            painter->drawImage(rect_background,img_gray1);
            break;
        case 2 :
            painter->drawImage(rect_curtain,img_curtain_2);
            painter->drawImage(rect_background,img_gray2);
            break;
        case 3 :
            painter->drawImage(rect_curtain,img_curtain_3);
            painter->drawImage(rect_background,img_gray3);
            break;
        case 4 :
            painter->drawImage(rect_curtain,img_curtain_4);
            painter->drawImage(rect_background,img_gray4);
            break;
    }
}

/**
 * @brief MainWidget::control_airconditioner
 * 控制空调槽函数
 */
void MainWidget::control_airconditioner(){
    is_airconditioner_on = !is_airconditioner_on;
    if(is_airconditioner_on){
        //空调定时器
        air_timer = new QTimer(this);
        air_timer->setInterval(500);
        connect(air_timer, SIGNAL(timeout()), this, SLOT(open_airconditioner()));
        air_timer->start();
    }else{
        air_timer->stop();
        wind_state = 1;
        btn_wind_1->hide();
        btn_wind_2->hide();
        btn_wind_3->hide();
    }
}

/**
 * @brief MainWidget::open_airconditioner
 * 打开空调槽函数
 */
void MainWidget::open_airconditioner(){

        if(wind_state>3){
           btn_wind_1->hide();
           btn_wind_2->hide();
           btn_wind_3->hide();
           wind_state = 1;
        }else if(wind_state == 1){
            btn_wind_2->hide();
            btn_wind_3->hide();
            btn_wind_1->show();
            wind_state++;
        }else if(wind_state == 2){
            btn_wind_2->show();
            wind_state++;
        }else if(wind_state == 3){
            btn_wind_3->show();
            wind_state++;
        }
}

/**
 * @brief MainWidget::control_add_wet
 * 控制加湿器槽函数
 */
void MainWidget::control_add_wet(){
    is_add_wet_on = !is_add_wet_on;
    if(is_add_wet_on){
        //加湿器定时器
        wet_timer = new QTimer(this);
        wet_timer->setInterval(500);
        connect(wet_timer, SIGNAL(timeout()), this, SLOT(open_add_wet()));
        wet_timer->start();
    }else{
        wet_timer->stop();
        fog_state = 1;
        btn_add_wet_1->hide();
        btn_add_wet_2->hide();
        btn_add_wet_3->hide();
    }
}

/**
 * @brief MainWidget::open_add_wet
 * 打开加湿器槽函数
 */
void MainWidget::open_add_wet(){

    if(fog_state>3){
       btn_add_wet_1->hide();
       btn_add_wet_2->hide();
       btn_add_wet_3->hide();
       fog_state = 1;
    }else if(fog_state == 1){
        btn_add_wet_2->hide();
        btn_add_wet_3->hide();
        btn_add_wet_1->show();
        fog_state++;
    }else if(fog_state == 2){
        btn_add_wet_2->show();
        btn_add_wet_3->hide();
        btn_add_wet_1->hide();
        fog_state++;
    }else if(fog_state == 3){
        btn_add_wet_3->show();
        btn_add_wet_1->hide();
        btn_add_wet_2->hide();
        fog_state++;
    }
}

/**
 * @brief MainWidget::show_setting_weiget
 * 弹出设置面板
 */
void MainWidget::show_setting_weiget(int y){
    QPropertyAnimation *anim=new QPropertyAnimation(setting_widget, "pos");
    anim->setDuration(250);
    anim->setStartValue(QPoint(1023, y));
    anim->setEndValue(QPoint(903, y));
    anim->start();
    is_setting_weiget_show = true;
}

/**
 * @brief MainWidget::hide_setting_weiget
 * 隐藏设置面板
 */
void MainWidget::hide_setting_weiget(){
    int y = setting_widget->geometry().y();//隐藏的时候从原位置隐藏
    QPropertyAnimation *anim=new QPropertyAnimation(setting_widget, "pos");
    anim->setDuration(250);
    anim->setStartValue(QPoint(903, y));
    anim->setEndValue(QPoint(1023, y));
    anim->start();
    is_setting_weiget_show = false;
}

/**
 * @brief MainWidget::show_port_setting_weiget
 * 弹出串口设置面板
 */
void MainWidget::show_port_setting_weiget(){

    int y = setting_widget->geometry().y()-5;//显示的位置由设置面板的y坐标决定
    if(y>395){
        y = 395;
    }
    QPropertyAnimation *anim=new QPropertyAnimation(port_setting_widget, "pos");
    anim->setDuration(250);
    anim->setStartValue(QPoint(1023, y));
    anim->setEndValue(QPoint(867, y));
    anim->start();
    is_port_setting_widget_show = true;
}

/**
 * @brief MainWidget::hide_port_setting_weiget
 * 隐藏串口设置面板
 */
void MainWidget::hide_port_setting_weiget(){
    int y = port_setting_widget->geometry().y();//隐藏的位置由自己的y坐标决定
    QPropertyAnimation *anim=new QPropertyAnimation(port_setting_widget, "pos");
    anim->setDuration(250);
    anim->setStartValue(QPoint(867, y));
    anim->setEndValue(QPoint(1023, y));
    anim->start();
    is_port_setting_widget_show = false;
}

/**
 * @brief MainWidget::open_setting_port_widget
 * 打开串口设置面板槽函数
 */
void MainWidget::open_setting_port_widget(){
    check_port();//先检测
    if(is_has_port){//如果有串口
        hide_setting_weiget();//这里要先隐藏，再显示
        show_port_setting_weiget();
        /*
        if(!is_port_opening){//如果串口没有打开的话
            open_port();
        }
        */
    }else{
        QMessageBox::warning(this,"警告","当前主机没有检测到串口，请检查！");
    }
}

/**
 * @brief MainWidget::show_password_widget
 * 弹出密码面板
 */
void MainWidget::show_password_widget(){
    if(setting_widget->btn_model->text()=="有人模式"){

        //切换到有人模式时要输入密码
        password_widget->line_password->setText("");//初始密码为空值
        int y = setting_widget->geometry().y()-5;//显示的位置由设置面板的y坐标决定
        if(y>450){
            y = 450;
        }
        QPropertyAnimation *anim=new QPropertyAnimation(password_widget, "pos");
        anim->setDuration(250);
        anim->setStartValue(QPoint(1023, y));
        anim->setEndValue(QPoint(803, y));
        anim->start();
        hide_setting_weiget();
        is_password_widget = true;

        if(is_has_port){
            this->port->control_RFID();
        }

        img_background.load(":/images/background.jpg");
        rect_background.setRect(0,0,1023,607);

    }else if(setting_widget->btn_model->text()=="无人模式"){
        img_doorground.load(":/images/door.jpg");
        rect_doorground.setRect(0,0,1023,607);
        this->switch_model();//切换到无人模式时直接切换
    }

}

/**
 * @brief MainWidget::hide_password_widget
 * 隐藏密码面板
 */
void MainWidget::hide_password_widget(){
    int y = password_widget->geometry().y();//隐藏的位置由自己的y坐标决定
    QPropertyAnimation *anim=new QPropertyAnimation(password_widget, "pos");
    anim->setDuration(250);
    anim->setStartValue(QPoint(803, y));
    anim->setEndValue(QPoint(1023, y));
    anim->start();
    is_password_widget = false;
}

/**
 * @brief MainWidget::check_password
 * 验证密码
 */
void MainWidget::check_password(){
    if(password_widget->line_password->text()=="ten"){
        switch_model();
        img_background.load(":/images/background.jpg");
        rect_background.setRect(0,0,1023,607);
        this->RFID = "";//RFID号滞空
         hide_password_widget();
    }
    else{

        if(!is_has_port)
        QMessageBox::warning(this,"警告","密码错误！");

      else{  if(this->shake){
            //有红外和震动时让蜂鸣器响
            this->port->control_buzzle();
            if(!is_timer_alarm_on){
                timer_alarm_light->start();
                is_timer_alarm_on = true; }
        }   else {
                if(is_timer_alarm_on){
                timer_alarm_light->stop();
                is_timer_alarm_on = false;
                is_alarm_light_on = false;//设置台灯最后的状态为关
                update();
                 }
              }

      }
    }

}

/**
 * @brief MainWidget::show_about_us_widget
 * 弹出关于我们对话框
 */
void MainWidget::show_about_us_widget(){
    QPropertyAnimation *anim=new QPropertyAnimation(about_us_widget, "pos");
    anim->setDuration(500);
    anim->setStartValue(QPoint(267, -300));
    anim->setEndValue(QPoint(267, 153));
    anim->setEasingCurve(QEasingCurve::OutBounce);
    anim->start();
    is_about_us_widget = true;
}

/**
 * @brief MainWidget::hide_about_us_widget
 * 隐藏关于我们面板
 */
void MainWidget::hide_about_us_widget(){
    QPropertyAnimation *anim1=new QPropertyAnimation(about_us_widget, "pos");
    QPropertyAnimation *anim2=new QPropertyAnimation(about_us_widget, "pos");
    anim1->setDuration(300);
    anim1->setStartValue(QPoint(267, 153));
    anim1->setEndValue(QPoint(267, 303));
    anim2->setDuration(150);
    anim2->setStartValue(QPoint(267, 303));
    anim2->setEndValue(QPoint(267, 607));
    QSequentialAnimationGroup *group = new QSequentialAnimationGroup;
    group->addAnimation(anim1);
    group->addAnimation(anim2);
    group->start();
    is_about_us_widget = false;
}

/**
 * @brief MainWidget::show_temAndHum_widget
 * 显示温湿度计面板
 */
void MainWidget::show_temAndHum_widget(){
    QPropertyAnimation *anim=new QPropertyAnimation(temAndHum_show_widget, "pos");
    anim->setDuration(500);
    anim->setStartValue(QPoint(-1023, 200));
    anim->setEndValue(QPoint(0, 200));
    anim->start();
    is_temAndHum = true;
}

/**
 * @brief MainWidget::hide_temAndHum_widget
 * 隐藏温湿度计面板
 */
void MainWidget::hide_temAndHum_widget(){
    QPropertyAnimation *anim=new QPropertyAnimation(temAndHum_show_widget, "pos");
    anim->setDuration(500);
    anim->setStartValue(QPoint(0, 200));
    anim->setEndValue(QPoint(1023, 200));
    anim->start();
    is_temAndHum = false;
}

/**
 * @brief MainWidget::show_temAndHum_line_widget
 * 显示温湿度曲线面板
 */
void MainWidget::show_temAndHum_line_widget(){
    QPropertyAnimation *anim=new QPropertyAnimation(temAndHum_widget, "pos");
    anim->setDuration(500);
    anim->setStartValue(QPoint(-1023, 200));
    anim->setEndValue(QPoint(0, 200));
    anim->start();
    is_temAndHum_line = true;
}

/**
 * @brief MainWidget::hide_temAndHum_line_widget
 * 隐藏温湿度曲线面板
 */
void MainWidget::hide_temAndHum_line_widget(){
    QPropertyAnimation *anim=new QPropertyAnimation(temAndHum_widget, "pos");
    anim->setDuration(500);
    anim->setStartValue(QPoint(0, 200));
    anim->setEndValue(QPoint(1023, 200));
    anim->start();
    is_temAndHum_line = false;
}

/**
 * @brief MainWidget::hide_temAndHum_and_show_line
 * 隐藏温湿度面板，显示温湿度曲线
 */
void MainWidget::hide_temAndHum_and_show_line(){
    hide_temAndHum_widget();
    show_temAndHum_line_widget();
}

/**
 * @brief MainWidget::hide_line_and_show_temAndHum
 * 隐藏温湿度曲线，显示温湿度计面板
 */
void MainWidget::hide_line_and_show_temAndHum(){
    hide_temAndHum_line_widget();
    show_temAndHum_widget();
}

/**
 * @brief MainWidget::shake_window
 * 窗口抖动槽函数--没有用到
 */
void MainWidget::shake_window(){

    int x = this->geometry().x();//当前窗口横坐标
    int y = this->geometry().y();//当前窗口纵坐标

    for (int i = 0; i < 40; i++) {
       if ((i & 1) == 0) {
           x += 10;
           y += 10;
       } else {
          x -= 10;
          y -= 10;
       }
       this->move(x,y);
    }


}



/**
 * @brief MainWidget::close_all
 * 关掉所有的电器
 */
void MainWidget::close_all(){
    if(is_airconditioner_on){
        this->air_timer->stop();
        wind_state = 1;
        btn_wind_1->hide();
        btn_wind_2->hide();
        btn_wind_3->hide();
    }
    if(is_big_light_on){
        is_big_light_on=!is_big_light_on;
    }
    if(is_alarm_light_on){
        is_alarm_light_on = !is_alarm_light_on;
    }
    if(is_night_light_on){
        is_night_light_on = !is_night_light_on;
    }
    if(is_add_wet_on){
       this->wet_timer->stop();
        fog_state = 1;
        btn_add_wet_1->hide();
        btn_add_wet_2->hide();
        btn_add_wet_3->hide();
    }
    if(is_temAndHum){
        hide_temAndHum_widget();
    }
    if(is_temAndHum_line){
        hide_temAndHum_line_widget();
    }
}

/**
 * @brief MainWidget::switch_model
 * 切换模式
 */
void MainWidget::switch_model(){
    if(setting_widget->btn_model->text()=="有人模式"){
        //切换至有人模式
        setting_widget->btn_model->setText("无人模式");
        setting_widget->btn_set_port->setEnabled(true);
        btn_curtain_open->setEnabled(true);
        btn_curtain_close->setEnabled(true);
        btn_big_light->setEnabled(true);
        btn_alarm_light->setEnabled(true);
        btn_night_light->setEnabled(true);
        btn_add_wet->setEnabled(true);
        btn_airconditioner->setEnabled(true);
        btn_temAndHum->setEnabled(true);
        if(is_timer_alarm_on){
            //如果报警灯在闪动，则关掉
            timer_alarm_light->stop();
            is_alarm_light_on = false;
        }
        update();
    }else if(setting_widget->btn_model->text()=="无人模式"){
        //切换至无人模式
        setting_widget->btn_model->setText("有人模式");
        btn_curtain_open->setEnabled(false);
        btn_curtain_close->setEnabled(false);
        btn_big_light->setEnabled(false);
        btn_alarm_light->setEnabled(false);
        btn_night_light->setEnabled(false);
        btn_add_wet->setEnabled(false);
        btn_airconditioner->setEnabled(false);
        btn_temAndHum->setEnabled(false);

        close_all();        
    }
    update();
}

/**
 * @brief MainWidget::paint_gray
 * @param painter
 * 画灰度
 */
void MainWidget::paint_gray(QPainter *painter){
    for(int i = 0;i<5;i++){
        painter->drawImage(rect_background,img_gray4);
    }
}

/**
 * @brief MainWidget::paintEvent
 * @param event
 * 绘图事件
 */
void MainWidget::paintEvent(QPaintEvent *event){
    painter = new QPainter(this);
    paint_background(painter);

    paint_curtain(painter,curtain_state);
    if(setting_widget->btn_model->text()=="有人模式"){
        paint_doorground(painter);
        paint_gray(painter);//画灰度
        paint_alarm_light(painter,is_alarm_light_on);
        paint_night_light(painter,is_night_light_on);
        btn_curtain_open->setEnabled(false);
        btn_curtain_close->setEnabled(false);
        btn_big_light->setEnabled(false);
        btn_night_light->setEnabled(false);
        btn_add_wet->setEnabled(false);
        btn_alarm_light->setEnabled(false);
        btn_airconditioner->setEnabled(false);
        btn_temAndHum->setEnabled(false);
    }else if(setting_widget->btn_model->text()=="无人模式"){
        //灯一定要放在窗帘后面，否则会出现色差
        paint_big_light(painter,is_big_light_on);
        paint_alarm_light(painter,is_alarm_light_on);
        paint_night_light(painter,is_night_light_on);
    }
}

/**
 * @brief MainWidget::mouseMoveEvent
 * @param event
 * 鼠标移动事件
 */
void MainWidget::mouseMoveEvent(QMouseEvent *event){

   int x = event->x();//鼠标x坐标
   int y = event->y()-40;//鼠标y坐标-40
    if(y>427){
        y = 440;
    }

    //每次只能显示其中一个面板

    //当没有其他面板显示的时候可以显示设置面板
    if(!is_port_setting_widget_show&&!is_password_widget){
       if(x>900&&!is_setting_weiget_show){
           if(y<0){
                y = 0;
           }
            show_setting_weiget(y);
       }
       if(x<=900&&is_setting_weiget_show){
           hide_setting_weiget();
       }
   }
    //同样的，没有其他面板显示的时候才可以显示串口设置面板
    if(!is_setting_weiget_show&&!is_password_widget){
        if(is_port_setting_widget_show&&x<=880){
            hide_port_setting_weiget();
        }
    }

    if(!is_setting_weiget_show&&!is_port_setting_widget_show){
        if(is_password_widget&&x<=800){
            hide_password_widget();
        }
    }
}

/**
 * @brief MainWidget::mousePressEvent
 * @param event
 * 鼠标点击事件
 */
void MainWidget::mousePressEvent(QMouseEvent *event){
    int x = event->x();//鼠标x坐标
    int y = event->y();//鼠标y坐标-40
    if(is_about_us_widget&&x>=267&&x<=767&&y>=153&&y<=453){
        hide_about_us_widget();
    }
}

/**
 * @brief MainWidget::closeEvent
 * @param event
 * 关闭窗口事件
 */
void MainWidget::closeEvent(QCloseEvent *event){
    QMessageBox::StandardButton button;
        button = QMessageBox::question(this, tr("退出程序"),
            QString(tr("当前程序正在运行,确认退出?")),
            QMessageBox::Yes | QMessageBox::No);

        if (button == QMessageBox::No) {
            event->ignore();  //忽略退出信号，程序继续运行
        }
        else if (button == QMessageBox::Yes) {
            //窗口渐入效果,效率不高
            for(int i=0; i< 2000000; i++){
              if(i<200000){
               this->setWindowOpacity(0.9);
              }else if(i<400000){
               this->setWindowOpacity(0.8);
              }else if(i<600000){
               this->setWindowOpacity(0.7);
              }else if(i<800000){
               this->setWindowOpacity(0.6);
              }else if(i<1000000){
               this->setWindowOpacity(0.5);
              }else if(i<1200000){
               this->setWindowOpacity(0.4);
              }else if(i<1400000){
               this->setWindowOpacity(0.3);
              }else if(i<1600000){
               this->setWindowOpacity(0.2);
              }else if(i<1800000){
               this->setWindowOpacity(0.1);
              }else{
               this->setWindowOpacity(0.0);
              }
            }
            event->accept();  //接受退出信号，程序退出
        }
}

MainWidget::~MainWidget()
{

}
