#ifndef HOST_H
#define HOST_H

#include <QObject>
#include <QThread>

#include "mainwindow.h"
#include "hostusbcustomhid.h"
#include "state.h"
/**
 * @brief The Presenter class - управляет проектом, связывает view и model, выступает вкачестве context для state
 \par Использует классы:
 - @ref MainWisow
 - @ref HostUsbCustomHid
 - @ref State
 - @ref QThread
 \par Использует файлы
 - @ref QObject
 - @ref QThread
 - @ref mainwindow.h
 - @ref hostusbcustomhid.h
 - @ref state.h
 */
///Класс описание Presenter и Context (паттерны MVP и State)
/** Содержит ссылки на view и model
 */

class Presenter : public QObject
{
    Q_OBJECT
public:
    ///Конструктор класса
    /**
     * @brief Presenter
     * @param view Окно main
     * @param model Работа с HID
     * @param parent
     */
    explicit Presenter(MainWindow* view, HostUsbCustomHid* model, QObject* parent = nullptr);
    /**
    * @brief ConnectPult Подключение к HID
    */
    void ConnectPult();
private:
    /**
     * @brief SetState определяет новое состояние системы
     * @param state Состояние - наследник State
     */
    template <typename T>
    void SetState();
    void CopyDataPackage(QString package);
    void DefineState(NameState result_check);
private slots:
    void ConnectModel(bool status_signal);
    void WriteInModel();
    void ReadModel(QString);
    void Restart();
    void Request();
    void SetPowerChannel();
    void WritePowerChannel();
    void StopChannel();

private:
    MainWindow* view;           ///< Представление (работа с интерфейсом пользователя)
    HostUsbCustomHid* model;    ///< Модель (работа с аппаратным интерфейсом usbHID)
    std::unique_ptr<State> current_state;
    QThread* thread_ConnectModel = new QThread();   ///< Поток подключения к hid
    QThread* thread_ReadModel = new QThread();      ///< Поток чтения hid
};

#endif // HOST_H
