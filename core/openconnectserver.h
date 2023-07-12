#ifndef OPENCONNECTSERVER_H
#define OPENCONNECTSERVER_H

#include "keypair.h"

namespace Start {

class OpenConnectServer
{
public:
    OpenConnectServer(QString serverAddress = QString());

    QString serverAddress() const;
    void setServerAddress(const QString &serverAddress);

    QString getGroupname() const;
    void setGroupname(const QString &groupname);

    unsigned get_server_hash(QByteArray& hash) const;
    void get_server_hash(QString& hash) const;
    void set_server_hash(const unsigned algo, const QByteArray& hash);

    const char* getProtocol_name() const;
    void setProtocol_name(const QString &protocol_name);

private:
    QString m_serverAddress;
    QString m_groupname;
    bool m_batch_mode;
    QString m_token_string;
    int m_token_type;
    int m_protocol_id;
    QString m_protocol_name;
    QByteArray m_server_hash;
    unsigned m_server_hash_algo;
    Cert m_ca_cert;
    KeyPair m_client;
};

} // namespace Symlex

#endif // OPENCONNECTSERVER_H
