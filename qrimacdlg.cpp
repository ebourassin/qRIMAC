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
#include "FileIOFilter.h"

//Qt
#include "qfiledialog.h"
#include <QString>
#include <QFile>
#include <QXmlStreamReader>
#include <QListWidgetItem>
#include <QImageReader>
#include <QMainWindow>
#include <QFileDialog>


qRIMACdlg::qRIMACdlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::qRIMACdlg)
{
    ui->setupUi(this);



    //Connexion of butons
    QObject::connect(ui->RVB_IN_SEARCH,SIGNAL(released()),this,SLOT(RVB_IN_SEARCH()));
    QObject::connect(ui->PIR_IN_SEARCH,SIGNAL(released()),this,SLOT(PIR_IN_SEARCH()));
    QObject::connect(ui->SWIR_IN_SEARCH,SIGNAL(released()),this,SLOT(SWIR_IN_SEARCH()));

}

qRIMACdlg::~qRIMACdlg()
{
    delete ui;
}

void qRIMACdlg::RVB_IN_SEARCH()
{


   //QDir::homePath() : In order to be able work on several computer and several environment
   QString fileName = QFileDialog::getOpenFileName(this, tr("Sélectionner le dossier contenant les nuages de points"),
                                                                QDir::homePath(),
                                                                tr("*.ply"));


   CCVector3d loadCoordinatesShift(0,0,0);
   bool loadCoordinatesTransEnabled = false;

   FileIOFilter::LoadParameters parameters;
    {
           parameters.alwaysDisplayLoadDialog = true;
           parameters.shiftHandlingMode = ccGlobalShiftManager::DIALOG_IF_NECESSARY;
           parameters.coordinatesShift = &loadCoordinatesShift;
           parameters.coordinatesShiftEnabled = &loadCoordinatesTransEnabled;
           parameters.parentWidget = this;
       }




    //the same for 'addToDB' (if the first one is not supported, or if the scale remains too big)
    CCVector3d addCoordinatesShift(0, 0, 0);

    CC_FILE_ERROR result = CC_FERR_NO_ERROR;
    static ccHObject* file = FileIOFilter::LoadFromFile(fileName, parameters, result);

    ui->RVB_IN->setText(fileName);
    m_app->addToDB(file);


}

void qRIMACdlg::PIR_IN_SEARCH()
{

   //QDir::homePath() : In order to be able work on several computer and several environment
   QString fileNamePIR = QFileDialog::getOpenFileName(this, tr("Sélectionner le dossier contenant les nuages de points"),
                                                                QDir::homePath(),
                                                                tr("*.ply"));


   CCVector3d loadCoordinatesShift(0,0,0);
   bool loadCoordinatesTransEnabled = false;

   FileIOFilter::LoadParameters parameters;
    {
           parameters.alwaysDisplayLoadDialog = true;
           parameters.shiftHandlingMode = ccGlobalShiftManager::DIALOG_IF_NECESSARY;
           parameters.coordinatesShift = &loadCoordinatesShift;
           parameters.coordinatesShiftEnabled = &loadCoordinatesTransEnabled;
           parameters.parentWidget = this;
       }

    CCVector3d addCoordinatesShift(0, 0, 0);

    CC_FILE_ERROR result = CC_FERR_NO_ERROR;
    static ccHObject* filePIR = FileIOFilter::LoadFromFile(fileNamePIR, parameters, result);

    ui->PIR_IN->setText(fileNamePIR);
    m_app->addToDB(filePIR);

}


void qRIMACdlg::SWIR_IN_SEARCH()
{


   //QDir::homePath() : In order to be able work on several computer and several environment
   QString fileName = QFileDialog::getOpenFileName(this, tr("Sélectionner le dossier contenant les nuages de points"),
                                                                QDir::homePath(),
                                                                tr("*.ply"));


   CCVector3d loadCoordinatesShift(0,0,0);
   bool loadCoordinatesTransEnabled = false;

   FileIOFilter::LoadParameters parameters;
    {
           parameters.alwaysDisplayLoadDialog = true;
           parameters.shiftHandlingMode = ccGlobalShiftManager::DIALOG_IF_NECESSARY;
           parameters.coordinatesShift = &loadCoordinatesShift;
           parameters.coordinatesShiftEnabled = &loadCoordinatesTransEnabled;
           parameters.parentWidget = this;
       }




    //the same for 'addToDB' (if the first one is not supported, or if the scale remains too big)
    CCVector3d addCoordinatesShift(0, 0, 0);

    CC_FILE_ERROR result = CC_FERR_NO_ERROR;
    static ccHObject* file = FileIOFilter::LoadFromFile(fileName, parameters, result);

    ui->SWIR_IN->setText(fileName);
    m_app->addToDB(file);


}

void qRIMACdlg::lancer()
{
    m_app->dispToConsole("Lancer le transfert d'attributs!",ccMainAppInterface::STD_CONSOLE_MESSAGE);
}
