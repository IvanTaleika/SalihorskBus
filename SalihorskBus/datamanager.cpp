#include "datamanager.h"

DataManager::DataManager(QObject* parent) : QObject(parent), schedule_(nullptr), downloader_(this) {

}


void DataManager::processUpdateDate(const QVariant& jsResult) {
  if (schedule_->updateDate() == jsResult.toString()) {
    emit updateRequired(false);
  } else {
    emit updateRequired(true);
  }
}

void DataManager::loadLinks(const QVariant& jsResult) {
  schedule_->setUpdateDate(jsResult.toString());
  downloader_.setParseJsCode(GET_BUSES_LINKS);
  downloader_.setJsResultProcessFunction(&DataManager::loadBuses);
  downloader_.execute();
}

void DataManager::loadBuses(const QVariant& jsResult) {
  schedule_->clearBuses();
  downloader_.setParseJsCode(GET_BUS_INFORMATION);
  downloader_.setJsResultProcessFunction(&DataManager::processBus);
  links = jsResult.toList();
  downloader_.setUrl(QUrl(SCHEDULE_SOURCE + links.at(0).toString()));
  downloader_.execute();
  currentNumber = 1;

}


void DataManager::processBus(const QVariant& jsResult) {
  Bus newBus;
  newBus.setNumber(jsResult.toMap()["number"].toString());
  if (!jsResult.toMap()["notes"].toList().isEmpty()) {
    QStringList notes;
    for (int i = 0; i < jsResult.toMap()["notes"].toList().size(); i++) {
      notes.push_back(jsResult.toMap()["notes"].toList().at(i).toString());
    }
    newBus.setNotes(notes);
  }
  newBus.setDirections(processDirections(jsResult.toMap()["directions"]));
  schedule_->pushBusBack(newBus);
  if (currentNumber < links.size()) {
    downloader_.setUrl(QUrl(SCHEDULE_SOURCE + links.at(currentNumber).toString()));
    downloader_.execute();
    currentNumber++;
    emit busProcessed(currentNumber * 100 / links.size());
  } else {
    downloader_.view()->stop();
    save();
    emit scheduleProcessed();

  }
}

void DataManager::processLocalData(const QJsonDocument& schedule) {
  QVariant variantData = schedule.toVariant();
  schedule_->setName(variantData.toMap()["name"].toString());
  schedule_->setUpdateDate(variantData.toMap()["update date"].toString());
  QList<QVariant> listData = variantData.toMap()["buses"].toList();
  QList<Bus> buses;
  for (size_t i = 0 ; i < listData.size(); i++) {
    Bus newBus;
    newBus.setNumber(listData.at(i).toMap()["number"].toString());
    if (!listData.at(i).toMap()["notes"].toList().isEmpty()) {
      QStringList notes;
      for (int j = 0; j < listData.at(i).toMap()["notes"].toList().size(); j++) {
        notes.push_back(listData.at(i).toMap()["notes"].toList().at(j).toString());
      }
      newBus.setNotes(notes);
    }
    newBus.setDirections(processDirections(listData.at(i).toMap()["directions"]));
    buses.push_back(newBus);
  }
  schedule_->setBuses(buses);
  emit scheduleProcessed();
}

QList<Direction> DataManager::processDirections(const QVariant& jsResult) {
  QList<Direction> directions;
  for (size_t i = 0; i < jsResult.toList().size(); i++) {
    Direction newDirection;
    newDirection.setName(jsResult.toList().at(i).toMap()["direction"].toString());
    newDirection.setStations(processStations(jsResult.toList().at(i).toMap()["stations"]));
    directions.push_back(newDirection);
  }
  return directions;
}

QList<Station> DataManager::processStations(const QVariant& jsResult) {
  QList<Station> stations;
  for (size_t i = 0; i < jsResult.toList().size(); i++) {
    Station newStation;
    newStation.setName(jsResult.toList().at(i).toMap()["station"].toString());
    newStation.setDayTypes(processDayTypes(jsResult.toList().at(i).toMap()["dayTypes"]));
    stations.push_back(newStation);
  }
  return stations;
}

QList<DayType> DataManager::processDayTypes(const QVariant& jsResult) {
  QList<DayType> dayTypes;
  for (size_t i = 0; i < jsResult.toList().size(); i++) {
    DayType newDayType;
    newDayType.setName(jsResult.toList().at(i).toMap()["dayType"].toString());
    newDayType.setTimetable(processTimetable(jsResult.toList().at(i).toMap()["timetable"]));
    dayTypes.push_back(newDayType);
  }
  return dayTypes;
}

QList<QString> DataManager::processTimetable(const QVariant& jsResult) {
  QList<QString> times;
  for (size_t i = 0; i < jsResult.toList().size(); i++) {
    times.push_back(jsResult.toList().at(i).toString());
  }
  return times;
}

bool DataManager::loadLocalData() {
  QFile source("../SalihorskBus/Resources/schedule.json");
  if (!source.open(QIODevice::ReadOnly)) {
    return false;
  }
  QJsonDocument schedule(QJsonDocument::fromJson(source.readAll()));
  source.close();
  if (schedule.isObject()) {
    processLocalData(schedule);
  } else {
    return false;
  }
  return true;
}

void DataManager::loadFromInternet() {
  downloader_.setDestination(this);
  downloader_.setUrl(QUrl(SCHEDULE_SOURCE));
  downloader_.setParseJsCode(GET_UPDATE_DATE_JS);
  downloader_.setJsResultProcessFunction(&DataManager::loadLinks);
  downloader_.execute();
}



void DataManager::checkForUpdates() {
  downloader_.setDestination(this);
  downloader_.setUrl(QUrl(SCHEDULE_SOURCE));
  downloader_.setParseJsCode(GET_UPDATE_DATE_JS);
  downloader_.setJsResultProcessFunction(&DataManager::processUpdateDate);
  downloader_.execute();
}

bool DataManager::save() {
  QFile destination("../SalihorskBus/Resources/schedule.json");
  if (!destination.open(QIODevice::WriteOnly)) {
    emit saveError();
    return false;
  }
  QJsonDocument saveDocument(schedule_->toJsonObject());
  destination.write(saveDocument.toJson());
  destination.close();
  return true;
}

Schedule* DataManager::schedule() const {
  return schedule_;
}

void DataManager::setSchedule(Schedule* schedule) {
  schedule_ = schedule;
}



void DataManager::stop() {
  downloader_.view()->stop();
}
