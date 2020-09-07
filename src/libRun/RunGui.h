/*
*------------------------------------------------------------------------
*RunGui.h
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
/*
*-------------------------------------------------------------------
* RunGui.h
* Copyright(c) 2019, The RgbVideoEditor Author (Shunguang Wu).
* All Rights Reserved.
* You may not use this file except in compliance with the License:
*      GNU GENERAL PUBLIC LICENSE (Version 3), 29 June 2007
*
* You may obtain a copy of the License at
*
*     https://www.gnu.org/licenses/gpl-3.0.en.html
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*-------------------------------------------------------------------
*/
#ifndef __RUN_GUI_H__
#define __RUN_GUI_H__

#include "libUtil/util.h"
#include "libCfg/Cfg.h"

#include "libGui/QtHeaders.h"
//#include "libGui/GuiUtil.h"
#include "libGui/AppGui.h"
#include "libGui/MsgBox.h"
#include "libGui/ProgDialog.h"

#include "libDc/DcUI.h"

#define POPUP_MSG_WIN_TITLE "xEyes"
#define MY_QT_CONN		Qt::UniqueConnection

namespace xeyes {
	class RunGui : public QMainWindow
	{
		Q_OBJECT

	public:
		//all the threads share the same cfg located at only one physical address
		RunGui(CfgPtr& cfg, QWidget* parent = 0);
		~RunGui();

	protected slots:
		virtual void on_actionExit_triggered();
		virtual void closeEvent(QCloseEvent* event);
		void on_actionHelp_triggered();
		void on_actionAbout_triggered();
		void on_actionDecreaseDispImgSz_triggered();

		void on_pushButton_startExit_clicked();
		void on_comboBoxDspCamImgSz_currentIndexChanged(int idx);

		void on_checkBox_camRec0_stateChgd(int state);
		void on_checkBox_camRec1_stateChgd(int state);
		void on_checkBox_camRec2_stateChgd(int state);
		void on_checkBox_camRec3_stateChgd(int state);

		void on_checkBox_disp0_stateChgd(int state);
		void on_checkBox_disp1_stateChgd(int state);
		void on_checkBox_disp2_stateChgd(int state);
		void on_checkBox_disp3_stateChgd(int state);

		void on_lineEdit_camName0_edited(const QString &s);
		void on_lineEdit_camName1_edited(const QString &s);
		void on_lineEdit_camName2_edited(const QString &s);
		void on_lineEdit_camName3_edited(const QString &s);

	protected:
		void createQuitDlg();
		void closeQuitDlg();

	protected:
		CfgPtr	m_cfg;                   //accessed by multi-threads
		DcUIPtr	m_dcUI;

		bool		m_guiReady;
		std::shared_ptr<AppGui> m_ui;

		std::shared_ptr<ProgDialog> m_quitProgDlg;

		boost::mutex m_runGuiMutex;
	};
}
#endif 

