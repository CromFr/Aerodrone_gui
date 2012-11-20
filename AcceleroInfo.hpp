#ifndef ACCELEROINFO_INCLUDED
#define ACCELEROINFO_INCLUDED

#include <iostream>
#include <sstream>
#include <cmath>
using namespace std;

#include <QtGui>

class AcceleroInfo : public QWidget
{

public:
	AcceleroInfo(QWidget* parent): QWidget(parent),
		m_linePainter(this),
		m_lblX("<span style=\"color:#FF0000\">X</span>", this), m_lblY("<span style=\"color:#00FF00\">Y</span>", this), m_lblZ("<span style=\"color:#0000FF\">Z</span>", this),
		m_lineX(75, 55, 75, 55), m_lineY(75, 55, 75, 55), m_lineZ(75, 55, 75, 55)
	{
		m_lblX.setGeometry(95, 35, 20, 20);
		m_lblY.setGeometry(55, 20, 20, 20);
		m_lblZ.setGeometry(50, 50, 20, 20);

		SetValues(60, 30, 10);

		show();
	}

	/**
	@brief Change the displayed values
	**/
	void SetValues(float x, float y, float z)
	{
		//P1 is fixed to the axis origin (75px,55px)
		m_lineX.setP2(QPoint(75+x*1.85, 55));
		m_lineY.setP2(QPoint(75, 55-y*1.85));
		m_lineZ.setP2(QPoint(75-z*cos(1.f)*1.85, 55+z*sin(1.f)*1.85));
		repaint();
	}

	/**
	@brief Called when the widget is painted
	**/
	void paintEvent(QPaintEvent *e)
	{
		m_linePainter.begin (this);
		m_linePainter.setPen(QColor(255,0,0));
		m_linePainter.drawLine(m_lineX);
		m_linePainter.setPen(QColor(0,255,0));
		m_linePainter.drawLine(m_lineY);
		m_linePainter.setPen(QColor(0,0,255));
		m_linePainter.drawLine(m_lineZ);
		m_linePainter.end();
	}




private:
	QPainter m_linePainter;
	QLabel m_lblX, m_lblY, m_lblZ;
	QLine m_lineX, m_lineY, m_lineZ;
};

#endif // ACCELEROINFO_INCLUDED
