#include "GroupItem.h"

CGroupItem::CGroupItem(QString &text, QString &key)
{
    setText(text);
    m_szKey = key;
}

CGroupItem::~CGroupItem()
{
}
