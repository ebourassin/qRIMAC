#include "ccinterpolation.h"

//System
#include <iostream>
#include <string>
#include <typeinfo>
#include <vector>
#include "math.h"

#include <QProgressDialog>

#include <mainwindow.h>


//qCC
#include "ccclassification.h"
#include "ccConsole.h"


#include "ccInterpolationDlg.h"
#include "ccItemSelectionDlg.h"
#include "ccProgressDialog.h"


//Qt
#include <QString>

//qCC_db
#include "ccPointCloud.h"

//CClib
#include <ScalarFieldTools.h>
#include "ReferenceCloud.h"
#include "ScalarField.h"
#include "GenericCloud.h"
#include "ChunkedPointCloud.h"
#include "ccGenericPointCloud.h"

using namespace CCLib;

ccInterpolation::ccInterpolation()
{
}

bool ccInterpolation::interpolate(const ccHObject::Container selectedEntities, ccMainAppInterface* m_app)
{
    std::cout << "Dans interpolate you motherfucker" << std::endl;

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
            std::cout << "Dans 1er if" << std::endl;
            sfIndexes.push_back(0);
        }
        else if (sfCount > 1)
        {
            std::cout << "Dans else if" << std::endl;

            ccItemSelectionDlg isDlg(true, m_app->getMainWindow(), "entity");
            std::cout << "Passé 1" << std::endl;

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
            std::cout << "Dans else" << std::endl;
            assert(false);
        }
        std::cout << "Passé else final" << std::endl;

    }
    catch (const std::bad_alloc&)
    {
        ccConsole::Error("Not enough memory");
        return false;
    }
    std::cout << "Passé catch" << std::endl;


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
