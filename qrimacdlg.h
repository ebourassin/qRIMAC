#ifndef QRIMACDLG_H
#define QRIMACDLG_H

#include <QDialog>
#include "ccPointListPickingDlg.h"

//qCC
//#include <ccGLWindow.h>
#include "ccMainAppInterface.h"

class ccHObject;
class ccGLWindow;
class ccPointListPickingDlg;
class ccPickingHub;

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


private:
    Ui::qRIMACdlg *ui;

    ccHObject::Container m_selectedEntities;
    ccPickingHub* m_pickingHub;
    ccPointListPickingDlg* m_plpDlg;


public slots:
    void RVB_IN_SEARCH();
    void PIR_IN_SEARCH();
    void SWIR_IN_SEARCH();
    void lancer();
    void lancer_classif();
    void choix_nuage();
    void VT_nuage();
    void select_point();
};

#endif // QRIMACDLG_H
