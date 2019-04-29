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
        ������������� �������
        i_wsCore = qobject_cast<i_wsCore>(wsCore)       
    */
    virtual bool initialize( QObject* wsCore);


	/*!
		���������� �������� �������
		������������ ��� ��������� ������������
	*/
	virtual QString getName() const;


    /*
        ����������� �� ������ ��������
    */
    virtual QStringList references() const;


	/*!
		���������� ������ �������
	*/
	virtual QIcon getIcon() const;


	/*!
		���������� �������� �������
	*/
	virtual QString about() const;

};

#endif // PLUGINLITEAV_H
