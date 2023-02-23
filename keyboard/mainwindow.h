#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QSerialPort>
#include <QVector>

#include "receiverthread.h"

using rowVector = QVector<std::pair<QPushButton*, QString>>;

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  MainWindow(QWidget* parent = nullptr);
  ~MainWindow();

 private:
  Ui::MainWindow* ui;
  QSerialPort m_serial;
  ReceiverThread m_thread;
  int m_RowCounter;
  QString m_Emotion;
  QVector<std::pair<QPushButton*, QString>> m_buttonsVector;
  QVector<std::pair<QPushButton*, QString>> m_Row1Vector;
  QVector<std::pair<QPushButton*, QString>> m_Row2Vector;
  QVector<std::pair<QPushButton*, QString>> m_Row3Vector;
  QVector<std::pair<QPushButton*, QString>> m_Row4Vector;
  QVector<std::pair<QPushButton*, QString>> m_Row5Vector;
  QVector<std::pair<QPushButton*, QString>> m_Row6Vector;
  QVector<rowVector> m_RowsVector;

  void ShowMessage(QString title, QString message, QMessageBox::Icon icon);
  void ShowError(QString title, QString message);
  void HighlightLetter(QPushButton* letter);
  void Delay(int msec);

 private slots:
  void on_actionCOM_triggered();
  void startReceiver();
  void showRequest(const QString& s);
  void processError(const QString& s);
  void processTimeout(const QString& s);
  void on_pushButtonClear_clicked();

 public slots:
  void scrollRows();
};
#endif  // MAINWINDOW_H
