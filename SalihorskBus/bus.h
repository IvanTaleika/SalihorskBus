#ifndef BUS_H
#define BUS_H

#include "direction.h"

class Bus {
  public:
    Bus();
    QString number() const;
    void setNumber(const QString& number);

    QList<Direction> directions() const;
    void setDirections(const QList<Direction>& directions);

    QJsonObject toJsonObject() const;

    QStringList notes() const;
    void setNotes(const QStringList& notes);

  private:
    QString number_;
    QList<Direction> directions_;
    QStringList notes_;

};

#endif // BUS_H
