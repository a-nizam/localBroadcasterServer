#include "server.h"

Server::Server() : serverStatus(0) {

}

void Server::setInfoBlock(QTextEdit *_infoBlock) {
    teInfoBlock = _infoBlock;
}

void Server::disconnectAll() {
    if(serverStatus == 1){
        foreach(int i, SClients.keys()) {
            QTextStream os(SClients[i]);
            os.setAutoDetectUnicode(true);
            os << QDateTime::currentDateTime().toString() << "\n";
            SClients[i]->close();
            SClients.remove(i);
        }
        tcpServer->close();
        qDebug() << QString::fromUtf8("Сервер остановлен!");
        serverStatus = 0;
    }
}


void Server::start() {
    if (!serverStatus) {
        tcpServer = new QTcpServer(this);
        connect(tcpServer, SIGNAL(newConnection()), this, SLOT(newUser()));
        if (!tcpServer->listen(QHostAddress::Any, 41017)) {
            qDebug() <<  QObject::tr("Unable to start the server: %1.").arg(tcpServer->errorString());
            teInfoBlock->append(tcpServer->errorString());
        } else {
            serverStatus = 1;
            qDebug() << tcpServer->isListening() << "TCPSocket listen on port";
            teInfoBlock->append(QString::fromUtf8("Сервер запущен!"));
            qDebug() << QString::fromUtf8("Сервер запущен!");
        }
    }
}

void Server::stop() {
    if(serverStatus){
        foreach(int i,SClients.keys()){
            QTextStream os(SClients[i]);
            os.setAutoDetectUnicode(true);
            os << QDateTime::currentDateTime().toString() << "\n";
            SClients[i]->close();
            SClients.remove(i);
        }
        tcpServer->close();
        teInfoBlock->append(QString::fromUtf8("Сервер остановлен!"));
        qDebug() << QString::fromUtf8("Сервер остановлен!");
        serverStatus = 0;
    }
}


void Server::newUser() {
    if(serverStatus==1){
        qDebug() << "У нас новое соединение!";
        teInfoBlock->append("У нас новое соединение!");
        QTcpSocket* clientSocket = tcpServer->nextPendingConnection();
        int idusersocs = clientSocket->socketDescriptor();
        SClients[idusersocs] = clientSocket;
//        qDebug() << "Хост: " << clientSocket->peerAddress() << " " << clientSocket->peerPort() << " " << clientSocket->peerName();
        connect(SClients[idusersocs], SIGNAL(readyRead()), this, SLOT(slotReadClient()));
    }
}

void Server::slotReadClient() {
    QTcpSocket* clientSocket = (QTcpSocket*)sender();
    int idusersocs = clientSocket->socketDescriptor();
//    QTextStream os(clientSocket);
//    os.setAutoDetectUnicode(true);
//    os << QDateTime::currentDateTime().toString();
    QByteArray buf;
    QDataStream io(clientSocket);
    io.setVersion(QDataStream::Qt_5_8);
    if (clientSocket->bytesAvailable() == sizeof(qint8)) {
        quint8 messageType;
        io >> messageType;
        if (messageType == MessageType::id) {
            io << quint8(1);
            if (clientSocket->waitForReadyRead()) {
                io >> buf;
                teInfoBlock->append(QString::fromUtf8(buf));
            }
        }
        if (messageType == MessageType::message) {
            io << quint8(1);
            if (clientSocket->waitForReadyRead()) {
                io >> buf;
                teInfoBlock->append(QString::fromUtf8(buf));
            }
        }
    }
}
