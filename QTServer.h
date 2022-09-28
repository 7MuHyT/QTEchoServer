#ifndef QTSERVER_H
#define QTSERVER_H
#include<QTcpServer>
#include<QTcpSocket>
#include <QVector>

class server : public QTcpServer
{

Q_OBJECT

public:
    server();
     QTcpSocket* socket;

private:
    QVector<QTcpSocket*> Sockets;
    QByteArray Data;
    void SendToClient(QString str);
    quint16 nextBlockSize;
    QString name;
    int SockDescr;

public slots:
    void incomingConnection(qintptr socketDescriptor);
    void SlotReadyRead();
    void OnDisc();

};

#endif // QTSERVER_H
