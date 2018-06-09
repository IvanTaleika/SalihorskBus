#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget* parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    salihorskSchedule_(Schedule("Расписание г. Солигорска")) {
    manager_.setSchedule(&salihorskSchedule_);
    ui->setupUi(this);
    setupConnections();
    loadData();
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::onDoubleClicked(QListWidgetItem* item) {
    QStackedWidget* widgetInsideTabWidget = dynamic_cast<QStackedWidget*>(item->listWidget()->parent());
    int itemIndex = 0;
    for (int i = 0; i < widgetInsideTabWidget->currentIndex(); i++) {

        if (dynamic_cast<QListWidget*>(widgetInsideTabWidget->widget(i))->item(dynamic_cast<QListWidget*>(widgetInsideTabWidget->widget(i))->count() - 1)->flags() == Qt::NoItemFlags) {
            itemIndex += dynamic_cast<QListWidget*>(widgetInsideTabWidget->widget(i))->count() - 1;
        } else {
            itemIndex += dynamic_cast<QListWidget*>(widgetInsideTabWidget->widget(i))->count();
        }
    }
    QTabWidget* tabWidget = dynamic_cast<QTabWidget*>(widgetInsideTabWidget->parent());
    addToBackWidgets(tabWidget, windowTitle());
    QWidget* baseWidget = dynamic_cast<QWidget*>(tabWidget->parent());
    dynamic_cast<QStackedLayout*>(baseWidget->layout())->setCurrentIndex(itemIndex + item->listWidget()->row(item) + 1);

    setWindowTitle(windowTitle() + "; " + item->text());

}

void MainWindow::onTimeDoubleClicked(QListWidgetItem* item) {
    QStackedWidget* widgetInsideTabWidget = dynamic_cast<QStackedWidget*>(item->listWidget()->parent());
    QTabWidget* tabWidget = dynamic_cast<QTabWidget*>(widgetInsideTabWidget->parent());
    QWidget* baseWidget = dynamic_cast<QWidget*>(tabWidget->parent());
    QStackedLayout* baseWidgetLayout = dynamic_cast<QStackedLayout*>(baseWidget->layout());
    BusTripDialog trip;
    QTabWidget* directions = dynamic_cast<QTabWidget*>(baseWidgetLayout->itemAt(0)->widget());
    trip.setWindowTitle(directions->tabText(directions->currentIndex()));
    QListWidget* stations = dynamic_cast<QListWidget*>(directions->currentWidget());
    int stationIndex = 0;
    for (int i = 0; i < directions->currentIndex(); i++) {
        if (dynamic_cast<QListWidget*>(directions->widget(i))->item(dynamic_cast<QListWidget*>(directions->widget(i))->count() - 1)->flags() == Qt::NoItemFlags) {
            stationIndex += dynamic_cast<QListWidget*>(directions->widget(i))->count() - 1;
        } else {
            stationIndex += dynamic_cast<QListWidget*>(directions->widget(i))->count();
        }
    }

    for (int i = 0; i < stations->count(); i++) {
        if (stations->item(i)->flags() == Qt::NoItemFlags) {
            continue;
        }
        QString time = dynamic_cast<QListWidget*>(dynamic_cast<QTabWidget*>(baseWidgetLayout->itemAt(stationIndex + 1 + i)->widget())
                                                  ->widget(tabWidget->currentIndex()))->item(item->listWidget()->row(item))->text();
        QString stationName = stations->item(i)->text();
        trip.addStation(stationName, time);
    }
    int result = trip.exec();
    if (result != 0) {
        baseWidgetLayout->setCurrentIndex(stationIndex + result);
        QTabWidget* newTabWidget = dynamic_cast<QTabWidget*>(baseWidgetLayout->currentWidget());
        newTabWidget->setCurrentIndex(widgetInsideTabWidget->currentIndex());
        addToBackWidgets(tabWidget, windowTitle());
        dynamic_cast<QListWidget*>(newTabWidget->widget(tabWidget->currentIndex()))
        ->item(item->listWidget()->row(item))->setSelected(true);
        setWindowTitle(windowTitle().left(windowTitle().lastIndexOf(';') + 1) + " " + stations->item(result - 1)->text());

    }
}

void MainWindow::onScheduleProcessed() {
    QString message = "Обрабтка данных...";
    emit windowBuildindStart(message);
    if (!ui->centralWidget->children().isEmpty()) {
        qDeleteAll(ui->centralWidget->children());
        backWidgets_.clear();
        ui->action_back->setEnabled(false);
        ui->action_forward->setEnabled(false);
        forwardWidgets_.clear();
        setWindowTitle("Расписание автобусов");
    }

    QTabWidget* dataWidget = new QTabWidget(ui->centralWidget);

    QStackedLayout* centralWidgetLayout = new QStackedLayout(ui->centralWidget);
    centralWidgetLayout->addWidget(dataWidget);

    dataWidget->addTab(addScheduleTab(dataWidget), salihorskSchedule_.name());
    for (int i = 0; i < salihorskSchedule_.buses().size(); i++) {
        centralWidgetLayout->addWidget(addDirectionsTabWidget(salihorskSchedule_.buses().at(i), ui->centralWidget));
    }
    emit windowReady();
}

void MainWindow::onLoadUpdatesAccepted() {
    QProgressDialog* loadProgressDialog = new QProgressDialog(this);
    loadProgressDialog->setLabelText(tr("Загрузка данных..."));
    loadProgressDialog->setCancelButtonText(tr("Отмена"));
    loadProgressDialog->setMinimum(0);
    loadProgressDialog->setMaximum(101);
    loadProgressDialog->setValue(0);
    loadProgressDialog->setWindowModality(Qt::WindowModal);
    loadProgressDialog->setWindowTitle("Обновление");
    connect(&manager_, SIGNAL(busProcessed(int)), loadProgressDialog, SLOT(setValue(int)));
    connect(this, SIGNAL(windowBuildindStart(QString)), loadProgressDialog, SLOT(setLabelText(QString)));
    connect(this, &MainWindow::windowReady, loadProgressDialog, &QProgressDialog::deleteLater);
    connect(loadProgressDialog, SIGNAL(canceled()), &manager_, SLOT(stop()));
    loadProgressDialog->show();
    manager_.loadFromInternet();
}

void MainWindow::onSaveProblemRecieved() {
    QMessageBox warning(QMessageBox::Warning, tr("Error"), "При сохранении данных произошла ошибка.");
    warning.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    warning.setButtonText(QMessageBox::Ok, tr("Пробовать ещё раз"));
    warning.setButtonText(QMessageBox::Cancel, tr("Отмена"));
    warning.setDefaultButton(QMessageBox::Ok);
    if (warning.exec() == QMessageBox::Ok) {
        manager_.save();
    }
}

QListWidget* MainWindow::addScheduleTab(QWidget* parent) {
    QListWidget* busesWidget = new QListWidget(parent);

    for (int i = 0; i < salihorskSchedule_.buses().size(); i++) {

        QListWidgetItem* bus = new QListWidgetItem(salihorskSchedule_.buses().at(i).number()
                                                   + ":   " + salihorskSchedule_.buses().at(i).directions().at(0).name(), busesWidget);
        QSize itemSize;
        itemSize.setHeight(40);
        bus->setSizeHint(itemSize);
    }
    connect(busesWidget, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(onDoubleClicked(QListWidgetItem*)));
    return busesWidget;
}

QWidget* MainWindow::addDirectionsTabWidget(const Bus& bus, QWidget* parent) {
    QWidget* baseWidget = new QWidget(parent);
    QTabWidget* directionsWidget = new QTabWidget(baseWidget);
    QStackedLayout* baseWidgetLayout = new QStackedLayout(baseWidget);
    baseWidgetLayout->addWidget(directionsWidget);

    for (int i = 0; i < bus.directions().size(); i++) {
        directionsWidget->addTab(addDirectionTab(bus.directions().at(i), bus.notes(), directionsWidget), bus.directions().at(i).name());
        for (int j = 0; j < bus.directions().at(i).stations().size(); j++) {
            baseWidgetLayout->addWidget(addDayTypesTabWidget(bus.directions().at(i).stations().at(j), baseWidget));
        }

    }
    return baseWidget;
}

QWidget* MainWindow::addDayTypesTabWidget(const Station& station, QWidget* parent) {
    QTabWidget* dayTypesWidget = new QTabWidget(parent);


    for (int i = 0; i < station.dayTypes().size(); i++) {
        dayTypesWidget->addTab(addDayTypeTab(station.dayTypes().at(i), dayTypesWidget), station.dayTypes().at(i).name());
    }

    return dayTypesWidget;
}

QListWidget* MainWindow::addDirectionTab(const Direction& direction, const QStringList& notes, QWidget* parent) {
    QListWidget* directionTab = new QListWidget(parent);
    for (int j = 0; j < direction.stations().size(); j++) {
        QListWidgetItem* station = new QListWidgetItem(direction.stations().at(j).name(), directionTab);
        QSize itemSize;
        itemSize.setHeight(40);
        station->setSizeHint(itemSize);
    }
    if (!notes.isEmpty()) {
        QString notesString = "Примечания:\n";
        int i = 0;
        while (true) {
            notesString += notes.at(i);
            i++;
            if (i < notes.size()) {
                notesString += "\n";
            } else {
                break;
            }
        }
        QListWidgetItem* note = new QListWidgetItem(notesString, directionTab);
        note->setFlags(Qt::NoItemFlags);
        QFont noteFont(note->font());
        noteFont.setItalic(true);
        note->setFont(noteFont);
    }
    connect(directionTab, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(onDoubleClicked(QListWidgetItem*)));
    return directionTab;
}

QListWidget* MainWindow::addDayTypeTab(const DayType& dayType, QWidget* parent) {
    QListWidget* dayTypeTab = new QListWidget(parent);
    dayTypeTab->setLayout(new QHBoxLayout);
    for (int i = 0; i < dayType.timetable().size(); i++) {
        QListWidgetItem* cell = new QListWidgetItem(dayType.timetable().at(i), dayTypeTab);
        QSize itemSize;
        itemSize.setHeight(40);
        cell->setSizeHint(itemSize);
    }
    connect(dayTypeTab, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(onTimeDoubleClicked(QListWidgetItem*)));

    return dayTypeTab;
}

void MainWindow::setupConnections() {
    connect(this, SIGNAL(acceptUpdateLoading()), this, SLOT(onLoadUpdatesAccepted()));
    connect(&manager_, SIGNAL(updateRequired(bool)), this, SLOT(onUpdatesRequired(bool)));
    connect(ui->action_about_Qt, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
    connect(&manager_, SIGNAL(scheduleProcessed()), this, SLOT(onScheduleProcessed()));
    connect(this, SIGNAL(windowReady()), this, SLOT(update()));
    connect(&manager_, SIGNAL(saveError()), this, SLOT(onSaveProblemRecieved()));

}

int MainWindow::localDataIsCorruptedDialog() {
    QMessageBox warning(QMessageBox::Warning, tr("Error"), "Файл schedule.json осутсвует или повреждён. Загрузить расписание (потребуется подключение к интернету)?");
    warning.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    warning.setButtonText(QMessageBox::Ok, tr("Да"));
    warning.setButtonText(QMessageBox::Cancel, tr("Нет"));
    warning.setDefaultButton(QMessageBox::Ok);
    return warning.exec();
}


void MainWindow::on_action_back_triggered() {
    QMap<QWidget*, QString> info = backWidgets_.pop();
    QWidget* widget = info.firstKey();
    if (backWidgets_.isEmpty()) {
        ui->action_back->setDisabled(true);
    }
    QWidget* baseWidget = dynamic_cast<QWidget*>(widget->parent());
    QStackedLayout* baseWidgetLayout = dynamic_cast<QStackedLayout*>(baseWidget->layout());
    if (forwardWidgets_.isEmpty()) {
        ui->action_forward->setEnabled(true);
    }
    QMap<QWidget*, QString> forwardInfo ;
    forwardInfo.insert(baseWidgetLayout->currentWidget(), windowTitle());
    forwardWidgets_.push(forwardInfo);
    baseWidgetLayout->setCurrentWidget(widget);
    setWindowTitle(info.first());
}

void MainWindow::on_action_forward_triggered() {
    QMap<QWidget*, QString> info = forwardWidgets_.pop();
    QWidget* widget = info.firstKey();
    if (forwardWidgets_.isEmpty()) {
        ui->action_forward->setDisabled(true);
    }
    QWidget* baseWidget = dynamic_cast<QWidget*>(widget->parent());
    QStackedLayout* baseWidgetLayout = dynamic_cast<QStackedLayout*>(baseWidget->layout());
    if (backWidgets_.isEmpty()) {
        ui->action_back->setEnabled(true);
    }
    QMap<QWidget*, QString> backInfo;
    backInfo.insert(baseWidgetLayout->currentWidget(), windowTitle());
    backWidgets_.push(backInfo);
    baseWidgetLayout->setCurrentWidget(widget);
    setWindowTitle(info.first());

}

void MainWindow::loadData() {
    if (manager_.loadLocalData() == false) {
        if (localDataIsCorruptedDialog() == QMessageBox::Ok) {
            emit acceptUpdateLoading();
        }
    }
}

void MainWindow::on_action_check_for_updates_triggered() {
    QMessageBox loading(QMessageBox::Question, tr("Обновления"), tr("Поиск обновлений..."));
    loading.setStandardButtons(QMessageBox::Cancel);
    loading.setButtonText(QMessageBox::Cancel, tr("Отмена"));
    connect(this, SIGNAL(endTasks(int)), &loading, SLOT(done(int)));
    manager_.checkForUpdates();
    if (loading.exec() == QMessageBox::Cancel) {
        manager_.stop();
        emit endTasks(1);
    }
}

void MainWindow::onUpdatesRequired(bool isNeeded) {
    emit endTasks(0);
    if (isNeeded) {
        QMessageBox question(QMessageBox::Question, tr("Обновления"), tr("Доступны обновления. Загрузить?"));
        question.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
        question.setButtonText(QMessageBox::Ok, tr("Да"));
        question.setButtonText(QMessageBox::Cancel, tr("Отмена"));
        question.setDefaultButton(QMessageBox::Ok);
        if (question.exec() == QMessageBox::Ok) {
            emit acceptUpdateLoading();
        }
    } else {
        QMessageBox message(QMessageBox::Information, tr("Обновления"), tr("Нет доступных обновлений."));
        message.setStandardButtons(QMessageBox::Close | QMessageBox::Ignore);
        message.setButtonText(QMessageBox::Close, tr("Отмена"));
        message.setButtonText(QMessageBox::Ignore, tr("Загрузить принудительно"));
        message.setDefaultButton(QMessageBox::Close);
        if (message.exec() == QMessageBox::Ignore) {
            emit acceptUpdateLoading();
        }
    }
}


void MainWindow::addToBackWidgets(QWidget* widget, QString string) {
    if (backWidgets_.isEmpty()) {
        ui->action_back->setEnabled(true);
    }
    QMap<QWidget*, QString> backInfo;
    backInfo.insert(widget, string);
    backWidgets_.push(backInfo);
    if (!forwardWidgets_.isEmpty()) {
        forwardWidgets_.clear();
        ui->action_forward->setDisabled(true);
    }
}



void MainWindow::on_action_about_program_triggered() {
    QMessageBox::about(this, tr("О программе"), POGRAM_DESCRIPTION.arg(SCHEDULE_SOURCE).arg(ICON_SOURCE).arg(MAJOR_VERSION).arg((MINOR_VERSION)).toUtf8());

}
