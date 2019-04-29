/*!

Определяет:
	- название хоста;
	- имя пользователя;
	- список процессов;

Отправляет на сервер
*/

#pragma once

#include <QObject>

class i_wsCore;
class Job : public QObject
{
	Q_OBJECT

public: 
	Job(i_wsCore* ws, QObject* parent = Q_NULLPTR);
	~Job();

private:
	struct SDATA;
	SDATA* m_data;

public slots:

	/*!
		Запустить процесс
	*/
	void run();

private slots:

	/*!
		Получить id хоста на сервере
	*/
	void slot_idHost(int);

	/*!
		Получить id пользователя на сервере
	*/
	void slot_idUser(int);
	
	/*!
		Показать параметры процесса
	*/
	void slot_showProcess(QString, QString);
};