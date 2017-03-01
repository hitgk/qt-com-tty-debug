/*

	threadcomport v.0.0.1
	author Golubkin Egor (Gorin), Russia
	///////////////////////////////////////////////

	Multiplatform asynchronous Serial Port Extension
	based on Wayne Roth's QExtSerialPort
	

*/  
 #include <threadcomport.h>
 #include <qextserialport.h>
 
 #include <QtDebug>
 #include <QObject>
 #include <QByteArray>
/*
==============
<CONSTRUCTORS>
==============
*/
Qthreadcomport::Qthreadcomport (const PortSettings *settings):
QextSerialPort(*settings)
{
	thread = NULL;
}

ReceiveThread::ReceiveThread()
{
	lastcount = 0;
}

QMainComThread::QMainComThread(QString name, const PortSettings *settings)
{
	comsettings = new PortSettings();
	*comsettings = *settings;
	comname = name;
	comport = NULL;
}
/*
==============
<DESTRSTRUCTORS>
==============
*/
Qthreadcomport::~Qthreadcomport()
{
	stopThread();
}
QMainComThread::~QMainComThread()
{
terminate();
wait();
delete comport;
comport = NULL;
delete comsettings;
comsettings = NULL;
}
/*
==============
<METHODS>
==============
*/
/*TMainComThread*/
Qthreadcomport* QMainComThread::getPort()
{
	return comport;
}

void QMainComThread::run()
{
	Qthreadcomport *port = new Qthreadcomport(comsettings);
	port->setPortName(comname);
	comport = port;

	exec();
}

/*QReceiveThread*/
void ReceiveThread::run()
{
	int count;
	forever 
	{
		msleep(1);
		mutex.lock();
		count = comport->bytesAvailable();
		mutex.unlock();
		if (0 < count)
		{
			emit newDataInPortThread(count);
			QTime timedb;
			qDebug()<<"thread count= "<<count<<"time= "<<"\t"<<timedb.currentTime().second()<<" "<<timedb.currentTime().msec();
		}
	}
}

void ReceiveThread::setPort(Qthreadcomport *port)
{
	comport = port;
}

/*Qthreadcomport*/
void Qthreadcomport::close()
{
	stopThread();
	QextSerialPort::close();
}

bool Qthreadcomport::open(QIODevice::OpenMode mode)
{
    thread = new ReceiveThread();
	thread->setPort(this);
	connect(thread,SIGNAL(newDataInPortThread(int)),this,SLOT(newDataInPortSlot(int)));
	bool bopen = QextSerialPort::open(mode);
	thread->start();
	return bopen;
}

qint64 Qthreadcomport::readData(char *data, qint64 maxSize)
{
	qint64 count = QextSerialPort::readData(data, maxSize);
	return count;
}

void Qthreadcomport::stopThread()
{
 if (thread)
 {
	 thread->terminate();
	 thread->wait();
	 delete thread;
	 thread = NULL;
 }
}

/*
==============
<SLOTS>
==============
*/
void Qthreadcomport::newDataInPortSlot(int count)
{
	if ((bytesAvailable() == 0) || (!(isOpen()))) return; 
	const int size = 4096;
	char data[size];
    int readcount = readData(data, count);
	QTime timedb;
	timedb = timedb.currentTime();
	qDebug()<<"readData"<<"time= "<<"\t\t\t"<<timedb.second()<<" "<<timedb.msec();
	data[readcount] = '\0';
	
	unsigned char *dataPtr;
	dataPtr = (unsigned char *)&data;
	emit newDataInPortSignal(timedb, dataPtr,readcount);
}

