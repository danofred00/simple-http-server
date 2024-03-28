#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QProcess>
#include <QLabel>
#include <QHostInfo>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void updateWindowName(const QStringView & name);
    bool isTheServerStarted();

signals:
    void quit();

public slots:
    void aboutToQuit();
    void onProcessStateChanged(QProcess::ProcessState state);
    void startServerProcess();
    void stopServerProcess();

private slots:
    void showAboutDialog();
    void handleCloseOperation();
    void chooseRootPath();
    void onProcessOutputChanged();
    void onProcessErrorOccurred(QProcess::ProcessError error);

private:

    void updateIps();

    Ui::MainWindow *ui;
    QProcess serverProcess;
    quint32 port = 8080;
    QString rootPath;
    QLabel serverProcessOutputLabel;
};

#endif // MAINWINDOW_H
