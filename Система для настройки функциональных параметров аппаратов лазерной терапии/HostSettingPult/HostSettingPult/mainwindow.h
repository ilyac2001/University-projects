#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QPushButton>

#define SETTING_MIN_POWER false
#define SETTING_MAX_POWER true

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void SetLableConnectStatusChannle(bool);
    void SetLableProgresBar();
    void ResetRadioBtn();
    std::tuple<bool, unsigned char> GetValuePower();
public slots:
    void ChangeLableConnectStatus(bool status); //true - Подключено, false - Не подключено
    void SetLableNameChannle(QString name, QString version, QString min_power, QString max_power);
    void SetSetNewMinMaxValueLablePower(unsigned char min, unsigned char max);
private:
    Ui::MainWindow *ui;
    void closeEvent(QCloseEvent *event);
    void reset();

    bool SetValuePower(std::function<void(unsigned char&)> callback);
    void incrementValue(unsigned char& value);
    void decrementValue(unsigned char& value);
    bool Convert(unsigned char& ucharValue, QLabel& LabelPower);
    bool Convert(QLabel& LabelPower, unsigned char& ucharValue);
    bool CheckValuePower(unsigned char min, unsigned char max);
signals:
    void isCloseWindow(void);
    void isClickStop(void);
    void isClickSetPower();
    void isClickSetValuePower();
    void isSetNewMinMaxValueLablePower(unsigned char min, unsigned char max);
};
#endif // MAINWINDOW_H
