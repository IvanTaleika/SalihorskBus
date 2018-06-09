#ifndef SCHEDULE_H
#define SCHEDULE_H

#include "bus.h"

class Schedule  {

  public:
    explicit Schedule(QString name);

    QList<Bus> buses() const;
    void setBuses(const QList<Bus>& buses);

    void pushBusBack(const Bus& bus);

    QString updateDate() const;
    void setUpdateDate(const QString& updateDate);

    QJsonObject toJsonObject() const;

    QString name() const;
    void setName(const QString& name);
    void clearBuses();
  private:
    QString name_;
    QString updateDate_;
    QList<Bus> buses_;

};

#endif // SCHEDULE_H
