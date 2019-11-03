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