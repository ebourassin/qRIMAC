//System
#include <iostream>
#include <string>
#include <typeinfo>
#include <vector>
#include "math.h"

#include <QProgressDialog>

#include <mainwindow.h>


//qCC
#include <ccGLWindow.h>
#include "qrimacdlg.h"
#include "ui_qrimacdlg.h"
#include "ccclassification.h"
#include "FileIOFilter.h"
#include "ccConsole.h"
#include "ccEntityAction.h"
#include "ccOrderChoiceDlg.h"


#include "ccInterpolationDlg.h"
#include "ccItemSelectionDlg.h"
#include "ccProgressDialog.h"
#include "ccPointListPickingDlg.h"
#include "ccPickingHub.h"


//Qt
#include "qfiledialog.h"
#include <QString>
#include <QFile>
#include <QXmlStreamReader>
#include <QListWidgetItem>
#include <QImageReader>
#include <QFileDialog>
#include <QMainWindow>
#include <QFileInfo>

//qCC_db
#include "ccPointCloud.h"
#include "ccOctreeProxy.h"

//CClib
#include <ScalarFieldTools.h>


using namespace CCLib;

qRIMACdlg::qRIMACdlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::qRIMACdlg)

{
    ui->setupUi(this);

    //Connexion of butons

    //TRANSFERT D'ATTRIBUTS
    QObject::connect(ui->RVB_IN_SEARCH,SIGNAL(released()),this,SLOT(RVB_IN_SEARCH()));
    QObject::connect(ui->PIR_IN_SEARCH,SIGNAL(released()),this,SLOT(PIR_IN_SEARCH()));
    QObject::connect(ui->SWIR_IN_SEARCH,SIGNAL(released()),this,SLOT(SWIR_IN_SEARCH()));
    QObject::connect(ui->lancer,SIGNAL(released()),this,SLOT(lancer()));

    //CLASSIFICATION
    QObject::connect(ui->choix_nuage,SIGNAL(released()),this,SLOT(choix_nuage()));
    QObject::connect(ui->lancer_classif,SIGNAL(released()),this,SLOT(lancer_classif()));

    //VERITE TERRAIN
    QObject::connect(ui->VT_nuage,SIGNAL(released()),this,SLOT(VT_nuage()));
    QObject::connect(ui->select_point,SIGNAL(released()),this,SLOT(select_point()));

}

qRIMACdlg::~qRIMACdlg()
{
    delete ui;
}

//TRANSFERT D'ATTRIBUTS

//permet de choisir notre nuage de points RVB
void qRIMACdlg::RVB_IN_SEARCH()
{


   //QDir::homePath() : In order to be able work on several computer and several environment
   QString fileName = QFileDialog::getOpenFileName(this, tr("Sélectionner le fichier  contenant les nuages de points"),
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
//permet de choisir notre nuage de points PIR
void qRIMACdlg::PIR_IN_SEARCH()
{

   //QDir::homePath() : In order to be able work on several computer and several environment
   QString fileNamePIR = QFileDialog::getOpenFileName(this, tr("Sélectionner le fichier contenant les nuages de points"),
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

//permet de choisir notre nuage de points SWIR
void qRIMACdlg::SWIR_IN_SEARCH()
{
   //QDir::homePath() : In order to be able work on several computer and several environment
   QString fileName = QFileDialog::getOpenFileName(this, tr("Sélectionner le fichier contenant les nuages de points"),
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
    //on affiche "On lance le transfert d'attributs!" lorsque l'utilisateur clique sur le bouton lancer
    m_app->dispToConsole("On lance le transfert d'attributs!",ccMainAppInterface::STD_CONSOLE_MESSAGE);

    this->m_selectedEntities= m_app->getSelectedEntities();

    ccInterpolation interpol;

    if (!interpol.interpolate(m_selectedEntities, m_app))
           return;
   }


//CLASSIFICATION


void qRIMACdlg::choix_nuage()
{
   //QDir::homePath() : In order to be able work on several computer and several environment
   QString fileName = QFileDialog::getOpenFileName(this, tr("Sélectionner le fichier contenant les nuages de points à classifier"),
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

    ui->choix->setText(fileName);
    m_app->addToDB(file);

}

void qRIMACdlg::lancer_classif()
{

    this-> m_selectedEntities = m_app->getSelectedEntities();
    ccClassification classif;
    ccPointCloud* pc = static_cast<ccPointCloud*>(m_selectedEntities[0]);
    classif.KMeans(pc);
}


//VERITE TERRAIN

void qRIMACdlg::VT_nuage()
{
   //QDir::homePath() : In order to be able work on several computer and several environment
   QString fileName = QFileDialog::getOpenFileName(this, tr("Sélectionner le fichier contenant les nuages de points à classifier"),
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

    ui->VT->setText(fileName);
    m_app->addToDB(file);

}

void qRIMACdlg::select_point()
{
    this-> m_selectedEntities= m_app->getSelectedEntities();

    this-> m_pickingHub = this->m_app->pickingHub();


    ccPointCloud* pc = ccHObjectCaster::ToPointCloud(m_selectedEntities[0]);
        if (!pc)
        {
            ccConsole::Error("Wrong type of entity");
            return;
        }
        if (!pc->isVisible() || !pc->isEnabled())
            {
                ccConsole::Error("Points must be visible!");
                return;
            }
        if (!m_plpDlg)
           {
               // m_plpDlg = new ccPointListPickingDlg(m_pickingHub, this);

           }


}



//void MainWindow::activatePointListPickingMode()
//{
//    ccGLWindow* win = getActiveGLWindow();
//    if (!win)
//        return;

//    //there should be only one point cloud in current selection!
//    if (!haveOneSelection())
//    {
//        ccConsole::Error("Select one and only one entity!");
//        return;
//    }

//    ccPointCloud* pc = ccHObjectCaster::ToPointCloud(m_selectedEntities[0]);
//    if (!pc)
//    {
//        ccConsole::Error("Wrong type of entity");
//        return;
//    }

//    if (!pc->isVisible() || !pc->isEnabled())
//    {
//        ccConsole::Error("Points must be visible!");
//        return;
//    }

//    if (!m_plpDlg)
//    {
//        m_plpDlg = new ccPointListPickingDlg(m_pickingHub, this);
//        connect(m_plpDlg, &ccOverlayDialog::processFinished, this, &MainWindow::deactivatePointListPickingMode);

//        registerOverlayDialog(m_plpDlg, Qt::TopRightCorner);
//    }

//    //DGM: we must update marker size spin box value (as it may have changed by the user with the "display dialog")
//    m_plpDlg->markerSizeSpinBox->setValue(win->getDisplayParameters().labelMarkerSize);

//    m_plpDlg->linkWith(win);
//    m_plpDlg->linkWithCloud(pc);

//    freezeUI(true);

//    //we disable all other windows
//    disableAllBut(win);

//    if (!m_plpDlg->start())
//        deactivatePointListPickingMode(false);
//    else
//        updateOverlayDialogsPlacement();
//}
