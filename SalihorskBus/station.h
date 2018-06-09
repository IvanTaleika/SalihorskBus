#ifndef BUSSTATION_H
#define BUSSTATION_H

#include "daytype.h"

class Station {
  public:
    Station();

    QString name() const;
    void setName(const QString& name);

    QJsonObject toJsonObject() const;

    QList<DayType> dayTypes() const;
    void setDayTypes(const QList<DayType>& dayTypes);

  private:
    QString name_;
    QList<DayType> dayTypes_;
};

#endif // BUSSTATION_H
