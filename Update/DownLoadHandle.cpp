#include "DownLoadHandle.h"
#include "Global/Global.h"
#include <QtXml>
#include "Version.h"
#include <QMessageBox>
#include "MainWindow.h"
#include <QFile>

CDownLoadHandleFile::CDownLoadHandleFile(QObject *parent):QObject(parent)
{}

CDownLoadHandleFile::~CDownLoadHandleFile()
{}

int CDownLoadHandleFile::SetFile(const QString &szFile)
{
    m_szFile = szFile;
    return 0;
}

int CDownLoadHandleFile::OnEnd(int nErrorCode)
{
    if(nErrorCode)
    {
        QMessageBox::critical(NULL, tr("Error"), tr("Download error:%1").arg(QString::number(nErrorCode)));
        return nErrorCode;
    }
    //启动安装程序  
    m_Process.start(m_szFile);
    //关闭程序  
    MainWindow* pMain = (MainWindow*)GET_MAINWINDOW;
    pMain->close();
    return 0;
}

CDownLoadHandleVersionFile::CDownLoadHandleVersionFile(const std::string &szFile, QObject *parent): QObject(parent)
{
    m_szFile = szFile;
}

int CDownLoadHandleVersionFile::SetFile(const std::string &szFile)
{
    m_szFile = szFile;
    return 0;
}

int CDownLoadHandleVersionFile::OnEnd(int nErrorCode)
{
    if(m_szFile.empty())
        return -1;

    QFile file(m_szFile.c_str());
    if(!file.open(QIODevice::ReadOnly))
    {
        LOG_MODEL_ERROR("Update", "Don't open file:%s", m_szFile.c_str());
        return -2;
    }

    QDomDocument doc;
    doc.setContent(&file);
    QDomElement startElem = doc.documentElement();
    QString szVersion = startElem.firstChildElement("version").text();
    if(szVersion <= VERSION)
    {
        LOG_MODEL_DEBUG("Update", "Is already the newest version.");
        return 0;
    }

    QString szUrl = startElem.firstChildElement("url").text();
    QUrl url(szUrl);
    QString szDownLoadFile = CGlobal::Instance()->GetDirApplicationDownLoad() + QDir::separator() + url.fileName();
    QString szForce = startElem.firstChildElement("force").text();
    if(szForce == "true")//强制更新  
    {
        m_DownLoadFile.SetFile(szDownLoadFile);
        return m_DownLoad.Start(szUrl.toStdString(), szDownLoadFile.toStdString(), &m_DownLoadFile);
    }

    QString szText;
    if(QMessageBox::Ok != 
       QMessageBox::information(NULL, tr("Update"), szText, QMessageBox::Ok, QMessageBox::No))
    {
        return 0;
    }

    m_DownLoadFile.SetFile(szDownLoadFile);
    return m_DownLoad.Start(szUrl.toStdString(), szDownLoadFile.toStdString(), &m_DownLoadFile);
}
