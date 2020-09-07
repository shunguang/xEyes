/*
*------------------------------------------------------------------------
*RgbCircularQ_h.h
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
//----------------------------------------------------------------------------------------
#ifndef _RGB_CIRCULAR_Q_H_H_
#define _RGB_CIRCULAR_Q_H_H_

#include "RgbFrm_h.h"
namespace xeyes {
	class  RgbCircularQ_h {
	public:
		RgbCircularQ_h( const uint32_t imgW0=32, const uint32_t imgH0=32, const uint32_t nTotItems=10, const uint32_t L=0, const std::string &name="RgbFrmQue");
		~RgbCircularQ_h();

		//reset size
		void resetName(const std::string &name );
		void resetSize(const uint32_t imgW0, const uint32_t imgH0, const uint32_t nTotItems, const uint32_t L);

		//reset <m_headW> and <m_headR> as zeros
		void reset();

		bool wrt(const RgbFrm_h *src);  	//host interface
		bool read(RgbFrm_h *dst);       	//host interface 

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

		std::vector<RgbFrm_hPtr>    m_q;
		std::vector<int>	        m_v;    //count the wrt (++) / read(--) activities in m_q[i]


		boost::mutex m_mutexRW;
		uint32_t m_w;
		uint32_t m_h;
		uint32_t m_L;         //pyramid level
		std::string m_name;
	};
	typedef std::shared_ptr<RgbCircularQ_h> RgbCircularQ_hPtr;
}

#endif		
