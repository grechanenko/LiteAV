#ifndef PLUGINLITEAV_H
#define PLUGINLITEAV_H

#include <QObject>

#include "pluginliteav_global.h"
#include "i_wsPlugin.h"

class PLUGINLITEAV_EXPORT pluginLiteAV : public QObject, public virtual i_wsPlugin
{
	Q_OBJECT
	Q_INTERFACES(i_wsPlugin)
	Q_PLUGIN_METADATA(IID "pluginLiteAV")

public:
	pluginLiteAV();
	~pluginLiteAV();

	/*!
        Инициализация плагина
        i_wsCore = qobject_cast<i_wsCore>(wsCore)       
    */
    virtual bool initialize( QObject* wsCore);


	/*!
		Возвращает название плагина
		Используется для механизма зависимостей
	*/
	virtual QString getName() const;


    /*
        Зависимости от других плагинов
    */
    virtual QStringList references() const;


	/*!
		Возвращает иконку плагина
	*/
	virtual QIcon getIcon() const;


	/*!
		Возвращает описание планига
	*/
	virtual QString about() const;

};

#endif // PLUGINLITEAV_H
