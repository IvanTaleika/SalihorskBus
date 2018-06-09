#ifndef DOWNLOADER_H
#define DOWNLOADER_H

#include <QObject>
#include <QWebEngineView>

#include "jquery.h"

class DataManager;

class Downloader : public QObject {
    Q_OBJECT
  public:
    explicit Downloader(QObject* parent = nullptr);

    QUrl url() const;
    void setUrl(const QUrl& url);

    bool execute();

    QString parseJsCode() const;
    void setParseJsCode(const QString& parseJsCode);

    DataManager* destination() const;
    void setDestination(DataManager* destination);

    void setJsResultProcessFunction(void (DataManager::*function)(const QVariant&));

    QWebEngineView* view() const;
    void setView(QWebEngineView* view);

  signals:
  public slots:
  private:
    DataManager* destination_;
    QWebEngineView* view_;
    QUrl url_;
    QString parseJsCode_;
    void (DataManager::*jsResultProcessFunction_)(const QVariant& returnValue);

  private slots:
    void pageLoadFinished(bool isSuccess);
};

template<typename Arg, typename R, typename C>
struct InvokeWrapper {
    R* receiver;
    void (C::*memberFun)(Arg);
    void operator()(Arg result) {
        (receiver->*memberFun)(result);
    }
};

template<typename Arg, typename R, typename C>
InvokeWrapper<Arg, R, C> invoke(R* receiver, void (C::*memberFun)(Arg)) {
    InvokeWrapper<Arg, R, C> wrapper = {receiver, memberFun};
    return wrapper;
}

#endif // DOWNLOADER_H
