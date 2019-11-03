#ifndef PROG_DIALOG_H__
#define PROG_DIALOG_H__

#include <chrono>
#include <thread>

#include <QtCore/QString>
#include <QtWidgets/QProgressDialog>
#include <QtWidgets/QApplication>
namespace xeyes {
	class ProgDialog : public QProgressDialog
	{
		Q_OBJECT
	public:
		ProgDialog(const char *winTitle, int x0 = 0, int y0 = 0, int w = 0, int h = 0)
			: QProgressDialog()
		{
			this->setWindowTitle(tr(winTitle));
			this->setCancelButton(0);
			this->setWindowModality(Qt::WindowModal);
			this->setMaximum(100);
			this->setMinimum(0);
			this->setAutoClose(true);
			this->setAutoReset(true);
			this->setWindowFlags(Qt::WindowStaysOnTopHint);

			if (w > 0 && h > 0) {
				this->setGeometry(x0, y0, w, h);
			}
		}


		void resetWinTitle(const char *winTitle)
		{
			this->setWindowTitle(tr(winTitle));
		}

		void resetGeometry(int xc, int yc, int w, int h)
		{
			int x0 = xc - (int)(w / 2);
			int y0 = yc - (int)(h / 2);

			if (x0 < 0) x0 = 1;
			if (y0 < 0) y0 = 1;

			this->setGeometry(x0, y0, w, h);
		}

		void setProgress(const int val, const std::string &msg = "")
		{
			if (this->wasCanceled())
				return;

			if (!msg.empty()) {
				this->setLabelText(QString::fromStdString(msg));
			}
			this->setValue(val);
			QApplication::processEvents();

			if (val == this->maximum()) {
				std::this_thread::sleep_for(std::chrono::milliseconds(200));
				this->cancel();
			}
		}

		void forceCancel()
		{
			if (!this->wasCanceled()) {
				this->cancel();
			}
		}
	};
}
#endif