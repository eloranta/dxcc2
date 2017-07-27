#ifndef SOCKETTEST_H
#define SOCKETTEST_H

#include <QtCore/QObject>
#include <QtCore/QDebug>
#include <QtNetwork/QTcpSocket>
#include <QtNetwork/QAbstractSocket>

struct BandMode
{
  QString band;
  QString mode;
  BandMode(const QString &band, const QString &mode)
  {
    this->band = band;
    this->mode = mode;
  }
};


class SocketTest : public QObject
{
    Q_OBJECT
public:
    explicit SocketTest(QObject *parent = 0);
    void Test();
    QString Country(const QString& call);
    BandMode Band(const QString& freq);
public slots:
    void connected();
    void disconnected();
    void bytesWritten(qint64 bytes);
    void readyRead();
private:
    QTcpSocket *socket;
    int state;
    int id;
};

#endif
