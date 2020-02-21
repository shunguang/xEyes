//----------------------------------------------------------------------------------------
#ifndef _YUV_CIRCULAR_Q_H_H_
#define _YUV_CIRCULAR_Q_H_H_

#include "YuvFrm_h.h"
#include "YuvFrm_d.h"
namespace xeyes {
	class  YuvCircularQ_h {
	public:
		YuvCircularQ_h(const uint32_t imgW=32, const uint32_t imgH=32, const uint32_t nTotItems=10, const std::string &name="YucFrmQue");
		~YuvCircularQ_h();

		//reset size
		void resetName(const std::string &name );
		void resetSize(const uint32_t imgW, const uint32_t imgH, const uint32_t nTotItems);

		//reset <m_headW> and <m_headR> as zeros
		void reset();

		//HD copy <x> into q[m_headW]
		bool wrt(const YuvFrm_h *src);  				//host interface
		bool wrt(const uint8_t *p, const uint32_t sz, const uint64_t fn=0); 	//host interface
#if APP_USE_CUDA
		bool wrt(const YuvFrm_d *src);  	//dev interface
		bool read(YuvFrm_d* dst);		//dev interface
#endif
		bool read(YuvFrm_h *dst);       	//host interface 
	protected:
		void allocQ(const uint32_t nTotItems);
		void freeQ();

	private:
		//-----------------------------------------------------------------------------
		//access queue
		//-----------------------------------------------------------------------------
		uint32_t		m_items;	//the predefined # of elements of the queue.
		uint32_t		m_headW;	//the index to write	
		uint32_t		m_headR;	//the index to write	

		std::vector<YuvFrm_hPtr>	m_q;
		std::vector<int>	m_v;        //count the wrt (++) / read(--) activities in m_q[i]


		boost::mutex m_mutexRW;
		uint32_t m_w;
		uint32_t m_h;
		std::string m_name;
	};
	typedef std::shared_ptr<YuvCircularQ_h> YuvCircularQ_hPtr;
}

#endif		