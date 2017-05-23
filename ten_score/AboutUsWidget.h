#ifndef ABOUTUSWIDGET_H
#define ABOUTUSWIDGET_H

#include <QLabel>
#include <QComboBox>
#include <QPushButton>
#include <QStringList>
#include <QGridLayout>
#include <QRect>
#include <QWidget>

/**
 * @brief The AboutUsWidget class
 * 自定义关于我们面板样式
 */
class AboutUsWidget: public QWidget{

    Q_OBJECT
public :
    AboutUsWidget(QWidget *parent =0);
    ~AboutUsWidget();
private:
    QPushButton *btn_logo;

    QLabel *label_text;
public:
    QPushButton *btn_ok;

};

#endif // ABOUTUSWIDGET_H
