#include "MsgBox.h"

using namespace std;
using namespace xeyes;

MsgBox::MsgBox(const std::string &winTitle, const std::string &msg, const int timeout_ms, QRect *rect)
	: QMessageBox()
	, m_timer(NULL)
	, m_isClosed(false)
{
	m_timer = new QTimer(this);
	this->setModal(true);
	this->setWindowTitle(QString::fromStdString(winTitle));
	this->setIcon(QMessageBox::Information);
	this->setStandardButtons(QMessageBox::Ok);
	this->setAttribute(Qt::WA_DeleteOnClose, true);
	this->setWindowFlags(Qt::WindowStaysOnTopHint);
	if (rect) {
		this->setGeometry(*rect);
	}
	this->setText(QString::fromStdString(msg));

	QObject::connect(m_timer, SIGNAL(timeout()), this, SLOT(autoClose()));

	m_timer->start(timeout_ms);
}

MsgBox::~MsgBox()
{
	if (m_timer) {
		delete m_timer;
	}
}

//always on top, non-auto close, block other application on GUI until <OK> is pressed
//always on top, non-auto close, block other application on GUI until <OK> is pressed
void xeyes::popupMsg(QWidget *parent, const std::string &winTitle, const std::string &msg, const QMessageBox::Icon myIcon)
{
	//todo: why the window no boder and title??

	QMessageBox *msgBox = new QMessageBox(myIcon,
		QString::fromStdString(winTitle),
		QString::fromStdString(msg),
		QMessageBox::Ok,
		parent, Qt::Dialog);

	msgBox->setWindowModality(Qt::ApplicationModal);
	msgBox->setModal(true);
	msgBox->setWindowFlags(Qt::WindowStaysOnTopHint);
	msgBox->setAttribute(Qt::WA_DeleteOnClose, true);
	//if (parent) {
	//	msgBox->exec(); //block other bapplication on GUI
	//}
	//else {
		msgBox->show(); //cannot block other bapplication on GUI
	//}
	QApplication::processEvents();
}

void xeyes::popupInfoMsg(const std::string &winTitle, const std::string &msg, const uint32_t timeout_ms, QRect *placeRect)
{
	QRect rect(100, 100, 300, 100);
	if (placeRect ) {
		rect = *placeRect;
	}
	MsgBox *x = new MsgBox(winTitle, msg, timeout_ms, &rect);
	x->show();
	QApplication::processEvents();

	//wus1 note: 
	//Do not worry about the delete of <x>, because we have setAttribute(Qt::WA_DeleteOnClose, true) in <MsgBox2>
	//google "QMessageBox delete on close" or see
	//http://stackoverflow.com/questions/26925630/qmessagebox-delete-on-close
}

