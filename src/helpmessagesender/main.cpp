#include "helpmessagewindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QSettings labSettings{"Economic Laboratory", "Labcontrol"};
    lcHelpMessageWindow w{labSettings.value("server_ip", "127.0.0.1").toString(),
                labSettings.value("client_help_server_port", "0").toUInt()};
    w.show();

    return a.exec();
}
