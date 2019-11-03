#include "AppGui.h"

using namespace std;
using namespace xeyes;

AppGui::AppGui()
	: m_cfg(0)
{
	//defined in appGui.qrc
	Q_INIT_RESOURCE(appGui);
	//and pre gnerated by coustom-buid tools
	m_vQPixmap[RES_IMG_RED_BOX]	= QPixmap(QString::fromUtf8(":/Resource/red_box.png"));
	m_vQPixmap[RES_IMG_GRN_BOX]	= QPixmap(QString::fromUtf8(":/Resource/green_box.png"));
	m_vQPixmap[RES_IMG_YEL_BOX] = QPixmap(QString::fromUtf8(":/Resource/yellow_box.png"));
	m_vQPixmap[RES_IMG_GRY_BOX] = QPixmap(QString::fromUtf8(":/Resource/gray_box.png"));
	m_vQPixmap[RES_IMG_LOGO]	= QPixmap(QString::fromUtf8(":/Resource/appLogo.png"));
	m_vQPixmap[RES_IMG_INIT]	= QPixmap(QString::fromUtf8(":/Resource/initScreen.jpg"));
	m_vQPixmap[RES_IMG_DEFAULT] = QPixmap(QString::fromUtf8(":/Resource/defaultImg.jpg"));
}

const QRect& AppGui::getGuiRect()
{
	return m_rectMainWin;
}


void AppGui::setupUi(QMainWindow *mainWin, CfgPtr &cfg)
{
	m_mainWin = mainWin;
	m_cfg = cfg;
	m_lv = m_cfg->getLocalView();

	//define main window 
	if (mainWin->objectName().isEmpty()) {
		mainWin->setObjectName(QStringLiteral("mainWin"));
	}

	mainWin->setAnimated(false);

	//set central Widget
	m_centralWidget = new QWidget(mainWin);
	m_centralWidget->setObjectName(QStringLiteral("m_centralWidget"));
	mainWin->setCentralWidget(m_centralWidget);

	setupMenu(mainWin);
	setupGroupBoxs();
	setupGuiTexts(mainWin);

	//after setup
	resizeGuiWin();
	resizeCtrlPanel();
	initSettings();
	QMetaObject::connectSlotsByName(mainWin);
}

void AppGui::setupGroupBoxs()
{
	for (int i = 0; i < GRP_BOX_CUNT; i++) {
		m_vGrpBox[i] = new QGroupBox(m_centralWidget);
		m_vGrpBox[i]->setObjectName(QStringLiteral("groupBox_img"));
	}

	QString name;
	//set display image widgets
	for (int i = 0; i < NUM_OF_CAMS; i++) {
		m_vLabelImg[i] = new ImgLabel(m_vGrpBox[i]);
		m_vLabelImg[i]->setObjectName(name);
		m_vLabelImg[i]->setEnabled(true);
	}

	//set widgets in control group box
	QGroupBox* pGrpBoxCtrl = m_vGrpBox[GRP_BOX_CTRL];
	for (int i = 0; i < NUM_OF_CAMS; i++) {
		m_vLabelCamId[i] = new QLabel(pGrpBoxCtrl);
		m_vLabelCamId[i]->setObjectName(name);

		m_vLabelCamIp[i] = new QLabel(pGrpBoxCtrl);
		m_vLabelCamIp[i]->setObjectName(name);

		m_vLineEditCamName[i] = new QLineEdit(pGrpBoxCtrl);
		m_vLineEditCamName[i]->setObjectName(name);

		m_vChkBoxCamRec[i] = new QCheckBox(pGrpBoxCtrl);
		m_vChkBoxCamRec[i]->setObjectName(name);

		m_vChkBoxCamDisp[i] = new QCheckBox(pGrpBoxCtrl);
		m_vChkBoxCamDisp[i]->setObjectName(name);

	}
	m_pushButtonExit = new QPushButton(pGrpBoxCtrl);
	m_pushButtonExit->setObjectName(QString("m_pushButtonExit"));

	m_labelDspCamImgSz = new QLabel(pGrpBoxCtrl);
	m_comboBoxDspCamImgSz = new QComboBox(pGrpBoxCtrl);

	m_labelLogo = new QLabel(pGrpBoxCtrl);
	m_labelLogo->setObjectName(QStringLiteral("m_labelLogo"));
	m_labelLogo->setEnabled(true);
	m_labelLogo->setCursor(QCursor(Qt::ArrowCursor));
	m_labelLogo->setAutoFillBackground(true);
	m_labelLogo->setPixmap(m_vQPixmap[RES_IMG_LOGO]);
	m_labelLogo->setScaledContents(true);
	m_labelLogo->setAlignment(Qt::AlignCenter);
}

void AppGui::initSettings()
{
	int initL = m_lv.dispPyrLev_;
	int w0 = m_lv.imgSz_L0_.w;
	int h0 = m_lv.imgSz_L0_.h;
	for (const auto &L : m_lv.vDispPyrLev_) {
		QString s = QStringLiteral("%d x %d").arg(w0 > L).arg(h0 > L);
		m_comboBoxDspCamImgSz->addItem(s);
	}
	m_comboBoxDspCamImgSz->setCurrentIndex(initL);
}



void AppGui::setupMenu(QMainWindow* mainWin)
{
	//menu actions
	m_actionExit = new QAction(mainWin);
	m_actionExit->setObjectName(QStringLiteral("m_actionExit"));
	m_actionExit->setCheckable(false);
	QIcon icon;
	icon.addFile(QStringLiteral(":/libGui/Resources/exit_icon.jpg"), QSize(), QIcon::Normal, QIcon::Off);
	m_actionExit->setIcon(icon);

	m_actionAbout = new QAction(mainWin);
	m_actionAbout->setObjectName(QStringLiteral("m_actionAbout"));
	m_actionAbout->setCheckable(true);

	m_actionHelp = new QAction(mainWin);
	m_actionHelp->setObjectName(QStringLiteral("m_actionHelp"));
	m_actionHelp->setCheckable(true);

	m_actionPreviousSz = new QAction(mainWin);
	m_actionPreviousSz->setObjectName(QStringLiteral("m_actionPreviousSz"));
	m_actionPreviousSz->setCheckable(true);

	//menu bar
	m_menuBar = new QMenuBar(mainWin);
	m_menuBar->setObjectName(QStringLiteral("m_menuBar"));
	m_menuBar->setNativeMenuBar(false);

	//File
	m_menuFile = new QMenu(m_menuBar);
	m_menuFile->setObjectName(QStringLiteral("m_menuFile"));

	//Help
	m_menuHelp = new QMenu(m_menuBar);
	m_menuHelp->setObjectName(QStringLiteral("m_menuHelp"));

	mainWin->setMenuBar(m_menuBar);
	//mainToolBar = new QToolBar(mainWin);
	//mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
	//mainWin->addToolBar(Qt::TopToolBarArea, mainToolBar);
	//statusBar = new QStatusBar(mainWin);
	//statusBar->setObjectName(QStringLiteral("statusBar"));
	//mainWin->setStatusBar(statusBar);

	//add actions to menu bar
	m_menuBar->addAction(m_menuFile->menuAction());
	m_menuBar->addAction(m_menuHelp->menuAction());

	m_menuFile->addAction(m_actionExit);

	m_menuHelp->addAction(m_actionHelp);
	m_menuHelp->addAction(m_actionPreviousSz);
	m_menuHelp->addSeparator();
	m_menuHelp->addAction(m_actionAbout);
}

void AppGui::setupGuiTexts(QMainWindow* mainWin)
{
	mainWin->setWindowTitle(QApplication::translate("mainWin", "XEyes Security System (v1.0)", 0));
	m_actionExit->setText(QApplication::translate("mainWin", "E&xit", 0));
	m_actionAbout->setText(QApplication::translate("mainWin", "A&bout", 0));
	m_actionHelp->setText(QApplication::translate("mainWin", "H&elp", 0));
	m_actionPreviousSz->setText(QApplication::translate("mainWin", "P&revious Size", 0));

	m_menuFile->setTitle(QApplication::translate("mainWin", "&File", 0));
	m_menuHelp->setTitle(QApplication::translate("mainWin", "&Help", 0));

	m_pushButtonExit->setText(QApplication::translate("mainWin", "Exit", 0));
	m_labelDspCamImgSz->setText(QApplication::translate("mainWin", "Single Camera Image Size", 0));
}

void AppGui::resizeCtrlPanel()
{
	int x=0, y=0, h=15;
	int b = 5;
	int w1 = 10, w2=30, w3=80, w4=15;

	int logoW = m_lv.ctrlGrpBoxH_ - 10;
	m_labelLogo->setGeometry(x, y, logoW, logoW);

	for (int i = 0; i < m_lv.nNumOfCams_; i++){
		x = logoW + b;
		m_vLabelCamId[i]->setGeometry(x,y,w1,h);
		
		x += w1 + b;
		m_vLabelCamIp[i]->setGeometry(x, y, w2, h);
		
		x += w2 + b;
		m_vLineEditCamName[i]->setGeometry(x, y, w3, h);

		x += w3 + b;
		m_vChkBoxCamRec[i]->setGeometry(x, y, w4, h);

		x += w4 + b;
		m_vChkBoxCamDisp[i]->setGeometry(x, y, w4, h);

		//------ for next line ------------
		y += h + b;
	}

	int x0 = x + w4 + b;
	int y0 = y;           //store last line

	w4 = 50;
	y = 0;
	m_labelDspCamImgSz->setGeometry(x0, y, w4, h);

	y += h + b;
	m_comboBoxDspCamImgSz->setGeometry(x0, y, w4, h);

	m_pushButtonExit->setGeometry(x0, y0-h-b, w2, h);
}

//need to update <m_lv> before calling this function
void AppGui::resizeGuiWin()
{
	ImgSize szGui = m_lv.getGuiWinSz();
	ImgSize szImg = m_lv.getDispImgSz();
	int b = 5;
	int x = 0, y = 0;
	int n = m_lv.nNumOfCams_;
	m_vGrpBox[GRP_BOX_CAM0]->setGeometry(x, y, szImg.w, szImg.h);
	if ( n>= 2) {
		x = szImg.w + b;
		m_vGrpBox[GRP_BOX_CAM1]->setGeometry(x, y, szImg.w, szImg.h);
	}
	if ( n >= 3) {
		x = 0; y = szImg.h + b;
		m_vGrpBox[GRP_BOX_CAM2]->setGeometry(x, y, szImg.w, szImg.h);
	}
	if ( n >= 4) {
		x = szImg.w + b;
		m_vGrpBox[GRP_BOX_CAM3]->setGeometry(x, y, szImg.w, szImg.h);
	}

	x = 0; y += szImg.h + b;
	m_vGrpBox[GRP_BOX_CTRL]->setGeometry(x, y, szGui.w, m_lv.ctrlGrpBoxH_);

	//show init image
	for (int i = 0; i < n; i++){
		m_vLabelImg[i]->setPixmap( m_vQPixmap[RES_IMG_INIT].scaled( szImg.w, szImg.h, Qt::IgnoreAspectRatio) );
	}
}

void AppGui::showImg(const int camId, const QPixmap &x)
{
	m_vLabelImg[camId]->setPixmap(x);
}
