#ifndef STATE_H
#define STATE_H

#include "twowireinterface.h"
#include "WorkerCommand.h"

enum NameState
{
    OK,
    ReadNameChannle,
    PowerOn,
    WritePowerChannle,
    StopChannle,
    Continue,
    Pause,
    EX
};

class State : public QObject
{
    Q_OBJECT
public:
    explicit State(QObject* parent = nullptr);
    virtual void Handle(std::function<void()> callback) = 0;
    virtual NameState Check() = 0;
    TwoWireInterface twi;
    std::unique_ptr<WorkerCommand> wc;
    /*
    * у переходов между состояниями есть чёткий порядок
    * при запуске/перезапуске включить->прочитать название->перейти в состояние паузы
    * перезапуск проиходит при любой ошибке
    * в состоянии паузы каждую секунду идёт опрос (команда) после которого определяется следующее состояние
    * следующее состояние определяет presenter
    * после завершения команд следующее состояние - пауза
    */
protected:
    QTimer* timer_for_read;
    NameState SetNextNameState(TransmitStatus transmit_status, NameState next_name_state);
    void WaitCompliteOrStartTransmit();
signals:
    void isNoExecutedCommand();
    void doRequest();
public slots:
    void EmitException();
};

class StateOn : public State
{
    Q_OBJECT
public:
    using State::State;
    NameState Check() override;
protected:
    void Handle(std::function<void()> callback) override;
};

class StatePause : public StateOn
{
    Q_OBJECT
public:
    explicit StatePause(QObject* parent = nullptr);
    NameState Check() override;
private:
    void Handle(std::function<void()> callback) override;
    QTimer* timer_for_handle;
};

class StateReadChannel : public State
{
    Q_OBJECT
public:
    using State::State;
    NameState Check() override;
    QString name;
    QString version;
    QString min_power;
    QString max_power;
    QString temp;
private:
    void Handle(std::function<void()> callback) override;
    unsigned char shift_name = 0;
    unsigned char shift_version = 0;
    QString* pointer_str = &name;
signals:
    void isGetName(QString name, QString version, QString min_power, QString max_power);
};

class StatePowerOn : public State
{
    Q_OBJECT
public:
    using State::State;
    NameState Check() override;
    void SetPower(bool power_min_max);
protected:
    void Handle(std::function<void()> callback) override;
private:
    bool power;
};

class StateWritePowerChannle : public State
{
    Q_OBJECT
public:
    using State::State;
    NameState Check() override;
    void SetPower(bool power_min_max, unsigned char value);
protected:
    void Handle(std::function<void()> callback) override;
private:
    bool power;
    unsigned char value;
};

class StateStopChannle : public State
{
    Q_OBJECT
public:
    using State::State;
    NameState Check() override;
protected:
    void Handle(std::function<void()> callback) override;
};
#endif // STATE_H
