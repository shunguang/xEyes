#include "ImgLabel.h"

using namespace std;
using namespace xeyes;

#define IMG_DRAW_RECT 0
ImgLabel::ImgLabel( QWidget *parent)
	: QLabel( parent )
	, drawDisabled( false )
	, isStarted( false )
	, isMoved( false )
	, myPenWidth (1)
	, myPenColor (Qt::blue)
	, begPoint(0, 0)
	, endPoint(0, 0)
#if IMG_DRAW_RECT
	, lastRect(0,0,0,0)
#else
	, lastRect(0, 0, 99999, 9999)
#endif
{
}


void ImgLabel::setDrawEnabled( const bool isEnabled )
{
	drawDisabled = !isEnabled;
}

void ImgLabel::resetNewFrm()
{
	lastRect.setSize(QSize(0, 0));
}

void ImgLabel::setPenColor(const QColor &newColor)
{

}

void ImgLabel::setPenWidth(int newWidth)
{

}

void ImgLabel::mousePressEvent(QMouseEvent *event)
{
	if (drawDisabled) {
		return;
	}

//	qDebug() << "mouse pressed:"<< event->pos().x() << "," << event->pos().y();
	if ( event->button() == Qt::LeftButton) {
		begPoint = event->pos();
		if (lastRect.contains(begPoint)) {
			isStarted = true;
			isMoved = false;
			emit sigLeftButtonPressed();
		}
	}
}

void ImgLabel::mouseMoveEvent(QMouseEvent *event)
{
	//qDebug() << event->pos().x() << "," << event->pos().y();
	if (drawDisabled || !isStarted) {
		return;
	}

	if (event->buttons() & Qt::LeftButton) {
		endPoint = event->pos();
		isMoved = true;
#if IMG_DRAW_RECT
		drawRect2();
#endif
	}
}

void ImgLabel::mouseReleaseEvent(QMouseEvent *event)
{
	//qDebug() << "mouse released:" << event->pos().x() << "," << event->pos().y();
	if (drawDisabled || !isStarted) {
		return;
	}

	if (event->button() == Qt::LeftButton ) {
		endPoint = event->pos();
#if IMG_DRAW_RECT
		drawRect2();
		if ( isMoved ) {
			emit sigLeftButtonReleased(endPoint);
		}
#else
		emit sigLeftButtonReleased(endPoint);
#endif
		isStarted = false;
	}
}

//draw based on <begPoint> and <endPoint>
void ImgLabel::drawRect1()
{
	//QPainter painter(&image);
	int w = abs(begPoint.x() - endPoint.x());
	int h = abs(begPoint.y() - endPoint.y());
	if (w == 0 || h == 0) {
		return;
	}
	int xo = min<int>(begPoint.x(), endPoint.x());  //-left
	int yo = min<int>(begPoint.y(), endPoint.y());  //-top

	QPixmap *p = (QPixmap *)this->pixmap();
	if (p) {
		QPainter painter(p);
		painter.setCompositionMode(QPainter::RasterOp_SourceXorDestination);
		painter.setRenderHint(QPainter::Antialiasing);
		painter.setPen(QColor(0xff, 0xff, 0xff));

		//erase the old one
		if (lastRect.width() > 0) {
			painter.drawRect(lastRect);  
		}
		//update <lastRect>
		lastRect.setRect(xo, yo, w, h);

		//draw it
		painter.drawRect(lastRect);

		update();
	}
}

void ImgLabel::drawRect2()
{
	QPixmap *p = (QPixmap *)this->pixmap();
	if (p) {
		QPainter painter(p);
		painter.setCompositionMode(QPainter::RasterOp_SourceXorDestination);
		painter.setRenderHint(QPainter::Antialiasing);
		painter.setPen(QColor(0xff, 0xff, 0xff));

		//erase the old one
		if (lastRect.width() > 0) {
			painter.drawRect(lastRect);
		}
		//update <lastRect>
		lastRect.moveCenter(endPoint);
		//lastRect.setRect(xo, yo, w, h);

		//draw it
		painter.drawRect(lastRect);

		update();
	}
}

//void ImgLabel::paintEvent(QPaintEvent *event)
//{
	//QPainter painter(this);
	//QRect dirtyRect = event->rect();
	//painter.drawImage(dirtyRect, image, dirtyRect);
//}

