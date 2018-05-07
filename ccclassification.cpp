//##########################################################################
//#                                                                        #
//#                              CLOUDCOMPARE                              #
//#                                                                        #
//#  This program is free software; you can redistribute it and/or modify  #
//#  it under the terms of the GNU General Public License as published by  #
//#  the Free Software Foundation; version 2 or later of the License.      #
//#                                                                        #
//#  This program is distributed in the hope that it will be useful,       #
//#  but WITHOUT ANY WARRANTY; without even the implied warranty of        #
//#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the          #
//#  GNU General Public License for more details.                          #
//#                                                                        #
//#                    COPYRIGHT: Daniel Girardeau-Montaut                 #
//#                                                                        #
//##########################################################################

#include "ccclassification.h"

<<<<<<< HEAD
#include <iostream>
#include <string>

//CClib
#include "ScalarFieldTools.h"
#include "ReferenceCloud.h"
#include "ScalarField.h"
#include "GenericCloud.h"
#include "ChunkedPointCloud.h"


#include "ccGenericPointCloud.h"


//qCC_db
#include "ccPointCloud.h"

using namespace CCLib;
=======
#include <mainwindow.h>


//qCC
#include <ccGLWindow.h>
#include "qrimacdlg.h"
#include "ui_qrimacdlg.h"

#include "ccProgressDialog.h"

//Qt
#include "qfiledialog.h"
#include <QString>
#include <QFile>
#include <QFileDialog>
#include <QMainWindow>
>>>>>>> 5d6db42a7119681658c2d2099ce37585fc0487f4

ccClassification::ccClassification()
{
}

void ccClassification::KMeans(ChunkedPointCloud* theCloud)
{
    unsigned n = theCloud->size();
        if (n == 0)
            std::cout << "Erreur..." << std::endl;

    for (unsigned i=0; i<4; ++i)
    {
        std::cout << "Nb Champs Scalaires " << theCloud->CCLib::ChunkedPointCloud::getNumberOfScalarFields() << std::endl;
        std::cout << "Point " << i << std::endl;
        const char champ1[] = "Scalar field";
        const char champ2[] = "PIR";
        const char champ3[] = "SF2";
        const char champ4[] = "SF3";
        ScalarType V1 = theCloud->CCLib::ChunkedPointCloud::getScalarFieldIndexByName(champ1);
        ScalarType V2 = theCloud->CCLib::ChunkedPointCloud::getScalarFieldIndexByName(champ2);
        ScalarType V3 = theCloud->CCLib::ChunkedPointCloud::getScalarFieldIndexByName(champ3);
        ScalarType V4 = theCloud->CCLib::ChunkedPointCloud::getScalarFieldIndexByName(champ4);

        if (CCLib::ScalarField::ValidValue(V1))
        {

            int v1 = (int) V1;
            theCloud->CCLib::ChunkedPointCloud::setCurrentOutScalarField(v1);

            std::cout << theCloud->CCLib::ChunkedPointCloud::getPointScalarValue(i) << std::endl;
        }

        if (CCLib::ScalarField::ValidValue(V2))
        {

            int v2 = (int) V2;
            theCloud->CCLib::ChunkedPointCloud::setCurrentOutScalarField(v2);

            std::cout << theCloud->CCLib::ChunkedPointCloud::getPointScalarValue(i) << std::endl;
        }

        if (CCLib::ScalarField::ValidValue(V3))
        {

            int v3 = (int) V3;
            theCloud->CCLib::ChunkedPointCloud::setCurrentOutScalarField(v3);

            std::cout << theCloud->CCLib::ChunkedPointCloud::getPointScalarValue(i) << std::endl;
        }

        if (CCLib::ScalarField::ValidValue(V4))
        {

            int v4 = (int) V4;
            theCloud->CCLib::ChunkedPointCloud::setCurrentOutScalarField(v4);

            std::cout << theCloud->CCLib::ChunkedPointCloud::getPointScalarValue(i) << std::endl;
        }


    }

}


