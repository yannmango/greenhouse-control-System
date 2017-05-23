#ifndef TEMPERATUREWIDGET_H
#define TEMPERATUREWIDGET_H

#include <QWidget>
#include <QPainter>
#include <QLabel>
#include <QStringList>
#include <QTimer>
#include <QPushButton>

/**
 * @brief The TemAndHumWidget class
 * 温湿度曲线面板
 */
class TemAndHumWidget:public QWidget{
    Q_OBJECT

public :
    TemAndHumWidget(QWidget *parent =0);
    ~TemAndHumWidget();
private:
    QPainter *painter;
    QTimer *timer;//定时器定时获取文件中的数据
    QStringList list_temperature;//温度数据点列表
    QStringList list_humidity;//湿度数据点列表
public :
    QPushButton *btn_go;
    QPushButton *btn_close;
public:
    void paint_background_rect(QPainter *painter);//画背景坐标系
    void paint_temperature(QPainter *painter);//画温度曲线
    void paint_humidity(QPainter *painter);//画湿度
    void get_data_from_file();
    void show_button();

private slots:
    void get_temperature();//获取文件中的温度数据
    void get_humidity();//获取文件中的湿度数据
protected:
    void paintEvent(QPaintEvent *event);
};

#endif // TEMPERATUREWIDGET_H
