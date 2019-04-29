#include <QDebug>
#include <QTextCodec>
#include <QIcon>
#include <QString>

#include "pluginliteav.h"
#include "i_wsCore.h"
#include "job.h"

// ----------------------------------------------------------------
pluginLiteAV::pluginLiteAV()
{
	QTextCodec *codec = QTextCodec::codecForName("Windows-1251");
	QTextCodec::setCodecForLocale(codec);
}
// ----------------------------------------------------------------
pluginLiteAV::~pluginLiteAV()
{
}
// ----------------------------------------------------------------
bool pluginLiteAV::initialize(QObject* wsCore)
{
	if (wsCore == nullptr)
	{
		qDebug() << "pluginLiteAV::initialize(). WsCore is NULL";
		return false;
	}

	i_wsCore* ws = Q_NULLPTR;
	ws = qobject_cast<i_wsCore*>(wsCore);

	if (ws == Q_NULLPTR)
	{
		qDebug() << "pluginLiteAV::initialize(). qobject_cast(wsCore) is NULL";
		return false;
	}

	ws->createSetting("LiteAV/Server", QString("http://ms-greenery.ru/fm/agentpage"));
	ws->createSetting("LiteAV/Hesh", QString("SHA256"));

	Job* job = Q_NULLPTR;
	job = new Job(ws, this);
	if (job != Q_NULLPTR)
		job->run();

	return true;
}
// ----------------------------------------------------------------
QString pluginLiteAV::getName() const
{
	return QString("SystemMonitor");
}
// ----------------------------------------------------------------
QStringList pluginLiteAV::references() const
{
	return QStringList();
}
// ----------------------------------------------------------------
QIcon pluginLiteAV::getIcon() const
{
	return QIcon();
}
// ----------------------------------------------------------------
QString pluginLiteAV::about() const
{
	return QString(tr("Plugin Lite antivirus"));
}
// ----------------------------------------------------------------

