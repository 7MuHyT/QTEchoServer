#include<QTServer.h>
#include<QTime>

server::server()
{
    nextBlockSize = 0;
    if(listen(QHostAddress::Any,1111))
    {
        qDebug()<<"Listening...";
    }
    else
    {
        qDebug()<<"error while starting: "<<errorString();
    }
}

void server::incomingConnection(qintptr socketDescriptor)                       //когда создаётся новове соединение вызывается этот метод и в
{                                                                               //сервер выделяет новый сокет socket в который передается дескриптор этого соединения
    socket = new QTcpSocket;                                                    //некий уникальный номер подключения
    socket->setSocketDescriptor(socketDescriptor);
    SockDescr = socket->socketDescriptor();
    connect(socket,&QTcpSocket::readyRead, this, &server::SlotReadyRead);
    connect(this,SIGNAL(disconnected),this, SLOT(deleteLater()));
    Sockets.push_back(socket);
    qDebug()<<"client № "<<socketDescriptor<<" connected";
    }

void server::SlotReadyRead()                                         //При получении сообщения от клиента сервер получает сигнал ReadyRead
{                                                                    //Для получения этого сообщения нужно создать слот sloReadyRead
    socket = (QTcpSocket*) sender();                                 //В переменную сокет нужно записать именно тот сокет с которого пришёл запрос.
    QDataStream in(socket);
    in.setVersion(QDataStream::Qt_6_3);
    if(in.status()==QDataStream::Ok)
    {
        for(;;)
        {
            if (nextBlockSize == 0)
            {
                qDebug()<<"nextBlockSize = 0";
                if(socket->bytesAvailable()<2)
                {
                    qDebug()<<"nextBlockSize < 2, break";
                    break;
                }
                in>>nextBlockSize;
            }
            if(socket->bytesAvailable()<nextBlockSize)
            {
                qDebug()<<"Data not full, break";
                break;
            }
            QString str;
            QTime time;
            in>>time>>str;
            nextBlockSize = 0;
            qDebug()<<str;
            SendToClient(str);
            break;
        }
    }
    else
    {
        qDebug()<<"Error QDataStream";
    }
}


void server::SendToClient(QString str)
{
    Data.clear();
    QDataStream out(&Data, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_6_3);
    out<<quint16(0)<<QTime::currentTime()<<str;
    out.device()->seek(0);
    out<<(quint16)(Data.size()-sizeof(quint16));
    for (int i = 0; i < Sockets.size(); i++)
    {
        Sockets[i]->write(Data);
    }
}

void server::OnDisc()
{
    for (int i = 0; i < Sockets.size(); ++i)
    {
        if(Sockets[i]->UnconnectedState == 0)
        {
            Sockets[i]->disconnectFromHost();
            delete Sockets[i];
        }
    }
}












