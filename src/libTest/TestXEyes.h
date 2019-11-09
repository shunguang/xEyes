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
#ifndef __TEST_XEYES_H__
#define __TEST_XEYES_H__

#include "libCap/RunCapCamA.h"
#include "libDet/RunDetBkgChg.h"

#include "TestGui.h"

namespace xeyes {
	class TestXEyes : public TestGui
	{
		Q_OBJECT

	public:
		//all the threads share the same cfg located at only one physical address
		TestXEyes(CfgPtr& cfg, QWidget* parent = 0);
		virtual ~TestXEyes();

	protected slots:
		virtual void on_actionExit_triggered();

	protected:
		void runAllThreads();
		void startCaptureThreads();
		void startDetectionThreads();
		void startDisplayThreads();

		void createCaptureThreads();
		void createDetectionThreads();
		void createDisplayThreads();

		void quitAllThreads();

	protected:
		std::vector<RunCapBasePtr>	m_vRunCaps;	//caprture thread
		std::vector<RunDetBasePtr>	m_vRunDets;	//detection threads

		std::vector<int> m_vCamIds;             //camera IDs, cannot be changed after luanched
		int				 m_threadIdCnt;

	};
}
#endif 

