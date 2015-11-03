#ifndef DLGSCANQRCODE_H
#define DLGSCANQRCODE_H

#include <QDialog>

namespace Ui {
class CDlgScanQRcode;
}

class CDlgScanQRcode : public QDialog
{
    Q_OBJECT
    
public:
    explicit CDlgScanQRcode(QWidget *parent = 0);
    ~CDlgScanQRcode();

    //识别二维码文件  
    int ProcessQRFile(QString szFile);

private:
    int ProcessMessage(QString szMessage);
    
private slots:
    void on_pushBrowse_clicked();
    
private:
    Ui::CDlgScanQRcode *ui;
};

#endif // DLGSCANQRCODE_H
