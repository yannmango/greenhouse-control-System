#include <PortSettingWidget.h>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QStringList>
#include <QFont>
#include <QPixmap>
#include <QPalette>
#include <QBitmap>

PortSettingWidget::PortSettingWidget(QWidget *parent)
    : QWidget(parent){

    QFont font;
    font.setFamily("黑体");
    font.setPointSize(12);
    //font.setBold(true);

    //字体设置颜色
    QPalette palette_text;
    palette_text.setColor(QPalette::WindowText,QColor(255,255,255));

    label_port_name = new QLabel("串口名:");
    label_port_name->setFont(font);
    label_port_name->setPalette(palette_text);

    label_baud_rate = new QLabel("波特率:");
    label_baud_rate->setFont(font);
    label_baud_rate->setPalette(palette_text);

    label_data_bits = new QLabel("数据位:");
    label_data_bits->setFont(font);
    label_data_bits->setPalette(palette_text);

    label_parity = new QLabel("校验位:");
    label_parity->setFont(font);
    label_parity->setPalette(palette_text);

    label_stop_bits = new QLabel("停止位:");
    label_stop_bits->setFont(font);
    label_stop_bits->setPalette(palette_text);


    combobox_port_name = new QComboBox;
    combobox_port_name->setFont(font);
    combobox_port_name->addItems(items_port_name);

    combobox_baud_rate = new QComboBox;
    combobox_baud_rate->setFont(font);
    items_baud_rate<<"9600"<<"19200"<<"38400"<< "115200";
    combobox_baud_rate->setCurrentIndex(3);
    combobox_baud_rate->addItems( items_baud_rate );

    combobox_data_bits = new QComboBox;
    combobox_data_bits->setFont(font);
    items_data_bits<<"8"<<"7"<<"6";
    combobox_data_bits->addItems( items_data_bits );

    combobox_parity = new QComboBox;
     combobox_parity->setFont(font);
    items_parity<<"无"<<"奇"<<"偶";
    combobox_parity->addItems( items_parity );

    combobox_stop_bits = new QComboBox;
     combobox_stop_bits->setFont(font);
    items_stop_bits<<"1"<<"2";
    combobox_stop_bits->addItems( items_stop_bits );

    btn_ok = new QPushButton("确定");
    btn_ok->setFont(font);
    btn_cancel = new QPushButton("取消");
    btn_cancel->setFont(font);

    layout_grid = new QGridLayout();
    layout_grid->addWidget(label_port_name,0,0);
    layout_grid->addWidget(combobox_port_name,0,1);
    layout_grid->addWidget(label_baud_rate,1,0);
    layout_grid->addWidget(combobox_baud_rate,1,1);
    layout_grid->addWidget(label_data_bits,2,0);
    layout_grid->addWidget(combobox_data_bits,2,1);
    layout_grid->addWidget(label_parity,3,0);
    layout_grid->addWidget(combobox_parity,3,1);
    layout_grid->addWidget(label_stop_bits,4,0);
    layout_grid->addWidget(combobox_stop_bits,4,1);
    layout_grid->addWidget(btn_ok,5,0);
    layout_grid->addWidget(btn_cancel,5,1);

    this->setLayout(layout_grid);


    /**
     * @brief pixmap
     * 加载背景图
     */
    QPixmap pixmap(":/images/port_setting_bg.png");
    QBitmap bitmap(pixmap);
    QPalette   palette;
    palette.setBrush(this->backgroundRole(),QColor(255,255,255));
    palette.setBrush(QPalette::Window,QColor(255,0,0,0));
    palette.setBrush(this->backgroundRole(),QBrush( pixmap ));
    palette.setBrush(QPalette::Window,QBrush( pixmap ));
    this->setPalette(palette);
    this->setMask(bitmap.mask());//可以将图片中透明部分显示为透明的
    this-> setAutoFillBackground( true );

    this->resize(156,215);
    this->show();
}

PortSettingWidget::~PortSettingWidget(){

}

