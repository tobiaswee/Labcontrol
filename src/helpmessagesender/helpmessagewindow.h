#ifndef HELPMESSAGEWINDOW_H
#define HELPMESSAGEWINDOW_H

#include "ui_helpmessagewindow.h"

#include <QMainWindow>
#include <QMessageBox>
#include <QtNetwork>

namespace Ui {
class HelpMessageWindow;
} // namespace Ui

class lcHelpMessageWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit lcHelpMessageWindow(const QString &argServerIP,
                                 const unsigned short int &argServerPort,
                                 QWidget *argParent = nullptr);
    ~lcHelpMessageWindow();

private:
    quint16 blockSize = 0;
    QTcpSocket *helpMessageSocket = nullptr;
    QNetworkSession *networkSession = nullptr;
    const quint16 serverPort = 0;
    const QHostAddress serverAddress;
    Ui::HelpMessageWindow *ui;

private slots:
    void RequestHelp();
    void ReadHelpReply();
    void DisplayError(QAbstractSocket::SocketError socketError);
    void OpenedSession();
};

#endif // HELPMESSAGEWINDOW_H
