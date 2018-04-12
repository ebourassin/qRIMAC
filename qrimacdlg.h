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
    void RVB_IN_SEARCH();
    void PIR_IN_SEARCH();
    void SWIR_IN_SEARCH();
    void lancer();
};

#endif // QRIMACDLG_H
