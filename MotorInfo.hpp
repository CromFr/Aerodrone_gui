#ifndef MOTORINFO_HPP_INCLUDED
#define MOTORINFO_HPP_INCLUDED


#include <iostream>
#include <sstream>
using namespace std;

#include <QtGui>
#include "misc.h"

class MotorInfo : public QWidget
{

public:
	MotorInfo(QWidget* parent, int MotorID): QWidget(parent),
		m_lvlMotorID(("Moteur N\xBA <strong>"+string(ToChar(MotorID))+"</strong>").c_str(), this),
		m_lblSpeed("Speed :", this), m_lblSpeedCont(this),
		m_probarSpeed(this)
	{
		m_probarSpeed.setOrientation(Qt::Vertical);
		m_probarSpeed.setGeometry(0, 0, 15, 180);
		m_probarSpeed.setTextVisible(false);

		m_lvlMotorID.setGeometry(25, 0, 155, 25);
		m_lvlMotorID.setAlignment(Qt::AlignCenter | Qt::AlignVCenter);
		m_lblSpeed.setGeometry(25, 35, 70, 25);
		m_lblSpeedCont.setGeometry(95, 35, 85, 25);
		m_lblSpeedCont.setAlignment(Qt::AlignCenter | Qt::AlignVCenter);

		SetMotorSpeed(50.0);
		show();
	}

	/**
	@brief Change the displayed speed value
	**/
	void SetMotorSpeed(float f)
	{
		m_lblSpeedCont.setText((ToString<float>(((int)(f*100))/100.0)+"%").c_str());
		m_probarSpeed.setValue(f);
		m_probarSpeed.repaint();
	}




private:
	QLabel m_lvlMotorID, m_lblSpeed, m_lblSpeedCont;
	QProgressBar m_probarSpeed;

};


#endif // MOTORINFO_HPP_INCLUDED
