#include "helpmessagewindow.h"

#include <iostream>

lcHelpMessageWindow::lcHelpMessageWindow( const QString &argServerIP, const unsigned short int &argServerPort, QWidget *argParent ) :
    QMainWindow{ argParent },
    helpMessageSocket { new QTcpSocket{ this } },
    serverPort{ argServerPort },
    serverAddress{ argServerIP },
    ui{ new Ui::HelpMessageWindow }
{
    ui->setupUi( this );
    connect( ui->PBAskForHelp, &QPushButton::clicked, this, &lcHelpMessageWindow::RequestHelp );
    connect( helpMessageSocket, &QTcpSocket::readyRead, this, &lcHelpMessageWindow::ReadHelpReply );
    connect( helpMessageSocket, SIGNAL( error( QAbstractSocket::SocketError ) ),
             this, SLOT( DisplayError( QAbstractSocket::SocketError ) ) );

    QNetworkConfigurationManager manager;
    if ( manager.capabilities() & QNetworkConfigurationManager::NetworkSessionRequired ) {
        // Get saved network configuration
        QSettings settings{ QSettings::UserScope, QLatin1String{ "QtProject" } };
        settings.beginGroup( QLatin1String{ "QtNetwork" } );
        const QString id = settings.value( QLatin1String{ "DefaultNetworkConfiguration" } ).toString();
        settings.endGroup();

        // If the saved network configuration is not currently discovered use the system default
        QNetworkConfiguration config = manager.configurationFromIdentifier( id );
        if ( ( config.state() & QNetworkConfiguration::Discovered ) != QNetworkConfiguration::Discovered ) {
            config = manager.defaultConfiguration();
        }

        networkSession = new QNetworkSession{ config, this };
        connect( networkSession, &QNetworkSession::opened, this, &lcHelpMessageWindow::OpenedSession );

        networkSession->open();
    }
}

lcHelpMessageWindow::~lcHelpMessageWindow() {
    delete ui;
}

void lcHelpMessageWindow::DisplayError( QAbstractSocket::SocketError socketError ) {
    QString errorMessage;
    switch ( socketError ) {
    case QAbstractSocket::RemoteHostClosedError:
        return;
    case QAbstractSocket::HostNotFoundError:
        errorMessage = tr( "An error occurred: The server could not be found for error reporting:\n" );
        break;
    case QAbstractSocket::ConnectionRefusedError:
        errorMessage = tr( "An error occurred: The connection was refused by the laboratory server:\n" );
        break;
    default:
        errorMessage = tr( "The following error occured:\n" );
    }
    errorMessage.append( tr("%1").arg( helpMessageSocket->errorString() ) );
    errorMessage.append( "\n\nPlease raise your hand to notify the experimenters." );
    ui->LSendingSuccess->setText( errorMessage );
}

void lcHelpMessageWindow::OpenedSession() {
    // Save the used configuration
    QNetworkConfiguration config = networkSession->configuration();
    QString id;
    if ( config.type() == QNetworkConfiguration::UserChoice )
        id = networkSession->sessionProperty( QLatin1String{ "UserChoiceConfiguration" } ).toString();
    else
        id = config.identifier();

    QSettings settings{ QSettings::UserScope, QLatin1String{ "QtProject" } };
    settings.beginGroup( QLatin1String{ "QtNetwork" } );
    settings.setValue( QLatin1String{ "DefaultNetworkConfiguration" }, id );
    settings.endGroup();
}

void lcHelpMessageWindow::ReadHelpReply() {
    QDataStream in( helpMessageSocket );
    in.setVersion( QDataStream::Qt_5_2 );

    if ( blockSize == 0 ) {
        if ( helpMessageSocket->bytesAvailable() < ( int )sizeof( quint16 ) )
            return;

        in >> blockSize;
    }

    if ( helpMessageSocket->bytesAvailable() < blockSize ) {
        return;
    }

    QString serverAnswer;
    in >> serverAnswer;

    if ( serverAnswer == "Help demand retrieved." ) {
        ui->LSendingSuccess->setText( tr( "Help message successfully sent.\nPlease wait for the experimenter to show up at your booth." ) );
        ui->PBAskForHelp->setEnabled( false );
    } else {
        ui->LSendingSuccess->setText( tr( "An error occurred sending the help message. Please raise your arm.\n\n'%1'" ).arg( serverAnswer ) );
    }
}

void lcHelpMessageWindow::RequestHelp() {
    blockSize = 0;
    helpMessageSocket->abort();
    helpMessageSocket->connectToHost( serverAddress, serverPort );
}
