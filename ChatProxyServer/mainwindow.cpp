#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    listClients = new QList<Clients*>;
    udpSocket = new QUdpSocket(this);
    udpSocket->bind(QHostAddress::Any, 7755);

    connect(udpSocket, SIGNAL(readyRead()),
            this, SLOT(readPendingDatagrams()));
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::readPendingDatagrams()
 {
     while (udpSocket->hasPendingDatagrams()) {
         QByteArray datagram;
         datagram.resize(udpSocket->pendingDatagramSize());
         QHostAddress sender;
         quint16 senderPort;


         udpSocket->readDatagram(datagram.data(), datagram.size(),
                                 &sender, &senderPort);

         ui->plainTextEdit->appendPlainText("( ip -> " + sender.toString() + "; port -> " +
                                            QString("%1").arg(senderPort)+ ";) " + QString(datagram));
        if(QString(datagram).left(6)=="[room]") {
            QString room = QString(datagram).mid(6);
            Clients * client = new Clients(sender, senderPort, room);
            listClients->append(client);
            QString Message =  "[refresh]";
            for(int i=0;i<listClients->size();i++)
                if(listClients->at(i)->Room==room)
                {
                    udpSocket->writeDatagram(Message.toLatin1(),  listClients->at(i)->IPAddress, listClients->at(i)->port);
                }
        }
        else
        if(QString(datagram).left(9)=="[message]")
        {
            QString Message =  QString(datagram);
            QString room;
            for(int i=0;i<listClients->size();i++)
                if(listClients->at(i)->IPAddress==sender && listClients->at(i)->port==senderPort)
                {
                    room=listClients->at(i)->Room;
                    break;
                }
            for(int i=0;i<listClients->size();i++)
                if(listClients->at(i)->Room==room)
                {
                    udpSocket->writeDatagram(Message.toLatin1(),  listClients->at(i)->IPAddress, listClients->at(i)->port);
                }
        }
        else
        if(QString(datagram).left(6)=="[exit]") {

            for(int i=0;i<listClients->size();i++)
                if(listClients->at(i)->IPAddress==sender && listClients->at(i)->port==senderPort)
                {
                    QString room=listClients->at(i)->Room;
                    QString Message = "[refresh]";
                    listClients->removeAt(i);
                    for(int j =0; j<listClients->size();j++)
                    {
                        if(listClients->at(j)->Room==room)
                        {
                            udpSocket->writeDatagram(Message.toLatin1(),  listClients->at(j)->IPAddress, listClients->at(j)->port);
                        }
                    }
                    break;
                }
        }
     }

 }
