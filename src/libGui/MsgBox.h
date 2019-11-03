#ifndef __MSG_BOX_H__
#define __MSG_BOX_H__

//#include "libUtil/DataTypes.h"
#include "QtHeaders.h"
#include "GuiDefs.h"

namespace xeyes {
	//auto close message box
	class MsgBox : public QMessageBox
	{
		Q_OBJECT
	public:
		MsgBox(const std::string &winTitle, const std::string &msg, const int timeout_ms, QRect *rect = NULL);
		~MsgBox();

		bool isClosed() {
			return m_isClosed;
		}

		public slots:
		void autoClose()
		{
			this->done(0);
			m_isClosed = true;
		}

		//-------------------------------
	private:
		QTimer* m_timer;
		bool m_isClosed;
	};

	//always on top, non-auto close, block other application on GUI until <OK> is pressed
	void GUI_EXPORT popupMsg(QWidget *parent,  const std::string &winTitle, const std::string &msg, const QMessageBox::Icon myIcon = QMessageBox::Critical);

	//popup msg automaticlly closing after <timeout_ms>
	void GUI_EXPORT popupInfoMsg(const std::string &winTitle, const std::string &msg, const uint32_t timeout_ms = 0, QRect *placeRect = NULL);

}
#endif