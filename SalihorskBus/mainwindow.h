#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLayout>
#include <QStackedLayout>
#include <QListWidget>
#include <QStackedWidget>
#include <QStack>
#include <QMessageBox>
#include <QProgressDialog>

#include "schedule.h"
#include "datamanager.h"
#include "bustripdialog.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT
  public:
    explicit MainWindow(QWidget* parent = 0);
    ~MainWindow();

  signals:
    void endTasks(int result);
    void windowBuildindStart(QString lableString);
    void windowReady();
    void acceptUpdateLoading();
  private slots:
    void onDoubleClicked(QListWidgetItem* item);
    void onTimeDoubleClicked(QListWidgetItem* item);
    void on_action_back_triggered();
    void on_action_forward_triggered();
    void onScheduleProcessed();
    void onLoadUpdatesAccepted();
    void onSaveProblemRecieved();
    void on_action_check_for_updates_triggered();
    void onUpdatesRequired(bool isNeeded);
    void on_action_about_program_triggered();

  private:
    Ui::MainWindow* ui;
    DataManager manager_;
    Schedule salihorskSchedule_;
    QStack<QMap<QWidget*, QString>> backWidgets_;
    QStack<QMap<QWidget*, QString>> forwardWidgets_;

    void addToBackWidgets(QWidget* widget, QString string);
    void loadData();
    QListWidget* addScheduleTab(QWidget* parent);

    QWidget* addDirectionsTabWidget(const Bus& bus, QWidget* parent);
    QListWidget* addDirectionTab(const Direction& direction, const QStringList& notes, QWidget* parent);

    QWidget* addDayTypesTabWidget(const Station& station, QWidget* parent);
    QListWidget* addDayTypeTab(const DayType& dayType, QWidget* parent);
    void setupConnections();

    int localDataIsCorruptedDialog();
};

#endif // MAINWINDOW_H
