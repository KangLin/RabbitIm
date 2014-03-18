#ifndef ROSTERITEM_H
#define ROSTERITEM_H

#include <QStandardItem>
#include "Roster.h"

class CRosterItem : public QStandardItem
{
public:
    CRosterItem(CRoster* pRoster);
    virtual ~CRosterItem();

private:
    CRoster* m_pRoster;
};

#endif // ROSTERITEM_H
