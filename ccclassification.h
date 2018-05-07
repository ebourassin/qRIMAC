#ifndef CCCLASSIFICATION_H
#define CCCLASSIFICATION_H

<<<<<<< HEAD
namespace CCLib
{
class ChunkedPointCloud;
}
=======
#include <QDialog>

//qCC
#include <ccGLWindow.h>
#include "ccMainAppInterface.h"
#include "qrimacdlg.h"
#include "ui_qrimacdlg.h"

>>>>>>> 5d6db42a7119681658c2d2099ce37585fc0487f4

class ccClassification

{
public:
    ccClassification();
    void KMeans(CCLib::ChunkedPointCloud *theCloud);
};


#endif // CCCLASSIFICATION_H
