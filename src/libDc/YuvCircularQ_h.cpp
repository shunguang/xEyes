/*
*------------------------------------------------------------------------
*YuvCircularQ_h.cpp
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
#include "YuvCircularQ_h.h"

using namespace std;
using namespace xeyes;

YuvCircularQ_h::YuvCircularQ_h(const uint32_t imgW, const uint32_t imgH, const uint32_t nTotItems, const std::string &name)
	: m_v()
	, m_q()
	, m_w(imgW)
	, m_h(imgH)
	, m_name(name)
	, m_wrtDropCnt(0)
{
	allocQ( nTotItems );
}

YuvCircularQ_h::~YuvCircularQ_h()
{
	freeQ();
}

void YuvCircularQ_h::resetSize(const uint32_t imgW, const uint32_t imgH, const uint32_t nTotItems)
{
	m_w = imgW;
	m_h = imgH;
	freeQ();
	allocQ(nTotItems);
	dumpLog("YuvCircularQ_h::resetSize(): name=%s, m_w=%d, m_h=%d, nTotItems=%d", m_name.c_str(), m_w, m_h, nTotItems);
}

void YuvCircularQ_h::resetName(const std::string &name )
{
	boost::mutex::scoped_lock lock(m_mutexRW);	
	m_name = name;
}

void YuvCircularQ_h::reset()
{
	boost::mutex::scoped_lock lock(m_mutexRW);
	m_headW = 0;
	m_headR = 0;
	m_v.resize(m_items, 0);
}


void YuvCircularQ_h::freeQ()
{
	boost::mutex::scoped_lock lock(m_mutexRW);
	
	m_q.clear();
	m_v.clear();
	m_headW = 0;
	m_headR = 0;
}

void YuvCircularQ_h::allocQ(const uint32_t nTotItems)
{
	boost::mutex::scoped_lock lock(m_mutexRW);

	m_items = nTotItems;
	dumpLog("YuvCircularQ_h::allocQ(), m_w=%d, m_h=%d, m_items=%d, m_name=%s", m_w, m_h, m_items, m_name.c_str());
	m_q.clear();
	for (uint32_t i = 0; i < m_items; ++i) {
		YuvFrm_hPtr p( new YuvFrm_h(m_w, m_h, 0) );
		p->fn_ = i;
		m_q.push_back( p );
	}
	m_v.resize(m_items, 0);
	m_headW = 0;
	m_headR = 0;
	cout << "YuvCircularQ_h::allocQ done!" << endl;
}

bool YuvCircularQ_h::wrt(const uint8_t *buf, const uint32_t sz, const uint64_t fn)	//host interface
{
	//dumpLog("YuvCircularQ_h::wrt(), m_w=%d, m_h=%d, m_items=%d, m_name=%s", m_w, m_h, m_items, m_name.c_str());
	bool sucWrt = false;
	{
		boost::mutex::scoped_lock lock(m_mutexRW);
		uint32_t &idx = m_headW;
		int   &cnt = m_v[idx];
		if (cnt == 0) {
			YuvFrm_h  *dst = m_q[idx].get();
			myAssert( sz == dst->sz_, "YuvCircularQ_h::wrt(): B- size does not match!");
			dst->hdCopyFromBuf(buf, sz, fn);	  //hard copy
			cnt = cnt + 1;

			//move head to the next slot
			++idx;
			if (idx >= m_items) {
				idx = 0;
			}
			sucWrt = true;
		}
	}

	if (!sucWrt) {
		m_wrtDropCnt++;
		if (m_wrtDropCnt > 999) {
			dumpLog("YuvCircularQ_h::wrt(): writen is too fast, %d frames droped--%s", m_wrtDropCnt, m_name.c_str() );
			m_wrtDropCnt = 0;
		}
	}
	return sucWrt;
}

//wrt from host
bool YuvCircularQ_h::wrt(const YuvFrm_h *src)
{
	return wrt(src->v_.data(), src->sz_, src->fn_);
}

//wrt from dev
#if APP_USE_CUDA
bool YuvCircularQ_h::wrt(const YuvFrm_d *src)
{
	bool sucWrt = false;
	{
		boost::mutex::scoped_lock lock(m_mutexRW);
		uint32_t &idx = m_headW;
		int   &cnt = m_v[idx];
		if (cnt == 0) {
			YuvFrm_h  *dst = m_q[idx].get();			
			myAssert(dst->w_ == src->size_.width && dst->h_ == src->size_.height, "YuvCircularQ_h::wrt(): B- size does not match!" );
			src->hdCopyTo( dst );
			cnt = cnt + 1;
			++idx;
			if (idx >= m_items) {
				idx = 0;
			}
			sucWrt = true;
		}
	}

	if (!sucWrt) {
		m_wrtDropCnt++;
		if (m_wrtDropCnt > 999) {
			dumpLog("YuvCircularQ_h::wrtNext(): writen is too fast, %d frames droped -- %s", m_wrtDropCnt, m_name.c_str());
			m_wrtDropCnt = 0;
		}
	}

	return sucWrt;
}
#endif

//the consumer read  <x> from que
bool YuvCircularQ_h::read( YuvFrm_h *dst )
{
	bool hasData = false;
	{
		//this lock moved to readNext() for specific RawFrmXYZ Type
		boost::mutex::scoped_lock lock(m_mutexRW);
		uint32_t &idx = m_headR;
		int   &cnt = m_v[idx];
		if (cnt > 0) {
			YuvFrm_h  *src = m_q[idx].get();			
			myAssert(src->sz_ == dst->sz_, "YuvCircularQ_h::read(): A- size does not match!" );
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

#if APP_USE_CUDA
bool YuvCircularQ_h::read(YuvFrm_d *dst)
{
	bool hasData = false;
	{
		//this lock moved to readNext() for specific RawFrmXYZ Type
		boost::mutex::scoped_lock lock(m_mutexRW);
		uint32_t &idx = m_headR;
		int   &cnt = m_v[idx];
		if (cnt > 0) {
			const YuvFrm_h  *src = m_q[idx].get();			
			myAssert(src->w_ == dst->size_.width && src->h_ == dst->size_.height, "YuvCircularQ_h::read(): B- size does not match!" );
			dst->hdCopyFrom( src ); //only copy image data

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
#endif

