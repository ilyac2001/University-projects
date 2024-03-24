#ifndef WORKERCOMMAND_H
#define WORKERCOMMAND_H

#include <queue>
#include <functional>
#include <QTimer>
#include <QtConcurrent>
#include <atomic>

enum class TransmitStatus
{
    Sending,
    AnswerReceived,
    Timeout,
    Complite,
    EX
};

class WorkerCommand : public QObject
{
    Q_OBJECT
public:
    explicit WorkerCommand(QObject* parent = nullptr);
    ~WorkerCommand();
    void AddCommand(std::function<void()> command);
    void StartExecuteCommands();
    std::atomic<TransmitStatus> transmit_status;
private:
    std::queue<std::function<void()>> commands;
    QThread* thread_for_execute_commands;
signals:
    void isExeption();
private  slots:
    void ExecuteCommands();
};

#endif // WORKERCOMMAND_H
