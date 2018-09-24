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
//#          COPYRIGHT: Emile Blettery, Emmanuel Bourassin                 #
//#                     Guillemette Fonteix (ENSG)                         #
//#                                                                        #
//##########################################################################

#ifndef CCCLASSIFICATION_H
#define CCCLASSIFICATION_H

#include "ccMainAppInterface.h"


namespace CCLib
{
class ChunkedPointCloud;
class MainWindow;
}

class ccClassification
{
public:
    ccClassification();
    float distanceEucl(CCLib::ChunkedPointCloud* theCloud1, unsigned iPoint1,CCLib::ChunkedPointCloud* theCloud2, unsigned iPoint2, unsigned nbSF);
    void KMeans(CCLib::ChunkedPointCloud* theCloud, unsigned nbIteration, unsigned nbClasse);
};


#endif // CCCLASSIFICATION_H
