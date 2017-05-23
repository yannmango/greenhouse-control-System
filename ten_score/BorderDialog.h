#ifndef PORTSETTINGDIALOG_H
#define PORTSETTINGDIALOG_H

#include <QDialog>

/**
 * @brief The PortSettingDialog class
 * 自定义边栏对话框样式
 */
class PortSettingDialog: public QDialog{

    Q_OBJECT;
public :
    PortSettingDialog(QWidget *parent =0);
    ~PortSettingDialog();
protected :
    void paintEvent(QPaintEvent *event);
};

#endif // BORDERDIALOG_H
