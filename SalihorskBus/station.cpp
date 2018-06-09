#include "station.h"

Station::Station() {

}

QString Station::name() const {
    return name_;
}

void Station::setName(const QString& name) {
    name_ = name;
}

QJsonObject Station::toJsonObject() const {
    QJsonObject station;
    station["station"] = name_;
    QJsonArray dayTypesArray;
    for (size_t i = 0; i < dayTypes_.size(); i++) {
        dayTypesArray.append(dayTypes_.at(i).toJsonObject());
    }
    station["dayTypes"] = dayTypesArray;
    return station;
}

QList<DayType> Station::dayTypes() const {
    return dayTypes_;
}

void Station::setDayTypes(const QList<DayType>& daySchedules) {
    dayTypes_ = daySchedules;
}
