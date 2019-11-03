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
