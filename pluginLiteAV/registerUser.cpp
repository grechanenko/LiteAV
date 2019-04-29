#include <QDomDocument>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QNetworkAccessManager>
#include <QDebug>

#include <Windows.h>

#include "i_wsCore.h"
#include "registerUser.h"

// ----------------------------------------------------------------
struct RegisterUser::SDATA
{
	i_wsCore* ws = Q_NULLPTR;
	int idHost = 0;
};
// ----------------------------------------------------------------
RegisterUser::RegisterUser(i_wsCore* ws, int idHost, QObject* parent)
	:QObject(parent)
{
	m_data = Q_NULLPTR;
	m_data = new SDATA;
	m_data->ws = ws;
	m_data->idHost = idHost;
}
// ----------------------------------------------------------------
RegisterUser::~RegisterUser()
{
	if (m_data != Q_NULLPTR)
	{
		m_data->ws = Q_NULLPTR;

		delete m_data;
		m_data = Q_NULLPTR;
	}
}
// ----------------------------------------------------------------
void RegisterUser::start()
{	
	// Заглушка
	/*{
		emit s_id(1);
		emit finished();
		return;
	}*/


	if (m_data == Q_NULLPTR)
	{
		qDebug() << "RegisterUser::start(). m_data id NULL";
		emit finished();
		return;
	}

	if (m_data->ws == Q_NULLPTR)
	{
		qDebug() << "RegisterUser::start(). ws is NULL";
		emit finished();
		return;
	}
	
	// Определить имя пользователя
	QString userName = QString();
	{		
		TCHAR  infoBuf[255];
		DWORD  bufCharCount = 255;
		if (GetUserName(infoBuf, &bufCharCount) == false)
		{
			qDebug() << "RegisterUser::start(). Can not get user name";
			return;
		}
		userName = QString().fromWCharArray(infoBuf);
	}

	
	if (userName.isEmpty() == true)
	{
		qDebug() << "RegisterUser::start(). Can not read host name";
		emit finished();
		return;
	}

	// Формируем запрос
	QString request = m_data->ws->getSetting("SystemMonitor/Server").toString();
	request += "?registerUser=" + userName + "&idHost=" + QString().setNum(m_data->idHost);
	
	// Отправить запрос о регистрации на сервер
	QByteArray hesh = QByteArray();

	QNetworkAccessManager *manager = new QNetworkAccessManager(this);
	connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)));

	manager->get(QNetworkRequest(QUrl(request.toUtf8())));
	
}
// ----------------------------------------------------------------
void RegisterUser::replyFinished(QNetworkReply* reply)
{
	if (reply == Q_NULLPTR)
	{
		qDebug() << "RegisterUser::replyFinished(). QNetworkReply is NULL";
		emit finished();
		return;
	}

	if (reply->error() == QNetworkReply::NoError)
	{
		QString html = QString(reply->readAll());
		if (html.length() > 0)
		{
			int id1 = html.indexOf("<idUser>") + QString("<idUser>").length();			
			int id2 = html.indexOf("</idUser>");

			if (id2 > id1)
			{
				int idUser = html.mid(id1, id2 - id1).toInt();				
				emit s_id(idUser);
			}
		}
	}
	else
	{
		qDebug() << "RegisterUser::replyFinished(). Reply error = " << reply->error();
	}

	reply->deleteLater();
	emit finished();
}
// ----------------------------------------------------------------