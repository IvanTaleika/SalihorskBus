#include "bus.h"

Bus::Bus() {

}




QString Bus::number() const {
    return number_;
}

void Bus::setNumber(const QString& number) {
    number_ = number;
}

QList<Direction> Bus::directions() const {
    return directions_;
}

void Bus::setDirections(const QList<Direction>& directions) {
    directions_ = directions;
}

QJsonObject Bus::toJsonObject() const {
    QJsonObject bus;
    bus["number"] = number_;
    QJsonArray notesArray;
    for (int i = 0; i < notes_.size(); i++) {
        notesArray.append(notes_.at(i));
    }
    bus["notes"] = notesArray;
    QJsonArray directionsArray;
    for (size_t i = 0; i < directions_.size(); i++) {
        directionsArray.append(directions_.at(i).toJsonObject());
    }
    bus["directions"] = directionsArray;
    return bus;
}

QStringList Bus::notes() const {
    return notes_;
}

void Bus::setNotes(const QStringList& notes) {
    notes_ = notes;
}



