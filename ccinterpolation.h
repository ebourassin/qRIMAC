#ifndef CCINTERPOLATION_H
#define CCINTERPOLATION_H

#include "ccMainAppInterface.h"


class ccPointCloud;
class ccGLWindow;
class ccHObject;




class ccInterpolation
{
public:
    ccInterpolation();
    bool interpolate(const ccHObject::Container selectedEntities, ccMainAppInterface* m_app);
};

#endif // CCINTERPOLATION_H
