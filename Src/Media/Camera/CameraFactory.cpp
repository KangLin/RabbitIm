#include "CameraFactory.h"
#ifdef RABBITIM_USE_PJSIP_CAMERA
    #include "CameraFactoryPjsip.h"
#else
    #include "CameraFactoryQt.h"
#endif

CCameraFactory::CCameraFactory()
{
}

CCameraFactory::~CCameraFactory()
{
    std::map<int, CCamera*>::iterator it;
    for (it = m_Camera.begin(); it != m_Camera.end(); it++)
    {
        CCamera* p = it->second;
        if (p)
            delete p;
    }
    m_Camera.clear();
}

CCamera* CCameraFactory::GetCamera(int nIndex)
{
    std::map<int, CCamera*>::iterator it = m_Camera.find(nIndex);
    if (m_Camera.end() == it)
    {
        CCamera* p = NewCamera(nIndex);
        if (p)
        {
            m_Camera.insert(std::pair<int, CCamera*>(nIndex, p));
            return p;
        }
    }
    else
        return it->second;
    return NULL;
}

CCameraFactory* CCameraFactory::Instance()
{
    static CCameraFactory* p = NULL;
    if (NULL == p)
    {
#ifdef RABBITIM_USE_PJSIP_CAMERA
        p = new CCameraFactoryPjsip();
#else
        //windows、linuxQt5.2.0以上版本可正常工作  
        //android只有QT5.2.X可正常工作  
        p = new CCameraFactoryQt();
#endif
    }
    return p;
}
