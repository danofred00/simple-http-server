
#include <QMessageBox>
#include <QFileDialog>
#include <QDir>
#include <QNetworkInterface>
#include "mainwindow.h"
#include "aboutdialog.h"
#include "ui_mainwindow.h"
#include "constants.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow), serverProcess{nullptr}
{
    ui->setupUi(this);
    ui->ipContainer->hide();
    serverProcessOutputLabel.setAlignment(Qt::AlignTop|Qt::AlignLeft);
    serverProcessOutputLabel.setWordWrap(true);
    ui->scrollArea->setWidget(&serverProcessOutputLabel);
    updateWindowName(QString());
    //
    connect(ui->startButton, SIGNAL(clicked(bool)), this, SLOT(startServerProcess()));
    connect(ui->actionStart_Server, SIGNAL(triggered(bool)), this, SLOT(startServerProcess()));

    connect(ui->closeButton, SIGNAL(clicked(bool)), this, SLOT(stopServerProcess()));
    connect(ui->actionStop_Server, SIGNAL(triggered(bool)), this, SLOT(stopServerProcess()));

    connect(ui->actionAbout_Qt, SIGNAL(triggered(bool)), qApp, SLOT(aboutQt()));
    connect(ui->actionAbout, SIGNAL(triggered(bool)), this, SLOT(showAboutDialog()));
    connect(ui->actionExit, SIGNAL(triggered(bool)), this, SLOT(aboutToQuit()));
    connect(ui->choosePathButton, SIGNAL(clicked(bool)), this, SLOT(chooseRootPath()));
    //
    connect(&serverProcess, SIGNAL(stateChanged(QProcess::ProcessState)), this, SLOT(onProcessStateChanged(QProcess::ProcessState)));
    connect(&serverProcess, SIGNAL(readyReadStandardOutput()), this, SLOT(onProcessOutputChanged()));

    serverProcess.setProcessChannelMode(QProcess::MergedChannels);
    serverProcess.setParent(this);
    serverProcess.setProgram("./Http_Server_CLI");
}

MainWindow::~MainWindow()
{
    stopServerProcess();
    delete ui;
}

void MainWindow::aboutToQuit()
{
    stopServerProcess();
    emit quit();
}

void MainWindow::startServerProcess()
{
    port = ui->portEdit->text().toInt();
    rootPath = ui->rootP->text();
    // update arguments
    auto arguments = QStringList();
    arguments << "-p" << QString::number(port);
    arguments << "-r" << rootPath;
    serverProcess.setArguments(arguments);
    //
    serverProcess.start();
}

void MainWindow::stopServerProcess()
{
    if(isTheServerStarted()) {
        serverProcess.close();
        serverProcessOutputLabel.clear();
    }
}

void MainWindow::showAboutDialog()
{
    AboutDialog(this).exec();
}

void MainWindow::onProcessStateChanged(QProcess::ProcessState state)
{
    auto isRunning = isTheServerStarted();
    QString text;
    if(!isRunning) {
        text = "Closed";
        ui->ipContainer->hide();
        handleCloseOperation();
    }
    else {
        text = "Running";
        updateIps();
        ui->ipContainer->show();
    }
    ui->closeButton->setDisabled(!isRunning);
    ui->startButton->setDisabled(isRunning);
    ui->statusLabel->setText(text);
    updateWindowName(QString("(%1)").toUpper().arg(text));
}

void MainWindow::onProcessOutputChanged()
{
    serverProcessOutputLabel.setText(serverProcess.readAllStandardOutput());
}

void MainWindow::handleCloseOperation()
{
    QString message;

    switch (serverProcess.exitCode()) {
    case EXIT_PORT_IS_USE:
        message = QString(QApplication::translate("MainWindow", "The port %1 is already use.")).arg(port);
        break;
    case EXIT_INVALID_ARGS:
        message = QString(QApplication::translate("MainWindow", "Failed to start The server, please check if the port and the rootPath are correct."));
        break;
    default:
        break;
    }

    if(!message.isEmpty())
        QMessageBox::critical(this, "Error", message);
}

void MainWindow::onProcessErrorOccurred(QProcess::ProcessError error)
{
    if(error == QProcess::ProcessError::Crashed) {
        QMessageBox::critical(
            this, "Error",
            QApplication::translate("MainWindow", "Something is Wrong. Please Restart the server"));
        return;
    }
}

void MainWindow::updateIps()
{
    QString ipAddresses;
    for(const auto & interface : QNetworkInterface::allInterfaces())
    {
        auto entry = interface.addressEntries().at(0);
        ipAddresses.append(QString("<a href=\"http://%1:%2\">%1:%2</a> ").arg(entry.ip().toString()).arg(port));
    }
    ui->ipContainer->show();
    ui->ips->setText(ipAddresses);

}

void MainWindow::chooseRootPath()
{
    auto path = QFileDialog::getExistingDirectory(
        this,
        QApplication::translate("MainWindow", "Choose the RootPath"),
        QDir::homePath());
    if(!path.isEmpty())
        ui->rootP->setText(path);
}

void MainWindow::updateWindowName(const QStringView & name)
{
    this->setWindowTitle(QString("%1 %2").arg(APPLICATION_NAME, name));
}

bool MainWindow::isTheServerStarted()
{
    return serverProcess.state() == QProcess::Running;
}
