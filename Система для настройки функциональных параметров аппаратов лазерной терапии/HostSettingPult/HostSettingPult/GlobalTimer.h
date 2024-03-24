#pragma once

#include <QObject>
#include <QTimer>
#define TIMEOUT_ON_READ 1000
class GlobalTimer : public QObject {
    Q_OBJECT

public:
    static GlobalTimer* instance();
    QTimer timer;
signals:
    void timeout();

public slots:
    void start();
    void stop();
private:
    GlobalTimer(QObject* parent = nullptr);
};
