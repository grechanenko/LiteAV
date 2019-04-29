#pragma once

#include <QString>
#include <QVector>

struct STRUCT_EXTENSIONS
{
	QString ResursName;
	QString ResursPath;
	QByteArray hash;
};

struct STRUCT_PROCESS
{	
	int idUser;			// id ������������ � ���� ������ �� �������
	QString path;		// ������ ���� � ����� ��������
	QString hash;	// ��� ����� ��������
	QString name;		// ��� ����� �������� 

	unsigned long id;	// id �������� � �������	
	QVector<STRUCT_EXTENSIONS> ext;	// �����������
};