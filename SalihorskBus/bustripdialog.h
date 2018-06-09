#ifndef BUSTRIPDIALOG_H
#define BUSTRIPDIALOG_H

#include <QDialog>
#include <QString>
#include <QListWidget>

namespace Ui {
class BusTripDialog;
}

class BusTripDialog : public QDialog {
    Q_OBJECT
  public slots:
    void stationClicked(QListWidgetItem* station);

  public:
    explicit BusTripDialog(QWidget* parent = 0);
    ~BusTripDialog();
    void addStation(QString stationName, QString time);

  private:
    Ui::BusTripDialog* ui;
};

#endif // BUSTRIPDIALOG_H
