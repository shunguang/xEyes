#include "Dc.h"
using namespace std;
using namespace xeyes;
Dc::Dc(const int camId,  CfgPtr &cfg)
: m_camId( camId )
, m_cfg( cfg )
, m_frmInfoQ(0)
{
	CfgCam cam		= cfg->getCam(camId);
	CfgLocalView lv = cfg->getLocalView();
	cout << "Dc::Dc()" << cam.toString() << endl;

	std::string name = "FrmInfoQ_" + std::to_string(camId);
	m_frmInfoQ.reset( new FrmInfoCircularQ(cam, lv, name) );

	//reset detection que
	//m_detRgbFrmQ.resetName( cam.cameraName_ +  "YUV capture Que" );
	//m_detRgbFrmQ.resetSize( cam.imgSz_.w, cam.imgSz_.h, cam.capQueSz_, cam.detPyrLev_);
	cout << "Dc::Dc(): done!" << endl;
}

