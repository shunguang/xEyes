/*
*------------------------------------------------------------------------
*AppGui.h
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
/********************************************************************************
** This file is modified from the form generated by reading UI file 'mygui.ui'
**
**  Steps on How to modify this class
**
** (1) menu bar ->QT5->launch designer -> open clientgui.ui
** (2) use QT gui designed to edit the GUI
** (3) during building process, QT will re-write ui_clientgui.h 
**     (note ui_clientgui.h  is exclued by the project, and you need to put Q_OBJECT in RunGui.h)
** (4) copy and past the relative stuff from ui_clientgui.h to this AppGui.h
********************************************************************************/

#ifndef __APP_GUI_H__
#define __APP_GUI_H__

#include "libUtil/AppEnums.h"
#include "libCfg/Cfg.h"

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTableView>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QCheckBox>

#include "ImgLabel.h"
#include "PlayerControls.h"

#include "GuiDefs.h"

inline void initMyResource() { Q_INIT_RESOURCE(appGuiRc); }

namespace xeyes {
	enum GrpBox{
		//----------do not chg order beg -----	
		GRP_BOX_IMG_WIN0 = 0,
		GRP_BOX_IMG_WIN1,
		GRP_BOX_IMG_WIN2,
		GRP_BOX_IMG_WIN3,
		//----------do not chg order end -----	
		GRP_BOX_LOGO,    //left 
		GRP_BOX_CTRL_M,  //middle
		GRP_BOX_CTRL_R,  //right
		GRP_BOX_CUNT,
	};
	static std::vector<std::string> g_grpBoxName = {"imgWin0", "imgWin1", "imgWin2", "imgWin3", "logo", "ctrlM", "ctrlR" };

	enum ResImg {
		RES_IMG_RED_BOX = 0,
		RES_IMG_GRN_BOX,
		RES_IMG_YEL_BOX,
		RES_IMG_GRY_BOX,
		RES_IMG_LOGO,
		RES_IMG_INIT,
		RES_IMG_DEFAULT,
		RES_IMG_CUNT
	};

	#define NUM_OF_CAMS 4

	class AppGui : public QWidget
	{
	public:
		AppGui();
		void setupUi(QMainWindow *mainWin, CfgPtr &cfg);
		void showImg( const int camId, const QPixmap &x );

		const QRect &getGuiRect();

		void resetGui();

		void setStartExitState(const char s) {
			m_startExitState = s;
			m_pushButtonStartExit->setText("Exit");
		}
		char getStartExitState() const {
			return m_startExitState;
		}

		int dispIdx2PyrL( int idx ) {
			int L = idx;
			return L;
		}
		int pyrL2dispIdx(int L) {
			int idx = L;
			return idx;
		}

		void setDispIdx(int L) {
			int idx = pyrL2dispIdx(L);
			m_comboBoxDspCamImgSz->setCurrentIndex(idx);
		}
	private:
		void setupMenu();
		void setupGuiTexts();
		void initSettings();
		void setupGroupBoxs();
		void resizeGuiWin();
		void resizeLogoAndCtrlPanel();

	public:
		//main window and menu bar
		QMainWindow *m_mainWin;
		QWidget		*m_centralWidget;
		QMenuBar	*m_menuBar;
		QMenu		*m_menuFile;
		QMenu		*m_menuHelp;
		QAction		*m_actionExit;
		QAction		*m_actionHelp;
		QAction		*m_actionDecreaseSz;
		QAction		*m_actionAbout;

		QGroupBox	*m_vGrpBox[GRP_BOX_CUNT];

		//widgets in GRP_BOX_CTRL
		QLabel		*m_vLabelCamId[NUM_OF_CAMS];
		QLabel		*m_vLabelCamIp[NUM_OF_CAMS];
		QLineEdit	*m_vLineEditCamName[NUM_OF_CAMS];
		QCheckBox	*m_vChkBoxCamRec[NUM_OF_CAMS];
		QCheckBox	*m_vChkBoxCamDisp[NUM_OF_CAMS];
		QLabel* m_vConnectStatus[NUM_OF_CAMS];
		QLabel* m_vTabTitle[5];

		ImgLabel  *m_vLabelImg[NUM_OF_CAMS];

		QLabel	    *m_labelLogo;
		QLabel	    *m_labelDspCamImgSz;
		QComboBox   *m_comboBoxDspCamImgSz;
		QPushButton *m_pushButtonStartExit;
	private:
		CfgPtr			m_cfg;		//shared data pointer
		CfgLocalView	m_lv;		//a hard cp from m_cfg, only used in gui thread

		QPixmap         m_vQPixmap[RES_IMG_CUNT];

		QRect			m_rectMainWin;
		QRect			m_vRectGrpBox[GRP_BOX_CUNT];
		char			m_startExitState;
	};

} // namespace Ui

#endif
