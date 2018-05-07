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
    QObject::connect(ui->choix_nuage,SIGNAL(released()),this,SLOT(choix_nuage()));
    QObject::connect(ui->lancer_classif,SIGNAL(released()),this,SLOT(lancer_classif()));

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


bool qRIMACdlg::interpolate(const ccHObject::Container &selectedEntities)
{
    if (selectedEntities.size() != 2)
    {
        ccConsole::Error("Select 2 entities (clouds or meshes)!");
        return false;
    }

    ccHObject* ent1 = selectedEntities[0];
    ccHObject* ent2 = selectedEntities[1];

    ccPointCloud* cloud1 = ccHObjectCaster::ToPointCloud(ent1);
    ccPointCloud* cloud2 = ccHObjectCaster::ToPointCloud(ent2);

    if (!cloud1 || !cloud2)
    {
        ccConsole::Error("Select 2 entities (clouds or meshes)!");
        return false;
    }

    if (!cloud1->hasScalarFields() && !cloud2->hasScalarFields())
    {
        ccConsole::Error("None of the selected entities has per-point or per-vertex colors!");
        return false;
    }

    ccPointCloud* source = cloud1;
    ccPointCloud* dest = cloud2;

    //show the list of scalar fields available on the source point cloud
    std::vector<int> sfIndexes;
    try
    {
        unsigned sfCount = source->getNumberOfScalarFields();
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
                    scalarFields << source->getScalarFieldName(i);
                }
            }
            isDlg.setItems(scalarFields, 0);
            if (!isDlg.exec())
            {
                //cancelled by the user
                return false;
            }
            isDlg.getSelectedIndexes(sfIndexes);
            if (sfIndexes.empty())
            {
                ccConsole::Error("No scalar field was selected");
                return false;
            }
        }
        else
        {
            assert(false);
        }
    }
    catch (const std::bad_alloc&)
    {
        ccConsole::Error("Not enough memory");
        return false;
    }

    //semi-persistent parameters
    static ccPointCloudInterpolator::Parameters::Method s_interpMethod = ccPointCloudInterpolator::Parameters::RADIUS;
    static ccPointCloudInterpolator::Parameters::Algo s_interpAlgo = ccPointCloudInterpolator::Parameters::NORMAL_DIST;
    static int s_interpKNN = 6;

    ccInterpolationDlg iDlg(m_app->getMainWindow());
    iDlg.setInterpolationMethod(s_interpMethod);
    iDlg.setInterpolationAlgorithm(s_interpAlgo);
    iDlg.knnSpinBox->setValue(s_interpKNN);
    iDlg.radiusDoubleSpinBox->setValue(dest->getOwnBB().getDiagNormd() / 100);

    if (!iDlg.exec())
    {
        //process cancelled by the user
        return false;
    }

    //setup parameters
    ccPointCloudInterpolator::Parameters params;
    params.method = s_interpMethod = iDlg.getInterpolationMethod();
    params.algo = s_interpAlgo = iDlg.getInterpolationAlgorithm();
    params.knn = s_interpKNN = iDlg.knnSpinBox->value();
    params.radius = iDlg.radiusDoubleSpinBox->value();
    params.sigma = iDlg.kernelDoubleSpinBox->value();

    ccProgressDialog pDlg(true,  m_app->getMainWindow());
    unsigned sfCountBefore = dest->getNumberOfScalarFields();

    if (ccPointCloudInterpolator::InterpolateScalarFieldsFrom(dest, source, sfIndexes, params, &pDlg))
    {
        dest->setCurrentDisplayedScalarField(static_cast<int>(std::min(sfCountBefore + 1, dest->getNumberOfScalarFields())) - 1);
        dest->showSF(true);
    }
    else
    {
        ccConsole::Error("An error occurred! (see console)");
    }

    dest->prepareDisplayForRefresh_recursive();

    return true;

}

void qRIMACdlg::lancer()
{
    //on affiche "On lance le transfert d'attributs!" lorsque l'utilisateur clique sur le bouton lancer
    m_app->dispToConsole("On lance le transfert d'attributs!",ccMainAppInterface::STD_CONSOLE_MESSAGE);

    ccInterpolationDlg* intdlg = new ccInterpolationDlg();

    intdlg->show();

//   // if ( !ccEntityAction::interpolateSFs(m_selectedEntities, m_app) )
//   //         return;
//    if (!qRIMACdlg::interpolate(m_selectedEntities))
//            return;
   }


 void qRIMACdlg::lancer_classif()
 {

     const ccHObject::Container& selectedEntities = m_app->getSelectedEntities();
     ccClassification classif;
     ccPointCloud* pc = static_cast<ccPointCloud*>(selectedEntities[0]);
     classif.KMeans(pc);
 }

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



//    std::vector<int> inSFIndexes;
//    inSFIndexes.push_back(0);

//    CCLib::GenericProgressCallback* progressCb=0;
//    unsigned char octreeLevel=0;

//    //initialisation de inSFIndexes
//    std::vector<int> sfIndexes;

//    try
//            {
//                unsigned sfCount = cloud1->getNumberOfScalarFields();
//                if (sfCount == 1)
//                {
//                    sfIndexes.push_back(0);
//                }
//                else if (sfCount > 1)
//                {
//                    ccItemSelectionDlg isDlg(true, m_app->getMainWindow(), "entity");
//                    QStringList scalarFields;
//                    {
//                        for (unsigned i = 0; i < sfCount; ++i)
//                        {
//                            scalarFields << cloud1->getScalarFieldName(i);
//                        }
//                    }
//                    isDlg.setItems(scalarFields, 0);
//                    if (!isDlg.exec())
//                    {
//                        m_app->dispToConsole("An error occurred! (see console)",ccMainAppInterface::STD_CONSOLE_MESSAGE);
//                    }
//                    isDlg.getSelectedIndexes(sfIndexes);
//                    if (sfIndexes.empty())
//                    {
//                        m_app->dispToConsole("An error occurred! (see console)",ccMainAppInterface::STD_CONSOLE_MESSAGE);
//                    }
//                }

//                }
//    catch (const std::bad_alloc&)
//            {
//                ccConsole::Error("Not enough memory");
//            }


//    //semi-persistent parameters
//    static ccPointCloudInterpolator::Parameters::Method s_interpMethod = ccPointCloudInterpolator::Parameters::RADIUS;
//    static ccPointCloudInterpolator::Parameters::Algo s_interpAlgo = ccPointCloudInterpolator::Parameters::NORMAL_DIST;
//    static int s_interpKNN = 6;

//    ccInterpolationDlg iDlg(m_app->getMainWindow());
//            iDlg.setInterpolationMethod(s_interpMethod);
//            iDlg.setInterpolationAlgorithm(s_interpAlgo);
//            iDlg.knnSpinBox->setValue(s_interpKNN);
//            iDlg.radiusDoubleSpinBox->setValue(cloud2->getOwnBB().getDiagNormd() / 100);

//            if (!iDlg.exec())
//            {
//               //process cancelled by the user
//                m_app->dispToConsole("An error occurredjnjbj! (see console)",ccMainAppInterface::STD_CONSOLE_MESSAGE);
//            }

//            //setup parameters
//            ccPointCloudInterpolator::Parameters params;
//            params.method = s_interpMethod = iDlg.getInterpolationMethod();
//            params.algo = s_interpAlgo = iDlg.getInterpolationAlgorithm();
//            params.knn = s_interpKNN = iDlg.knnSpinBox->value();
//            params.radius = iDlg.radiusDoubleSpinBox->value();
//            params.sigma = iDlg.kernelDoubleSpinBox->value();

//            ccProgressDialog pDlg(true, m_app->getMainWindow());
//            unsigned sfCountBefore = cloud2->getNumberOfScalarFields();

//    // Retourne un booléan: ccPointCloudInterpolator::InterpolateScalarFieldsFrom
//    if( ccPointCloudInterpolator::InterpolateScalarFieldsFrom(cloud1,
//                                                              cloud2,
//                                                              sfIndexes,
//                                                              params,
//                                                              progressCb,
//                                                              octreeLevel
//                                                              ))

//    {

//        cloud1->setCurrentDisplayedScalarField(static_cast<int>(std::min(sfCountBefore + 1, cloud1->getNumberOfScalarFields())) - 1);
//       // cloud1->showSF(true);
//        m_app->addToDB(cloud1);
//    }
//    else
//            {
//                m_app->dispToConsole("An error occurred! (see console)",ccMainAppInterface::STD_CONSOLE_MESSAGE);
//            }
//    cloud1->prepareDisplayForRefresh_recursive();

//}
