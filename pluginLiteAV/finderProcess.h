#pragma once

#include <QObject>
#include <QVector>

#include <Windows.h>
#include <TlHelp32.h>
#include <Psapi.h>
#include <conio.h>
#include <string.h>
#include <stdio.h>
#include <tchar.h>
#include "structProcess.h"

class FinderProcess : public QObject
{
	Q_OBJECT

public:
	FinderProcess(int idUser, QObject* parent = Q_NULLPTR);
	~FinderProcess();	

private:
	struct SDATA;
	SDATA* m_data;

signals:

	/*!
		Высылает данные о найденном процессе
	*/
	void s_process(STRUCT_PROCESS);
	
	/*!
		Сообщение о завершении
	*/
	void finished();

public slots:

	/*!
		Запустить процесс
	*/
	void start();
};