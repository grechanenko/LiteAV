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
	RegisterProcess* rProc;		    // Регистрирует запущенные процессы на сервере
	int idHost = 0;					// id хоста на сервере
	int idUser = 0;					// id пользователя на сервере
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
		
	// Регистрируемся на сервере
	RegisterHost* rHost = new RegisterHost(m_data->ws);
	QThread* thread = new QThread;
	rHost->moveToThread(thread);

	// Получить id хоста на сервере
	connect(rHost, SIGNAL(s_id(int)), this, SLOT(slot_idHost(int)));
	// Запустить задачу 
	connect(thread, SIGNAL(started()), rHost, SLOT(start()));
	// Остановить поток
	connect(rHost, SIGNAL(finished()), thread, SLOT(quit()));
	// Удалить класс 
	connect(rHost, SIGNAL(finished()), rHost, SLOT(deleteLater()));
	// Удалить поток 
	connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
	// Запустить поток 
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
			
	// Регистрируемся на сервере
	RegisterUser* rUser = new RegisterUser(m_data->ws, m_data->idHost);
	QThread* thread = new QThread;
	rUser->moveToThread(thread);
	
	// Получить id хоста на сервере
	connect(rUser, SIGNAL(s_id(int)), this, SLOT(slot_idUser(int)));
	// Запустить задачу 
	connect(thread, SIGNAL(started()), rUser, SLOT(start()));
	// Остановить поток
	connect(rUser, SIGNAL(finished()), thread, SLOT(quit()));
	// Удалить класс 
	connect(rUser, SIGNAL(finished()), rUser, SLOT(deleteLater()));
	// Удалить поток 
	connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
	// Запустить поток 
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

	// Регистрируем на сервере запущенные процессы
	m_data->rProc = new RegisterProcess(m_data->idUser, m_data->ws);
	QThread* thread = new QThread;
	m_data->rProc->moveToThread(thread);
	
	// Показать данные процесса
	connect(m_data->rProc, SIGNAL(s_dataProcess(QString, QString)), SLOT(slot_showProcess(QString, QString)));
	// Запустить задачу 
	connect(thread, SIGNAL(started()), m_data->rProc, SLOT(start()));
	// Остановить поток
	connect(m_data->rProc, SIGNAL(finished()), thread, SLOT(quit()));
	// Удалить класс 
	connect(m_data->rProc, SIGNAL(finished()), m_data->rProc, SLOT(deleteLater()));
	// Удалить поток 
	connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
	// Запустить поток 
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