#include "mainwindow.h"

#include <QDebug>
#include <QDialog>
#include <QScreen>
#include <QTimer>

#include "comportdialog.h"
#include "receiverthread.h"
#include "ui_mainwindow.h"

static const int WAIT_ROWS_INTERVAL = 1400;
static const int SCROLL_ITEMS_INTERVAL = 1200;
static const int SCROLL_ROWS_INTERVAL = 50;
static const int HIGHLIGHT_INTERVAL = 50;

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), m_RowCounter{0} {
  ui->setupUi(this);
  move(QGuiApplication::screens().at(0)->geometry().center() -
       frameGeometry().center());

  m_Row1Vector = {
      {ui->pushButton_A, "А"},  {ui->pushButton_B, "Б"},
      {ui->pushButton_V, "В"},  {ui->pushButton_G, "Г"},
      {ui->pushButton_D, "Д"},  {ui->pushButton_E, "Е"},
      {ui->pushButton_YO, "Ё"}, {ui->pushButton_ZH, "Ж"},
      {ui->pushButton_Z, "З"},  {ui->pushButton_I, "И"},
  };

  m_Row2Vector = {
      {ui->pushButton_Y, "Й"}, {ui->pushButton_K, "К"}, {ui->pushButton_L, "Л"},
      {ui->pushButton_M, "М"}, {ui->pushButton_N, "Н"}, {ui->pushButton_O, "О"},
      {ui->pushButton_P, "П"}, {ui->pushButton_R, "Р"}, {ui->pushButton_S, "С"},
      {ui->pushButton_T, "Т"},
  };

  m_Row3Vector = {
      {ui->pushButton_U, "У"},   {ui->pushButton_F, "Ф"},
      {ui->pushButton_H, "Х"},   {ui->pushButton_TS, "Ц"},
      {ui->pushButton_CH, "Ч"},  {ui->pushButton_SH, "Ш"},
      {ui->pushButton_SH2, "Щ"}, {ui->pushButton_TV, "Ъ"},
      {ui->pushButton_IY, "Ы"},  {ui->pushButton_MZ, "Ь"},
  };

  m_Row4Vector = {
      {ui->pushButton_EE, "Э"},   {ui->pushButton_UU, "Ю"},
      {ui->pushButton_YA, "Я"},   {ui->pushButton_BKSPS, " "},
      {ui->pushButton_ZPT, ","},  {ui->pushButton_TCHK, "."},
      {ui->pushButton_VSKL, "!"}, {ui->pushButton_VOPR, "?"},
      {ui->pushButton_DASH, "-"}, {ui->pushButton_AT, "@"},
  };

  m_Row5Vector = {
      {ui->pushButton_1, "1"}, {ui->pushButton_2, "2"}, {ui->pushButton_3, "3"},
      {ui->pushButton_4, "4"}, {ui->pushButton_5, "5"}, {ui->pushButton_6, "6"},
      {ui->pushButton_7, "7"}, {ui->pushButton_8, "8"}, {ui->pushButton_9, "9"},
      {ui->pushButton_0, "0"},
  };

  m_Row6Vector = {
      {ui->pushButton_ASTERISK, "*"},  {ui->pushButton_SLASH_RL, "/"},
      {ui->pushButton_SLASH_LR, "\""}, {ui->pushButton_LESS, "<"},
      {ui->pushButton_MORE, ">"},      {ui->pushButton_PLUS, "+"},
      {ui->pushButton_PERCENT, "%"},   {ui->pushButton_EQUAL, "="},
      {ui->pushButton_SADNES, ":-("},   {ui->pushButton_SMILE, ":-)"},
  };

  m_RowsVector.append(m_Row1Vector);
  m_RowsVector.append(m_Row2Vector);
  m_RowsVector.append(m_Row3Vector);
  m_RowsVector.append(m_Row4Vector);
  m_RowsVector.append(m_Row5Vector);
  m_RowsVector.append(m_Row6Vector);
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::Delay(int msec) {
  QEventLoop loop;
  QTimer::singleShot(msec, &loop, &QEventLoop::quit);
  loop.exec();
}

void MainWindow::HighlightLetter(QPushButton* letter) {
  letter->setEnabled(false);
  Delay(HIGHLIGHT_INTERVAL);
  letter->setEnabled(true);
  Delay(HIGHLIGHT_INTERVAL);
  letter->setEnabled(false);
  Delay(HIGHLIGHT_INTERVAL);
  letter->setEnabled(true);
  Delay(HIGHLIGHT_INTERVAL);
  letter->setEnabled(false);
}

void MainWindow::scrollRows() {
  auto row = m_RowsVector.at(m_RowCounter);
  auto max_count = m_RowsVector.size() - 1;
  auto row_len = row.size();

  for (auto& item : row) {
    item.first->setStyleSheet(
        "QPushButton{background-color: rgb(144, 228, 238);}");
  }

  if (m_RowCounter == 0) {
    row = m_RowsVector.at(max_count);
    for (auto& item : row) {
      item.first->setStyleSheet(
          "QPushButton{background-color: rgb(238, 238, 236);}");
    }
  } else {
    row = m_RowsVector.at(m_RowCounter - 1);
    for (auto& item : row) {
      item.first->setStyleSheet(
          "QPushButton{background-color: rgb(238, 238, 236);}");
    }
  }

  m_Emotion = {""};
  Delay(WAIT_ROWS_INTERVAL);
  if (m_Emotion == "happy") {
    row = m_RowsVector.at(m_RowCounter);
    auto items_counter = 0;
    for (auto& item : row) {
      item.first->setEnabled(true);
      if (items_counter == 0) {
        row.at(row_len - 1).first->setEnabled(false);
      } else {
        row.at(items_counter - 1).first->setEnabled(false);
      }
      m_Emotion = {""};
      Delay(SCROLL_ITEMS_INTERVAL);
      if (m_Emotion == "happy") {
        ui->lineEdit->setText(ui->lineEdit->text() + item.second);
        HighlightLetter(item.first);
        m_Emotion = {""};
      }
      items_counter++;
    }

    Delay(SCROLL_ITEMS_INTERVAL);
    row.at(row_len - 1).first->setEnabled(false);

  } else if (m_Emotion == "angry") {
    // ui->lineEdit->setText("");
  } else if (m_Emotion == "surprise") {
    // ui->lineEdit->setText("");
    auto str = ui->lineEdit->text();
    str.truncate(str.length() - 1);
    ui->lineEdit->setText(str);
  }
  m_Emotion = {""};

  m_RowCounter++;
  if (m_RowCounter > max_count) {
    m_RowCounter = 0;
  }
  QTimer::singleShot(SCROLL_ROWS_INTERVAL, this, SLOT(scrollRows()));
}

void MainWindow::on_actionCOM_triggered() {
  ComPortDialog dialog;
  dialog.Init();

  if (dialog.exec() == QDialog::Accepted) {
    QString port_name = "/dev/" + dialog.getPortName();

    connect(&m_thread, &ReceiverThread::request, this,
            &MainWindow::showRequest);
    connect(&m_thread, &ReceiverThread::error, this, &MainWindow::processError);
    connect(&m_thread, &ReceiverThread::timeout, this,
            &MainWindow::processTimeout);

    m_thread.startReceiver(port_name, 10000);

    QTimer::singleShot(SCROLL_ROWS_INTERVAL, this, SLOT(scrollRows()));
  }
}

void MainWindow::startReceiver() {}

void MainWindow::showRequest(const QString& s) {
  if (s != "neutral") {
    m_Emotion = s;
  }
}

void MainWindow::processError(const QString& s) { qDebug() << s; }

void MainWindow::processTimeout(const QString& s) { qDebug() << s; }

void MainWindow::ShowMessage(QString title, QString message,
                             QMessageBox::Icon icon) {
  QMessageBox box(title, message, icon, QMessageBox::Ok, QMessageBox::NoButton,
                  QMessageBox::NoButton);
  box.exec();
}

void MainWindow::ShowError(QString title, QString message) {
  ShowMessage(title, message, QMessageBox::Critical);
}

void MainWindow::on_pushButtonClear_clicked() { ui->lineEdit->clear(); }
