#include "WorkerCommand.h"

WorkerCommand::WorkerCommand(QObject* parent)
    : QObject(parent), thread_for_execute_commands(new QThread(this))
{
    moveToThread(thread_for_execute_commands);
    connect(thread_for_execute_commands, &QThread::started, this, &WorkerCommand::ExecuteCommands);
}

WorkerCommand::~WorkerCommand()
{
    this->thread_for_execute_commands->quit();
    this->thread_for_execute_commands->wait();
}

void WorkerCommand::AddCommand(std::function<void()> command)
{
    commands.push(command);
}

void WorkerCommand::StartExecuteCommands()
{
    thread_for_execute_commands->start();
}

void WorkerCommand::ExecuteCommands()
{
    QEventLoop eventLoop;
    QTimer* timer_for_transmited = new QTimer(this);
    timer_for_transmited->setInterval(1000);
    QMetaObject::Connection connection = connect(timer_for_transmited, &QTimer::timeout, [&eventLoop, this]() {
        if (transmit_status.load() == TransmitStatus::Sending) {
            qDebug() << "TIMEOUT!!!";
            transmit_status = TransmitStatus::Timeout;
        }
        eventLoop.quit();
        });

    while (!commands.empty()){
        qDebug() << "Execute command:";
        this->commands.front()();
        this->commands.pop();
        timer_for_transmited->start();
        while (transmit_status.load() == TransmitStatus::Sending && timer_for_transmited->isActive()) {
            eventLoop.processEvents();
        }
        timer_for_transmited->stop();

        if (transmit_status.load() == TransmitStatus::Timeout) {
            disconnect(connection);
            emit isExeption();
            return;
        }

        if (transmit_status.load() == TransmitStatus::Complite) {
            disconnect(connection);
            return;
        }
    }
    disconnect(connection);
    this->transmit_status = TransmitStatus::Complite;
    qInfo("complite!");
}
