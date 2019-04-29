/*!
	Регистрирует процесс на сервере
*/

#pragma once

#include <QObject>
#include "structProcess.h"

class i_wsCore;
class QNetworkReply;
class SenderProcess : public QObject
{
	Q_OBJECT
public:

	SenderProcess(STRUCT_PROCESS s_process, i_wsCore* ws, QObject* parent = Q_NULLPTR);
	~SenderProcess();

private: 
	struct SDATA;
	SDATA* m_data;

signals:

	/*!
		Высылает id процесса
	*/
	void s_id(int);

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
