#include "mainwindow.h"
#include "ui_mainwindow.h"
/**
 * @file mainwindow.cpp
 * @brief MainWindow::MainWindow
 * @param parent
 */
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->progressBar->setRange(0, 255);
    connect(ui->pushButton_STOP, &QPushButton::clicked, [=]() {
        ResetRadioBtn();
        emit isClickStop();
        });

    connect(ui->radioButton_MinPower, &QPushButton::toggled, [=](bool checked) {
        if (checked) {
            emit isClickSetPower();
        }
        });

    connect(ui->radioButton_MaxPower, &QPushButton::toggled, [=](bool checked) {
        if (checked) {
            emit isClickSetPower();
        }
        });

    connect(ui->pushButton_APP, &QPushButton::clicked, [=]() {
        if (SetValuePower(std::bind(&MainWindow::incrementValue, this, std::placeholders::_1))) {
            SetLableProgresBar();
            emit isClickSetValuePower();
        }
        });

    connect(ui->pushButton_DOWN, &QPushButton::clicked, [=]() {
        if (SetValuePower(std::bind(&MainWindow::decrementValue, this, std::placeholders::_1))) {
            SetLableProgresBar();
            emit isClickSetValuePower();
        }
        });
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::SetLableConnectStatusChannle(bool status)
{
    reset();
    if (status)
    {
        this->ui->lable_Name->setText("<FONT COLOR=#008000>Загрузка</FONT>");
    }
    else
    {
        this->ui->lable_Name->setText("<FONT COLOR=#800000>Нет устройства</FONT>");
    }
}

void MainWindow::ChangeLableConnectStatus(bool status)
{
    if (status)
    {
        ui->lable_Name->setText("<FONT COLOR=#008000></FONT>");
    }
    else
    {
        ui->lable_Name->setText("<FONT COLOR=#800000>Нет пульта</FONT>");
    }
    reset();
}

void MainWindow::SetLableNameChannle(QString name, QString version, QString min_power, QString max_power)
{
    this->ui->lable_Name->setText(name);
    this->ui->lable_Version->setText(version);
    this->ui->lable_ValueMinPower->setText(min_power);
    this->ui->lable_ValueMaxPower->setText(max_power);
    ResetRadioBtn();
}

void MainWindow::SetSetNewMinMaxValueLablePower(unsigned char min, unsigned char max)
{
    Convert(*ui->lable_ValueMinPower, min);
    Convert(*ui->lable_ValueMaxPower, max);
}

void MainWindow::SetLableProgresBar()
{
    unsigned char value = 0;
    if (ui->radioButton_MinPower->isChecked()) {
        this->ui->label_ValueProgressBar->setText(this->ui->lable_ValueMinPower->text());
    }
    else if (ui->radioButton_MaxPower->isChecked()) {
        this->ui->label_ValueProgressBar->setText(this->ui->lable_ValueMaxPower->text());
    }
    else {
        this->ui->label_ValueProgressBar->setText("");
    }
    Convert(value, *ui->label_ValueProgressBar);
    ui->progressBar->setValue(value);
}

void MainWindow::ResetRadioBtn()
{
    this->ui->radioButton_MinPower->setAutoExclusive(false);
    this->ui->radioButton_MaxPower->setAutoExclusive(false);
    this->ui->radioButton_MinPower->setChecked(false);
    this->ui->radioButton_MaxPower->setChecked(false);
    this->ui->radioButton_MinPower->setAutoExclusive(true);
    this->ui->radioButton_MaxPower->setAutoExclusive(true);
    SetLableProgresBar();
}

void MainWindow::closeEvent(QCloseEvent* event)
{
    emit isCloseWindow();
}

void MainWindow::reset()
{
    this->ui->lable_Version->setText("");
    this->ui->lable_ValueMinPower->setText("");
    this->ui->lable_ValueMaxPower->setText("");
    ResetRadioBtn();
}

bool MainWindow::SetValuePower(std::function<void(unsigned char&)> callback)
{
    unsigned char ucharMinPower, ucharMaxPower;
    if (Convert(ucharMinPower, *ui->lable_ValueMinPower) && Convert(ucharMaxPower, *ui->lable_ValueMaxPower)) {
        if (ui->radioButton_MinPower->isChecked()) {
            callback(ucharMinPower);
        }
        else if (ui->radioButton_MaxPower->isChecked()) {
            callback(ucharMaxPower);
        }
        if (CheckValuePower(ucharMinPower, ucharMaxPower)) {
            emit isSetNewMinMaxValueLablePower(ucharMinPower, ucharMaxPower);
            return true;
        }
    }
    return false;
}

void MainWindow::incrementValue(unsigned char& value)
{
    ++value;
}

void MainWindow::decrementValue(unsigned char& value)
{
    --value;
}

bool MainWindow::Convert(unsigned char& ucharValue, QLabel& LabelPower)
{
    bool ok;
    QString strValue = LabelPower.text();
    ucharValue = static_cast<unsigned char>(strValue.toUShort(&ok));
    return ok;
}

bool MainWindow::Convert(QLabel& LabelPower, unsigned char& ucharValue)
{
    QString strValue = QString::number(ucharValue);
    LabelPower.setText(strValue);
    return true;
}

bool MainWindow::CheckValuePower(unsigned char min, unsigned char max)
{
    if (ui->radioButton_MinPower->isChecked() || ui->radioButton_MaxPower->isChecked()) {
        if (min >= 5 && max <= 250 && min < max) {
            return true;
        }
    }
    return false;
}

std::tuple<bool, unsigned char> MainWindow::GetValuePower()
{
    bool setting_min_max_power;
    unsigned char value_power;
    if (Convert(value_power, *ui->label_ValueProgressBar)) {
        if (ui->radioButton_MinPower->isChecked()) {
            setting_min_max_power = SETTING_MIN_POWER;
        }
        else if (ui->radioButton_MaxPower->isChecked()) {
            setting_min_max_power = SETTING_MAX_POWER;
        }
    }
    else {
        throw std::runtime_error("в lable не число!");
    }

    return std::make_tuple(setting_min_max_power, value_power);
}
