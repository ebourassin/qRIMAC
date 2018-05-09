#ifndef CCCLASSIFICATION_H
#define CCCLASSIFICATION_H


namespace CCLib
{
class ChunkedPointCloud;
}

class ccClassification
{
public:
    ccClassification();
    float distanceEucl(CCLib::ChunkedPointCloud* theCloud1, unsigned iPoint1,CCLib::ChunkedPointCloud* theCloud2, unsigned iPoint2, unsigned nbSF);
    void KMeans(CCLib::ChunkedPointCloud* theCloud, unsigned nbIteration, unsigned nbClasse);
};


#endif // CCCLASSIFICATION_H
