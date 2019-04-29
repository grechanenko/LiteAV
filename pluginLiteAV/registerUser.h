#pragma once

/*!
	Регистрирует пользователя на сервере
*/

#include <QObject>

class i_wsCore;
class QNetworkReply;
class RegisterUser : public QObject
{
	Q_OBJECT

public:
	RegisterUser(i_wsCore* ws, int idHost, QObject* parent = Q_NULLPTR);
	~RegisterUser();

private:
	struct SDATA;
	SDATA* m_data;

signals:

	/*!
		Высылает id этого пользователя на сервере
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
