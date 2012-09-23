#include <QtCore/QCoreApplication>
#include <QApplication>
#include <QtDBus/QtDBus>
#include <QMessageBox>

#define PROTOCOL "PROTOCOL 8"

QString     prepareLogin(char* av)
{
    QString param = av;

    if (param.startsWith("skype:"))
        param.remove(0,6);
    if (param.startsWith("///"))
        param.remove(0,3);
    int i = param.indexOf("?");

    if (i != -1)
        param.resize(i);
    return (param);
}

void        showError(QString message)
{
    QMessageBox box;
    box.setWindowTitle("Skype Handler");
    box.setText(message);
    box.exec();
    exit(EXIT_FAILURE);
}

bool        skypeInit(QDBusInterface & iface)
{
   QDBusReply<QString> reply;
   reply = iface.call("Invoke", "NAME skypehandler");
   if (!reply.isValid())
       showError("DBus Com Error");
   if(reply.value() != "OK")
        showError("Authefication with Skype: " + reply.value());
   return true;
}

bool        protocolVerification(QDBusInterface & iface)
{
   QDBusReply<QString> reply;
   reply = iface.call("Invoke", PROTOCOL);
   if (!reply.isValid())
       showError("DBus Com Error");
   if(reply.value() != PROTOCOL)
        showError("Protocol Verification Failed: " + reply.value());
   return true;
}

bool        invokeCall(QDBusInterface & iface, QString param)
{
   QDBusReply<QString> reply;
   reply = iface.call("Invoke", "CALL "+ param);
   if (!reply.isValid())
       showError("DBus Com Error");
   if (QString(reply.value()).startsWith("ERROR"))
       showError(reply.value());
   return true;
}


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    if (!QDBusConnection::sessionBus().isConnected()) {
        fprintf(stderr, "Cannot connect to the D-Bus session bus.\n"
                "To start it, run:\n"
                "\teval `dbus-launch --auto-syntax`\n");
        return (EXIT_FAILURE);
    }

    QDBusInterface iface("com.Skype.API",  "/com/Skype", "com.Skype.API", QDBusConnection::sessionBus());
    if (iface.isValid()) {
        skypeInit(iface);
        protocolVerification(iface);
        invokeCall(iface, prepareLogin(argv[1]));
    }
    else
       showError("Please start Skype, and retry");
    return (EXIT_SUCCESS);
}
