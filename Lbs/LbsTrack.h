#ifndef LBSMAIN_H
#define LBSMAIN_H

#include <QWidget>
#include <QGeoPositionInfoSource>
#include <QGeoPositionInfo>

namespace Ui {
class CLbsTrack;
}

class CLbsTrack : public QWidget
{
    Q_OBJECT
    
public:
    explicit CLbsTrack(QWidget *parent = 0);
    ~CLbsTrack();

private slots:
    void positionUpdated(const QGeoPositionInfo &info);

private:
    Ui::CLbsTrack *ui;
    QGeoPositionInfoSource *m_Source;
};

#endif // LBSMAIN_H
