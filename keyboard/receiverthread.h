#ifndef RECEIVERTHREAD_H
#define RECEIVERTHREAD_H

#include <QMutex>
#include <QThread>
#include <QWaitCondition>

class ReceiverThread : public QThread {
  Q_OBJECT

 public:
  explicit ReceiverThread(QObject* parent = nullptr);
  ~ReceiverThread();

  void startReceiver(const QString& portName, int waitTimeout);

 signals:
  void request(const QString& s);
  void error(const QString& s);
  void timeout(const QString& s);

 private:
  void run() override;

  QString m_portName;
  int m_waitTimeout = 0;
  bool m_quit = false;
};

#endif  // RECEIVERTHREAD_H
