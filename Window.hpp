#ifndef WINDOW_HPP_INCLUDED
#define WINDOW_HPP_INCLUDED

#include <iostream>
#include <sys/time.h>
#include <cstdint>
using namespace std;

#include <QtGui>
#include <QtNetwork>
#include <QThread>

#include "misc.h"

#include "ConfigFile.hpp"

#include "ConnectionPanel.hpp"
#include "MotorInfo.hpp"
#include "AcceleroInfo.hpp"
#include "UpdateThread.hpp"


#define NET_INFOREQUEST (uint8_t)1
#define NET_INFODATA (uint8_t)2
#define NET_SETGLOBALMOTORSPEED (uint8_t)3
#define NET_RESETINTEGRATOR (uint8_t)4
#define NET_CRITICALLAND (uint8_t)5




class Window : QWidget
{
	Q_OBJECT

public:
	Window() :
		m_sockDev(this),
		m_thread(this, &m_sockDev),
		m_pbutConnect("&Connect device", this),
		m_pbutDisconnect("&Disconnect", this),
		m_lblIp("<em>IP : </em>", this), m_lblPort("<em>Port : </em>", this),
		m_leIp(this), m_lePort(this),
		m_lblAerodrone(this),
		m_sliderGlobalSpeed(this), m_lblGlobalSpeed("0", this),
		m_pbutCritLand("Crit. Error\n&Land", this)
	{
		m_cfg.Load("./config.cfg");

		m_thread.start();
		m_nBlockSize = 0;
		connect(&m_sockDev, SIGNAL(readyRead()), this, SLOT(OnDataReceived()));
		connect(&m_sockDev, SIGNAL(connected()), this, SLOT(OnConnected()));
		connect(&m_sockDev, SIGNAL(disconnected()), this, SLOT(UpdateIPDeviceInfo()));
		connect(&m_sockDev, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(OnConnectionFailed(QAbstractSocket::SocketError)));


		//=================== GUI
		setFixedSize(800, 600);

		//Connectivity bar
		m_pbutConnect.setGeometry(10, 10, 150, 25);

		m_pbutDisconnect.setGeometry(170, 10, 150, 25);
		m_pbutDisconnect.setEnabled(false);

		m_lblIp.setGeometry(370, 10, 50, 25);
		m_lblIp.setAlignment(Qt::AlignRight | Qt::AlignVCenter);
		m_leIp.setGeometry(430, 10, 150, 25);
		m_lblPort.setGeometry(620, 10, 50, 25);
		m_lblPort.setAlignment(Qt::AlignRight | Qt::AlignVCenter);
		m_lePort.setGeometry(680, 10, 80, 25);

		connect(&m_pbutConnect, SIGNAL(clicked()), this, SLOT(Connect()));
		connect(&m_pbutDisconnect, SIGNAL(clicked()), this, SLOT(Disconnect()));

		m_leIp.setText(m_cfg.GetStringValue("device_ip").c_str());
		m_lePort.setText(m_cfg.GetStringValue("device_port").c_str());
		//


		//Aerodrone part
		m_lblAerodrone.setGeometry(200, 75, 400, 400);
		m_lblAerodrone.setPixmap(QPixmap("Aerodrone.png"));

		for(int i=0 ; i<4 ; i++)
			m_motorInfo[i] = new MotorInfo(this, i+1);

		m_motorInfo[0]->setGeometry(610, 75, 180, 180);
		m_motorInfo[1]->setGeometry(610, 295, 180, 180);
		m_motorInfo[2]->setGeometry(10, 295, 180, 180);
		m_motorInfo[3]->setGeometry(10, 75, 180, 180);

		m_acceleroInfo = new AcceleroInfo(this);
		m_acceleroInfo->setGeometry(325, 365, 150, 110);
		//


		m_sliderGlobalSpeed.setOrientation(Qt::Horizontal);
        m_sliderGlobalSpeed.setGeometry(10, 550, 200, 25);
        m_sliderGlobalSpeed.setRange(0, 100);
        m_lblGlobalSpeed.setGeometry(220, 550, 30, 25);
		m_lblGlobalSpeed.setAlignment(Qt::AlignCenter | Qt::AlignVCenter);
		connect(&m_sliderGlobalSpeed, SIGNAL(valueChanged(int)), &m_lblGlobalSpeed, SLOT(setNum(int)));
		connect(&m_sliderGlobalSpeed, SIGNAL(valueChanged(int)), this, SLOT(SendGlobalMotorSpeed()));

		m_pbutCritLand.setGeometry(710, 510, 80, 80);
		connect(&m_pbutCritLand, SIGNAL(pressed()), this, SLOT(SendCriticalLand()));


		show();
	}

	~Window()
	{
		m_thread.terminate();

		for(int i=0 ; i<4 ; i++)
			delete m_motorInfo[i];

		delete m_acceleroInfo;
	}

private:
	ConfigFile m_cfg;
	QTcpSocket m_sockDev;
	uint16_t m_nBlockSize;

	UpdateThread m_thread;

	//Conection bar
	QPushButton m_pbutConnect;
	QPushButton m_pbutDisconnect;
	QLabel m_lblIp, m_lblPort;
	QLineEdit m_leIp, m_lePort;

	//Aerodrone infos
	QLabel m_lblAerodrone;
	MotorInfo* m_motorInfo[4];
	AcceleroInfo* m_acceleroInfo;

	QSlider m_sliderGlobalSpeed;
	QLabel m_lblGlobalSpeed;

	QPushButton m_pbutCritLand;



private slots:
	/**
	@brief Executed clicked on the connect button
	**/
	void Connect()
	{
		m_pbutConnect.setEnabled(false);
		m_sockDev.connectToHost(m_leIp.text(), CharTo<int>(m_lePort.text().toStdString().c_str()));
	}

	void OnConnected()
	{
		m_thread.Pause(false);
		UpdateIPDeviceInfo();
	}

	/**
	@brief Executed when the TCP connection has failed or is stopped anormally
	**/
	void OnConnectionFailed(QAbstractSocket::SocketError err)
	{
		QMessageBox::critical(this, "Connection failed", "Error:\n"+m_sockDev.errorString());
		m_pbutConnect.setEnabled(true);
		m_thread.Pause(true);
	}

	/**
	@brief Executed when the client disconnect
	**/
	void Disconnect()
	{
		m_sockDev.disconnectFromHost();
		m_thread.Pause(false);
	}

	/**
	@brief Updates the IP Informations of the device
	**/
	void UpdateIPDeviceInfo()
	{
		if(m_sockDev.state() == QAbstractSocket::ConnectedState)
		{
			m_pbutConnect.setEnabled(false);
			m_pbutDisconnect.setEnabled(true);
			m_leIp.setEnabled(false);
			m_lePort.setEnabled(false);
		}
		else
		{
			m_pbutConnect.setEnabled(true);
			m_pbutDisconnect.setEnabled(false);
			m_leIp.setEnabled(true);
			m_lePort.setEnabled(true);
		}
		//m_leIp.setText(m_sockDev.localAddress().toString());
		//m_lePort.setText(QString::number(m_sockDev.peerPort()));
	}


	/**
	@brief Executed when a packet is received
	**/
	void OnDataReceived()
	{
		//Waiting to receive the entire packet
		QDataStream in(&m_sockDev);

		if(m_nBlockSize == 0)
		{
			if(m_sockDev.bytesAvailable() < (int)sizeof(uint16_t))
				return;

			in >> m_nBlockSize;
     	}

		if(m_sockDev.bytesAvailable() < m_nBlockSize)
			return;

		//The entire packet has been received

		//Extract the action (meaning) of the packet
		uint8_t nAction = 0;
		in >> nAction;

		if(nAction == NET_INFOREQUEST)
		{
			//cout<<"NET_INFOREQUEST"<<endl;
		}
		else if(nAction == NET_INFODATA)
		{
			//cout<<"NET_INFODATA"<<endl;


			//Extracting motor info
			uint16_t nMot1, nMot2, nMot3, nMot4;
			in >> nMot1;
			in >> nMot2;
			in >> nMot3;
			in >> nMot4;

			m_motorInfo[0]->SetMotorSpeed(nMot1/655.36);
			m_motorInfo[1]->SetMotorSpeed(nMot2/655.36);
			m_motorInfo[2]->SetMotorSpeed(nMot3/655.36);
			m_motorInfo[3]->SetMotorSpeed(nMot4/655.36);

			//Extracting accelerometer info
			uint16_t nAccelX, nAccelY, nAccelZ;
			in >> nAccelX;
			in >> nAccelY;
			in >> nAccelZ;
			m_acceleroInfo->SetValues(nAccelX/2184.53, nAccelY/2184.53, nAccelZ/2184.53);

			QString Packet;
			in >> Packet;
		}
		else
		{
			cout<<"Unknown packet received :("<<endl;
			cout<<"Action='"<<nAction<<"'"<<endl;
			QString s = 0;
			in >> s;
			cout<<"Packet data='"<<s.toStdString()<<"'"<<endl;
		}

		m_nBlockSize=0;
	}

	/**
	@brief
	**/
	void SendGlobalMotorSpeed()
	{
		QByteArray packet;
		QDataStream out(&packet, QIODevice::WriteOnly);
		out.setVersion(QDataStream::Qt_4_0);

		//Allocate space for packet size
		out << (uint16_t)0;

		//Insert data
		out << NET_SETGLOBALMOTORSPEED;

		out << (uint16_t)(m_sliderGlobalSpeed.value()*655.36);

		//Write packet size
		out.device()->seek(0);
		out << (uint16_t)(packet.size() - sizeof(uint16_t));

		m_sockDev.write(packet);
	}

	void SendCriticalLand()
	{
		QByteArray packet;
		QDataStream out(&packet, QIODevice::WriteOnly);
		out.setVersion(QDataStream::Qt_4_0);

		//Allocate space for packet size
		out << (uint16_t)0;

		//Insert data
		out << NET_CRITICALLAND;

		//Write packet size
		out.device()->seek(0);
		out << (uint16_t)(packet.size() - sizeof(uint16_t));

		m_sockDev.write(packet);
	}











};

#include "moc_Window.cpp"

#endif // WINDOW_HPP_INCLUDED
