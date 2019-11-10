//----------------------------------------------------------------------------------------
#ifndef _FRM_INFO_CIRCULAR_Q_H_H_
#define _FRM_INFO_CIRCULAR_Q_H_H_

#include "libCfg/CfgCam.h"
#include "libCfg/CfgLocalView.h"
#include "FrmInfo.h"
namespace xeyes {
	enum ReadYuvFlag {
		READ_YUV_4_DET = 0,
		READ_YUV_4_DSP
	};

	class  FrmInfoCircularQ {
	public:
		FrmInfoCircularQ(const CfgCam &cam, const CfgLocalView &lv,	const std::string &name="FrmInfoQ");
		~FrmInfoCircularQ();

		//reset size
		void resetName(const std::string &name );
		void resetSize(int w0, int h0, int detPyrL, int dspPyrL, const uint32_t nTotItems);

		//reset <m_headW> and <m_headR> as zeros
		void reset();

		//wrt and read YuvFrms
		bool wrtYuv(const YuvFrm_h *src);  	//host interface
		bool readYuv(YuvFrm_h *dst, const ReadYuvFlag f);       	//host interface 
#if APP_USE_CUDA
		bool wrtYuv(const YuvFrm_d *src);  	//dev interface
		bool readYuv(YuvFrm_d* dst);		//dev interface
#endif
		//wrt and read DetFrms
		bool wrtDet(const DetFrm_h *src);  	//host interface
		bool readDet(DetFrm_h *dst);		//host interface 

		//wrt and read DspFrms
		bool wrtDsp(const DspFrm_h *src);  	//host interface
		bool readDsp( DspFrm_h *dst);       //host interface 

	protected:
		void allocQ();
		void freeQ();

	private:
		//-----------------------------------------------------------------------------
		//access queue
		//-----------------------------------------------------------------------------
		uint32_t		m_items;		//the predefined # of elements of the queue.
		uint32_t		m_yuvWrtH;		//the index to write YuvFrms
		uint32_t		m_yuvReadH4Det;	//the index to read  YuvFrms for detetcion thread
		uint32_t		m_yuvReadH4Dsp;	//the index to read  YuvFrms for display thread
		uint32_t		m_detWrtH;		//the index to write DetFrms
		uint32_t		m_detReadH;		//the index to read  DetFrms
		uint32_t		m_dspWrtH;		//the index to write DspFrms
		uint32_t		m_dspReadH;		//the index to read  DspFrms

		std::vector<FrmInfoPtr>	m_q;
		std::vector<int>		m_vYuvWrtCnt;       //m_vYuvWrtCnt[i]: wrt (++)/read(--) activities in m_q[i] for YuvFrm
		std::vector<int>		m_vDetWrtCnt;       //m_vDetWrtCnt[i]: wrt (++)/read(--) activities in m_q[i] for DetFrm
		std::vector<int>		m_vDspWrtCnt;       //m_vDspWrtCnt[i]: wrt (++)/read(--) activities in m_q[i] for DspFrm

		boost::mutex			m_mutex4YuvRW;
		boost::mutex			m_mutex4DetRW;      //read/wrt detection frms
		boost::mutex			m_mutex4DspRW;		//read/wrt dispaly frms
		boost::mutex			m_mutex4All;

		//params for queque
		uint32_t	m_w0;
		uint32_t	m_h0;
		int			m_detPyrL;
		int			m_dspPyrL;
		std::string m_name;
		int			m_camId;
	};
	typedef std::shared_ptr<FrmInfoCircularQ> FrmInfoCircularQPtr;
}

#endif		