#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_QtWidgetsApplication3.h"
#include <qtcpserver.h>
#include <QAbstractSocket>
#include <QTcpServer>
class QTcpSocket;
class QFile;

class QtWidgetsApplication3 : public QMainWindow
{
    Q_OBJECT

public:
    QtWidgetsApplication3(QWidget *parent = Q_NULLPTR);
public slots:
    void start();
    void acceptConnection();
    void updateServerProgress();
    void displayError(QAbstractSocket::SocketError socketError);
    void startBtnClicked();

private:
    Ui::QtWidgetsApplication3Class ui;
    QTcpServer m_tcpServer;
    QTcpSocket* m_tcpServerConnection;
    qint64 m_totalBytes;
    qint64 m_bytesReceived;
    qint64 m_fileNameSize;
    QString m_fileName;
    QFile* m_localFile;
    QByteArray m_inBlock;
};
