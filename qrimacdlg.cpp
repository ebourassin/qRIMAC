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
#include "ccConsole.h"


#include <ui_interpolationDlg.h>
#include "ccInterpolationDlg.h"
#include "ccItemSelectionDlg.h"
#include "ccProgressDialog.h"

//Qt
#include "qfiledialog.h"
#include <QString>
#include <QFile>
#include <QXmlStreamReader>
#include <QListWidgetItem>
#include <QImageReader>
#include <QMainWindow>
#include <QFileDialog>

//qCC_db
#include "ccPointCloud.h"
#include "ccPointCloudInterpolator.h"
#include "ccOctreeProxy.h"

//CClib
#include <ScalarFieldTools.h>

qRIMACdlg::qRIMACdlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::qRIMACdlg)

{
    ui->setupUi(this);



    //Connexion of butons
    QObject::connect(ui->RVB_IN_SEARCH,SIGNAL(released()),this,SLOT(RVB_IN_SEARCH()));
    QObject::connect(ui->PIR_IN_SEARCH,SIGNAL(released()),this,SLOT(PIR_IN_SEARCH()));
    QObject::connect(ui->SWIR_IN_SEARCH,SIGNAL(released()),this,SLOT(SWIR_IN_SEARCH()));
    QObject::connect(ui->lancer,SIGNAL(released()),this,SLOT(lancer()));

}

qRIMACdlg::~qRIMACdlg()
{
    delete ui;
}

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

    //On demande à l'utilisateur de selectionner le nuage de points
    QString filedestname = QFileDialog::getOpenFileName(this, tr("Sélectionner le fichier contenant les nuages de points"),
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

    static ccHObject* filedest  = FileIOFilter::LoadFromFile(filedestname, parameters, result);
    //On affiche le nuage de points
    m_app->addToDB(filedest);

    QString filename = QFileDialog::getOpenFileName(this, tr("Sélectionner le dossier contenant les nuages de points"),
                                                                 QDir::homePath(),
                                                                 tr("*.ply"));


    static ccHObject* file  = FileIOFilter::LoadFromFile(filename, parameters, result);
    //On affiche le deuxième nuage de points
    m_app->addToDB(file);

    //On convertit nos nuages de points en objet ccPointCloud
    ccPointCloud* cloud1 = ccHObjectCaster::ToPointCloud(filedest);
    ccPointCloud* cloud2 = ccHObjectCaster::ToPointCloud(file);

    std::vector<int> inSFIndexes;
    inSFIndexes.push_back(0);

    CCLib::GenericProgressCallback* progressCb=0;
    unsigned char octreeLevel=0;

    //initialisation de inSFIndexes
    std::vector<int> sfIndexes;

    try
            {
                unsigned sfCount = cloud1->getNumberOfScalarFields();
                if (sfCount == 1)
                {
                    sfIndexes.push_back(0);
                }
                else if (sfCount > 1)
                {
                    ccItemSelectionDlg isDlg(true, m_app->getMainWindow(), "entity");
                    QStringList scalarFields;
                    {
                        for (unsigned i = 0; i < sfCount; ++i)
                        {
                            scalarFields << cloud1->getScalarFieldName(i);
                        }
                    }
                    isDlg.setItems(scalarFields, 0);
                    if (!isDlg.exec())
                    {
                        m_app->dispToConsole("An error occurred! (see console)",ccMainAppInterface::STD_CONSOLE_MESSAGE);
                    }
                    isDlg.getSelectedIndexes(sfIndexes);
                    if (sfIndexes.empty())
                    {
                        m_app->dispToConsole("An error occurred! (see console)",ccMainAppInterface::STD_CONSOLE_MESSAGE);
                    }
                }

                }
    catch (const std::bad_alloc&)
            {
                ccConsole::Error("Not enough memory");
            }


    //semi-persistent parameters
    static ccPointCloudInterpolator::Parameters::Method s_interpMethod = ccPointCloudInterpolator::Parameters::RADIUS;
    static ccPointCloudInterpolator::Parameters::Algo s_interpAlgo = ccPointCloudInterpolator::Parameters::NORMAL_DIST;
    static int s_interpKNN = 6;

    ccInterpolationDlg iDlg(m_app->getMainWindow());
            iDlg.setInterpolationMethod(s_interpMethod);
            iDlg.setInterpolationAlgorithm(s_interpAlgo);
            iDlg.knnSpinBox->setValue(s_interpKNN);
            iDlg.radiusDoubleSpinBox->setValue(cloud2->getOwnBB().getDiagNormd() / 100);

            if (!iDlg.exec())
            {
               //process cancelled by the user
                m_app->dispToConsole("An error occurredjnjbj! (see console)",ccMainAppInterface::STD_CONSOLE_MESSAGE);
            }

            //setup parameters
            ccPointCloudInterpolator::Parameters params;
            params.method = s_interpMethod = iDlg.getInterpolationMethod();
            params.algo = s_interpAlgo = iDlg.getInterpolationAlgorithm();
            params.knn = s_interpKNN = iDlg.knnSpinBox->value();
            params.radius = iDlg.radiusDoubleSpinBox->value();
            params.sigma = iDlg.kernelDoubleSpinBox->value();

            ccProgressDialog pDlg(true, m_app->getMainWindow());
            unsigned sfCountBefore = cloud2->getNumberOfScalarFields();

    // Retourne un booléan: ccPointCloudInterpolator::InterpolateScalarFieldsFrom
    if( ccPointCloudInterpolator::InterpolateScalarFieldsFrom(cloud1,
                                                              cloud2,
                                                              sfIndexes,
                                                              params,
                                                              progressCb,
                                                              octreeLevel
                                                              ))

    {

        cloud1->setCurrentDisplayedScalarField(static_cast<int>(std::min(sfCountBefore + 1, cloud1->getNumberOfScalarFields())) - 1);
       // cloud1->showSF(true);
        m_app->addToDB(cloud1);
    }
    else
            {
                m_app->dispToConsole("An error occurred! (see console)",ccMainAppInterface::STD_CONSOLE_MESSAGE);
            }
    cloud1->prepareDisplayForRefresh_recursive();

}
