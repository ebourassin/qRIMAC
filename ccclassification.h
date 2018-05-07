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
    void KMeans(CCLib::ChunkedPointCloud *theCloud);
};


#endif // CCCLASSIFICATION_H
