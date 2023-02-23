#include "receiverthread.h"

#include <QDebug>
#include <QSerialPort>
#include <QTime>

ReceiverThread::ReceiverThread(QObject* parent) : QThread(parent) {}

ReceiverThread::~ReceiverThread() {
  m_quit = true;
  wait();
}

void ReceiverThread::startReceiver(const QString& portName, int waitTimeout) {
  m_portName = portName;
  m_waitTimeout = waitTimeout;
  if (!isRunning()) {
    start();
  }
}

void ReceiverThread::run() {
  int currentWaitTimeout = m_waitTimeout;
  QSerialPort serial;

  serial.setPortName(m_portName);

  if (!serial.open(QIODevice::ReadWrite)) {
    emit error(
        tr("Can't open %1, error code %2").arg(m_portName).arg(serial.error()));
    return;
  }

  while (!m_quit) {
    if (serial.waitForReadyRead(currentWaitTimeout)) {
      // read request
      QByteArray requestData = serial.readAll();
      while (serial.waitForReadyRead(10)) {
        requestData += serial.readAll();
      }
      // qDebug() << QString::fromUtf8(requestData);
      const QString request = QString::fromUtf8(requestData);
      emit this->request(request);
      // write response
      QString currentResponse = "Ok\r";
      const QByteArray responseData = currentResponse.toUtf8();
      serial.write(responseData);
    }
  }
}
