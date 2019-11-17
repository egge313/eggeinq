#include "workerthread.h"
#include "curlget.h"

void WorkerThread::run ()
{
  QString data;
  QString errmsg;
  QString result;

  if (curlget (data, errmsg))
    {
      // ui->textEdit->append ("<h3>IP number (IPv4):</h3>");
      data.replace(QString("\n"), QString(""));
      data.replace(QString("\r"), QString(""));	  
      // ui->textEdit->append (data);
      result = data;
    }	

  else
    {
      errmsg.replace(QString("\n"), QString(""));
      errmsg.replace(QString("\r"), QString(""));	  
      // ui->textEdit->append (errmsg);
      // ui->textEdit->append ("Are you sure your computer is connected to the Internet?");
      result = errmsg;
    }	
  emit resultReady(result);
}

// ------------ end of workerthread.cpp --------------
