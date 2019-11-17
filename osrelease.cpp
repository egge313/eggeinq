// Obtain information about operating system.

#include <QString>
#include <QStringList>
#include <QTextStream>
#include <QFile>
#include "osrelease.h"

void os_release (QStringList & retval)
{
#ifndef linux
  retval << "Sorry, not implemented but for Linux.";
  return;
#else
  QFile file("/etc/os-release");
  if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
      if (!file.exists())
	{
      retval << "Error: File '/etc/os-release' does not exists.";
	}
      else
	{
      retval << "Error: Can't open '/etc/os-release'.";
	}
      return;
    }
    QTextStream in (&file);
    QString line = in.readLine();
    while (!line.isNull()) 
      {
        retval << line;
        line = in.readLine();
      }
#endif // linux
}
