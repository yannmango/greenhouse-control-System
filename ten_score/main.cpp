#include "MainWidget.h"
#include <QApplication>
#include <QDesktopWidget>
#include <PortSettingWidget.h>
#include <SettingWidget.h>
#include <AboutUsWidget.h>
#include <TemAndHumWidget.h>
#include <TemAndHumShowWidget.h>
#include <PasswordWidget.h>

int main(int argc, char *argv[])
{

    QApplication a(argc, argv);
    QApplication::setStyle("cleanlooks");
    MainWidget w;
    //PasswordWidget w;
    //TemAndHumShowWidget w;
    //TemAndHumWidget w;
    //AboutUsWidget w;
    //PortSettingWidget w;
    //SettingWidget w;
    //以下两句代码将窗口显示在桌面中央
    QDesktopWidget* desktop = QApplication::desktop();
    w.move((desktop->width() - w.width())/2, (desktop->height() - w.height())/2);
    //w.setWindowOpacity(0.3);//设置透明度，全部组件都透明化
    //透明
    /*
    w.setWindowOpacity(1);
    w.setWindowFlags(Qt::FramelessWindowHint);
    w.setAttribute(Qt::WA_TranslucentBackground);
    */
    w.show();
    return a.exec();
}
