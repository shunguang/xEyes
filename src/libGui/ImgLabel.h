/*
*------------------------------------------------------------------------
*ImgLabel.h
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
#ifndef _IMG_LABEL_H_
#define _IMG_LABEL_H_
#include <QtCore/QtCore>
#include <QtGui/QtGui>
#include <QtWidgets/QLabel>
#include <QtWidgets/QWidget>

namespace xeyes {
	class ImgLabel : public QLabel{
		Q_OBJECT

	public:
		ImgLabel(QWidget *parent = 0);


		//bool saveImage(const QString &fileName, const char *fileFormat);
		void resetNewFrm();
		void setDrawEnabled(const bool isEnabled);
		void setPenColor(const QColor &newColor);
		void setPenWidth(int newWidth);
		void setInitRect(const QRect &r) { QRect lastRect = r; }

		QColor penColor() const			{ return myPenColor; }
		int penWidth() const			{ return myPenWidth; }
		const QRect &getSelectedRect() const { return lastRect; }

	signals:
		void sigLeftButtonReleased(const QPoint &RoiCenter);
		void sigLeftButtonPressed();

	protected:
		void mousePressEvent(QMouseEvent *event) override;
		void mouseMoveEvent(QMouseEvent *event) override;
		void mouseReleaseEvent(QMouseEvent *event) override;
		//void paintEvent(QPaintEvent *event) override;

	private:
		void	drawRect1();
		void	drawRect2();

		bool    drawDisabled;  //do not change to <drawEnabled>
		bool	isStarted;
		bool	isMoved;
		int		myPenWidth;
		QColor	myPenColor;
		QPoint	begPoint;
		QPoint  endPoint;
		QRect   lastRect;
	};
}
#endif
