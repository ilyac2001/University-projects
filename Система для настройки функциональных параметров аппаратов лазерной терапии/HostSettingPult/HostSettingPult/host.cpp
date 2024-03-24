#include "host.h"
/**
 * @file host.cpp
 * @brief Presenter::Presenter
 * @param view
 * @param model
 * @param parent
 */

Presenter::Presenter( MainWindow* view, HostUsbCustomHid* model, QObject *parent)
    : QObject{parent}
{
    this->view = view;
    this->model = model;
    this->SetState<StateOn>();
    //связь model и view на отображение состояния системы о соединении с пультом и повторном подключении
    QObject::connect(this->model, SIGNAL(isConnected(bool)),
                     this->view, SLOT(ChangeLableConnectStatus(bool)));
    QObject::connect(this->model, SIGNAL(isConnected(bool)),
                     this, SLOT(ConnectModel(bool)));
    //подключение в отдельном потоке
    this->model->QThread::moveToThread(this->thread_ConnectModel);
    QObject::connect(this->thread_ConnectModel, SIGNAL(started()),
                     this->model, SLOT(ConnectUsbDevice()));
    QObject::connect(this->model, SIGNAL(isConnected(bool)),
                     this->thread_ConnectModel, SLOT(quit()));
    //дать указания после чтения
    QObject::connect(this->model, SIGNAL(isRead(QString)),
                     this, SLOT(ReadModel(QString)));
    //подключение к ui
    connect(this->view, &MainWindow::isSetNewMinMaxValueLablePower, [=](unsigned char min, unsigned char max) {
        if (typeid(*current_state.get()) == typeid(StatePause)) {
            this->view->SetSetNewMinMaxValueLablePower(min, max);
        }
        });
    connect(this->view, &MainWindow::isClickStop, this, &Presenter::StopChannel);
    connect(this->view, &MainWindow::isClickSetPower, this, &Presenter::SetPowerChannel);
    connect(this->view, &MainWindow::isClickSetValuePower, this, &Presenter::WritePowerChannel);
}

void Presenter::ConnectModel(bool status_signal)
{
    this->thread_ConnectModel->usleep(10000); //на полной скорости в неожиданный момент времени поток может упасть или не сработать на сигнал, почему хз, но так кажется работатет
    if (status_signal == false)
    {
        if (this->current_state) {
            disconnect(this->current_state.get(), &State::isNoExecutedCommand, this, &Presenter::Restart);
        }
        this->thread_ConnectModel->start();
    }
    else
    {
        this->Restart();
    }
}

void Presenter::ConnectPult() {
    this->model->ConnectUsbDevice();
}

void Presenter::WriteInModel()
{
    QByteArray b_send(SIZE_BUFFER_WRITE_DATA, (char)0x00);
    if(this->current_state != nullptr)
    {
        for(int i = 0; i < __min(SIZE_BUFFER_WRITE_DATA, SIZE_PACKAGE); i++)
        {
            b_send[i] = this->current_state->twi.GetElementPackage(i);
        }
    }
    this->model->WriteData(b_send);
}
//преполагает под собой чтение данных от model, дать указания view и state
void Presenter::ReadModel(QString answer)
{
    //текущее сотсояние не проверяется на null т.к. ответ ни при каких обстоятельствах не может прийти в таком случае
    answer = answer.remove(0, 1); //передать строку(без usb id)
    CopyDataPackage(answer);
    if (answer[IndexElementSentPackageCodeCommand] == this->current_state->twi.GetElementPackage(IndexElementSentPackageCodeCommand)
        && answer[IndexElementSentPackageChecksum] == this->current_state->twi.GetElementPackage(IndexElementSentPackageChecksum)) {
        DefineState(this->current_state->Check());
    }
    else {
        Restart();
    }
}

void Presenter::CopyDataPackage(QString package)
{
    for (int i = 1; i < package.size() - 1; i++) //копирование пакета без КС и названия команды (первый и последний элементы, КС - пересичтывается и сранивается, код команды ответа должен совпадать с запросом)
    {
        QChar charAtIndex = package.at(i);
        unsigned char value = static_cast<unsigned char>(charAtIndex.toLatin1());
        this->current_state->twi.SetElementPackage(value, i);
    }
}


//вызывать после получения ответа или на действия пользователя (но в таком случае чек-лист проверить)
void Presenter::DefineState(NameState result_check)
{
    switch (result_check)
    {
    case OK:
        qInfo("Смена состояния на StateON");
        this->SetState<StateOn>();
        break;
    case ReadNameChannle:
        qInfo("смена состояния на чтение названия");
        this->view->SetLableConnectStatusChannle(true);
        this->SetState<StateReadChannel>();
        QObject::connect(this->current_state.get(), SIGNAL(isGetName(QString, QString, QString, QString)),
            this->view, SLOT(SetLableNameChannle(QString, QString, QString, QString)));
        break;
    case PowerOn:
        qInfo() << "\n------\n" << "ЗАПУСТИТЬ ЛАЗЕР: " << std::get<0>(this->view->GetValuePower()) << "\n------\n";
        this->SetState<StatePowerOn>();
        if (auto statePowerOn = dynamic_cast<StatePowerOn*>(this->current_state.get())) {
            statePowerOn->SetPower(std::get<0>(this->view->GetValuePower()));
        }
        break;
    case WritePowerChannle:
        qInfo() << "\n------\n" << "ЗАПИСАТЬ ЗНАЧЕНИЕ: " << std::get<1>(this->view->GetValuePower()) << "\n------\n";
        this->SetState<StateWritePowerChannle>();
        if (auto stateWritePowerChannle = dynamic_cast<StateWritePowerChannle*>(this->current_state.get())) {
            stateWritePowerChannle->SetPower(std::get<0>(this->view->GetValuePower()), std::get<1>(this->view->GetValuePower()));
        }
        break;
    case StopChannle:
        this->SetState<StateStopChannle>();
        break;
    case Pause:
        this->SetState<StatePause>();
        this->connection = connect(this->current_state.get(), &StatePause::doRequest, this, &Presenter::Request);
        qInfo("ожидание изменения состояния из вне");
        return;
    case Continue:
        qInfo("не надо менять состояние, ВЫПОЛНЯЮТСЯ команды текущего состояния");
        return;
    default:
        Restart();
        return;
    }
    this->Request();
}

template <typename T>
void Presenter::SetState()
{
    if (this->current_state) {
        disconnect(this->current_state.get(), &State::isNoExecutedCommand, this, &Presenter::Restart);
    }
    this->current_state.reset();
    this->current_state = std::make_unique<T>();
    connect(this->current_state.get(), &State::isNoExecutedCommand, this, &Presenter::Restart);
}
//Базовое сотсояние, отвалилась головка
void Presenter::Restart()
{
    qInfo("RESTART!!!");
    this->view->SetLableConnectStatusChannle(false);
    DefineState(NameState::OK);
}
//вызывается Handle текущего состояния
void Presenter::Request()
{
    if(this->current_state != nullptr) {
        this->current_state->Handle(std::bind(&Presenter::WriteInModel, this));
    } else {
        this->Restart();
    }
}

void Presenter::SetPowerChannel()
{
    if (typeid(*current_state.get()) == typeid(StatePause)) {
        this->view->SetLableProgresBar();
        DefineState(NameState::PowerOn);
    }
}

void Presenter::WritePowerChannel()
{
    if (typeid(*current_state.get()) == typeid(StatePause)) {
        this->view->SetLableProgresBar();
        DefineState(NameState::WritePowerChannle);
    }
}

void Presenter::StopChannel()
{
    if (typeid(*current_state.get()) == typeid(StatePause)) {
        DefineState(NameState::StopChannle);
        this->view->ResetRadioBtn();
    }
}