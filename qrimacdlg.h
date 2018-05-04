#ifndef QRIMACDLG_H
#define QRIMACDLG_H

#include <QDialog>

//qCC
//#include <ccGLWindow.h>
#include "ccMainAppInterface.h"


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
   // inline virtual const ccHObject::Container& getSelectedEntities() const override
    //{
    //    return m_selectedEntities ;
   // }
    void doActionInterpolateScalarFields();
   // virtual void refreshAll(bool only2D = false) override;
    //virtual void updateUI() override;


private:
    Ui::qRIMACdlg *ui;
    //! Currently selected entities;
    ccHObject::Container m_selectedEntities;


public slots:
    void RVB_IN_SEARCH();
    void PIR_IN_SEARCH();
    void SWIR_IN_SEARCH();
    void lancer();
    void choix_nuage();
};

#endif // QRIMACDLG_H
