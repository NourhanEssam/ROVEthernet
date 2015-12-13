#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    socket = new QUdpSocket(this);

    QTimer *timer = new QTimer();
    timer->setInterval(250);
    timer->start();

    connect(timer,SIGNAL(timeout()),this,SLOT(broadcast()));

    //testing
    socket2 = new QUdpSocket( this );
    socket2->bind( 9988 );
    connect( socket2, SIGNAL(readyRead()), this, SLOT(dataPending()) );

}

void MainWindow::broadcast()
{
    QByteArray buffer(50,0);
    QDataStream stream (&buffer,QIODevice::WriteOnly); //stream into the buffer
    stream.setVersion(QDataStream::Qt_4_0);

    stream << QString("character");
    stream <<qint32(5);

    socket->writeDatagram(buffer,QHostAddress::Broadcast,9988);
}


void MainWindow::dataPending()
{
while( socket2->hasPendingDatagrams() )
{
QByteArray buffer( socket2->pendingDatagramSize(), 0 );
socket2->readDatagram( buffer.data(), buffer.size() );
QDataStream stream2( buffer );
stream2.setVersion( QDataStream::Qt_4_0 );
QString str;
qint32 value;
stream2 >> str >> value;
qDebug()<< str << value;
}
}


MainWindow::~MainWindow()
{
    delete ui;
}
