/*
*------------------------------------------------------------------------
*MsgBox.h
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
