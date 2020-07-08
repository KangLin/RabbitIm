#include "CameraFactory.h"
#include <iostream>

/************************************************************************/
/* 视频处理类                                                            */
/************************************************************************/
class CameraHander : public CCamera::CHanderFrame
{
public:
	virtual int OnFrame(const std::shared_ptr<CVideoFrame> frame)
	{
		std::cout << "format:" << frame->m_VideoInfo.Format << ";width:" << frame->m_VideoInfo.nWidth
			<< ";height:" << frame->m_VideoInfo.nHeight << ";time:" << frame->m_Timestamp << std::endl;
		return 0;
	}
};

/************************************************************************/
/* 使用示例                                                              */
/************************************************************************/
int main()
{
	int nRet = 0;
	std::vector<CCameraInfo::CamerInfo> lstInfo;
	nRet = CCameraFactory::Instance()->EnumDevice(lstInfo);
	std::vector<CCameraInfo::CamerInfo>::iterator it;
	for (it = lstInfo.begin(); it != lstInfo.end(); it++)
	{
		std::cout << "index:" << it->nIndex
			<< ";name:" << it->szName << ";";
		if(it->Format.size() > 0)
			std::cout << "pixmap:" << VideoFormatToName(it->Format[0].Format);
		std::cout <<  std::endl;

	}

	CameraHander hander;
	VideoInfo vi;
	vi.Format = VIDEO_FORMAT_RGB24;
	vi.nHeight = 480;
	vi.nWidth = 640;
	vi.nRatio = 15;
	CCameraFactory::Instance()->GetCamera(0)->Open(&hander, &vi);
	CCameraFactory::Instance()->GetCamera(0)->Start();
	while (1)
	{
		;
	}
	CCameraFactory::Instance()->GetCamera(0)->Stop();
	CCameraFactory::Instance()->GetCamera(0)->Close();

	return nRet;
}
