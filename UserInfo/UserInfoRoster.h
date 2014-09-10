#ifndef USERINFOROSTER_H
#define USERINFOROSTER_H

#include "UserInfo.h"
#include <QSet>

class CUserInfoRoster : public CUserInfo
{
    Q_OBJECT
public:
    CUserInfoRoster(QObject *parent = NULL);

#ifndef QT_NO_DATASTREAM
    friend QDataStream & operator <<(QDataStream &output, const CUserInfoRoster &roster);
    friend QDataStream & operator >>(QDataStream &input, CUserInfoRoster &roster);
#endif

private:
    QSet<QString> m_Group;
};

#endif // USERINFOROSTER_H
