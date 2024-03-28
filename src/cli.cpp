#include <QCoreApplication>
#include <QCommandLineParser>
#include <QCommandLineOption>
#include <filesystem>
#include <QDir>
#include "httpserver.h"
#include "constants.h"

namespace fs = std::filesystem;

/**
 * Usage: http_server_cli <options> [host] [port]
 */
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QCoreApplication::setApplicationName("Http Server CLI");
    QCoreApplication::setOrganizationName("Daniel Leussa");
    QCoreApplication::setApplicationVersion("v0.1.0");

    // parse Arguments
    QCommandLineParser parser;
    parser.addHelpOption();
    parser.addVersionOption();
    auto optionHost = QCommandLineOption(
        QStringList() << "p" << "port",
        QCoreApplication::translate("main", "The port to use (default: 8080)."),
        "port", QString::number(DEFAULT_PORT));
    parser.addOption(optionHost);
    auto optionRootPath = QCommandLineOption(
        QStringList() << "r" << "root",
        QCoreApplication::translate("main", "The Dir to use As a base (Absolute path required)"),
        "rootPath");
    parser.addOption(optionRootPath);

    parser.process(QCoreApplication::arguments());
    if(!parser.isSet("root"))
    {
        parser.showHelp(EXIT_INVALID_ARGS);
    }

    bool isOk;
    quint32 port = parser.value("port").toUInt(&isOk);
    auto rootPath = parser.value("root");

    if(!isOk || !(QDir::isAbsolutePath(rootPath) && fs::is_directory(rootPath.toStdString()))) {
        qDebug() << "INVALID_ARGS. Use --help|-h option to get help" ;
        return EXIT_INVALID_ARGS;
    }

    // creating the Server
    HttpServer server { QHostAddress::Any , port };

    QObject::connect(&server, &HttpServer::failedToStart, []() {
        QCoreApplication::exit(EXIT_PORT_IS_USE);
        exit(EXIT_PORT_IS_USE);
    });

    QObject::connect(&server, &HttpServer::closed, []() {
        QCoreApplication::quit();
    });

    server.setRootPath(rootPath);
    server.start();

    return a.exec();
}
