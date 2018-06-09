#ifndef DIRECTION_H
#define DIRECTION_H

#include "station.h"

class Direction {
  public:
    Direction();

    QString name() const;
    void setName(const QString& name);

    QList<Station> stations() const;
    void setStations(const QList<Station>& stations);

    QJsonObject toJsonObject() const;

  private:
    QString name_;
    QList<Station> stations_;
};

#endif // DIRECTION_H
