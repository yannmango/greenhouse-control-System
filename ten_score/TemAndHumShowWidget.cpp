#include <TemAndHumShowWidget.h>
#include <QBitmap>
#include <QPixmap>
#include <QPalette>

TemAndHumShowWidget :: TemAndHumShowWidget(QWidget *parent)
    :QWidget(parent){

    QPixmap pixmap(":/images/temAndhum_bg.png");
    QBitmap bitmap(pixmap);
    QPalette   palette;
    palette.setBrush(this->backgroundRole(),QColor(255,255,255));
    palette.setBrush(QPalette::Window,QColor(255,0,0,0));
    palette.setBrush(this->backgroundRole(),QBrush( pixmap ));
    palette.setBrush(QPalette::Window,QBrush( pixmap ));
    this->setPalette(palette);
    this->setMask(bitmap.mask());
    this-> setAutoFillBackground( true );
    this->show_label_button();
    this->show_temperature(26);
    this->show_humidity(25);
    this->resize(1023,200);
    this->show();
}

/**
 * @brief TemAndHumShowWidget::show_label
 * 显示标签
 */
void TemAndHumShowWidget::show_label_button(){

    label_temperature = new QLabel(this);
    QFont font ;
    font.setFamily("楷体");
    font.setPixelSize(30);

    label_temperature->setText("温度");
    label_temperature->setFont(font);
    QPalette palette_tem;
    palette_tem.setColor(QPalette::WindowText,Qt::blue);
    label_temperature->setPalette(palette_tem);
    label_temperature->resize(80,60);
    label_temperature->move(130,40);


    label_humidity = new QLabel(this);
    label_humidity->setText("湿度");
    label_humidity->setFont(font);
    QPalette palette_hum;
    palette_hum.setColor(QPalette::WindowText,Qt::blue);
    label_humidity->setPalette(palette_hum);
    label_humidity->resize(80,60);
    label_humidity->move(530,40);


    QFont font_btn;
    font_btn.setUnderline(true);
    font_btn.setPixelSize(15);
    QPalette palette;
    palette.setColor(QPalette::ButtonText,QColor(0,0,255));

    btn_go_line = new QPushButton(this);
    btn_go_line->setPalette(palette);
    btn_go_line->setFont(font_btn);
    btn_go_line->setText("查看温湿度曲线");
    btn_go_line->setStyleSheet("QPushButton{border-radius:5px;border-width:0px;}");
    btn_go_line->setCursor(Qt::PointingHandCursor);
    btn_go_line->resize(160,40);
    btn_go_line->move(800,150);

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
 * @brief TemAndHumShowWidget::show_temperature
 * @param temperature
 * 显示温度数据
 */
void TemAndHumShowWidget::show_temperature(float temperature){
    num_temperature = new QLCDNumber(this);
    num_temperature->move(200,50);
    QPalette palette_tem;
    palette_tem.setColor(QPalette::WindowText,Qt::blue);
    num_temperature->setPalette(palette_tem);
    num_temperature->setFixedHeight(100);
    num_temperature->setFixedWidth(200);
    num_temperature->setFrameStyle(10);
    num_temperature->setSegmentStyle(QLCDNumber::Filled);
    num_temperature->setStyleSheet("QLCDNumber{border-radius:5px;border-width:0px;}");
    num_temperature->setOctMode();
    num_temperature->setSmallDecimalPoint(true);
    num_temperature->display(QString::number(temperature));
}

/**
 * @brief TemAndHumShowWidget::show_humidity
 * @param humidity
 * 显示湿度数据
 */
void TemAndHumShowWidget::show_humidity(int humidity){
    num_humidity = new QLCDNumber(this);
    num_humidity->move(600,50);
    QPalette palette_hum;
    palette_hum.setColor(QPalette::WindowText,Qt::blue);
    num_humidity->setPalette(palette_hum);
    num_humidity->setFixedHeight(100);
    num_humidity->setFixedWidth(200);
    num_humidity->setSegmentStyle(QLCDNumber::Filled);
    num_humidity->setStyleSheet("QLCDNumber{border-radius:5px;border-width:0px;}");
    num_humidity->setOctMode();
    num_humidity->setSmallDecimalPoint(true);
    num_humidity->display(QString::number(humidity));
}

/**
 * @brief TemAndHumShowWidget::set_temperature_and_humidity
 * @param temperature
 * @param humidity
 * 根据温度和湿度数据显示
 */
void TemAndHumShowWidget::set_temperature_and_humidity(float temperature,int humidity){
    num_temperature->display(QString::number(temperature));
    num_humidity->display(QString::number(humidity));
}

TemAndHumShowWidget::~TemAndHumShowWidget(){

}
