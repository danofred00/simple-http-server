#ifndef HTTPSERVER_H
#define HTTPSERVER_H

#include <QObject>
#include <QHostAddress>
#include <QHttpServer>

class HttpServer : public QObject
{
    Q_OBJECT
public:

    HttpServer(
        QHostAddress host = QHostAddress::Any,
        quint32 port = 8080,
        QObject *parent = nullptr);

    void start();
    void close();

    inline QHostAddress host() const { return m_host; }
    inline quint32 port() const { return m_port; }
    inline bool isStarted() const { return m_started; }

    QString rootPath() const;
    void setRootPath(const QString &newRootPath);

signals:
    void started();
    void closed();
    void failedToStart();
    void readyToClose();
    void rootPathChanged();

private:

    void handleRequests();
    static void missingHandler(const QHttpServerRequest & request, QHttpServerResponder && responder);

    QString m_rootPath;
    QHttpServer m_server;
    QHostAddress m_host;
    quint32 m_port;
    bool m_started;
};

#endif // HTTPSERVER_H
