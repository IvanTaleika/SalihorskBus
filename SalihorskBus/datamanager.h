#ifndef DATAMANAGER_H
#define DATAMANAGER_H

#include <QObject>
#include <QJsonDocument>
#include "downloader.h"
#include "schedule.h"

#include "utils.h"

class DataManager : public QObject {
    Q_OBJECT

  public:
    explicit DataManager(QObject* parent = nullptr);

    bool loadLocalData();
    void checkForUpdates();
    bool save();

    Schedule* schedule() const;
    void setSchedule(Schedule* schedule);



  signals:
    void updateRequired(bool isRequired);
    void busProcessed(int progress);
    void scheduleProcessed();
    void saveError();
  public slots:
    void stop();
    void loadFromInternet();

  private:
    Downloader downloader_;
    Schedule* schedule_;
    QList<QVariant> links;
    int currentNumber;

    void processUpdateDate(const QVariant& jsResult);

    void loadLinks(const QVariant& jsResult);
    void loadBuses(const QVariant& jsResult);

    void processBus(const QVariant& jsResult);
    void processLocalData(const QJsonDocument& schedule);

    QList<Direction> processDirections(const QVariant& jsResult);
    QList<Station> processStations(const QVariant& jsResult);
    QList<DayType> processDayTypes(const QVariant& jsResult);
    QList<QString> processTimetable(const QVariant& jsResult);


};

#endif // DATAMANAGER_H
