#ifndef MENUMESSAGESEND_H
#define MENUMESSAGESEND_H

#include <QMenu>

class CMenuMessageSend : public QMenu
{
    Q_OBJECT
public:
    explicit CMenuMessageSend(QWidget *parent = 0);

private:
    int InitMenu();
    QMenu m_MenuSend;
    QAction* m_ActionEnter;
    QAction* m_ActionCtrlEnter;
private slots:
    void slotActionEnter();
    void slotActionCtrlEnter();
    void slotUpdateMenu();

signals:
    
public slots:
    
};

#endif // MENUMESSAGESEND_H
