#include <BorderDialog.h>
#include <QLineEdit>
#include <QPainter>

BorderDialog::BorderDialog(QWidget *parent) : QDialog(parent){

}

BorderDialog::~BorderDialog(){

}

void BorderDialog::paintEvent(QPaintEvent *event){
    QPainter *p = new QPainter(this );
    //p.fillRect(rect(), QColor(0,0xff,0,30));
}
