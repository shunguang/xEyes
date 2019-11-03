//config for display
#ifndef __CFG_LOCAL_VIEW_H__
#define __CFG_LOCAL_VIEW_H__

#include "CfgBase.h"
namespace xeyes {
	class  CFG_EXPORT CfgLocalView : public CfgBase {
	public:
		CfgLocalView();
		CfgLocalView( const CfgLocalView &x ) = default;
		CfgLocalView& operator = (const CfgLocalView &x);
		virtual ~CfgLocalView()= default;

		virtual boost::property_tree::ptree toPropertyTree();
		virtual void fromPropertyTree(const boost::property_tree::ptree &pt);
		virtual std::string toString();
		
		ImgSize getDispImgSz() const {
			assert(dispPyrLev_ >= 0 && dispPyrLev_ <= 3);
			return imgSz_L0_.getSize(dispPyrLev_);
		}
		ImgSize getGuiWinSz() const;
	public:
		const int	minGuiWinW_;      //minimum gui window width 
		const int   ctrlGrpBoxH_;

		ImgSize		maxGuiWinSz_;     //maximu whole size of the gui windows

		int 		dispQueSz_;      //disp frm que size
		int 		dispPyrLev_;     //disp pyramid level		
		int			nNumOfCams_;

		ImgSize			 imgSz_L0_;	         //disp single camera image sz
		std::vector<int> vDispPyrLev_ = {0,1,2,3};
	};
	typedef std::shared_ptr<CfgLocalView>		CfgLocalViewPtr;
}
#endif
