#include "server.h"

Server::Server() {

}

void Server::setInfoBlock(QTextEdit *_infoBlock) {
    teInfoBlock = _infoBlock;
}

void Server::disconnectAll() {
    if(server_status == 1){
        foreach(int i, SClients.keys()) {
            QTextStream os(SClients[i]);
            os.setAutoDetectUnicode(true);
            os << QDateTime::currentDateTime().toString() << "\n";
            SClients[i]->close();
            SClients.remove(i);
        }
        tcpServer->close();
        qDebug() << QString::fromUtf8("Сервер остановлен!");
        server_status = 0;
    }
}


void Server::start()
{
    tcpServer = new QTcpServer(this);
    connect(tcpServer, SIGNAL(newConnection()), this, SLOT(newuser()));
    if (server_status == 0 && !tcpServer->listen(QHostAddress::Any, 41017)) {
        qDebug() <<  QObject::tr("Unable to start the server: %1.").arg(tcpServer->errorString());
        teInfoBlock->append(tcpServer->errorString());
    } else {
        server_status=1;
        qDebug() << tcpServer->isListening() << "TCPSocket listen on port";
        teInfoBlock->append(QString::fromUtf8("Сервер запущен!"));
        qDebug() << QString::fromUtf8("Сервер запущен!");
    }
}

void Server::stop()
{
    if(server_status==1){
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
        server_status=0;
    }
}


void Server::newuser()
{
    if(server_status==1){
        qDebug() << QString::fromUtf8("У нас новое соединение!");
        teInfoBlock->append(QString::fromUtf8("У нас новое соединение!"));
        QTcpSocket* clientSocket=tcpServer->nextPendingConnection();
        int idusersocs=clientSocket->socketDescriptor();
        SClients[idusersocs]=clientSocket;
        connect(SClients[idusersocs],SIGNAL(readyRead()),this, SLOT(slotReadClient()));
    }
}

void Server::slotReadClient()
{
    QTcpSocket* clientSocket = (QTcpSocket*)sender();
    int idusersocs=clientSocket->socketDescriptor();
    QTextStream os(clientSocket);
    os.setAutoDetectUnicode(true);
    os << "HTTP/1.0 200 Ok\r\n"
          "Content-Type: text/html; charset=\"utf-8\"\r\n"
          "\r\n"
          "<h1>Nothing to see here</h1>\n"
          << QDateTime::currentDateTime().toString() << "\n";
    teInfoBlock->append("ReadClient:"+clientSocket->readAll()+"\n\r");
    // Если нужно закрыть сокет
    clientSocket->close();
    SClients.remove(idusersocs);
}
