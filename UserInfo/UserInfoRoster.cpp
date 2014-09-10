#include "UserInfoRoster.h"

CUserInfoRoster::CUserInfoRoster(QObject *parent) : CUserInfo(parent)
{
}

QDataStream & operator <<(QDataStream &output, const CUserInfoRoster &roster)
{
    output << (CUserInfo&)roster;
    output << roster.m_Group.size();
    QString szGroup;
    foreach(szGroup, roster.m_Group)
    {
        output << szGroup;
    }
    return output;
}

QDataStream & operator >>(QDataStream &input, CUserInfoRoster &roster)
{
    input >> (CUserInfo&)roster;
    int nSize = 0;
    input >> nSize;
    while(nSize--)
    {
        QString szGroup;
        input >> szGroup;
        roster.m_Group << szGroup;
    }
    return input;
}
