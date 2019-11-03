/*
*-------------------------------------------------------------------
* TestGui.h
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
#ifndef __TEST_GUI_H__
#define __TEST_GUI_H__

#include "libUtil/util.h"
#include "libCfg/Cfg.h"

#include "libGui/QtHeaders.h"
#include "libGui/GuiUtil.h"
#include "libGui/AppGui.h"
#include "libGui/MsgBox.h"
#include "libGui/ProgDialog.h"

#include "libDs/DcUI.h"

namespace xeyes {
	class TestGui : public QMainWindow
	{
		Q_OBJECT

	public:
		//all the threads share the same cfg located at only one physical address
		TestGui(CfgPtr& cfg, QWidget* parent = 0);
		~TestGui();

	public slots:
		void on_actionExit_triggered();
		void on_actionHelp_triggered();
		void on_actionAbout_triggered();

	private:
		virtual void closeEvent(QCloseEvent* event);

	private:
		CfgPtr	m_cfg;                   //accessed by multi-threads
		DcUIPtr	m_dcUI;

		bool		m_guiReady;
		std::shared_ptr<AppGui> m_ui;

		boost::mutex m_runGuiMutex;

	};
}
#endif // CLIENTGUI_H

