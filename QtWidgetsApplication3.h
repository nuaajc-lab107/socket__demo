#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_QtWidgetsApplication3.h"
#include <qtcpserver.h>
#include <QAbstractSocket>
#include <QTcpServer>


class QtWidgetsApplication3 : public QMainWindow
{
    Q_OBJECT

public:
    QtWidgetsApplication3(QWidget *parent = Q_NULLPTR);
public slots:
    void startBtnClicked();

private:
    Ui::QtWidgetsApplication3Class ui;
    QTcpServer tcpServer;
    qint64 totalBytes;
    qint64 bytesReceived;
    qint64 fileNameSize;
    QString m_fileName;
    QByteArray m_inBlock;
};
