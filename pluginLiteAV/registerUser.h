#pragma once

/*!
	������������ ������������ �� �������
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
		�������� id ����� ������������ �� �������
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
