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
