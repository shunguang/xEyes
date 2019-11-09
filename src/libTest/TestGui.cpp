/*
*-------------------------------------------------------------------
* TestGui.cpp
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
#include "TestGui.h"
using namespace std;
using namespace xeyes;
#define POPUP_MSG_WIN_TITLE "xEyes"
#define MY_QT_CONN		Qt::UniqueConnection
TestGui::TestGui(CfgPtr& cfg, QWidget* parent)
	: m_cfg(cfg)
	, m_dcUI(0)
	, m_guiReady(false)
	, m_ui(new AppGui())
{

	m_dcUI.reset(new DcUI(cfg));

	//--------------------------------------------------------------------
	//create other object
	//--------------------------------------------------------------------

	//start GUI
	m_ui->setupUi(this, m_cfg);

	//Menu item actions
	QObject::connect(m_ui->m_actionExit, SIGNAL(triggered()), this, SLOT(on_actionExit_triggered()), MY_QT_CONN);
	QObject::connect(m_ui->m_actionAbout, SIGNAL(triggered()), this, SLOT(on_actionAbout_triggered()), MY_QT_CONN);
	QObject::connect(m_ui->m_actionDecreaseSz, SIGNAL(triggered()), this, SLOT(on_actionDecreaseDispImgSz_triggered()), MY_QT_CONN);
	QObject::connect(m_ui->m_actionHelp, SIGNAL(triggered()), this, SLOT(on_actionHelp_triggered()), MY_QT_CONN);
	
	QObject::connect(m_ui->m_pushButtonStartExit, SIGNAL(clicked()), this, SLOT(on_pushButton_startExit_clicked()), MY_QT_CONN);
	QObject::connect(m_ui->m_comboBoxDspCamImgSz, SIGNAL(currentIndexChanged(int)), this, SLOT(on_comboBoxDspCamImgSz_currentIndexChanged(int)), MY_QT_CONN);

	QLineEdit	*m_vLineEditCamName[NUM_OF_CAMS];
	QCheckBox	*m_vChkBoxCamRec[NUM_OF_CAMS];
	QCheckBox	*m_vChkBoxCamDisp[NUM_OF_CAMS];

	THREAD_SLEEP(100);
	m_guiReady = true;
}


TestGui::~TestGui()
{
}


void TestGui::on_actionHelp_triggered()
{
	std::vector<std::string> v;

	v.push_back("=== Auto Clip Cut ===");
	v.push_back("line1");
	v.push_back("L2 ...");
	v.push_back("L3 ...");
	v.push_back("L4 ...");
	v.push_back("L5 ...");

	std::string s = "\n";
	for (int i = 0; i < v.size(); ++i) {
		s += v[i] + "\n";
	}

	QMessageBox::information(this, POPUP_MSG_WIN_TITLE, QString::fromStdString(s));

}
void TestGui::on_actionAbout_triggered()
{
	QMessageBox::information(this, POPUP_MSG_WIN_TITLE, "          Auto Clip Cut\n          Shunguang Wu 2018.         ");
}


void TestGui::closeEvent(QCloseEvent* event)
{
	QMessageBox::StandardButton resBtn = QMessageBox::question(this, tr(POPUP_MSG_WIN_TITLE),
		tr("Do you really want to close?\n"),
		QMessageBox::No | QMessageBox::Yes,
		QMessageBox::Yes);

	if (resBtn != QMessageBox::Yes) {
		event->ignore();
	}
	else {
		on_actionExit_triggered();
		//event->accept();
	}
}

void TestGui::on_actionExit_triggered()
{

	std::shared_ptr<ProgDialog> progDlg(new ProgDialog( "Starting ...", 0, 0, 300, 100));
	progDlg->setProgress(50, "Reading configuration file!");

	progDlg->setProgress(100, "Reading configuration file!");

	progDlg->reset();
	progDlg->resize(200, 100);
	progDlg->resetWinTitle("Preparing Quit");

	dumpLog("TestGui::on_actionExit_triggered()--B");

	progDlg->setProgress(50, "Trerminal internal threads ...");

	//clear up 
	//m_runMgr->forceAllWorkersQuit();

	progDlg->setProgress(100, "Successfully clean up!");
	boost::this_thread::sleep(boost::posix_time::milliseconds(100));
	QApplication::quit();
}

void TestGui::on_pushButton_startExit_clicked()
{
	if ( 'S' == m_ui->getStartExitState()) {
		m_ui->setStartExitState('E');
	}
	else {
		on_actionExit_triggered();
	}
}

void TestGui::on_comboBoxDspCamImgSz_currentIndexChanged(int idx)
{
	int newL = m_ui->dispIdx2PyrL(idx);
	m_cfg->updateDispImgPryLevel(newL);
	m_ui->resetGui();
}

void TestGui::on_actionDecreaseDispImgSz_triggered()
{
	int L = m_cfg->increaseDispImgPyrL();
	m_ui->setDispIdx(L);
	m_ui->resetGui();
}

void TestGui::on_checkBox_camRec0_stateChgd(int state)
{
	bool  chked = (state == Qt::Checked);
	m_cfg->updateRecFlag(0, chked);
}
void TestGui::on_checkBox_camRec1_stateChgd(int state)
{
	bool  chked = (state == Qt::Checked);
	m_cfg->updateRecFlag(1, chked);
}
void TestGui::on_checkBox_camRec2_stateChgd(int state)
{
	bool  chked = (state == Qt::Checked);
	m_cfg->updateRecFlag(2, chked);
}
void TestGui::on_checkBox_camRec3_stateChgd(int state)
{
	bool  chked = (state == Qt::Checked);
	m_cfg->updateRecFlag(3, chked);
}

void TestGui::on_checkBox_disp0_stateChgd(int state)
{
	bool  chked = (state == Qt::Checked);
	m_cfg->updateDispFlag(0, chked);
}
void TestGui::on_checkBox_disp1_stateChgd(int state)
{
	bool  chked = (state == Qt::Checked);
	m_cfg->updateDispFlag(1, chked);
}
void TestGui::on_checkBox_disp2_stateChgd(int state)
{
	bool  chked = (state == Qt::Checked);
	m_cfg->updateDispFlag(2, chked);
}
void TestGui::on_checkBox_disp3_stateChgd(int state)
{
	bool  chked = (state == Qt::Checked);
	m_cfg->updateDispFlag(3, chked);
}

void TestGui::on_lineEdit_camName0_edited(const QString &s)
{
	m_cfg->updateCamName(0, s.toStdString() );
}

void TestGui::on_lineEdit_camName1_edited(const QString &s)
{
	m_cfg->updateCamName(1, s.toStdString());
}

void TestGui::on_lineEdit_camName2_edited(const QString &s)
{
	m_cfg->updateCamName(2, s.toStdString());
}

void TestGui::on_lineEdit_camName3_edited(const QString &s)
{
	m_cfg->updateCamName(3, s.toStdString());
}
