/*!

����������:
	- �������� �����;
	- ��� ������������;
	- ������ ���������;

���������� �� ������
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
		��������� �������
	*/
	void run();

private slots:

	/*!
		�������� id ����� �� �������
	*/
	void slot_idHost(int);

	/*!
		�������� id ������������ �� �������
	*/
	void slot_idUser(int);
	
	/*!
		�������� ��������� ��������
	*/
	void slot_showProcess(QString, QString);
};