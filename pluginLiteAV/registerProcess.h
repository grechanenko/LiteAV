/*!
	������������ ���������� �������� �� �������
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
		���������� ���������� ������� ��� �������� ������ �� ������
		�� 1 �� ������������� ���������� ���� � �������.
		Default 1
	*/
	void setCountThread(unsigned int value);


private:
	struct SDATA;
	SDATA* m_data;

signals:

	/*!
		��������� � ���������� ������
	*/
	void finished();

	/*!
		�������� ��� �������� � ���
	*/
	void s_dataProcess(QString name, QString hash);

public slots:

	/*!
		��������� �������
	*/
	void start();

	/*!
		���������� �������
	*/
	//void stop();

private slots:

	/*!
		��������� � ������� ����� �������
	*/
	void slot_addProcessToEnqueue(STRUCT_PROCESS);

	/*!
		���������� �� ���������� ������ ���������
	*/
	void slot_fProcessFinished();

	/*!
		���������� �� ��������� �������� ������ � ������ �� ������
	*/
	void slot_senderProcessFinished();

	/*!
		��������� ������ � �������� �� ������
	*/
	void slot_sendProcessToServer();
};

Q_DECLARE_METATYPE(STRUCT_PROCESS);