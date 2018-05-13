#ifndef CCCLASSIFICATION_H
#define CCCLASSIFICATION_H

#include "ccMainAppInterface.h"


namespace CCLib
{
class ChunkedPointCloud;
class ccHObject;
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
