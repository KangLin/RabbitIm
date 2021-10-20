#ifndef FRMLBSMAIN_H
#define FRMLBSMAIN_H

#include <QFrame>
#include "LbsMotion.h"
#include "LbsTrack.h"
#include "lbs_export.h"

namespace Ui {
class CFrmLbsMain;
}

class LBS_EXPORT CFrmLbsMain : public QFrame
{
    Q_OBJECT
    
public:
    explicit CFrmLbsMain(QWidget *parent = 0);
    ~CFrmLbsMain();
    
private:
    Ui::CFrmLbsMain *ui;
    
    CLbsMotion m_Motion;
    CLbsTrack m_Track;
};

#endif // FRMLBSMAIN_H
