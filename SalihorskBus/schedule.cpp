#include "schedule.h"

Schedule::Schedule(QString name) : name_(name) {
}


QString Schedule::updateDate() const {
    return updateDate_;
}

void Schedule::setUpdateDate(const QString& updateDate) {
    updateDate_ = updateDate;
}

QJsonObject Schedule::toJsonObject() const {
    QJsonObject schedule;
    schedule["name"] = name_;
    schedule["update date"] = updateDate_;
    QJsonArray busesArray;
    for (size_t i = 0; i < buses_.size(); i++) {
        busesArray.append(buses_.at(i).toJsonObject());
    }
    schedule["buses"] = busesArray;
    return schedule;
}

QString Schedule::name() const {
    return name_;
}

void Schedule::setName(const QString& name) {
    name_ = name;
}

void Schedule::clearBuses() {
    buses_.clear();
}


QList<Bus> Schedule::buses() const {
    return buses_;
}

void Schedule::setBuses(const QList<Bus>& buses) {
    buses_ = buses;
}

void Schedule::pushBusBack(const Bus& bus) {
    buses_.push_back(bus);
}


