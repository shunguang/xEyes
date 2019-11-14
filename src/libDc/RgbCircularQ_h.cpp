#include "RgbCircularQ_h.h"

using namespace std;
using namespace cv;
using namespace xeyes;

RgbCircularQ_h::RgbCircularQ_h(const uint32_t imgW0, const uint32_t imgH0, const uint32_t nTotItems, const uint32_t L, const std::string &name)
	: m_v()
	, m_q()
	, m_w(imgW0>>L)
	, m_h(imgH0>>L)
	, m_L(L)
	, m_name(name)
{
	allocQ( nTotItems );
}

RgbCircularQ_h::~RgbCircularQ_h()
{
	freeQ();
}

void RgbCircularQ_h::resetSize(const uint32_t imgW0, const uint32_t imgH0, const uint32_t nTotItems,const uint32_t L)
{
	m_w = imgW0>>L;
	m_h = imgH0>>L;
    m_L = L;
	freeQ();
	allocQ(nTotItems);
	dumpLog("RgbCircularQ_h::resetSize(): name=%s, m_w=%d, m_h=%d, nTotItems=%d, m_L=%d", m_name.c_str(), m_w, m_h, nTotItems, m_L);
}

void RgbCircularQ_h::resetName(const std::string &name )
{
	boost::mutex::scoped_lock lock(m_mutexRW);	
	m_name = name;
}

void RgbCircularQ_h::reset()
{
	boost::mutex::scoped_lock lock(m_mutexRW);
	m_headW = 0;
	m_headR = 0;
	m_v.resize(m_items, 0);
}

void RgbCircularQ_h::freeQ()
{
	boost::mutex::scoped_lock lock(m_mutexRW);
	
	m_q.clear();
	m_v.clear();
	m_headW = 0;
	m_headR = 0;
}

void RgbCircularQ_h::allocQ(const uint32_t nTotItems)
{
	boost::mutex::scoped_lock lock(m_mutexRW);

	m_items = nTotItems;
	dumpLog("RgbCircularQ_h::allocQ(), m_w=%d, m_h=%d, m_items=%d, m_name=%s", m_w, m_h, m_items, m_name.c_str());
	m_q.clear();
	for (uint32_t i = 0; i < m_items; ++i) {
		RgbFrm_hPtr p( new RgbFrm_h(m_w, m_h, m_L, i) );
		m_q.push_back( p );
	}
	m_v.resize(m_items, 0);
	m_headW = 0;
	m_headR = 0;
	cout << "RgbCircularQ_h::allocQ done!" << endl;
}

//wrt from host
bool RgbCircularQ_h::wrt(const RgbFrm_h *src)
{
	static int wrtDropCnt = 0;
	bool sucWrt = false;
	{
		boost::mutex::scoped_lock lock(m_mutexRW);
		uint32_t &idx = m_headW;
		int   &cnt = m_v[idx];
		if (cnt == 0) {
			RgbFrm_h  *dst = m_q[idx].get();			
			dst->hdCopyFrom(src);	  //hard copy
			cnt = cnt + 1;

			//move head to the next slot
			++idx;
			if (idx >= m_items) {
				idx = 0;
			}
			sucWrt = true;
		}
	}

	if ( !sucWrt ) {
		++wrtDropCnt;
		if (wrtDropCnt > 999) {
			dumpLog("RgbCircularQ_h::wrt(): writen is too fast, %d frames droped", wrtDropCnt);
			wrtDropCnt = 0;
		}
	}
	return sucWrt;
}

//the consumer read  <x> from que
bool RgbCircularQ_h::read( RgbFrm_h *dst )
{
	bool hasData = false;
	{
		//this lock moved to readNext() for specific RawFrmXYZ Type
		boost::mutex::scoped_lock lock(m_mutexRW);
		uint32_t &idx = m_headR;
		int   &cnt = m_v[idx];
		if (cnt > 0) {
			RgbFrm_h  *src = m_q[idx].get();			
			src->hdCopyTo( dst );		//hd copy the poiter
			cnt = 0;
			hasData = true;
			//move head to the next slot
			++idx;
			if (idx >= m_items) {
				idx = 0;
			}
		}
	}
	return hasData;
}
