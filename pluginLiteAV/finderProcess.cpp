#include <QDebug>
#include <QThread>
#include <QFile>
#include <QCryptographicHash>
#include <QFileInfo>
#include "finderProcess.h"

// ----------------------------------------------------------------
struct FinderProcess::SDATA
{
	int idUser = 0;
};
// ----------------------------------------------------------------
FinderProcess::FinderProcess(int idUser, QObject* parent)
	: QObject(parent)
{
	m_data = Q_NULLPTR;
	m_data = new SDATA();
	m_data->idUser = idUser;
}
// ----------------------------------------------------------------
FinderProcess::~FinderProcess()
{
	if (m_data != Q_NULLPTR)
	{
		delete m_data;
		m_data = Q_NULLPTR;
	}
}
// ----------------------------------------------------------------
void FinderProcess::start()
{
	if (m_data == Q_NULLPTR)
	{
		qDebug() << "FinderProcess::start(). m_data is NULL";
		emit finished();
		return;
	}

	HANDLE CONST hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	{
		PROCESSENTRY32 peProcessEntry;
		TCHAR szBuff[1024];
		DWORD dwTemp;
		HANDLE CONST hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

		if (INVALID_HANDLE_VALUE == hSnapshot)
		{
			qDebug() << "RegisterProcess::getAllProcess(). CreateToolhelp32Snapshot failed";
			return;
		}

		peProcessEntry.dwSize = sizeof(PROCESSENTRY32);
		Process32First(hSnapshot, &peProcessEntry);
		do {
			STRUCT_PROCESS proc;
			proc.name = QString().fromWCharArray(peProcessEntry.szExeFile);
			proc.id = (unsigned long)peProcessEntry.th32ProcessID;
			proc.idUser = m_data->idUser;
								
			// Определяем путь к файлу процесса
			QString path = QString();
			{
				// Take a snapshot of all modules in the specified process.
				HANDLE hModuleSnap = NULL;
				hModuleSnap = ::CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, peProcessEntry.th32ProcessID);
				if (hModuleSnap == INVALID_HANDLE_VALUE)
				{					
					continue;
				}

				MODULEENTRY32 me32 = { 0 };
				me32.dwSize = sizeof(MODULEENTRY32);
				if (!::Module32First(hModuleSnap, &me32))
				{
					::CloseHandle(hModuleSnap);					
					continue;
				}
				::CloseHandle(hModuleSnap);
				path = QString().fromWCharArray(me32.szExePath);
			}			
				
			if (path.isEmpty() != true)
			{
				proc.path = path;

				proc.name = QFileInfo(path).fileName();


				// Вычисляем хеш
				{					
					QFile f(proc.path);
					if (f.open(QFile::ReadOnly))
					{
						// Сделать выбор алгоритма
						proc.hash = QCryptographicHash::hash(f.readAll(), QCryptographicHash::Sha256).toHex();						
						f.close();
					}					
				}				
				emit s_process(proc);		

				QThread::sleep(0);
			}
		} while (Process32Next(hSnapshot, &peProcessEntry));
		CloseHandle(hSnapshot);
	}

	emit finished();
}
// ----------------------------------------------------------------