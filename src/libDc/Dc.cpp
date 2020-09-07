/*
*------------------------------------------------------------------------
*Dc.cpp
*
* This code was developed by Shunguang Wu in his spare time. No government
* or any client funds were used.
*
*
* THE SOFTWARE IS PROVIDED AS-IS AND WITHOUT WARRANTY OF ANY KIND,
* EXPRESS, IMPLIED OR OTHERWISE, INCLUDING WITHOUT LIMITATION, ANY
* WARRANTY OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.
*
* IN NO EVENT SHALL THE AUTHOR OR DISTRIBUTOR BE LIABLE FOR
* ANY SPECIAL, INCIDENTAL, INDIRECT OR CONSEQUENTIAL DAMAGES OF ANY KIND,
* OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
* WHETHER OR NOT ADVISED OF THE POSSIBILITY OF DAMAGE, AND ON ANY THEORY OF
* LIABILITY, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE
* OF THIS SOFTWARE.
*
* Permission to use, copy, modify, distribute, and sell this software and
* its documentation for any purpose is prohibited unless it is granted under
* the author's written notice.
*
* Copyright(c) 2020 by Shunguang Wu, All Right Reserved
*-------------------------------------------------------------------------
*/
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

