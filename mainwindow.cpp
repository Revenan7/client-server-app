#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    socket = new QTcpSocket(this);
    connect(socket, &QTcpSocket::readyRead, this, &MainWindow::slotReadyRead);
    connect(socket, &QTcpSocket::disconnected, socket, &QTcpSocket::deleteLater);
    nextBlockSize = 0;

    this->setStyleSheet("background-color: #1A1A1A;"); // Чёрный фон
    ui->pushButton->setStyleSheet("background-color: #007ACC; color: white;"); // Синий фон кнопки с белым текстом
    ui->pushButton_2->setStyleSheet("background-color: #007ACC; color: white;");
    ui->lineEdit->setStyleSheet("background-color: white; color: black;"); // Белый фон текстового поля с чёрным текстом
    ui->textBrowser->setStyleSheet("background-color: #1A1A1A; color: #FFFFFF; border: 1px solid #007ACC;"); // Чёрный фон textBrowser с белым текстом и синей рамкой

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    socket -> connectToHost("127.0.0.1", 2323);
    qDebug() << "connetcedtry";
}

void MainWindow::SendToServer(QString str)
{
    Data.clear();
    QDataStream out(&Data, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_6_6);
    out << quint16(0) << QTime::currentTime() << str;
    out.device() -> seek(0);
    out << quint16(Data.size() - sizeof(quint16));
    socket -> write(Data);
    ui -> lineEdit -> clear();
}

void MainWindow::slotReadyRead()
{
    QDataStream in(socket);
    in.setVersion(QDataStream::Qt_6_6);

    while (socket->bytesAvailable() > 0) // Проверяем, что вообще есть данные для чтения
    {
        // Читаем размер блока, если его еще нет
        if (nextBlockSize == 0 && !readNextBlockSize(in))
        {
            break; // Если не удалось прочитать размер, выходим
        }

        // Если недостаточно данных для полного сообщения, ждем
        if (socket->bytesAvailable() < nextBlockSize)
        {
            break;
        }

        // Читаем данные
        QString str;
        QTime time;
        in >> time >> str;

        // Сбрасываем размер блока для следующего сообщения
        nextBlockSize = 0;

        // Отображаем данные
        ui->textBrowser->append(time.toString() + " " + str);
    }
}

bool MainWindow::readNextBlockSize(QDataStream &in)
{
    // Проверяем, достаточно ли данных для чтения размера блока (2 байта)
    if (socket->bytesAvailable() < sizeof(quint16))
    {
        return false; // Данных недостаточно — ждём
    }

    in >> nextBlockSize; // Читаем размер следующего блока
    return true;
}



void MainWindow::on_pushButton_2_clicked()
{
    SendToServer(ui->lineEdit->text());

}


void MainWindow::on_lineEdit_returnPressed()
{
    SendToServer(ui->lineEdit->text());

}

