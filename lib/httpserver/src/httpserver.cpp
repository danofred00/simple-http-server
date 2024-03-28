#include <QTcpServer>
#include <QHostInfo>
#include <QDir>
#include <QStringEncoder>
#include "httpserver.h"
#include "helpers.h"
#include "file.h"


HttpServer::HttpServer(QHostAddress host, quint32 port, QObject *parent)
    : QObject{parent}, m_started{false}, m_port{port}, m_host{host}
{
    handleRequests();
}


void HttpServer::start()
{
    if(!m_server.listen(m_host, m_port))
    {
        emit failedToStart();
    }
    qDebug() << "Server started : " << QString("To %1:%2 In %3").arg(m_host.toString()).arg(m_port).arg(m_rootPath) ;
    m_started = true;
    emit started();
}

void HttpServer::close()
{
    emit readyToClose();
    for(const auto & tcpServer : m_server.servers())
    {
        if(tcpServer->isListening())
            tcpServer->close();
    }
    m_started = false;
    emit closed();
}

QString HttpServer::rootPath() const
{
    return m_rootPath;
}

void HttpServer::setRootPath(const QString &newRootPath)
{
    if (m_rootPath == newRootPath)
        return;
    m_rootPath = newRootPath;
    emit rootPathChanged();
}

void HttpServer::handleRequests()
{
    m_server.setMissingHandler(HttpServer::missingHandler);

    m_server.route("^.*$", [=](const QHttpServerRequest & request) {

        const auto path = request.url().path();
        const auto fullPath = helpers::makePath(m_rootPath, path);
        if(!helpers::exists(fullPath)) {

            auto buffer = helpers::readFile(":/template/error-404.html");
            helpers::replaceString(buffer, "${path}", path.toLatin1());

            auto response = QHttpServerResponse(buffer, QHttpServerResponse::StatusCode::NotFound);
            response.setHeader("Content-Type", "text/html");

            return response;
        }

        if(helpers::is_regular_file(fullPath))
            return QHttpServerResponse::fromFile(fullPath);

        QByteArray responseContent = helpers::readFile(":/template/index.html");
        File dirFile {"Simple Http Server", fullPath};
        //
        if(dirFile.is_directory()){
            // Getting the content
            QByteArray contentHtml { "<ul>" };
            for(const auto & file : dirFile.getChildrenAsFiles()){
                const auto relative = helpers::absoluteToRelativePath(file.path, m_rootPath);
                const auto html = QString("<li><a href=\"/%1\">%2</a></li>\n").arg(relative, file.name);
                contentHtml.append(html.toUtf8());
            }
            contentHtml.append("</ul>");

            // append the Generated Content in the responseContent
            helpers::replaceString(responseContent, "${dirName}", dirFile.name.toUtf8());
            helpers::replaceString(responseContent, "${dirContent}", contentHtml);
        }
        //
        auto response = QHttpServerResponse(responseContent, QHttpServerResponse::StatusCode::Ok);
        response.setHeader("Content-Type", "text/html;charset=utf-8");

        return response;
    });
}

void HttpServer::missingHandler(const QHttpServerRequest & request, QHttpServerResponder && responder)
{
    qDebug() << "[HttpServer::missingHandler] started ... ";
}

