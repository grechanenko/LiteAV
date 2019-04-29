#include <QDebug>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>

#include "senderProcess.h"
#include "i_wsCore.h"

// ----------------------------------------------------------------
struct SenderProcess::SDATA
{
	i_wsCore* ws = Q_NULLPTR;
	STRUCT_PROCESS s_process;			// Данные о процессе
};
// ----------------------------------------------------------------
SenderProcess::SenderProcess(STRUCT_PROCESS s_process, i_wsCore* ws, QObject* parent)
	: QObject(parent)
{
	m_data = Q_NULLPTR;
	m_data = new SDATA;
	m_data->s_process = s_process;
	m_data->ws = ws;
}
// ----------------------------------------------------------------
SenderProcess::~SenderProcess()
{
	if (m_data != Q_NULLPTR)
	{
		m_data->ws = Q_NULLPTR;

		m_data->s_process.ext.clear();
	
		delete m_data;
		m_data = Q_NULLPTR;
	}
}
// ----------------------------------------------------------------
void SenderProcess::start()
{
	if (m_data == Q_NULLPTR)
	{
		qDebug() << "SenderProcess::start(). m_data is NULL";
		emit finished();
		return;
	}

	if (m_data->ws == Q_NULLPTR)
	{
		qDebug() << "SenderProcess::start(). ws is NULL";
		emit finished();
		return;
	}
	
	m_data->s_process.path = m_data->s_process.path.replace("\\", "/");
		
	// Формируем запрос
	QString request = m_data->ws->getSetting("SystemMonitor/Server").toString();	
	request += "?registerProcess=" + m_data->s_process.path;
	request += "&name=" + m_data->s_process.name;
	request += "&hesh=" + m_data->s_process.hash;
	request += "&userID=" + QString().setNum(m_data->s_process.idUser);
	
	qDebug() << "";	
	qDebug() << "Process name = " << m_data->s_process.name;
	qDebug() << "Hash = " << m_data->s_process.hash;
	//qDebug() << "Path = " << m_data->s_process.path;
	//qDebug() << "Request = " << request;

	// Отправить запрос о регистрации процесса на сервере
	QNetworkAccessManager *manager = new QNetworkAccessManager(this);
	connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)));

	if (manager->get(QNetworkRequest(QUrl(request))) == Q_NULLPTR)
		emit finished();
}
// ----------------------------------------------------------------
void SenderProcess::replyFinished(QNetworkReply* reply)
{
	if (reply == Q_NULLPTR)
	{
		qDebug() << "SenderProcess::replyFinished(). QNetworkReply is NULL";
		emit finished();
		return;
	}	 

	if (reply->error() == QNetworkReply::NoError)
	{
		QString html = QString(reply->readAll());
		if (html.length() > 0)
		{
			int id1 = html.indexOf("<idProcess>") + QString("<idProcess>").length();
			int id2 = html.indexOf("</idProcess>");

			if (id2 > id1)
			{
				int idProcess = html.mid(id1, id2 - id1).toInt();

				qDebug() << "register process id = " << idProcess;
				emit s_id(idProcess);
			}
		}
	}
	else
	{
		qDebug() << "SenderProcess::replyFinished(). Reply error = " << reply->error();
	}
	reply->deleteLater();

	emit finished();
}
// ----------------------------------------------------------------