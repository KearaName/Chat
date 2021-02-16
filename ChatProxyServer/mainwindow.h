#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QUdpSocket>
#include <QtNetwork>
#include <QList>
#include <clients.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    QUdpSocket * udpSocket;

public slots:
    void readPendingDatagrams();

private:
    Ui::MainWindow *ui;

    QList<Clients*> * listClients;
};
#endif // MAINWINDOW_H
