#include "direction.h"

Direction::Direction() {

}

QString Direction::name() const {
    return name_;
}

void Direction::setName(const QString& name) {
    name_ = name;
}

QList<Station> Direction::stations() const {
    return stations_;
}

void Direction::setStations(const QList<Station>& stations) {
    stations_ = stations;
}

QJsonObject Direction::toJsonObject() const {
    QJsonObject direction;
    direction["direction"] = name_;
    QJsonArray stationsArray;
    for (size_t i = 0; i < stations_.size(); i++) {
        stationsArray.append(stations_.at(i).toJsonObject());
    }
    direction["stations"] = stationsArray;
    return direction;
}



