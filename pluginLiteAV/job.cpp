#include <QDebug>
#include <QMessageBox>
#include <QThread>
#include <QFile>


#include "job.h"
#include "i_wsCore.h"
#include "registerHost.h"
#include "registerUser.h"
#include "registerProcess.h"

// ----------------------------------------------------------------
struct Job::SDATA
{	
	i_wsCore* ws = Q_NULLPTR;
	RegisterProcess* rProc;		    // ������������ ���������� �������� �� �������
	int idHost = 0;					// id ����� �� �������
	int idUser = 0;					// id ������������ �� �������
};
// ----------------------------------------------------------------
Job::Job(i_wsCore* ws, QObject* parent)
	:QObject(parent)
{
	m_data = Q_NULLPTR;
	m_data = new SDATA();
	m_data->ws = ws;
}
// ----------------------------------------------------------------
Job::~Job()
{
	if (m_data != Q_NULLPTR)
	{
		if (m_data->rProc != Q_NULLPTR)
		{
			//m_data->rProc->stop();
		}

		delete m_data;
		m_data = Q_NULLPTR;
	}
}
// ----------------------------------------------------------------
void Job::run()
{
	if (m_data == Q_NULLPTR)
	{
		qDebug() << "Job::run(). m_data id NULL";
		return;
	}
		
	// �������������� �� �������
	RegisterHost* rHost = new RegisterHost(m_data->ws);
	QThread* thread = new QThread;
	rHost->moveToThread(thread);

	// �������� id ����� �� �������
	connect(rHost, SIGNAL(s_id(int)), this, SLOT(slot_idHost(int)));
	// ��������� ������ 
	connect(thread, SIGNAL(started()), rHost, SLOT(start()));
	// ���������� �����
	connect(rHost, SIGNAL(finished()), thread, SLOT(quit()));
	// ������� ����� 
	connect(rHost, SIGNAL(finished()), rHost, SLOT(deleteLater()));
	// ������� ����� 
	connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
	// ��������� ����� 
	thread->start();	
}
// ----------------------------------------------------------------
void Job::slot_idHost(int value)
{
	if (m_data == Q_NULLPTR)
	{
		qDebug() << "Job::slot_idHost(). m_data is NULL";
		return;
	}
	
	m_data->idHost = value;
			
	// �������������� �� �������
	RegisterUser* rUser = new RegisterUser(m_data->ws, m_data->idHost);
	QThread* thread = new QThread;
	rUser->moveToThread(thread);
	
	// �������� id ����� �� �������
	connect(rUser, SIGNAL(s_id(int)), this, SLOT(slot_idUser(int)));
	// ��������� ������ 
	connect(thread, SIGNAL(started()), rUser, SLOT(start()));
	// ���������� �����
	connect(rUser, SIGNAL(finished()), thread, SLOT(quit()));
	// ������� ����� 
	connect(rUser, SIGNAL(finished()), rUser, SLOT(deleteLater()));
	// ������� ����� 
	connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
	// ��������� ����� 
	thread->start();
}
// ----------------------------------------------------------------
void Job::slot_idUser(int value)
{
	if (m_data == Q_NULLPTR)
	{
		qDebug() << "Job::slot_idUser(). m_data is NULL";
		return;
	}

	m_data->idUser = value;

	// ������������ �� ������� ���������� ��������
	m_data->rProc = new RegisterProcess(m_data->idUser, m_data->ws);
	QThread* thread = new QThread;
	m_data->rProc->moveToThread(thread);
	
	// �������� ������ ��������
	connect(m_data->rProc, SIGNAL(s_dataProcess(QString, QString)), SLOT(slot_showProcess(QString, QString)));
	// ��������� ������ 
	connect(thread, SIGNAL(started()), m_data->rProc, SLOT(start()));
	// ���������� �����
	connect(m_data->rProc, SIGNAL(finished()), thread, SLOT(quit()));
	// ������� ����� 
	connect(m_data->rProc, SIGNAL(finished()), m_data->rProc, SLOT(deleteLater()));
	// ������� ����� 
	connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
	// ��������� ����� 
	thread->start();
}
// ----------------------------------------------------------------
void Job::slot_showProcess(QString name, QString hash)
{
	//QMessageBox::information(Q_NULLPTR, name, hash);

	/*QFile file("d:/log.txt");
	if (file.open(QFile::Append))
	{
		file.write(name.toLocal8Bit());
		file.write(" ");
		file.write(hash.toLocal8Bit());
		file.write("\n");

		file.close();
	}*/
}
// ----------------------------------------------------------------