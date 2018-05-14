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

#include <mainwindow.h>


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
    // theCloud1 : premier nuage
    // iPoint1 : indice du point considéré dans le premier nuage
    // theCloud2 : second nuage
    // iPoint2 : induce du point considéré dans le second nuage

    // on considere que les deux points ont le meme nombre de champs scalaires
    float res = 0;
    float val1 = 0;
    float val2 = 0;
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
    // n : nombre de point du nuage theCloud
        if (n == 0)
            std::cout << "Erreur..." << std::endl;

    unsigned nbSF = theCloud->CCLib::ChunkedPointCloud::getNumberOfScalarFields() ;
    // nbSF : nombre de champs scalaires avant l'ajout du champ "classe"

    // ajout d'un champ scalaire de classe
    theCloud->ChunkedPointCloud::addScalarField("classe");

    // creation du nuage des centroides
    ChunkedPointCloud* centroides = new ChunkedPointCloud();

    centroides->reserve(nbClasse*(nbSF+1)*8);

    int nb_aleatoire = 0;
    srand(time(NULL)); // initialisation de rand

    // Ajout des champs scalaires au nuage centroides
    for(unsigned j=0;j<nbSF+1;++j)
    {
        std::string s = "SF_"+std::to_string(j);
        char const *nom = s.c_str();
        centroides->ChunkedPointCloud::addScalarField(nom);
        unsigned index_tmp = centroides->CCLib::ChunkedPointCloud::getScalarFieldIndexByName(nom);
        CCLib::ScalarField* sf = centroides->CCLib::ChunkedPointCloud::getScalarField(index_tmp);
        sf->reserve(nbSF*10); // A REGLER !!!!!!!
    }

    // Initialisation de centroides a partir de points choisis aleatoirement
    for(unsigned i=0; i<nbClasse; i++)
    {
        nb_aleatoire = rand()%n;
        const CCVector3 point =  *theCloud->getPoint(nb_aleatoire);
        centroides->ChunkedPointCloud::addPoint(point);

        for(unsigned j=0;j<nbSF;j++)
        {
            // on se place dans le champ scalaire a considerer
            centroides->CCLib::ChunkedPointCloud::setCurrentInScalarField(j);
            theCloud->CCLib::ChunkedPointCloud::setCurrentOutScalarField(j);
            // on recupere la valeur du champ scalaire du nuage
            float val_SF = theCloud->CCLib::ChunkedPointCloud::getPointScalarValue(nb_aleatoire);
            // on initialise le champ scalaire
            centroides->CCLib::ChunkedPointCloud::setPointScalarValue(i, val_SF);
        }
        // on se place dans le champ scalaire de classe
        centroides->CCLib::ChunkedPointCloud::setCurrentInScalarField(nbSF);
        // on l'initialise à i pour le point d'indice i
        centroides->CCLib::ChunkedPointCloud::setPointScalarValue(i, i);
    }


    // boucle des iterations
    for(unsigned iter=0; iter<nbIteration; ++iter)
    {
        std::cout<<"Iteration "<< iter <<std::endl;

        int stats_classe[nbClasse];
        for (unsigned i=0; i<nbClasse;i++){
            stats_classe[i]=0;
        }
        //boucle sur les points
        for(unsigned i=0; i<n; ++i)
        {
            float minDist = 9999999999999; // à modifier !
            unsigned j_min = 0;

            //boucle sur les centroides
            for(unsigned j=0; j<nbClasse;++j)
            {
                // si la distance au centroide est inferieure a la distance mini a un centroide
                if (minDist > ccClassification::distanceEucl(theCloud, i, centroides, j, nbSF))
                {

                    // on attribut a minDist la valeur de la nouvelle distance
                    minDist = ccClassification::distanceEucl(theCloud, i, centroides, j, nbSF);
                    // on attribut a l'indice du centroide le plus proche l'indice du centroide courant
                    j_min = j;
                }
            }
            // on se place dans le champ scalaire de la classe
            theCloud->CCLib::ChunkedPointCloud::setCurrentInScalarField(nbSF);
            // on attribut au point d'indice i le centroide d'indice j_min
            theCloud->CCLib::ChunkedPointCloud::setPointScalarValue(i, j_min);
            stats_classe[j_min]+=1;



        }
        // recalcul des champs scalaires des centroides
        for(unsigned l=0; l<nbClasse; ++l) // boucle sur les centroides
        {
            // mettre les SF des centroides à 0
            for(unsigned i_SF=0; i_SF<nbSF; ++i_SF)
            {
                // on se place dans le champ scalaire a considerer
                centroides->CCLib::ChunkedPointCloud::setCurrentInScalarField(i_SF);

                // on initialise la valeur a 0
                centroides->CCLib::ChunkedPointCloud::setPointScalarValue(l, 0);
            }

            unsigned cpt_modif = 0;

            for(unsigned k=0; k<n; ++k) // boucle sur les points
            {
                // on se place dans le SF de la classe
                theCloud->CCLib::ChunkedPointCloud::setCurrentOutScalarField(nbSF);
                // si la valeur de classe = centroide qu'on considere
                if (theCloud->getPointScalarValue(k) == l)
                {
                    cpt_modif += 1;

                    // pour chaque SF du centroide : += SF du point
                    for(unsigned i_SF=0; i_SF<nbSF; ++i_SF)
                    {
                        // on se place dans le champ scalaire a considerer
                        theCloud->CCLib::ChunkedPointCloud::setCurrentOutScalarField(i_SF);
                        centroides->CCLib::ChunkedPointCloud::setCurrentOutScalarField(i_SF);
                        // on recupere la valeur correspondante
                        float val1 = theCloud->CCLib::ChunkedPointCloud::getPointScalarValue(k);
                        float val2 = centroides->CCLib::ChunkedPointCloud::getPointScalarValue(l);
                        // on ajoute a la variable resultat la diffence des valeurs au carre
                        float sum = val1+val2;
                        centroides->CCLib::ChunkedPointCloud::setCurrentInScalarField(i_SF);
                        centroides->CCLib::ChunkedPointCloud::setPointScalarValue(l, sum);
                    }
                }
            }

            for(unsigned i_SF=0; i_SF<nbSF; ++i_SF)
            {
                // on se place dans le champ scalaire a considerer
                centroides->CCLib::ChunkedPointCloud::setCurrentOutScalarField(i_SF);

                float val = centroides->CCLib::ChunkedPointCloud::getPointScalarValue(l)/cpt_modif;
                centroides->CCLib::ChunkedPointCloud::setCurrentInScalarField(i_SF);
                centroides->CCLib::ChunkedPointCloud::setPointScalarValue(l, val);
            }

        }
        for (unsigned i=0; i<nbClasse;i++){
            std::cout<<"Classe "<<i<<" : "<<stats_classe[i]<<" points"<<std::endl;
        }
    }
    delete centroides;
}
