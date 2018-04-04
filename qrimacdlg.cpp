//System
#include <iostream>
#include <string>
#include <typeinfo>
#include <vector>
#include "math.h"
//qCC
#include <ccGLWindow.h>

#include "qrimacdlg.h"
#include "ui_qrimacdlg.h"

qRIMACdlg::qRIMACdlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::qRIMACdlg)
{
    ui->setupUi(this);

    //Connexion of butons
    QObject::connect(ui->start,SIGNAL(released()),this,SLOT(start()));
}

qRIMACdlg::~qRIMACdlg()
{
    delete ui;
}
void qRIMACdlg::start()
{
    // Loading of folders containing the images and the XML files of parameters
    std::cout<<"Loading folders containning images and files of parameters..."<<std::endl;


}
