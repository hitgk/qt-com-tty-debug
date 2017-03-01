/*

	threadcomport v.0.0.1
	author Golubkin Egor (Gorin), Russia
	///////////////////////////////////////////////

	Multiplatform asynchronous Serial Port Extension
	based on Wayne Roth's QExtSerialPort
	

*/ 

 #ifndef THREADCOMPORT_H	
 #define THREADCOMPORT_H

 #include <qextserialport.h>

 #include <QThread>
 #include <QTime>
 #include <QMutex>

class Qthreadcomport;
class ReceiveThread;
class QMainComThread;

class QMainComThread : public QThread
{
	Q_OBJECT
public:
	QMainComThread(QString name, const PortSettings *settings);
	~QMainComThread();
	Qthreadcomport* getPort();//return * on Qthreadcomport, used for set settings
protected:
	PortSettings *comsettings;
	Qthreadcomport *comport;
	QString comname;
	void run();
};

class Qthreadcomport : public QextSerialPort {
	Q_OBJECT
public:
	Qthreadcomport(const PortSettings *settings = 0);
	~Qthreadcomport();
	virtual void close();
	virtual bool open(QIODevice::OpenMode mode=QIODevice::ReadWrite);
	virtual qint64 readData(char *data, qint64 maxSize);

signals:
	void newDataInPortSignal(const QTime timesl, const unsigned char *data, const int size);

private:
	ReceiveThread *thread;
	void stopThread();

private slots:
	void newDataInPortSlot(int count);

};

class ReceiveThread : public QThread
{
	Q_OBJECT
public:
	ReceiveThread();
	void setPort(Qthreadcomport *port);

signals:
	void newDataInPortThread(int count);

protected:
	void run();

private:
	QMutex mutex;
	Qthreadcomport *comport;
	int lastcount;
};

 #endif