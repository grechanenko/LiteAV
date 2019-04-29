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
	int idUser;			// id пользователя в базе данных на сервере
	QString path;		// Полный путь к файлу процесса
	QString hash;	// Хеш файла процесса
	QString name;		// Имя файла процесса 

	unsigned long id;	// id процесса в системе	
	QVector<STRUCT_EXTENSIONS> ext;	// Зависимости
};