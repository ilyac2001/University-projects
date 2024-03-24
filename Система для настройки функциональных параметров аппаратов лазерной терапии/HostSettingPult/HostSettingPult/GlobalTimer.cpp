#include "GlobalTimer.h"

GlobalTimer::GlobalTimer(QObject* parent) : QObject(parent) {}

GlobalTimer* GlobalTimer::instance() {
    static GlobalTimer instance;
    return &instance;
}

void GlobalTimer::start() {
    timer.start(TIMEOUT_ON_READ);
}

void GlobalTimer::stop() {
    timer.stop();
    emit &GlobalTimer::timeout;
}
