#include <SerialPort.h>
#include <QIODevice>
#include <QMessageBox>
#include <QDebug>
/**
 * @brief SerialPort::SerialPort
 * 构造方法
 */
SerialPort::SerialPort(){
    //实例化
    port = new QSerialPort(this);
    this->temperature=24;
    this->humidity=40;
    this->shake = false;
    this->infrared = false;
    this->light = false;
    this->RFID="";//初始为空
    is_port_open  = false;//没有打开
    write_data = 1;
    timer_write = new QTimer(this);
    timer_read = new QTimer(this);
    timer_write->setInterval(150);//每150毫秒请求一次
    timer_read->setInterval(150);
    connect(timer_write,SIGNAL(timeout()),this,SLOT(port_write()));
    connect(timer_read,SIGNAL(timeout()),this,SLOT(port_read()));
}

/**
 * @brief SerialPort::port_set_name
 * @param port_name
 * 设置串口名称
 */
void SerialPort::port_set_name(QString port_name){
    port->setPortName(port_name);
}

/**
 * @brief SerialPort::port_open
 * 打开串口
 */
void SerialPort::port_open(){
    port->open(QIODevice::ReadWrite);
    is_port_open = true;   
}

/**
 * @brief SerialPort::port_set
 * @param baudRate
 * @param dataBits
 * @param stopBits
 * @param parity
 * 设置串口参数
 */
void SerialPort::port_set( QString baudRate,
                           QString dataBits,
                           QString stopBits,
                           QString parity){
    //设置波特率
    if(baudRate == tr("2400")){
        port->setBaudRate(QSerialPort::Baud2400);
    }else if(baudRate == tr("4800")){
        port->setBaudRate(QSerialPort::Baud4800);
    }else if(baudRate == tr("9600")){
        port->setBaudRate(QSerialPort::Baud9600);
    }else if(baudRate == tr("19200")){
        port->setBaudRate(QSerialPort::Baud19200);
    }else if(baudRate == tr("115200")){
        port->setBaudRate(QSerialPort::Baud115200);
    }
    //设置数据位
    if(dataBits == tr("8")){
        port->setDataBits(QSerialPort::Data8);
    }else if(dataBits == tr("7")){
        port->setDataBits(QSerialPort::Data7);
    }else if(dataBits == tr("6")){
        port->setDataBits(QSerialPort::Data6);
    }else if(dataBits == tr("5")){
        port->setDataBits(QSerialPort::Data5);
    }
    //设置停止位
    if(stopBits == tr("1")){
        port->setStopBits(QSerialPort::OneStop);
    }else if(stopBits == tr("2")){
        port->setStopBits(QSerialPort::TwoStop);
    }
    //设置校验位
    if(parity == tr("无")){
        port->setParity(QSerialPort::NoParity);
    }else{
        //奇偶校验位先不考虑
        port->setParity(QSerialPort::NoParity);
    }
    /*
    else if(parity == tr("偶")){
        port->setParity(QSerialPort::);
    }
    */
     //设置数据流控制，我们使用无数据流控制的默认设置
    port->setFlowControl(QSerialPort::NoFlowControl);
}

/**
 * @brief SerialPort::port_write
 * 写串口程序,循环写,循环请求
 */
void SerialPort::port_write(){
    QString data_to_write;

    switch(write_data){
        case 1:
            data_to_write = "1";break;
        case 2:
            data_to_write = "2";break;
        case 3:
            data_to_write = "3";break;
        case 4:
            data_to_write = "4";break;
        case 5:
            data_to_write = "5";break;
        default:
               break;
    }
    qDebug()<<"=======write data is : "<<data_to_write<<endl;
    write_data ++;
    if(write_data == 6){
        write_data = 1;
    }
    port->write(data_to_write.toUtf8());//写数据
}


/**
 * @brief SerialPort::control_buzzle
 * 控制蜂鸣器响
 */
void SerialPort::control_buzzle(){
    QString data_to_write;
    data_to_write = "6";
    port->write(data_to_write.toUtf8());
}

/**
 * @brief SerialPort::control_electrical_machine_left
 * 控制电机正转
 */
void SerialPort::control_electrical_machine_left(){
    QString data_to_write;
    data_to_write = "7";
    port->write(data_to_write.toUtf8());
}

/**
 * @brief SerialPort::control_electrical_machine_right
 * 控制电机反转
 */
void SerialPort::control_electrical_machine_right(){
    QString data_to_write;
    data_to_write = "8";
    port->write(data_to_write.toUtf8());
}

/**
 * @brief SerialPort::control_led
 * 控制LED灯
 */
void SerialPort::control_led(){
    QString data_to_write;
    data_to_write = "9";
    port->write(data_to_write.toUtf8());
}

/**
 * @brief SerialPort::port_read
 * 读串口程序
 */
void SerialPort::port_read(){
   // if(is_port_open){
        read_data = port->readAll();
        port->clear();
        //qDebug()<<"****read_data is :"<<read_data.toHex()<<endl;
        this->get_data();
   // }
}

/**
 * @brief SerialPort::control_RFID
 * 请求RFID卡号
 */
void SerialPort::control_RFID(){
    QString data_to_write;
    data_to_write = "0";
    port->write(data_to_write.toUtf8());
}

/**
 * @brief SerialPort::port_check
 * 检查数据帧是否有误
 */
bool SerialPort::port_check(){

    qDebug()<<"读取到的卡号："<<read_data.toHex().toLower()<<endl;

    /*这里加上刷卡模块*/
    //正常卡号:db 96 0a 18 ff 04
    QByteArray read_second;//数据第二字节
    read_second[0] = read_data[1];
    if(read_data.length()==4&&read_second.toHex().toLower()=="96"){//卡号为4字节情况
        RFID = read_data.toHex().toLower();//RFID卡号在这里得到，不需要再get_data方法中写了
        RFID +="ff04";//
        qDebug()<<"RFID is"<<RFID<<endl;
        return false;//这里return false;
    }
    if(read_data.length()>=6&&read_second.toHex().toLower()=="96"){//卡号为>=6字节情况
        QByteArray temp_RFID;
        for(int i =0;i<6;i++){
            temp_RFID[i] = read_data[i];
        }
        RFID = temp_RFID.toHex().toLower();
        return false;
    }
    /*刷卡模块到这里结束*/

    /*直接根据帧头f8截取四个字节数据*/
    QByteArray temp_read_data;
    for(int i = 0;i<read_data.length();i++){
        QByteArray temp_array;
        temp_array[0] = read_data[i];
        if(temp_array.toHex().toLower()=="f8"){
            temp_read_data[0] = read_data[i];
            temp_read_data[1] = read_data[i+1];
            temp_read_data[2] = read_data[i+2];
            temp_read_data[3] = read_data[i+3];
            break;
        }
    }
    read_data = temp_read_data;

    qDebug()<<"读取到的卡号："<<read_data.toHex().toLower()<<endl;


    bool is_data_head_right = false;//帧头是否正确
    bool is_parity_right = false;//校验码是否正确
    QByteArray read_data_head;//帧头
    QByteArray read_data_parity;//校验码
    read_data_head[0] = read_data[0];
    read_data_parity[0] = read_data[3];
    qDebug()<<"帧头为："<<read_data_head.toHex()<<endl;
    //测试帧头
    if(read_data_head.toHex().toLower() == "f8"){
        is_data_head_right = true;
        qDebug()<<"测试帧头是否为f8："<<"**帧头正确**"<<endl;
    }else{
        is_data_head_right = false;
        qDebug()<<"测试帧头是否为f8："<<"**帧头不正确**"<<endl;
    }
    QByteArray calc_parity;//待计算校验码
    calc_parity[0] = read_data[0]^read_data[1]^read_data[2];
    if(calc_parity == read_data_parity){
        is_parity_right = true;//校验码正确
        qDebug()<<"计算所得校验码为："<<calc_parity.toHex()<<endl;
        qDebug()<<"校验码是否正确:"<<"==校验码正确=="<<endl;
    }else{
        qDebug()<<"校验码是否正确:"<<"==校验码不正确=="<<endl;
    }
    if(is_data_head_right&&is_parity_right){
        //如果帧头和校验码都正确
        return true;
    }else{
        return false;
    }
}

/**
 * @brief SerialPort::get_data
 * 解析数据
 */
void SerialPort::get_data(){
    if(port_check()){
        QByteArray sensorId;//传感器编号
        QByteArray sensorData;//传感器数据
        sensorId[0] = read_data[1];
        sensorData[0] = read_data[2];
        if(sensorId.toHex().toLower() == "31"){
            //温度
            this->temperature = converseHexTo(sensorData.toHex().toLower())*0.5;
        }else if(sensorId.toHex().toLower() == "32"){
            //湿度
            this->humidity = converseHexTo(sensorData.toHex().toLower());
        }else if(sensorId.toHex().toLower() == "33"){
            //震动
            if(sensorData.toHex().toLower() == "ff"){
                this->shake = true;
            }else if(sensorData.toHex().toLower() == "11"){
                this->shake = false;
            }
        }else if(sensorId.toHex().toLower() == "34"){
            //热源红外
            if(sensorData.toHex().toLower() == "ff"){
                this->infrared = true;
            }else if(sensorData.toHex().toLower() == "11"){
                this->infrared = false;
            }
        }else if(sensorId.toHex().toLower() == "35"){
            //光敏
            if(sensorData.toHex().toLower() == "ff"){
                this->light = true;
            }else if(sensorData.toHex().toLower() == "11"){
                this->light = false;
            }
        }
    }else{
        qDebug()<<"数据帧有误！"<<endl;
    }
}

/**
 * @brief SerialPort::converseHexTo
 * @param arr
 * @return
 * 转换为十进制
 */
int SerialPort::converseHexTo(QString str){
    int x,y;
    if(str.left(1)=="0"
            ||str.left(1)=="1"
            ||str.left(1)=="2"
            ||str.left(1)=="3"
            ||str.left(1)=="4"
            ||str.left(1)=="5"
            ||str.left(1)=="6"
            ||str.left(1)=="7"
            ||str.left(1)=="8"
            ||str.left(1)=="9"){
        x = str.left(1).toInt();
        qDebug()<<"x is:"<<x<<endl;
    }else if(str.left(1) == "a"){
        x = 10;
    }else if(str.left(1) == "b"){
        x = 11;
    }else if(str.left(1) == "c"){
        x = 12;
    }else if(str.left(1) == "d"){
        x = 13;
    }else if(str.left(1) == "e"){
        x = 14;
    }else if(str.left(1) == "f"){
        x = 15;
    }

    if(str.right(1)=="0"
            ||str.right(1)=="1"
            ||str.right(1)=="2"
            ||str.right(1)=="3"
            ||str.right(1)=="4"
            ||str.right(1)=="5"
            ||str.right(1)=="6"
            ||str.right(1)=="7"
            ||str.right(1)=="8"
            ||str.right(1)=="9"){
        y = str.right(1).toInt();
        qDebug()<<"y is:"<<y<<endl;
    }else if(str.right(1) == "a"){
        y = 10;
    }else if(str.right(1) == "b"){
        y = 11;
    }else if(str.right(1) == "c"){
        y = 12;
    }else if(str.right(1) == "d"){
        y = 13;
    }else if(str.right(1) == "e"){
        y = 14;
    }else if(str.right(1) == "f"){
        y = 15;
    }
    return (16*x+y);
}

/**
 * @brief SerialPort::get_temperature
 * @return
 * 获取温度
 */
float SerialPort::get_temperature(){
    return this->temperature;
}

/**
 * @brief SerialPort::get_humidity
 * @return
 * 获取湿度
 */
int SerialPort::get_humidity(){
    return this->humidity;
}

/**
 * @brief SerialPort::get_shake
 * @return
 * 获取震动
 */
bool SerialPort::get_shake(){
    return this->shake;
}

/**
 * @brief SerialPort::get_light
 * @return
 * 获取光敏
 */
bool SerialPort::get_light(){
    return this->light;
}

/**
 * @brief SerialPort::get_infrared
 * @return
 * 获取红外
 */
bool SerialPort::get_infrared(){
    return this->infrared;
}

/**
 * @brief SerialPort::get_RFID
 * @return
 * 获取RFID卡号
 */
QString SerialPort::get_RFID(){
    return this->RFID;
}

void SerialPort::reset_RFID(){
    this->RFID = "";
}

/**
 * @brief SerialPort::port_start
 * 定时器开始
 */
void SerialPort::port_start(){
    timer_write->start();
    timer_read->start();
}

/**
 * @brief SerialPort::port_stop
 * 定时器停止
 */
void SerialPort::port_stop(){
    timer_write->stop();
    timer_read->stop();
}

/**
 * @brief SerialPort::port_close
 * 关闭串口
 */
void SerialPort::port_close(){
    port->clear();//清空数据！！
    port->close();
    is_port_open = false;
}

/**
 * @brief SerialPort::~SerialPort
 * 析构函数
 */
SerialPort::~SerialPort(){

}
