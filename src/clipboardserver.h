#ifndef CLIPBOARDSERVER_H
#define CLIPBOARDSERVER_H

#include "app.h"
#include <QLocalServer>
#include <QProcess>
#include "client_server.h"

class MainWindow;
class ClipboardBrowser;
class ClipboardItem;
class Arguments;
class QxtGlobalShortcut;

class ClipboardServer : public App
{
    Q_OBJECT

public:
    typedef enum {
        CommandSuccess,
        CommandError,
        CommandBadSyntax
    } CommandStatus;

    explicit ClipboardServer(int &argc, char **argv);
    ~ClipboardServer();
    bool isListening() { return m_server->isListening(); }
    CommandStatus doCommand(Arguments &args, QByteArray *response,
                            ClipboardBrowser *target_browser = NULL);
    void sendMessage(QLocalSocket* client, const QByteArray &message, int exit_code = 0);
    void sendMessage(QLocalSocket* client, const QString &message, int exit_code = 0) {
        sendMessage( client, message.toLocal8Bit(), exit_code );
    }

    void startMonitoring();
    void stopMonitoring();
    bool isMonitoring() {
        return m_monitor && m_monitor->state() != QProcess::NotRunning;
    }

    static QString serverName() { return ::serverName("CopyQserver"); }
    static QString monitorServerName() { return ::serverName("CopyQmonitor_server"); }

    Arguments *createGlobalShortcut(const QString &shortcut);

private:
    QLocalServer *m_server;
    QLocalServer *m_monitorserver;
    QLocalSocket *m_socket;
    MainWindow* m_wnd;
    QProcess *m_monitor;
    uint m_lastHash;
    QMap<QxtGlobalShortcut*, Arguments> m_shortcutActions;

public slots:
    void changeClipboard(const ClipboardItem *item);
    void loadSettings();

private slots:
    void newConnection();
    void newMonitorConnection();
    void readyRead();
    void monitorStateChanged( QProcess::ProcessState newState );
    void monitorStandardError();
    void shortcutActivated(QxtGlobalShortcut *shortcut);

};

#endif // CLIPBOARDSERVER_H
