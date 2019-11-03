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
	if (m_mainWin->objectName().isEmpty()) {
		m_mainWin->setObjectName(QStringLiteral("mainWin"));
	}

	m_mainWin->setAnimated(false);

	//set central Widget
	m_centralWidget = new QWidget(m_mainWin);
	m_centralWidget->setObjectName(QStringLiteral("m_centralWidget"));
	m_mainWin->setCentralWidget(m_centralWidget);

	setupMenu();
	setupGroupBoxs();
	setupGuiTexts();

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
		name = QStringLiteral("ImgFrmCam%1").arg(i);
		m_vLabelImg[i]->setObjectName(name);
		m_vLabelImg[i]->setEnabled(true);
	}

	//set widgets in control group box
	QGroupBox* pGrpBoxCtrl = m_vGrpBox[GRP_BOX_CTRL];
	for (int i = 0; i < NUM_OF_CAMS; i++) {
		name = QStringLiteral("CamId%1").arg(i);
		m_vLabelCamId[i] = new QLabel(pGrpBoxCtrl);
		m_vLabelCamId[i]->setObjectName(name);
		m_vLabelCamId[i]->setGeometry(0, 0, 0, 0);

		name = QStringLiteral("CamIp%1").arg(i);
		m_vLabelCamIp[i] = new QLabel(pGrpBoxCtrl);
		m_vLabelCamIp[i]->setObjectName(name);
		m_vLabelCamIp[i]->setGeometry(0, 0, 0, 0);

		name = QStringLiteral("CamName%1").arg(i);
		m_vLineEditCamName[i] = new QLineEdit(pGrpBoxCtrl);
		m_vLineEditCamName[i]->setObjectName(name);
		m_vLineEditCamName[i]->setGeometry(0, 0, 0, 0);

		name = QStringLiteral("CamRec%1").arg(i); //recording
		m_vChkBoxCamRec[i] = new QCheckBox(pGrpBoxCtrl);
		m_vChkBoxCamRec[i]->setObjectName(name);
		m_vChkBoxCamRec[i]->setGeometry(0, 0, 0, 0);

		name = QStringLiteral("CamDisp%1").arg(i);
		m_vChkBoxCamDisp[i] = new QCheckBox(pGrpBoxCtrl);
		m_vChkBoxCamDisp[i]->setObjectName(name);
		m_vChkBoxCamDisp[i]->setGeometry(0, 0, 0, 0);

		name = QStringLiteral("ConnectionStatus%1").arg(i);
		m_vConnectStatus[i] = new QLabel(pGrpBoxCtrl);
		m_vConnectStatus[i]->setObjectName(name);
		m_vConnectStatus[i]->setGeometry(0, 0, 0, 0);
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
	m_labelLogo->setScaledContents(true);
	m_labelLogo->setAlignment(Qt::AlignCenter);

	for (int i = 0; i < 5; i++) {
		name = QStringLiteral("TableTitle%1").arg(i);
		m_vTabTitle[i] = new QLabel(pGrpBoxCtrl);
		m_vTabTitle[i]->setObjectName(name);
		m_vTabTitle[i]->setGeometry(0, 0, 0, 0);
	}
}

void AppGui::initSettings()
{
	int initL = m_lv.dispPyrLev_;
	int w0 = m_lv.imgSz_L0_.w;
	int h0 = m_lv.imgSz_L0_.h;
	for (const auto &L : m_lv.vDispPyrLev_) {
		QString s = QStringLiteral("%1 x %2").arg(w0 >> L).arg(h0 >> L);
		m_comboBoxDspCamImgSz->addItem(s);
	}
	QString txt;
	for (int i = 0; i < NUM_OF_CAMS; i++) {
		txt = QStringLiteral("Camera %1:").arg(i);
		m_vLabelCamId[i]->setText(txt);

		txt = QStringLiteral("192.168.10.%1").arg(i);
		m_vLabelCamIp[i]->setText(txt);

		txt = QStringLiteral("Input Camera Name %1").arg(i);
		m_vLineEditCamName[i]->setText(txt);

		m_vConnectStatus[i]->setPixmap(m_vQPixmap[RES_IMG_RED_BOX]);
	}

	m_comboBoxDspCamImgSz->setCurrentIndex(initL);
	m_labelLogo->setPixmap(m_vQPixmap[RES_IMG_LOGO].scaled( m_lv.logoSz_.w, m_lv.logoSz_.h, Qt::IgnoreAspectRatio) );

	m_vTabTitle[0]->setText("IP");
	m_vTabTitle[1]->setText("Name");
	m_vTabTitle[2]->setText("Rec");
	m_vTabTitle[3]->setText("Disp");
	m_vTabTitle[4]->setText("Status");
}



void AppGui::setupMenu()
{
	//menu actions
	m_actionExit = new QAction(m_mainWin);
	m_actionExit->setObjectName(QStringLiteral("m_actionExit"));
	m_actionExit->setCheckable(false);
	QIcon icon;
	icon.addFile(QStringLiteral(":/libGui/Resources/exit_icon.jpg"), QSize(), QIcon::Normal, QIcon::Off);
	m_actionExit->setIcon(icon);

	m_actionAbout = new QAction(m_mainWin);
	m_actionAbout->setObjectName(QStringLiteral("m_actionAbout"));
	m_actionAbout->setCheckable(true);

	m_actionHelp = new QAction(m_mainWin);
	m_actionHelp->setObjectName(QStringLiteral("m_actionHelp"));
	m_actionHelp->setCheckable(true);

	m_actionPreviousSz = new QAction(m_mainWin);
	m_actionPreviousSz->setObjectName(QStringLiteral("m_actionPreviousSz"));
	m_actionPreviousSz->setCheckable(true);

	//menu bar
	m_menuBar = new QMenuBar(m_mainWin);
	m_menuBar->setObjectName(QStringLiteral("m_menuBar"));
	m_menuBar->setNativeMenuBar(false);

	//File
	m_menuFile = new QMenu(m_menuBar);
	m_menuFile->setObjectName(QStringLiteral("m_menuFile"));

	//Help
	m_menuHelp = new QMenu(m_menuBar);
	m_menuHelp->setObjectName(QStringLiteral("m_menuHelp"));

	m_mainWin->setMenuBar(m_menuBar);
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

void AppGui::setupGuiTexts()
{
	m_mainWin->setWindowTitle(QApplication::translate("mainWin", "XEyes Security System (v1.0)", 0));
	m_actionExit->setText(QApplication::translate("mainWin", "E&xit", 0));
	m_actionAbout->setText(QApplication::translate("mainWin", "A&bout", 0));
	m_actionHelp->setText(QApplication::translate("mainWin", "H&elp", 0));
	m_actionPreviousSz->setText(QApplication::translate("mainWin", "P&revious Size", 0));

	m_menuFile->setTitle(QApplication::translate("mainWin", "&File", 0));
	m_menuHelp->setTitle(QApplication::translate("mainWin", "&Help", 0));

	m_pushButtonExit->setText(QApplication::translate("mainWin", "Exit", 0));
	m_labelDspCamImgSz->setText(QApplication::translate("mainWin", "Image Size", 0));
}

void AppGui::resizeCtrlPanel()
{
	const int x00 = m_lv.logoSz_.w + 20;
	int x=0, y=0, h=17;
	int b = 5;
	int w1 = 60, w2=100, w3=150, w4 = 25, w5 = 25, w6=10;

	m_labelLogo->setGeometry(x, y, m_lv.logoSz_.w, m_lv.logoSz_.h);

	y = 0; x = x00 + w1 + b;
	m_vTabTitle[0]->setGeometry(x, y, w2, h);
	x += w2 + b; m_vTabTitle[1]->setGeometry(x, y, w3, h);
	x += w3 + b; m_vTabTitle[2]->setGeometry(x, y, w4, h);
	x += w4 + b; m_vTabTitle[3]->setGeometry(x, y, w5, h);
	x += w5 + b; m_vTabTitle[4]->setGeometry(x, y, 30, h);

	y += h + b/2;
	for (int i = 0; i < m_lv.nNumOfCams_; i++){
		x = x00;
		m_vLabelCamId[i]->setGeometry(x,y,w1,h);
		
		x += w1 + b;
		m_vLabelCamIp[i]->setGeometry(x, y, w2, h);
		
		x += w2 + b;
		m_vLineEditCamName[i]->setGeometry(x, y, w3, h);

		x += w3 + b;
		m_vChkBoxCamRec[i]->setGeometry(x, y, w4, h);

		x += w4 + b;
		m_vChkBoxCamDisp[i]->setGeometry(x, y, w5, h);

		x += w5 + b;
		m_vConnectStatus[i]->setGeometry(x, y, w6, h);

		//------ for next line ------------
		y += h + b;
	}

	const int x0 = x + w6 + 40;
	y = 0;
	w1 = 60; w2 = 85;
	x = x0;  	
	m_labelDspCamImgSz->setGeometry(x, y, w1, h);
	
	x += w1 + b;
	m_comboBoxDspCamImgSz->setGeometry(x, y, w2, h);
	
	y += 2*h + b;
	m_pushButtonExit->setGeometry(x0, y, w1+w2+b, 2*h);
}

//need to update <m_lv> before calling this function
void AppGui::resizeGuiWin()
{
	ImgSize szGui = m_lv.getGuiWinSz();
	ImgSize szImg = m_lv.getDispImgSz();
	int b = 2;
	int x = b, y = b;
	int n = m_lv.nNumOfCams_;


	m_mainWin->setGeometry( 10, 25, szGui.w, szGui.h );
	m_mainWin->setMaximumSize(szGui.w, szGui.h);
	m_mainWin->resize(szGui.w, szGui.h);

	m_centralWidget->setGeometry(b, b, szGui.w-2*b, szGui.h-2*b);
	m_menuBar->setGeometry(QRect(b, b, szGui.w-2*b, 21));

	m_vGrpBox[GRP_BOX_CAM0]->setGeometry(x, y, szImg.w, szImg.h);
	if ( n>= 2) {
		x = szImg.w + 2*b;
		m_vGrpBox[GRP_BOX_CAM1]->setGeometry(x, y, szImg.w, szImg.h);
	}
	if ( n >= 3) {
		x = b; y = szImg.h + 2*b;
		m_vGrpBox[GRP_BOX_CAM2]->setGeometry(x, y, szImg.w, szImg.h);
	}
	if ( n >= 4) {
		x = szImg.w + 2*b;
		m_vGrpBox[GRP_BOX_CAM3]->setGeometry(x, y, szImg.w, szImg.h);
	}

	b = 5;
	x = 0; y += szImg.h + b;
	m_vGrpBox[GRP_BOX_CTRL]->setGeometry(x, y, szGui.w, m_lv.logoSz_.h);

	//show init image
	for (int i = 0; i < n; i++){
		m_vLabelImg[i]->setPixmap( m_vQPixmap[RES_IMG_INIT].scaled( szImg.w, szImg.h, Qt::IgnoreAspectRatio) );
	}
}

void AppGui::showImg(const int camId, const QPixmap &x)
{
	m_vLabelImg[camId]->setPixmap(x);
}
