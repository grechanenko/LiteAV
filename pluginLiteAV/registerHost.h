#pragma once

/*!
	������������ ���� �� �������
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
		�������� id ����� ����� �� �������
	*/
	void s_id(int id);

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