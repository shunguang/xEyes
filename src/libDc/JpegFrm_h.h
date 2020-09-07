/*
*------------------------------------------------------------------------
*JpegFrm_h.h
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
#ifndef __JPEG_FRM_H_H__
#define __JPEG_FRM_H_H__
#include "libUtil/util.h"
#include "DsDefs.h"
namespace xeyes {
	//hold jpeg frm in host memory
	class DS_EXPORT JpegFrm_h{
	public:
		static uint64_t    getJpgImgNum(const std::string &nameWoFolder, const std::string &filePrefixTag = "");
		static std::string getJpgFileName(const std::string &folder, const std::string &filePrefixTag = "", const uint64_t fn = 0);
		static void        dumpJpg(const std::string &folder, const std::string &filePrefixTag, const uint64_t fn, const uint8_t *buf, const uint32_t sz);

	public:
		JpegFrm_h( const uint32_t  sz,  const uint64_t fn=0);						//sz = w*h
		JpegFrm_h( const JpegFrm_h &x );
		~JpegFrm_h()=default;

		//hard copy
		JpegFrm_h& operator = (const JpegFrm_h &x);

		//hard copy
		void hdCpyFrom(const uint8_t *buf, const uint32_t bufSz, const uint64_t fn);

		//hard copy
		void hdCpyTo( JpegFrm_h *des ) {
			*des = *this;
		}

		void hdCpyFrom(const JpegFrm_h *src) {
			*this = *src;
		}

		void	dump(const std::string &folder, const std::string &filePrefixTag="" );

	private:
		void creatBuf();
		void deleteBuf();

	public:
		uint64_t  fn_;					//frm # in original video for debug purpose
		uint32_t  sz_;					//actual data size
		std::vector<uint8_t>   v_;
	};
	typedef std::shared_ptr<JpegFrm_h>		JpegFrm_hPtr;
}
#endif
