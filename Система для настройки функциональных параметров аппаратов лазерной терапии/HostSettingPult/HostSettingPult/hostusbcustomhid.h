#ifndef HOSTUSBCUSTOMHID_H
#define HOSTUSBCUSTOMHID_H

#include <QObject>
#include <QThread>
#include <QHidDevice>

#define SETTING_PULT_VID 0x0483
#define SETTING_PULT_PID 0x5750
//размер определяется протоколом
#define SIZE_BUFFER_READ_DATA (6 - 1)
#define SIZE_BUFFER_WRITE_DATA (6 - 1)

#define ID_BUFFER_READ_DATA 0x01
#define ID_BUFFER_WRITE_DATA 0x02
/**
 * @brief The HostUsbCustomHid class - Model
 * @ref HostUsbCustomHid
 * Класс работы с пультом, использует библиотеку QtUSB https://github.com/fpoussin/QtUsb
 */
class HostUsbCustomHid : public QObject, public QThread
{
    Q_OBJECT
public:
    explicit HostUsbCustomHid(const short int VID, const short int PID, QObject *parent = nullptr);
    ~HostUsbCustomHid();

private:
    short int VID, PID; ///< индентификаторы HID устройства, определяется производителем
    QHidDevice* my_hid_dev;
    QByteArray my_send, my_recv;
    QThread* thread_ReadData = new QThread();
    void run();

public slots:
    /**
     * @brief ConnectUsbDevice - подключение к устройству, VID и PID определяется при создании объекта
     */
    void ConnectUsbDevice(void);
    /**
     * @brief WriteData - запись данных в USB,
     * @param buf - буфер для записи
     */
    void WriteData(QByteArray buf);
    /**
     * @brief ReadData - чтение данных
     * @throw Отключение hid во время чтения
     * Пример чтения и обработки исключения
     * @code
        this->my_hid_dev->read(&buf, buf.size()); //программа не выполняется дальше, если читать нечего
        if((str == "") & (buf.size() == 0)) //при отключении читается 0, исключительная ситуаци
        {
            this->my_hid_dev->close();
            emit isConnected(false);
        }
     */
    void ReadData();

signals:
    void isConnected(bool); //true - Подключение, false - Отключение
    void isRead(QString);
};

#endif // HOSTUSBCUSTOMHID_H
