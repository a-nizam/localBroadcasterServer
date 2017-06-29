#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QDateTime>
#include <QTextEdit>

class Server : public QObject {
public:
    Server();
public:
    QTcpServer *tcpServer;
    int server_status;
    QMap<int,QTcpSocket *> SClients;
signals:
    void readyRead();
public slots:
    void newuser();
    void slotReadClient();
private:
    QTextEdit *teInfoBlock;
public:
    void disconnectAll();
    void setInfoBlock(QTextEdit *_infoBlock);
    void start();
    void stop();
};

#endif // SERVER_H
