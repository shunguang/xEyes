/*
*------------------------------------------------------------------------
*CfgLocalView.h
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
		ImgSize getCentralWidgetSz() const;
	public:
		const int	minGuiWinW_;     //minimum gui window width 
		const int	minCtrlGrpH_;  //minimum gui window width 
		const int	minCtrlGrpW_M;      //w of middle control grp
		const int	minCtrlGrpW_R;      //r of right control  grp

		ImgSize     logoSz_;

		ImgSize		maxGuiWinSz_;     //maximu whole size of the gui windows

		int 		dispQueSz_;      //disp frm que size
		int 		dispPyrLev_;     //disp pyramid level		
		int 		maxPyrLev_;      //disp pyramid level		
		int			nNumOfCams_;

		ImgSize			 imgSz_L0_;	         //disp single camera image sz
		std::vector<int> vDispPyrLev_ = {0,1,2,3};
	};
	typedef std::shared_ptr<CfgLocalView>		CfgLocalViewPtr;
}
#endif
