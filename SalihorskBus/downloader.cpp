#include "downloader.h"


Downloader::Downloader(QObject* parent) : QObject(parent), view_(new QWebEngineView) {
    connect(view_, SIGNAL(loadFinished(bool)), this, SLOT(pageLoadFinished(bool)));
}

QUrl Downloader::url() const {
    return url_;
}

void Downloader::setUrl(const QUrl& url) {
    url_ = url;
}

bool Downloader::execute() {
    if (url_.isEmpty()) {
        return false;
    }

    view_->setUrl(url_);
    return true;
}

QString Downloader::parseJsCode() const {
    return parseJsCode_;
}

void Downloader::setParseJsCode(const QString& parseJsCode) {
    parseJsCode_ = parseJsCode;
}

void Downloader::setJsResultProcessFunction(void (DataManager::*function)(const QVariant&)) {
    jsResultProcessFunction_ = function;
}


DataManager* Downloader::destination() const {
    return destination_;
}

void Downloader::setDestination(DataManager* destination) {
    destination_ = destination;
}

void Downloader::pageLoadFinished(bool isSuccess) {
    if (isSuccess) {
        view_->page()->runJavaScript(JQuery::Instance()->Code());
        view_->page()->runJavaScript(parseJsCode_, invoke(destination_, jsResultProcessFunction_));
    }
}


QWebEngineView* Downloader::view() const {
    return view_;
}

void Downloader::setView(QWebEngineView* view) {
    view_ = view;
}
