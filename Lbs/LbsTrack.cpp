#include "LbsTrack.h"
#include "ui_LbsTrack.h"
#include <QDebug>

CLbsTrack::CLbsTrack(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CLbsTrack)
{
    ui->setupUi(this);
    
    m_Source = QGeoPositionInfoSource::createDefaultSource(this);
    if (m_Source) {
        bool check = connect(m_Source, SIGNAL(positionUpdated(QGeoPositionInfo)),
                this, SLOT(positionUpdated(QGeoPositionInfo)));
        Q_ASSERT(check);
        //TODO:
        m_Source->startUpdates();
    }
}

CLbsTrack::~CLbsTrack()
{
    delete ui;
}

void CLbsTrack::positionUpdated(const QGeoPositionInfo &info)
{
        qDebug() << "Position updated:" << info;
}
