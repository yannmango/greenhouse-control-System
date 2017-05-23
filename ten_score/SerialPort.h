#ifndef SERIALPORT_H
#define SERIALPORT_H

#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QTimer>
#include <QByteArray>

class SerialPort : public QObject{
    Q_OBJECT

public:
    explicit SerialPort();
    ~SerialPort();

private:
    float temperature;//温度
    int humidity;//湿度
    bool shake;//震动
    bool infrared;//红外
    bool light;//光敏
    QString RFID;//RFID卡号

    QTimer *timer_write;//用定时器实现轮询
    QTimer *timer_read;

    QSerialPort *port;//串口对象声明

    bool is_port_open;
    int write_data;//写入串口请求数据，即硬件sensorID
    QByteArray read_data;//读出的数据

public :
    void port_set_name(QString port_name);//设置串口名
    void port_open();//打开串口
    void port_close();//关闭串口
    void port_set(
                  QString baudRate,
                  QString dataBits,
                  QString stopBits,
                  QString parity);//串口设置参数
    void port_start();//定时器开始
    void port_stop();//定时器结束
    void get_data();//解析数据帧
    bool port_check();//检查数据帧是否有误，有误要重传

    int converseHexTo(QString str);//将十六进制数转为十进制

    float get_temperature();//获取温度
    int get_humidity();//获取湿度
    bool get_infrared();//获取红外
    bool get_light();//获取光敏
    bool get_shake();//获取震动
    QString get_RFID();//获取RFID卡号
    void reset_RFID();//重置RFID

    void control_electrical_machine_left();//控制电机正转
    void control_electrical_machine_right();//控制电机反转

    void control_buzzle();//控制蜂鸣器响

    void control_led();//控制LED灯

    void control_RFID();//请求RFID卡号

private slots:
    void port_write();//写串口
    void port_read();//读串口
};

#endif // SERIALPORT_H
