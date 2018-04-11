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
    QObject::connect(ui->lancer,SIGNAL(released()),this,SLOT(lancer()));

}

qRIMACdlg::~qRIMACdlg()
{
    delete ui;
}

void qRIMACdlg::lancer()
{

   m_app->dispToConsole("Lancer le transfert d'attributs!",ccMainAppInterface::STD_CONSOLE_MESSAGE);

   }



/*
void qRIMACdlg::select_cloud()
{

}

void qRIMACdlg::sup_pts_sans_ppv()
{

}
void qRIMACdlg::lancer()
{

}
void qRIMACdlg::bar_progression()
{

}
void qRIMACdlg::rvb()
{

}
void qRIMACdlg::swir()
{

}
void qRIMACdlg::pir()
{

}

void qRIMACdlg::select_rvb()
{

}

void qRIMACdlg::select_pir()
{

}
void qRIMACdlg::select_swir()
{

}
void qRIMACdlg::dist_voxel()
{

}
void qRIMACdlg::dist_choice()
{

}
void qRIMACdlg::out()
{

}
void qRIMACdlg::dist_unite()
{

}
void qRIMACdlg::dist_choice()
{

}
*/
