/*!
	Регистрирует запущенные процессы на сервере
*/

#pragma once

#include <QObject>
#include "structProcess.h"

class i_wsCore;
class RegisterProcess : public QObject
{
	Q_OBJECT

public:
	
	RegisterProcess(int idUser, i_wsCore* ws, QObject* parent = Q_NULLPTR);
	~RegisterProcess();

	/*!
		Установить количество потоков для отправки данных на сервер
		от 1 до максимального количества ядер в системе.
		Default 1
	*/
	void setCountThread(unsigned int value);


private:
	struct SDATA;
	SDATA* m_data;

signals:

	/*!
		Сообщение о завершении работы
	*/
	void finished();

	/*!
		Высылает имя процесса и хэш
	*/
	void s_dataProcess(QString name, QString hash);

public slots:

	/*!
		Запустить процесс
	*/
	void start();

	/*!
		Остановить процесс
	*/
	//void stop();

private slots:

	/*!
		Добавляет в очередь новый процесс
	*/
	void slot_addProcessToEnqueue(STRUCT_PROCESS);

	/*!
		Вызывается по завершению поиска процессов
	*/
	void slot_fProcessFinished();

	/*!
		Вызывается по звершению отправки данных о потоке на сервер
	*/
	void slot_senderProcessFinished();

	/*!
		Отправить данные о процессе на сервер
	*/
	void slot_sendProcessToServer();
};

Q_DECLARE_METATYPE(STRUCT_PROCESS);