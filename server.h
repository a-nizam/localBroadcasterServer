#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QDateTime>
#include <QTextEdit>

class Server : public QObject {
    Q_OBJECT

public:
    Server();
public:
    QTcpServer *tcpServer;
    int serverStatus;
    QMap<int, QTcpSocket *> SClients;
signals:
    void readyRead();
public slots:
    void newUser();
    void slotReadClient();
private:
    QTextEdit *teInfoBlock;
    int isNotRunning;
public:
    enum MessageType { sendId };
    void disconnectAll();
    void setInfoBlock(QTextEdit *_infoBlock);
    void start();
    void stop();
};

#endif // SERVER_H
