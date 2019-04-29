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
		�������� ������ � ��������� ��������
	*/
	void s_process(STRUCT_PROCESS);
	
	/*!
		��������� � ����������
	*/
	void finished();

public slots:

	/*!
		��������� �������
	*/
	void start();
};