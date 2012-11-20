#ifndef UPDATETHREAD_HPP_INCLUDED
#define UPDATETHREAD_HPP_INCLUDED

#include <QThread>
#include <QtNetwork>


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
				out << (quint16)0;

				//Insert data
				out << (quint8)(1);//NET_INFOREQUEST;

				//Write packet size
				out.device()->seek(0);
				out << (quint16)(packet.size() - sizeof(quint16));

				m_socket->write(packet);
			}
			QThread::msleep(100);
		}
	}

	QTcpSocket* m_socket;


};


#endif // UPDATETHREAD_HPP_INCLUDED
