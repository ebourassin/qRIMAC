#ifndef QRIMACDLG_H
#define QRIMACDLG_H

#include <QDialog>

//qCC
//#include <ccGLWindow.h>
#include "ccMainAppInterface.h"
class ccHObject;
class ccGLWindow;

namespace Ui {
class qRIMACdlg;
}

class qRIMACdlg : public QDialog
{
    Q_OBJECT

public:
    explicit qRIMACdlg(QWidget *parent = 0);
    ~qRIMACdlg();
    ccGLWindow* win;
    ccMainAppInterface* m_app;

    bool interpolate(const ccHObject::Container &selectedEntities);

   // void doActionInterpolateScalarFields();


private:
    Ui::qRIMACdlg *ui;

    //! Currently selected entities;
    ccHObject::Container m_selectedEntities;


public slots:
    void RVB_IN_SEARCH();
    void PIR_IN_SEARCH();
    void SWIR_IN_SEARCH();
    void lancer();
    void lancer_classif();
    void choix_nuage();
    void VT_nuage();
};

#endif // QRIMACDLG_H
