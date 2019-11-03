#include "Dc.h"
using namespace std;
using namespace xeyes;
Dc::Dc(const int camId,  CfgPtr &cfg)
: m_camId( camId )
, m_cfg( cfg )
, m_capYuvFrmQ()
, m_detRgbFrmQ()
{
	CfgCam cam = cfg->getCam( camId );
	
	cout << cam.toString() << endl;
	//reset capture que
	m_capYuvFrmQ.resetName( cam.cameraName_ +  "YUV capture Que" );
	m_capYuvFrmQ.resetSize( cam.imgSz_.w, cam.imgSz_.h, cam.capQueSz_);

	//reset detection que
	m_detRgbFrmQ.resetName( cam.cameraName_ +  "YUV capture Que" );
	m_detRgbFrmQ.resetSize( cam.imgSz_.w, cam.imgSz_.h, cam.capQueSz_, cam.detPyrLev_);
}

