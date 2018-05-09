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

#include <iostream>
#include <string>


#include <stdio.h>
#include <stdlib.h>
#include <time.h>



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

ccClassification::ccClassification()
{
}


float ccClassification::distanceEucl(ChunkedPointCloud* theCloud1, unsigned iPoint1,ChunkedPointCloud* theCloud2, unsigned iPoint2, unsigned nbSF)
{

    // on considere que les deux points ont le meme nombre de champs scalaires
    unsigned res = 0;
    unsigned val1 = 0;
    unsigned val2 = 0;
    for(unsigned i=0; i<nbSF; ++i)
    {
        // on se place dans le champ scalaire a considerer
        theCloud1->CCLib::ChunkedPointCloud::setCurrentOutScalarField(i);
        theCloud2->CCLib::ChunkedPointCloud::setCurrentOutScalarField(i);
        // on recupere la valeur correspondante
        val1 = theCloud1->CCLib::ChunkedPointCloud::getPointScalarValue(iPoint1);
        val2 = theCloud2->CCLib::ChunkedPointCloud::getPointScalarValue(iPoint2);
        // on ajoute a la variable resultat la diffence des valeurs au carre
        res += pow(val2-val1,2);
    }
    // on renvoie la racine carree de res
    return pow(res,0.5);
}


void ccClassification::KMeans(ChunkedPointCloud* theCloud, unsigned nbIteration, unsigned nbClasse)
{
    unsigned n = theCloud->size();
        if (n == 0)
            std::cout << "Erreur..." << std::endl;

    unsigned nbSF = theCloud->CCLib::ChunkedPointCloud::getNumberOfScalarFields() ;

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

    // ajout d'un champ scalaire de classe
    theCloud->ChunkedPointCloud::addScalarField("classe");

    // creation des centroides
    ChunkedPointCloud* centroides;

    centroides->reserve(nbClasse);

    int nb_aleatoire = 0;
    srand(time(NULL)); // initialisation de rand

    for(unsigned i=0; i<nbClasse; i++){
        nb_aleatoire = rand()%n;
        const CCVector3 point =  *theCloud->getPoint(nb_aleatoire);
        centroides->ChunkedPointCloud::addPoint(point);
        printf("%d ",nb_aleatoire);
    }


    // boucle des iterations
    for(unsigned iter=0; iter<nbIteration; ++iter)
    {
        //boucle sur les points
        for(unsigned i=0; i<n; ++i)
        {
            float minDist = -1; // à modifier !
            unsigned j_min = 0;
            //boucle sur les centroides
            for(unsigned j=0; j<nbClasse;++j)
            {
                if (minDist < ccClassification::distanceEucl(theCloud, i, centroides, j, nbSF))
                {
                    minDist = ccClassification::distanceEucl(theCloud, i, centroides, j, nbSF);
                    j_min = j;
                }
            }
            theCloud->CCLib::ChunkedPointCloud::setCurrentOutScalarField(nbSF+1);
            theCloud->CCLib::ChunkedPointCloud::setPointScalarValue(i, j_min);

            // recalcul des champs scalaires des centroides

        }


    }



}
