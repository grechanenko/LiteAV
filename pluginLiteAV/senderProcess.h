/*!
	������������ ������� �� �������
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
		�������� id ��������
	*/
	void s_id(int);

	/*!
		��������� � ���������� ������
	*/
	void finished();

public slots:
	
	/*!
		��������� �������
	*/
	void start();

private slots:

	/*!
		�������� ����� �� �������
	*/
	void replyFinished(QNetworkReply*);
};
