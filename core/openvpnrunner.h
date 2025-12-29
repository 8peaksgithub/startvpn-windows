#ifndef OPENVPNRUNNER_H
/*
 * Copyright (C) 2022 StartVPN
 * Developed By MSGJewel
 */

#define OPENVPNRUNNER_H

#include <QObject>

class QProcess;
class QTcpServer;
class QTcpSocket;
class QTemporaryFile;

class OpenVpnRunner : public QObject
{
    Q_OBJECT

public:
    OpenVpnRunner(QObject *parent = 0);
    ~OpenVpnRunner();
    const QString &disconnectReason() const { return m_disconnectReason; }

public slots:
    bool connect(const QString &config, const QString &username, const QString &password);
    void disconnect();

signals:
    void connected();
    void connecting();
    void disconnected();
    void processExit();
    void transfer(quint64 down, quint64 up);

public:
    QString escape(const QString &in);
    void managementSetup();
    QString m_disconnectReason;
    QString m_username;
    QString m_password;
    QProcess *m_process;
    QTcpServer *m_managementServer;
    QTcpSocket *m_managementConnection;
    QTemporaryFile *m_configFile;
    bool m_hasDisconnected;

private slots:
    void managmentReadLine();
};

#endif // OPENVPNRUNNER_H
