#ifndef PORTSETTINGDIALOG_H
#define PORTSETTINGDIALOG_H

#include <QLabel>
#include <QComboBox>
#include <QPushButton>
#include <QStringList>
#include <QGridLayout>
#include <QRect>
#include <QWidget>

/**
 * @brief The PortSettingWidget class
 * 自定义串口设置面板样式
 */
class PortSettingWidget: public QWidget{

    Q_OBJECT
public :
    PortSettingWidget(QWidget *parent =0);
    ~PortSettingWidget();
private:
    QLabel *label_port_name;//串口名称
    QLabel *label_baud_rate;//波特率
    QLabel *label_data_bits;//数据位
    QLabel *label_parity;//校验位
    QLabel *label_stop_bits;//停止位
public:
    QComboBox *combobox_port_name;
    QComboBox *combobox_baud_rate;
    QComboBox *combobox_data_bits;
    QComboBox *combobox_parity;
    QComboBox *combobox_stop_bits;
private:
    QStringList items_port_name;
    QStringList items_baud_rate;
    QStringList items_data_bits;
    QStringList items_parity;
    QStringList items_stop_bits;
    QGridLayout *layout_grid;
public:
    QPushButton *btn_ok;//确认按钮
    QPushButton *btn_cancel;//取消按钮

};

#endif // PORTSETTINGDIALOG_H
