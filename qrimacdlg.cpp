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
#include "ccclassification.h"
#include "ccWorkSite.h"

//Qt
#include "qfiledialog.h"
#include <QString>
#include <QFile>
#include <QXmlStreamReader>
#include <QListWidgetItem>
#include <QImageReader>

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

   //QDir::homePath() : In order to be able work on several computer and several environment
   QString dirImgStr = QFileDialog::getExistingDirectory(this, tr("Sélectionner le dossier contenant les images"),
                                                                QDir::homePath(),
                                                                QFileDialog::ShowDirsOnly
                                                                | QFileDialog::DontResolveSymlinks);

   }

   //this->currentWorkSite->initialise(dirImgStr);
  // ui->label_img->setText(dirImgStr);



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
