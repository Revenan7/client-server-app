#include "server.h"



Server::Server()
{
    if(this -> listen (QHostAddress::Any, 2323))
    {
        qDebug() << "me";
    }
    else
    {
        qDebug() << "no";
    }
    nextBlockSize = 0;
}


void Server::incomingConnection(qintptr socketDescriptor)
{
    socket = new QTcpSocket;
    socket -> setSocketDescriptor(socketDescriptor);
    connect(socket, &QTcpSocket::readyRead, this, &Server::slotReadyRead);
    connect(socket, &QTcpSocket::disconnected, socket, &QTcpSocket::deleteLater);

    Sockets.push_back(socket);

    qDebug() << "conntect" << socketDescriptor;
}

void Server::slotReadyRead()
{
    socket = qobject_cast<QTcpSocket*>(sender()); // Приводим sender() к QTcpSocket
    QDataStream in(socket);
    in.setVersion(QDataStream::Qt_6_6);

    // Проверяем, что состояние потока в порядке
    if (in.status() != QDataStream::Ok) {
        qDebug() << "Ошибка чтения данных из сокета";
        return;
    }

    // Читаем данные из сокета, пока они есть
    while (socket->bytesAvailable() > 0)
    {
        // Читаем размер блока, если его еще не прочитали
        if (nextBlockSize == 0 && !readNextBlockSize(in))
            return;



        // Если данных недостаточно для полного сообщения, ждем
        if (socket->bytesAvailable() < nextBlockSize)
            return;


        // Читаем сообщение от клиента
        QString message;
        QTime time;
        if (!readMessage(in, time, message))
        {
            qDebug() << "Ошибка при чтении сообщения";
            return;
        }

        // Сбрасываем размер блока для следующего сообщения
        nextBlockSize = 0;

        // Логируем и отправляем сообщение всем клиентам
        qDebug() << "Получено сообщение:" << message;
        SendToClient(message);

    }
}

bool Server::readNextBlockSize(QDataStream& in)
{
    // Проверяем, достаточно ли данных для чтения размера блока (2 байта)
    if (socket->bytesAvailable() < sizeof(quint16))
        return false; // Данных недостаточно — ждём


    in >> nextBlockSize; // Читаем размер следующего блока
    qDebug() << "Размер следующего блока данных:" << nextBlockSize;
    return true;
}

bool Server::readMessage(QDataStream& in, QTime& time, QString& message)
{
    // Читаем сообщение от клиента
    in >> time >> message;
    if (in.status() != QDataStream::Ok)
        return false;


    qDebug() << "Время:" << time.toString() << "Сообщение:" << message;
    return true;
}


void Server::SendToClient(QString str)
{
    Data.clear();
    QDataStream out(&Data, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_6_6);
    out << quint16(0) << QTime::currentTime() << str;
    out.device() -> seek(0);
    out << quint16(Data.size() - sizeof(quint16));
    //out << socketDescriptor;


    for(int i = 0; i < Sockets.size(); i++)  Sockets[i] -> write(Data);



}
