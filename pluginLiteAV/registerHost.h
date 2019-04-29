#pragma once

/*!
	Регистрирует хост на сервере
*/

#include <QObject>

class i_wsCore;
class QNetworkReply;
class RegisterHost : public QObject
{
	Q_OBJECT

public:
	RegisterHost(i_wsCore* ws, QObject* parent = Q_NULLPTR);
	~RegisterHost();

private:
	struct SDATA;
	SDATA* m_data;

signals:

	/*!
		Высылает id этого хоста на сервере
	*/
	void s_id(int id);

	/*!
		Сообщение о завершении работы
	*/
	void finished();

public slots:

	/*!
		Запустить процесс
	*/
	void start();

private slots:

	/*!
		Получает ответ от сервера
	*/
	void replyFinished(QNetworkReply*);
};