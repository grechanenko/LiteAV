#include <QDebug>
#include <QFile>
#include <QCryptographicHash>
#include <QQueue>
#include <QThread>
#include <QMutexLocker>

#include "i_wsCore.h"
#include "registerProcess.h"
#include "finderProcess.h"
#include "senderProcess.h"

// ----------------------------------------------------------------
struct RegisterProcess::SDATA
{
	i_wsCore* ws = Q_NULLPTR;
	int idUser = 0;							// id ������������ �� �������
	FinderProcess* fProcess = Q_NULLPTR;    // ���� ��������
	QQueue<STRUCT_PROCESS> queue;			// �������
	QMutex mutex;
	bool flag_runingFProcess = false;		// true - ���� ����� ������ ��������� ��� ��������
	unsigned int countWorkidThread = 0;		// ���������� ���������� ������� ��� �������� ������ �� ������
	unsigned int countThread = 1;			// ����������� ���������� ������� ��� �������� ������ �� ������
};
// ----------------------------------------------------------------
RegisterProcess::RegisterProcess(int idUser, i_wsCore* ws, QObject* parent)
	:QObject(parent)
{
	qRegisterMetaType<STRUCT_PROCESS>("STRUCT_PROCESS");

	m_data = Q_NULLPTR;
	m_data = new SDATA();
	m_data->idUser = idUser;
	m_data->ws = ws;
}
// ----------------------------------------------------------------
RegisterProcess::~RegisterProcess()
{
	if (m_data != Q_NULLPTR)
	{
		if (m_data->fProcess != Q_NULLPTR)
		{
			m_data->fProcess->deleteLater();
		}

		if (m_data->queue.size() > 0)
		{
			m_data->queue.clear();
		}

		delete m_data;
		m_data = Q_NULLPTR;
	}
}
// ----------------------------------------------------------------
void RegisterProcess::setCountThread(unsigned int value)
{
	if (m_data == Q_NULLPTR)
	{
		qDebug() << "RegisterProcess::setCountThread(). m_data is NULL";
		return;
	}
	
	if (value < 1)
		value = 1;

	// ���������� ������������ ���������� ������� ������ ���������� ����������,
    // ���� ������ ������
	if (value > QThread::idealThreadCount())
		value = QThread::idealThreadCount();

	m_data->countThread = value;
}
// ----------------------------------------------------------------
void RegisterProcess::start()
{
	if (m_data == Q_NULLPTR)
	{
		qDebug() << "RegisterProcess::run(). m_data is NULL";
		emit finished();
		return;
	}

	// ��������� ����� ���������� ���������
	m_data->fProcess = new FinderProcess(m_data->idUser);
	QThread* thread = new QThread;
	m_data->fProcess->moveToThread(thread);

	// �������� �������
	connect(m_data->fProcess, SIGNAL(s_process(STRUCT_PROCESS)), this, SLOT(slot_addProcessToEnqueue(STRUCT_PROCESS)));
	// �������� � ���������� ������ ������ ���������
	connect(m_data->fProcess, SIGNAL(finished()), this, SLOT(slot_fProcessFinished()));
	// ��������� ������ 
	connect(thread, SIGNAL(started()), m_data->fProcess, SLOT(start()));
	// ���������� �����
	connect(m_data->fProcess, SIGNAL(finished()), thread, SLOT(quit()));
	// ������� ����� 
	connect(m_data->fProcess, SIGNAL(finished()), m_data->fProcess, SLOT(deleteLater()));
	// ������� ����� 
	connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
	
	m_data->flag_runingFProcess = true;
	// ��������� ����� 
	thread->start();
}
// ----------------------------------------------------------------
//void RegisterProcess::stop()
//{
	//emit finished();
//}
// ----------------------------------------------------------------
void RegisterProcess::slot_addProcessToEnqueue(STRUCT_PROCESS newProcess)
{
	emit s_dataProcess(newProcess.name, newProcess.hash);

	if (m_data != Q_NULLPTR)
	{
		QMutexLocker locker(&m_data->mutex);
		m_data->queue.enqueue(newProcess);
	}
	else
	{
		qDebug() << "RegisterProcess::slot_addProcessToEnqueue(). m_data is NULL";
	}
	this->slot_sendProcessToServer();
}
// ----------------------------------------------------------------
void RegisterProcess::slot_fProcessFinished()
{
	if (m_data != Q_NULLPTR)
	{
		QMutexLocker locker(&m_data->mutex);
		m_data->flag_runingFProcess = false;
	}
	else
	{
		qDebug() << "RegisterProcess::slot_fProcessFinished(). m_data is NULL";
	}
	this->slot_sendProcessToServer();
}
// ----------------------------------------------------------------
void RegisterProcess::slot_senderProcessFinished()
{
	if (m_data != Q_NULLPTR)
	{
		QMutexLocker locker(&m_data->mutex);
		m_data->countWorkidThread--;
	}
	else
	{
		qDebug() << "RegisterProcess::slot_senderProcessFinished(). m_data is NULL";
	}
}
// ----------------------------------------------------------------
void RegisterProcess::slot_sendProcessToServer()
{
	if (m_data == Q_NULLPTR)
	{
		qDebug() << "RegisterProcess::slot_sendProcess(). m_data is NULL";
		return;
	}

	// ��������� ������, ���� ��� �������� �������
	if ((m_data->countWorkidThread == 0) && (m_data->flag_runingFProcess == false) && m_data->queue.size() == 0 )
	{		
		//emit finished();
		return;
	}	
	
	/*
	    ��������� ������ � �������� �� ������,
		���� ���������� ���������� �������
		������ ����������� ����������,
		����� ��������� ����� �����
	*/
	if (m_data->countWorkidThread < m_data->countThread)
	{
		if (m_data->queue.size() > 0)
		{
			SenderProcess* senderProcess = new SenderProcess(m_data->queue.dequeue(), m_data->ws);
			QThread* thread = new QThread;
			senderProcess->moveToThread(thread);

			// ��������� ������� 
			connect(thread, SIGNAL(started()), senderProcess, SLOT(start()));

			// �������� � ���������� ������ ������ �����
			connect(senderProcess, SIGNAL(finished()), SLOT(slot_senderProcessFinished()));

			// ��������� ��������� �����
			connect(senderProcess, SIGNAL(finished()), SLOT(slot_sendProcessToServer()));

			// ���������� �����
			connect(senderProcess, SIGNAL(finished()), thread, SLOT(quit()));

			// ������� ����� 
			connect(senderProcess, SIGNAL(finished()), senderProcess, SLOT(deleteLater()));

			// ������� ����� 
			connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));

			m_data->countWorkidThread++;

			// ��������� ����� 
			thread->start();
		}
	}
}
// ----------------------------------------------------------------