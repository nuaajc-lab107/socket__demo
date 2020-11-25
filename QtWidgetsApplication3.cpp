#include "QtWidgetsApplication3.h"
#include "ui_QtWidgetsApplication3.h"
#include <QtNetwork>
#include <QDebug>
#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif


QtWidgetsApplication3::QtWidgetsApplication3(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    setWindowTitle(QString::fromUtf8("计算机网络课设，2018023417"));
    ui.startButton->setText("开始监听");
    ui.StatusLabel->setText("设置监听端口");
    connect(&tcpServer, SIGNAL(newConnection()), this, SLOT(acceptConnection()));
    connect(ui.startButton, &QPushButton::clicked, this, &QtWidgetsApplication3::startBtnClicked);

}

void QtWidgetsApplication3::startBtnClicked()
{
    if (!tcpServer.listen(QHostAddress::LocalHost, 10086)) {
        qDebug() << tcpServer.errorString();
        close();
        return;
    }
    ui.startButton->setEnabled(false);
    totalBytes = 0;
    bytesReceived = 0;
    fileNameSize = 0;
    ui.StatusLabel->setText("监听中");
    ui.progressBar->reset();
}

void QtWidgetsApplication3::acceptConnection()
{
    tcpServerConnection = tcpServer.nextPendingConnection();
    connect(tcpServerConnection, SIGNAL(readyRead()), this, SLOT(updateServerProgress()));
    connect(tcpServerConnection, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(displayError(QAbstractSocket::SocketError)));
    ui.StatusLabel->setText("接受连接");
    //关闭服务器不再监听，直接进入数据收发
    tcpServer.close();
}

void QtWidgetsApplication3::updateServerProgress()
{
    QDataStream in(tcpServerConnection);
    in.setVersion(QDataStream::Qt_5_7);
    // 如果接收到的数据小于16个字节，保存到来的文件头结构
    if (bytesReceived <= sizeof(qint64) * 2) 
    {
        if ((tcpServerConnection->bytesAvailable() >= sizeof(qint64) * 2) && (fileNameSize == 0)) 
        {
            // 接收数据总大小信息和文件名大小信息
            in >> totalBytes >> fileNameSize;
            bytesReceived += sizeof(qint64) * 2;
        }
        if ((tcpServerConnection->bytesAvailable() >= fileNameSize) && (fileNameSize != 0)) 
        {
            // 接收文件名，并建立文件
            in >> fileName;
            ui.StatusLabel->setText(tr("接收文件 %1 …").arg(fileName));
            bytesReceived += fileNameSize;
            localFile = new QFile(fileName);
            if (!localFile->open(QFile::WriteOnly)) 
            {
                qDebug() << "文件打开失败";
                return;
            }
        }
        else 
        {
            return;
        }
    }
    // 如果接收的数据小于总数据，那么写入文件
    if (bytesReceived < totalBytes) 
    {
        bytesReceived += tcpServerConnection->bytesAvailable();
        inBlock = tcpServerConnection->readAll();
        localFile->write(inBlock);
        inBlock.resize(0);
    }
    ui.progressBar->setMaximum(totalBytes);
    ui.progressBar->setValue(bytesReceived);

    // 接收数据完成时
    if (bytesReceived == totalBytes) 
    {
        tcpServerConnection->close();
        localFile->close();
        ui.startButton->setEnabled(true);
        ui.StatusLabel->setText(tr("接收文件 %1 成功！").arg(fileName));
    }
}

void QtWidgetsApplication3::displayError(QAbstractSocket::SocketError socketError)
{
    Q_UNUSED(socketError)
    qDebug() << tcpServerConnection->errorString();
    tcpServerConnection->close();
    ui.progressBar->reset();
    ui.StatusLabel->setText("服务端就绪");
    ui.startButton->setEnabled(true);
}