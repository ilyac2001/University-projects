#include <QMainWindow>

#include "state.h"
#include "host.h"

State::State(QObject* parent)
    : QObject(parent)
{
    this->wc = std::make_unique<WorkerCommand>();
    connect(this->wc.get(), &WorkerCommand::isExeption, this, &State::EmitException);
}

void State::EmitException() {
    emit isNoExecutedCommand();
}

NameState State::SetNextNameState(TransmitStatus transmit_status, NameState next_name_state)
{
    NameState result = EX;
    switch (transmit_status)
    {
    case TransmitStatus::Sending:
        result = Continue;
        break;
    case TransmitStatus::Complite:
        this->wc = std::make_unique<WorkerCommand>();
        connect(this->wc.get(), &WorkerCommand::isExeption, this, &State::EmitException);
        result = next_name_state;
        break;
    default: //Timeout, EX
        result = EX;
        break;
    }
    return result;
}

void State::WaitCompliteOrStartTransmit()
{
    QEventLoop eventLoop;
    QTimer* timer = new QTimer(this);
    timer->setInterval(500);
    connect(timer, &QTimer::timeout, this, [this, timer, &eventLoop]() {
        this->wc->transmit_status = TransmitStatus::Timeout;
        qInfo("Какая-то ошибка при отправке или отслеживания таймаута, ошибка в работе класса WorkerCommand!");
        timer->stop();
        timer->deleteLater();
        eventLoop.quit();
        });

    timer->start();
    while (this->wc->transmit_status.load() == TransmitStatus::AnswerReceived && timer->isActive()) {
        eventLoop.processEvents();
    }
    timer->stop();
}

void StateOn::Handle(std::function<void()> callback)
{
    this->wc->AddCommand([this, callback]()
        {
            twi.SetPackageGetStatus(); //сформировать пакет
            callback();
            this->wc->transmit_status = TransmitStatus::Sending;
        });
    this->wc->StartExecuteCommands();
}

NameState StateOn::Check()
{
    this->wc->transmit_status = TransmitStatus::AnswerReceived;
    this->WaitCompliteOrStartTransmit();
    return SetNextNameState(this->wc->transmit_status, ReadNameChannle);
}

void StateReadChannel::Handle(std::function<void()> callback)
{
    this->shift_name = 0;
    this->shift_version = 0;
    this->wc->AddCommand([this, callback]()
        {
            this->pointer_str = &this->name;
            this->wc->transmit_status = TransmitStatus::AnswerReceived;
        });
    for (int i = 0; i < SIZE_BYTE_READ_NAME; i++) {
        this->wc->AddCommand([this, callback]()
            {
                twi.SetPackageReadEEPROM(START_LOW_BYTE_READ_NAME, START_HI_BYTE_READ_NAME);
                twi.SetElementPackage(START_LOW_BYTE_READ_NAME + this->shift_name, IndexElementSentPackageLowestByteData);
                callback();
                this->shift_name++;
                this->wc->transmit_status = TransmitStatus::Sending;
            });
    }
    this->wc->AddCommand([this, callback]()
        {
            qInfo() << "---ИМЯ: " << *this->pointer_str;
            this->pointer_str = &this->version;
            this->wc->transmit_status = TransmitStatus::AnswerReceived;
        });
    for (int i = 0; i < SIZE_BYTE_READ_VERSION; i++) {
        this->wc->AddCommand([this, callback]()
            {
                twi.SetPackageReadEEPROM(START_LOW_BYTE_READ_VERSION, START_HI_BYTE_READ_VERSION);
                twi.SetElementPackage(START_LOW_BYTE_READ_VERSION + this->shift_version, IndexElementSentPackageLowestByteData);
                callback();
                this->shift_version++;
                this->wc->transmit_status = TransmitStatus::Sending;
            });
    }
    this->wc->AddCommand([this, callback]()
        {
            qInfo() << "---ВЕРСИЯ: " << *this->pointer_str;
            this->pointer_str = &this->min_power;
            twi.SetPackageReadEEPROM(START_LOW_BYTE_MIN_POWER, START_HI_BYTE_MIN_POWER);
            callback();
            this->wc->transmit_status = TransmitStatus::Sending;
        });
    this->wc->AddCommand([this, callback]()
        {
            this->pointer_str = &this->max_power;
            twi.SetPackageReadEEPROM(START_LOW_BYTE_MAX_POWER, START_HI_BYTE_MAX_POWER);
            callback();
            this->wc->transmit_status = TransmitStatus::Sending;
        });
    wc->StartExecuteCommands();
}

NameState StateReadChannel::Check()
{
    if (twi.GetElementPackage(IndexElementSentPackageHighestByteData) == 0x00) {
        this->pointer_str = &temp;
    }
    unsigned char element = this->twi.GetElementPackage(IndexElementSentPackageHighestByteData);
    if (this->pointer_str == &name || this->pointer_str == &version) {
        *this->pointer_str += this->twi.charMap.value(element);
    }
    else if (this->pointer_str == &min_power || this->pointer_str == &max_power) {
        *this->pointer_str += QString::number(static_cast<int>(element));
    }
    this->wc->transmit_status = TransmitStatus::AnswerReceived;
    this->WaitCompliteOrStartTransmit();
    if (this->wc->transmit_status == TransmitStatus::Complite) {
        emit isGetName(this->name, this->version, this->min_power, this->max_power);
    }
    return SetNextNameState(this->wc->transmit_status, Pause);
}

StatePause::StatePause(QObject* parent)
    : StateOn(parent)
{
    timer_for_handle = new QTimer(this);
    timer_for_handle->setInterval(1100);
    connect(timer_for_handle, &QTimer::timeout, [=]() {
        qInfo("Проверка связи...");
        emit doRequest();
        timer_for_handle->start();
        });
    timer_for_handle->start();
}

NameState StatePause::Check()
{
    this->wc->transmit_status = TransmitStatus::AnswerReceived;
    this->WaitCompliteOrStartTransmit();
    return SetNextNameState(this->wc->transmit_status, Continue);
}

void StatePause::Handle(std::function<void()> callback)
{
    StateOn::Handle(callback);
}

void StatePowerOn::Handle(std::function<void()> callback)
{
    this->wc->AddCommand([this, callback]()
        {
            twi.SetPackageSetFrequency();
            callback();
            this->wc->transmit_status = TransmitStatus::Sending;
        });
    this->wc->AddCommand([this, callback]()
        {
            if (power == SETTING_MIN_POWER) {
                twi.SetPackageSetMinPower();
            }
            else if (power == SETTING_MAX_POWER) {
                twi.SetPackageSetMaxPower();
            }
            callback();
            this->wc->transmit_status = TransmitStatus::Sending;
        });
    this->wc->AddCommand([this, callback]()
        {
            twi.SetPackageStart();
            callback();
            this->wc->transmit_status = TransmitStatus::Sending;
        });
    this->wc->StartExecuteCommands();
}


NameState StatePowerOn::Check()
{
    this->wc->transmit_status = TransmitStatus::AnswerReceived;
    this->WaitCompliteOrStartTransmit();
    return SetNextNameState(this->wc->transmit_status, Pause);
}

void StatePowerOn::SetPower(bool power_min_max)
{
    this->power = power_min_max;
}

NameState StateWritePowerChannle::Check()
{
    this->wc->transmit_status = TransmitStatus::AnswerReceived;
    this->WaitCompliteOrStartTransmit();
    return SetNextNameState(this->wc->transmit_status, Pause);
}

void StateWritePowerChannle::SetPower(bool power_min_max, unsigned char value)
{
    this->power = power_min_max;
    this->value = value;
}

void StateWritePowerChannle::Handle(std::function<void()> callback)
{
    this->wc->AddCommand([this, callback]()
        {
            if (power == SETTING_MIN_POWER) {
                twi.SetPackageWriteEEPROM(WRITE_LOW_BYTE_MIN_POWER, value);
            }
            else if (power == SETTING_MAX_POWER) {
                twi.SetPackageWriteEEPROM(WRITE_LOW_BYTE_MAX_POWER, value);
            }
            callback();
            this->wc->transmit_status = TransmitStatus::Sending;
        });
    this->wc->AddCommand([this, callback]()
        {
            if (power == SETTING_MIN_POWER) {
                twi.SetPackageSetMinPower();
            }
            else if (power == SETTING_MAX_POWER) {
                twi.SetPackageSetMaxPower();
            }
            callback();
            this->wc->transmit_status = TransmitStatus::Sending;
        });
    this->wc->StartExecuteCommands();
}

NameState StateStopChannle::Check()
{
    this->wc->transmit_status = TransmitStatus::AnswerReceived;
    this->WaitCompliteOrStartTransmit();
    return SetNextNameState(this->wc->transmit_status, Pause);
}

void StateStopChannle::Handle(std::function<void()> callback)
{
    this->wc->AddCommand([this, callback]()
        {
            twi.SetPackageStop();
            callback();
            this->wc->transmit_status = TransmitStatus::Sending;
        });
    this->wc->StartExecuteCommands();
}
