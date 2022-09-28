#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTime>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    socket = new QTcpSocket(this);
    connect(socket,&QTcpSocket::readyRead, this, &MainWindow::slotReadyRead);

    nextBlockSize = 0;
    ipStr = ui->lineEdit_IP->text();
    portStr = ui->lineEdit_port->text().toUShort();
    socket->connectToHost(ipStr, portStr);
    ui->lineEdit_IP->setEnabled(false);
    ui->lineEdit_port->setEnabled(false);
    ui->lineEdit_name->setEnabled(false);
    ui->pushButton->setEnabled(false);
    ui->pushButton_disconnected->setEnabled(true);
    ui->pushButton_3->setEnabled(true);
    ui->lineEdit_send->setEnabled(true);
    sendToServer(ui->lineEdit_name->text()+" connected");
}


void MainWindow::slotReadyRead ()
{
    socket = (QTcpSocket*) sender();                                 //В переменную сокет нужно записать именно тот сокет с которого пришёл запрос.
    QDataStream in(socket);
    in.setVersion(QDataStream::Qt_6_3);
    if(in.status()==QDataStream::Ok)
    {
        for(;;){
            if (nextBlockSize == 0)
            {
                if(socket->bytesAvailable()<2)
                {
                    break;
                }
                in>>nextBlockSize;
            }
            if(socket->bytesAvailable()<nextBlockSize)
            {
                break;
            }
            QString str;
            QTime time;
            in>>time>>str;
            ui->textBrowser->append("<" + time.toString() + ">" + str);
            nextBlockSize = 0;
        }
    }
    else
    {
        ui->textBrowser->append("Read error");
    }
}

void MainWindow::sendToServer(QString str)
{
    Data.clear();
    QDataStream out(&Data, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_6_3);
    out<<quint16(0)<<QTime::currentTime()<<str;
    out.device()->seek(0);
    out<<quint16(Data.size()-sizeof(quint16));
    socket->write(Data);
}

void MainWindow::on_pushButton_3_clicked()
{
    sendToServer("Msg by " + ui->lineEdit_name->text() + ": " + ui->lineEdit_send->text());
    ui->lineEdit_send->clear();
}

void MainWindow::on_pushButton_disconnected_clicked()
{
    ui->pushButton_3->setEnabled(false);
    ui->lineEdit_send->setEnabled(false);
    ui->pushButton_disconnected->setEnabled(false);
    ui->lineEdit_IP->setEnabled(true);
    ui->lineEdit_port->setEnabled(true);
    ui->lineEdit_name->setEnabled(true);
    ui->pushButton->setEnabled(true);
    sendToServer(ui->lineEdit_name->text() + " disconnected");
    if(socket->disconnect() == false)
    {
        ui->textBrowser->append("Something went wrong. You are not disconnected from the server");
    }
    else
    {
        sendToServer(ui->lineEdit_name->text() + " disconnected");
        ui->textBrowser->append("You're successful disconnected");
        socket->disconnect();
        delete socket;
    }
}



