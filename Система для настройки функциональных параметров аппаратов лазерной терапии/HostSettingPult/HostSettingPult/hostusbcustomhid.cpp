#include "hostusbcustomhid.h"

HostUsbCustomHid::HostUsbCustomHid(const short int VID, const short int PID, QObject *parent)
    : QObject{parent}, my_hid_dev(new QHidDevice())
{
    this->VID = VID;
    this->PID = PID;
    this->my_recv.resize(SIZE_BUFFER_READ_DATA);
    this->my_send.resize(SIZE_BUFFER_WRITE_DATA);
}

HostUsbCustomHid::~HostUsbCustomHid()
{
    if(this->my_hid_dev->isOpen())
    {
        qInfo("Disconnect!");
        this->my_hid_dev->close();
        if(this->QThread::isRunning())
        {
            //this->QThread::wait();
            //this->QThread::quit();
            this->QThread::terminate();
        }
    }
}

void HostUsbCustomHid::ConnectUsbDevice() //запускает чтение
{
    if (this->my_hid_dev->open(this->VID, this->PID)) {
        qInfo("Connect!");
        this->QThread::start();
        emit isConnected(true);
    } else {
        qWarning("Could not connect device!");
        emit isConnected(false);
    }
}

void HostUsbCustomHid::WriteData(QByteArray buf)
{
    buf.prepend(ID_BUFFER_WRITE_DATA);
    if(this->my_hid_dev->isOpen())
    {
        qDebug() << "Writing: " << buf << " " << buf.size();
        if (my_hid_dev->write(&(QByteArray)buf, buf.size()) < 0)
        {
            qWarning("Write failed!");
        }
    }
    else
    {
        emit isConnected(false);
    }
}

void HostUsbCustomHid::ReadData()
{
    if(this->my_hid_dev->isOpen())
    {
        QByteArray buf(SIZE_BUFFER_READ_DATA, (char)0x00);
        buf.prepend(ID_BUFFER_READ_DATA);
        QString str = "";

        this->my_hid_dev->read(&buf, buf.size()); //программа не выполняется дальше, если читать нечего
        this->my_recv = buf;
        qDebug() << "Reading: " << buf << buf.size();

        if ((str == "") && (buf.size() == 0)) //при отключении читается 0
        {
            this->my_hid_dev->close();
            emit isConnected(false);
        }
        else
        {
            for(int i = 0; i < buf.size(); i++)
            {
                str += buf[i]; //что-то тут с типами данных, но как будто норм (в консоль выводится 16-формат, а в Lable ASCI)
            }
            emit isRead(str);
        }
    }
}

void HostUsbCustomHid::run()
{
    while(this->my_hid_dev->isOpen())
    {
        this->ReadData();
    }
}
