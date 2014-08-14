#ifndef FRMOPTIONS_H
#define FRMOPTIONS_H

#include <QFrame>

namespace Ui {
class CFrmOptions;
}

class CFrmOptions : public QFrame
{
    Q_OBJECT
    
public:
    static CFrmOptions * Instance();

signals:
    void sigRefresh();

private slots:
    void on_pbOK_clicked();
    void on_pbCancel_clicked();
    void on_pbLocalUserColor_clicked();
    void on_pbRosterColor_clicked();
    void on_pbLocalUserMessageColor_clicked();
    void on_pbRosterMessageColor_clicked();

    void on_pbUnreadMessageCountColor_clicked();
    
protected:
    virtual void showEvent(QShowEvent* );
    virtual void closeEvent(QCloseEvent*);

private:
    explicit CFrmOptions(QWidget *parent = 0);
    ~CFrmOptions();

    Ui::CFrmOptions *ui;

    QWidget* m_pParent;
};

#endif // FRMOPTIONS_H
