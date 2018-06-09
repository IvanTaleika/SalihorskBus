#include "daytype.h"

DayType::DayType() {

}


QString DayType::name() const {
    return name_;
}

void DayType::setName(const QString& name) {
    name_ = name;
}

QJsonObject DayType::toJsonObject() const {
    QJsonObject dayType;
    dayType["dayType"] = name_;
    QJsonArray timetable;
    for (size_t i = 0; i < timetable_.size(); i++) {
        timetable.append(timetable_.at(i));
    }
    dayType["timetable"] = timetable;
    return dayType;
}

QList<QString> DayType::timetable() const {
    return timetable_;
}

void DayType::setTimetable(const QList<QString>& timetable) {
    timetable_ = timetable;
}



