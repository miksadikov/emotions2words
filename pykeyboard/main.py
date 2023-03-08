import sys
from PyQt5 import QtCore, QtWidgets, QtSerialPort
from PyQt5.QtWidgets import QDialog, QDesktopWidget
from PyQt5.QtCore import QTimer, QEventLoop

import MainWindow

WAIT_ROWS_INTERVAL = 1400
SCROLL_ITEMS_INTERVAL = 1200
SCROLL_ROWS_INTERVAL = 50
HIGHLIGHT_INTERVAL = 50

class MainWindow(QtWidgets.QMainWindow, MainWindow.Ui_MainWindow):
  def __init__(self):
    super().__init__()
    self.setupUi(self)

    self.serial = QtSerialPort.QSerialPort(
      '/dev/tty1',
      baudRate=QtSerialPort.QSerialPort.Baud9600,
      readyRead=self.Receive)

    self.Emotion = ""
    self.RowsVector = []
    self.RowCounter = 0

    self.Row1Vector = [
      [self.pushButton_A, "А"],  [self.pushButton_B, "Б"],
      [self.pushButton_V, "В"],  [self.pushButton_G, "Г"],
      [self.pushButton_D, "Д"],  [self.pushButton_E, "Е"],
      [self.pushButton_YO, "Ё"], [self.pushButton_ZH, "Ж"],
      [self.pushButton_Z, "З"],  [self.pushButton_I, "И"]
    ]

    self.Row2Vector = [
      [self.pushButton_Y, "Й"], [self.pushButton_K, "К"],
      [self.pushButton_L, "Л"], [self.pushButton_M, "М"],
      [self.pushButton_N, "Н"], [self.pushButton_O, "О"],
      [self.pushButton_P, "П"], [self.pushButton_R, "Р"], 
      [self.pushButton_S, "С"], [self.pushButton_T, "Т"]
    ]

    self.Row3Vector = [
      [self.pushButton_U, "У"],   [self.pushButton_F, "Ф"],
      [self.pushButton_H, "Х"],   [self.pushButton_TS, "Ц"],
      [self.pushButton_CH, "Ч"],  [self.pushButton_SH, "Ш"],
      [self.pushButton_SH2, "Щ"], [self.pushButton_TV, "Ъ"],
      [self.pushButton_IY, "Ы"],  [self.pushButton_MZ, "Ь"]
    ]

    self.Row4Vector = [
      [self.pushButton_EE, "Э"],   [self.pushButton_UU, "Ю"],
      [self.pushButton_YA, "Я"],   [self.pushButton_BKSPS, " "],
      [self.pushButton_ZPT, ","],  [self.pushButton_TCHK, "."],
      [self.pushButton_VSKL, "!"], [self.pushButton_VOPR, "?"],
      [self.pushButton_DASH, "-"], [self.pushButton_AT, "@"]
    ]

    self.Row5Vector = [
      [self.pushButton_1, "1"], [self.pushButton_2, "2"],
      [self.pushButton_3, "3"], [self.pushButton_4, "4"],
      [self.pushButton_5, "5"], [self.pushButton_6, "6"],
      [self.pushButton_7, "7"], [self.pushButton_8, "8"],
      [self.pushButton_9, "9"], [self.pushButton_0, "0"]
    ]

    self.Row6Vector = [
      [self.pushButton_ASTERISK, "*"],  [self.pushButton_SLASH_RL, "/"],
      [self.pushButton_SLASH_LR, "\""], [self.pushButton_LESS, "<"],
      [self.pushButton_MORE, ">"],      [self.pushButton_PLUS, "+"],
      [self.pushButton_PERCENT, "%"],   [self.pushButton_EQUAL, "="],
      [self.pushButton_SADNES, ":-("],  [self.pushButton_SMILE, ":-)"]
    ]

    self.RowsVector.append(self.Row1Vector)
    self.RowsVector.append(self.Row2Vector)
    self.RowsVector.append(self.Row3Vector)
    self.RowsVector.append(self.Row4Vector)
    self.RowsVector.append(self.Row5Vector)
    self.RowsVector.append(self.Row6Vector)

  def Center(self):
    qr = self.frameGeometry()
    cp = QDesktopWidget().availableGeometry().center()
    qr.moveCenter(cp)
    self.move(qr.topLeft())

  @QtCore.pyqtSlot()
  def Receive(self):
    self.Emotion = self.serial.readAll().data().decode()

  def Delay(self, msec):
    loop = QEventLoop()
    QTimer.singleShot(msec, loop.quit)
    loop.exec_()

  def HighlightLetter(self, letter):
    letter.setEnabled(False);
    self.Delay(HIGHLIGHT_INTERVAL);
    letter.setEnabled(True);
    self.Delay(HIGHLIGHT_INTERVAL);
    letter.setEnabled(False);
    self.Delay(HIGHLIGHT_INTERVAL);
    letter.setEnabled(True);
    self.Delay(HIGHLIGHT_INTERVAL);
    letter.setEnabled(False);

  def ScrollRows(self):
    row = self.RowsVector[self.RowCounter]
    max_count = len(self.RowsVector) - 1
    row_len = len(row)

    for item in row:
      item[0].setStyleSheet(
          "QPushButton{background-color: rgb(144, 228, 238);}");

    if self.RowCounter == 0:
      row = self.RowsVector[max_count]
      for item in row:
       item[0].setStyleSheet(
            "QPushButton{background-color: rgb(238, 238, 236);}");
    else:
      row = self.RowsVector[self.RowCounter - 1]
      for item in row:
        item[0].setStyleSheet(
            "QPushButton{background-color: rgb(238, 238, 236);}");

    self.Emotion = ""
    self.Delay(WAIT_ROWS_INTERVAL)
    if self.Emotion == "happy":
      row = self.RowsVector[self.RowCounter]
      items_counter = 0
      for item in row:
        item[0].setEnabled(True)
        if items_counter == 0:
          row[row_len - 1][0].setEnabled(False);
        else:
          row[items_counter - 1][0].setEnabled(False)

        self.Emotion = ""
        self.Delay(SCROLL_ITEMS_INTERVAL)
        if self.Emotion == "happy":
          self.lineEdit.setText(self.lineEdit.text() + item[1])
          self.HighlightLetter(item[0])
          self.Emotion = ""

        items_counter += 1

      self.Delay(SCROLL_ITEMS_INTERVAL);
      row[row_len - 1][0].setEnabled(False)

    elif self.Emotion == "surprise":
      print("surprise")
      str = self.lineEdit.text()
      str = str[:-1]
      self.lineEdit.setText(str)

    self.Emotion = ""

    self.RowCounter += 1
    if self.RowCounter > max_count:
      self.RowCounter = 0

    if not self.serial.isOpen():
      self.serial.open(QtCore.QIODevice.ReadWrite)
    QTimer.singleShot(SCROLL_ROWS_INTERVAL, self.ScrollRows())


if __name__ == '__main__':
  app = QtWidgets.QApplication(sys.argv)
  window = MainWindow()
  window.Center()
  window.show()
  QTimer.singleShot(SCROLL_ROWS_INTERVAL, window.ScrollRows())
  app.exec_()
