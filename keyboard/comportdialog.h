#ifndef COMPORTDIALOG_H
#define COMPORTDIALOG_H

#include <QDialog>

namespace Ui {
class ComPortDialog;
}

class ComPortDialog : public QDialog {
  Q_OBJECT

 public:
  explicit ComPortDialog(QWidget* parent = 0);
  ~ComPortDialog();

 public:
  void Init();
  QString getPortName();

 private slots:
  void on_pushButtonCancel_clicked();
  void on_pushButtonOk_clicked();

 private:
  Ui::ComPortDialog* ui;
};

#endif  // COMPORTDIALOG_H
