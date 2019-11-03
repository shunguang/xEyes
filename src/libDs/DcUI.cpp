#include "DcUI.h"
using namespace std;
using namespace xeyes;
DcUI :: DcUI( CfgPtr &cfg )
: m_cfg( cfg )
, m_nTotCams(0)
, m_dcMap()
, m_dispRgbFrmQ()
{
   initSharedDc();
}

//initial shared data container
void DcUI::initSharedDc()
{
   //create data container for each camera
   vector<int> vCamId;
   m_cfg->getCamIds( vCamId );

   m_nTotCams = vCamId.size();
   for (const auto& camId : vCamId) {       
      DcPtr dc( new Dc( camId,  m_cfg ) );      
      m_dcMap[camId] = dc;
   }

   //create display Q
   CfgLocalView dsp = m_cfg->getLocalView();
   ImgSize sz = dsp.getDispImgSz();

   m_dispRgbFrmQ.reset();
	m_dispRgbFrmQ.resetName( "Disp Que" );
	m_dispRgbFrmQ.resetSize( sz.w, sz.h, dsp.dispQueSz_, dsp.dispPyrLev_);
}
