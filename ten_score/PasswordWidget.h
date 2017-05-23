#ifndef PASSWORDWIDGET_H
#define PASSWORDWIDGET_H

#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>

/**
 * @brief The PasswordWidget class
 * 自定义密码框样式
 */
class PasswordWidget: public QWidget{

    Q_OBJECT
public :
    PasswordWidget(QWidget *parent =0);
    ~PasswordWidget();
public:
    QLabel *label_option;

    QLineEdit *line_password;

    QPushButton *btn_Ok;
    QPushButton *btn_Cancel;

};

#endif // PASSWORDWIDGET_H
