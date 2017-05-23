#include <TemAndHumWidget.h>
#include <QFile>
#include <QDebug>
#include <QBitmap>
#include <QPixmap>
#include <QPalette>
#include <QPen>
#include <QVector>
#include <QBrush>

TemAndHumWidget :: TemAndHumWidget(QWidget *parent)
    :QWidget(parent){

    QPixmap pixmap(":/images/temAndhum_bg_1.png");
    QBitmap bitmap(pixmap);
    QPalette   palette;
    palette.setBrush(this->backgroundRole(),QColor(255,255,255));
    palette.setBrush(QPalette::Window,QColor(255,0,0,0));
    palette.setBrush(this->backgroundRole(),QBrush( pixmap ));
    palette.setBrush(QPalette::Window,QBrush( pixmap ));
    this->setPalette(palette);
    this->setMask(bitmap.mask());//可以将图片中透明部分显示为透明的
    this-> setAutoFillBackground( true );
    this->show_button();
    get_temperature();
    get_humidity();

    this->get_data_from_file();//开始从文件中获取数据

    this->resize(1023,200);
    this->show();
}

/**
 * @brief TemAndHumWidget::show_button
 * 显示按钮
 */
void TemAndHumWidget::show_button(){
    QFont font_btn;
    font_btn.setUnderline(true);
    font_btn.setPixelSize(15);
    font_btn.setFamily("楷体");
    QPalette palette;
    palette.setColor(QPalette::ButtonText,QColor(0,0,255));


    btn_go = new QPushButton(this);
    btn_go->setPalette(palette);
    btn_go->setFont(font_btn);
    btn_go->setText("查看温湿度计");
    btn_go->setStyleSheet("QPushButton{border-radius:5px;border-width:0px;}");
    btn_go->setCursor(Qt::PointingHandCursor);
    btn_go->resize(160,40);
    btn_go->move(800,150);

    btn_close = new QPushButton(this);
    btn_close->setPalette(palette);
    btn_close->setFont(font_btn);
    btn_close->setText("关闭");
    btn_close->setStyleSheet("QPushButton{border-radius:5px;border-width:0px;}");
    btn_close->setCursor(Qt::PointingHandCursor);
    btn_close->resize(80,40);
    btn_close->move(920,150);
}

/**
 * @brief TemAndHumWidget::get_data_from_file
 * 开启定时器
 */
void TemAndHumWidget::get_data_from_file(){
    timer = new QTimer(this);
    timer->setInterval(250);
    connect(timer,SIGNAL(timeout()),this,SLOT(get_temperature()));
    connect(timer,SIGNAL(timeout()),this,SLOT(get_humidity()));
    timer->start();
}

/**
 * @brief TemAndHumWidget::get_temperature
 * 获取文件中的温度数据
 */
void TemAndHumWidget::get_temperature(){
    int index = 0;
    list_temperature.clear();
    QFile f_t("./temperature.txt");
    if(!f_t.open(QIODevice::ReadWrite | QIODevice::Text))
    {
        qDebug() << "Open failed." << endl;
    }

    QTextStream txtInput(&f_t);
    QString lineStr;
    while(!txtInput.atEnd())
    {
        index ++;
        if(index > 20){
             break;
        }
        lineStr = txtInput.readLine();
        list_temperature.append(lineStr);
    }
    f_t.close();
    this->update();
}

/**
 * @brief TemAndHumWidget::get_humidity
 * 获取文件中的湿度数据
 */
void TemAndHumWidget::get_humidity(){
    list_humidity.clear();
    int index = 0;
    QFile f_h("./humidity.txt");
    if(!f_h.open(QIODevice::ReadWrite | QIODevice::Text))
    {
        qDebug() << "Open failed." << endl;
    }

    QTextStream txtInput(&f_h);
    QString lineStr;
    while(!txtInput.atEnd())
    {   index ++;
        if(index > 20){
            break;
        }
        lineStr = txtInput.readLine();
        list_humidity.append(lineStr);
    }
    f_h.close();
    this->update();
}

/**
 * @brief TemAndHumWidget::paint_temperature
 * @param painter
 * 画温度曲线
 */
void TemAndHumWidget::paint_temperature(QPainter *painter){
    int index = 0;
    int x1 = 100;
    int x2 = 130;
    int y1;//纵坐标
    int y2;
    QString y1_string;
    QString y2_string;

    for (QStringList::Iterator it=list_temperature.begin();it!= list_temperature.end();){
        y1_string = *it;
        qDebug()<<"y1 is :"<<y1_string;
        y1 = y1_string.toFloat()*5;
        y1 = 200 - y1;

        ++it;
        if(it == list_temperature.end()){
            qDebug()<<"到最后了"<<endl;
            break;
        }
        y2_string = *it;
        qDebug()<<"y2 is :"<<y2_string;
        y2 = y2_string.toFloat()*5;
        y2 = 200 - y2;
        painter->setPen(Qt::red);

        painter->drawLine(x1,y1,x2,y2);//画线
        x1 = x1 + 30;
        x2 = x2 + 30;

        //超过20个点后面自动截掉
        index++;
        if(index==19){
            break;
        }
    }
    QFont font;
    font.setPixelSize(20);
    font.setBold(true);
    painter->setFont(font);
    painter->drawText(670,y2,180,20,Qt::AlignHCenter|Qt::AlignVCenter,"温度："+y1_string+"°C");
}

/**
 * @brief TemAndHumWidget::paint_humidity
 * @param painter
 * 画湿度曲线
 */
void TemAndHumWidget::paint_humidity(QPainter *painter){
    int index = 0;
    int x1 = 100;
    int x2 = 130;
    int y1;//纵坐标
    int y2;
    QString y1_string;
    QString y2_string;
    for (QStringList::Iterator it=list_humidity.begin();it!= list_humidity.end();){
        y1_string = *it;
        y1 = y1_string.toFloat()*2;
        y1 = 200 - y1;
        ++it;
        if(it == list_humidity.end()){
            break;
        }
        y2_string = *it;
        qDebug()<<"y2 is :"<<y2_string;
        y2 = y2_string.toFloat()*2;
        y2 = 200 - y2;
        painter->setPen(Qt::green);
        painter->drawLine(x1,y1,x2,y2);
        x1 = x1 + 30;
        x2 = x2 + 30;
       //超过20个点后面自动截掉
        index++;
        if(index==19){
            break;
        }
    }
    painter->drawText(670,y2,180,20,Qt::AlignHCenter|Qt::AlignVCenter,"湿度："+y1_string+"%RH");
}

/**
 * @brief TemAndHumWidget::paint_background_rect
 * @param painter
 * 画背景坐标系
 */
void TemAndHumWidget::paint_background_rect(QPainter *painter){
        QPen pen;
        pen.setBrush(QBrush(Qt::gray));
        QVector<qreal> dashes;
        qreal space = 3;
        dashes << 2 << space << 2 <<space;
        pen.setDashPattern(dashes);
        pen.setWidth(0.5);
        painter->setPen(pen);
    for(int i = 0;i<200;i+=20){
        painter->drawLine(0,i,1023,i);
    }
    for(int i = 0;i<1023;i+=20){
         painter->drawLine(i,0,i,200);
    }
}


/**
 * @brief TemAndHumWidget::paintEvent
 * @param event
 * 绘图事件
 */
void TemAndHumWidget::paintEvent(QPaintEvent *event){
    painter = new QPainter(this);
    paint_background_rect(painter);
    paint_temperature(painter);
    paint_humidity(painter);
}

TemAndHumWidget::~TemAndHumWidget(){

}
