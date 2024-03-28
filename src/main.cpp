
#include <QApplication>
#include "mainwindow.h"

int main(int argc, char * argv[])
{
    QApplication app { argc, argv };

    MainWindow window;

    QObject::connect(&window, &MainWindow::quit, &app, &QApplication::quit, Qt::QueuedConnection);

    window.show();

    return app.exec();
}
