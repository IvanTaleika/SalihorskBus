#ifndef TIMETABLE_H
#define TIMETABLE_H

#include <QString>
#include <QList>
#include <QJsonObject>
#include <QJsonArray>


class DayType {
  public:
    DayType();
    QString name() const;
    void setName(const QString& name);

    QJsonObject toJsonObject() const;

    QList<QString> timetable() const;
    void setTimetable(const QList<QString>& timetable);

  private:
    QString name_;
    QList<QString> timetable_;
};

#endif // TIMETABLE_H
