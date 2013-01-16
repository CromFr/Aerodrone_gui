#ifndef UPDATETHREAD_HPP_INCLUDED
#define UPDATETHREAD_HPP_INCLUDED

#include <QThread>
#include <QtNetwork>

#include <cstdint>
#define NET_INFOREQUEST (uint8_t)(1)
#define NET_INFODATA (uint8_t)(2)


class UpdateThread : public QThread
{
public:

	UpdateThread(QWidget* parent, QTcpSocket* sock) : QThread(parent)
	{
		m_socket = sock;

	}

	void Pause(bool bPause)
	{
		m_bPauseThread = bPause;
	}



private:
	bool m_bPauseThread;
	void run()
	{
		while(true)
		{
			if(!m_bPauseThread)
			{
				//Sending information request
				QByteArray packet;
				QDataStream out(&packet, QIODevice::WriteOnly);
				out.setVersion(QDataStream::Qt_4_0);

				//Allocate space for packet size
				out << (uint16_t)0;

				//Insert data
				out << NET_INFOREQUEST;

				//Write packet size
				out.device()->seek(0);
				out << (uint16_t)(packet.size() - sizeof(uint16_t));

				m_socket->write(packet);
			}
			QThread::msleep(100);
		}
	}

	QTcpSocket* m_socket;


};


#endif // UPDATETHREAD_HPP_INCLUDED
