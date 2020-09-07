/*
*------------------------------------------------------------------------
*DcUI.cpp
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
#include "DcUI.h"
using namespace std;
using namespace xeyes;
DcUI::DcUI(CfgPtr &cfg)
	: m_cfg(cfg)
	, m_nTotCams(0)
	, m_dcMap()
{
	initSharedDc();
}

//initial shared data container
void DcUI::initSharedDc()
{
	//create data container for each camera
	vector<int> vCamId;
	m_cfg->getCamIds(vCamId);
	m_nTotCams = vCamId.size();
	for (const auto& camId : vCamId) {
		CfgCam p = m_cfg->getCam(camId);
		std::string name = "FrmInfoQ_" + std::to_string(camId);
		DcPtr dc(new Dc( camId, m_cfg));
		m_dcMap[camId] = dc;
	}
}
