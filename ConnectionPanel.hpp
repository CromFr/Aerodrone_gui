#ifndef CONNECTIONPANEL_HPP_INCLUDED
#define CONNECTIONPANEL_HPP_INCLUDED


#include <iostream>
using namespace std;

#include <QtGui>
#include <QtNetwork>

#include "misc.h"


class ConnectionPanel : QDialog
{
	Q_OBJECT

public:
	ConnectionPanel(QWidget* parent, QTcpSocket* cursock) : QDialog(parent),
		lblIP("IP", this), /*lblMask("Mask", this),*/ lblPort("Port", this),
		leIP(this), /*leMask(this),*/ lePort(this),
		pbutOk("Connect", this), pbutCancel("Cancel", this)
	{
		setFixedSize(250, 250);
		m_socket = cursock;

		lblIP.setGeometry(10,10,40,25);
		leIP.setGeometry(50,10,190,25);
//		lblMask.setGeometry(10,45,40,25);
//		leMask.setGeometry(50,45,190,25);
		lblPort.setGeometry(10,80,40,25);
		lePort.setGeometry(50,80,190,25);

		pbutOk.setGeometry(10, 180, 230, 25);
		pbutCancel.setGeometry(10, 215, 230, 25);

		QObject::connect(&pbutOk, SIGNAL(clicked()), this, SLOT(Connect()));
		QObject::connect(&pbutCancel, SIGNAL(clicked()), this, SLOT(hide()));


		pbutOk.setFocus();
	}

	void PopUp(const QString& sIp, const QString& sPort)
	{
		leIP.setText(sIp);
		lePort.setText(sPort);

		show();
	}

public slots:
	void Connect()
	{
		cout<<"@"<<(long)m_socket<<"Connecting to "<<leIP.text().toStdString()<<":"<<lePort.text().toStdString()<<endl;
		m_socket->connectToHost(leIP.text(), CharTo<int>(lePort.text().toStdString().c_str()));

		bool bConnected = false;
		if(bConnected)
		{
			QMessageBox::information(this, "Connection successful", "The device is now connected to your PC");

		}
		else
		{
			QMessageBox::critical(this, "Connection failed", "############ERR CODE");
		}

	}


private:
	QTcpSocket* m_socket;
	QLabel lblIP, /*lblMask,*/ lblPort;
	QLineEdit leIP,/* leMask, */lePort;
	QPushButton pbutOk, pbutCancel;


};

#include "moc_ConnectionPanel.cpp"

#endif // CONNECTIONPANEL_HPP_INCLUDED
