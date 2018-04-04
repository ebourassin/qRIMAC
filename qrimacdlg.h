#ifndef QRIMACDLG_H
#define QRIMACDLG_H

#include <QDialog>

//qCC
#include <ccGLWindow.h>
#include "ccMainAppInterface.h"
#include "ccPickingListener.h"


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

private:
    Ui::qRIMACdlg *ui;


public slots:
    void lancer();
/*
    void select_cloud();
    void sup_pts_sans_ppv();
    void name_out();
    void lancer();
    void bar_progression();
    void rvb();
    void swir();
    void pir();
    void select_rvb();
    void select_pir();
    void select_swir();
    void dist_voxel();
    void dist_choice();
    void out();
    void dist_unite();
    void dist_choice();
*/
};

#endif // QRIMACDLG_H
