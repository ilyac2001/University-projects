#include <QApplication>
#include <QTimer>
#include "host.h"

///Точка входа в программу
/**
 * @mainpage Приложение для настройки исполняемых блоков
Состоит из следующих частей:
- @ref Presenter - определяет действия системы
- @ref HostUsbCustomHid - обмен данными с пультом - HID
- @ref View - пользовательский интерфейс
- @ref State - состояние системы (настраиваемого блока)
 * @author Ilya Tsimerman
 * @copyright ООО Бином
 * @date 01-06-2023
*/

/**
 * @page Точка входа в программу
 * Создание объектов view и model, передача их в конструктор класса Presenter. Подключение к пульту
Пример использования MVP:
@code
    MainWindow* host_window = new MainWindow();
    HostUsbCustomHid* host_setting_pult = new HostUsbCustomHid(SETTING_PULT_VID, SETTING_PULT_PID);
    Presenter host_context(host_window, host_setting_pult);
@endcode
*/

/**
 * @file main.cpp
 * @brief main
 * @param argc
 * @param argv
 * @return
 */
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    qInfo("Run");
    MainWindow* host_window = new MainWindow();
    host_window->setWindowTitle("Setting pult");
    HostUsbCustomHid* host_setting_pult = new HostUsbCustomHid(SETTING_PULT_VID, SETTING_PULT_PID);
    host_window->show();
    Presenter host_context(host_window, host_setting_pult);
    host_context.ConnectPult();

    return a.exec();
}
