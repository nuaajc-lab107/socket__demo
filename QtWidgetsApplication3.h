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
};
