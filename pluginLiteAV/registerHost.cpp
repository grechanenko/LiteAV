#include <QTextCodec>
#include <QDomDocument>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QNetworkAccessManager>
#include <QDebug>

#include <Windows.h>

#include "i_wsCore.h"
#include "registerHost.h"

// ----------------------------------------------------------------
struct RegisterHost::SDATA
{
	i_wsCore* ws = Q_NULLPTR;
	
};
// ----------------------------------------------------------------
RegisterHost::RegisterHost(i_wsCore* ws, QObject* parent)
	:QObject(parent)
{
	QTextCodec *codec = QTextCodec::codecForName("Windows-1251");
	QTextCodec::setCodecForLocale(codec);

	m_data = Q_NULLPTR;
	m_data = new SDATA;
	m_data->ws = ws;
}
// ----------------------------------------------------------------
RegisterHost::~RegisterHost()
{
	if (m_data != Q_NULLPTR)
	{
		m_data->ws = Q_NULLPTR;

		delete m_data;
		m_data = Q_NULLPTR;
	}	
}
// ----------------------------------------------------------------
void RegisterHost::start()
{
	/*{
		emit s_id(1);
		emit finished();
		return;
	}*/

	if (m_data == Q_NULLPTR)
	{
		qDebug() << "RegisterHost::start(). m_data id NULL";
		emit finished();
		return;
	}

	if (m_data->ws == Q_NULLPTR)
	{
		qDebug() << "RegisterHost::start(). ws is NULL";
		emit finished();
		return;
	}
	
	// Определить имя хоста
	QString hostName = QString();
	{
		TCHAR  infoBuf[255];
		DWORD  bufCharCount = 255;
		if (GetComputerName(infoBuf, &bufCharCount) == false)
		{
			qDebug() << "RegisterHost::start(). Can not get cumputer name";
			return;
		}
		hostName = QString().fromWCharArray(infoBuf);
	}
	
	

	if (hostName.isEmpty() == true)
	{
		qDebug() << "RegisterHost::start(). Can not read host name";
		emit finished();
		return;
	}

	// Формируем запрос
	QString request = m_data->ws->getSetting("SystemMonitor/Server").toString();
	request += "?registerHost=" + hostName;	
	
	// Отправить запрос о регистрации на сервер
	QByteArray hesh = QByteArray();

	QNetworkAccessManager *manager = new QNetworkAccessManager(this);
	connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)));

	manager->get(QNetworkRequest(QUrl(request)));
	
}
// ----------------------------------------------------------------
void RegisterHost::replyFinished(QNetworkReply* reply)
{
	if (reply == Q_NULLPTR)
	{
		qDebug() << "RegisterHost::replyFinished(). QNetworkReply is NULL";
		emit finished();
		return;
	}

	if (reply->error() == QNetworkReply::NoError)
	{
		QString html = QString(reply->readAll());
		if (html.length() > 0)
		{
			int id1 = html.indexOf("<idHost>") + QString("<idHost>").length();			
			int id2 = html.indexOf("</idHost>");

			if (id2 > id1)
			{
				int idHost = html.mid(id1, id2 - id1).toInt();
				emit s_id(idHost);
			}
		}
	}
	else
	{
		qDebug() << "RegisterHost::replyFinished(). Reply error = " << reply->error();
	}

	reply->deleteLater();
	emit finished();
}
// ----------------------------------------------------------------