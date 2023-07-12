#include "openconnectserver.h"

namespace Start {

OpenConnectServer::OpenConnectServer(QString serverAddress)
    : m_serverAddress(serverAddress)
{

}

QString OpenConnectServer::serverAddress() const
{
    return m_serverAddress;
}

void OpenConnectServer::setServerAddress(const QString &serverAddress)
{
    m_serverAddress = serverAddress;
}

unsigned OpenConnectServer::get_server_hash(QByteArray &hash) const
{
    hash = this->m_server_hash;
    return this->m_server_hash_algo;
}

void OpenConnectServer::get_server_hash(QString &hash) const
{
    if (this->m_server_hash_algo == 0) {
        hash = "";
    } else {
        hash = gnutls_mac_get_name((gnutls_mac_algorithm_t)this->m_server_hash_algo);
        hash += ":";
        hash += this->m_server_hash.toHex();
    }
}

void OpenConnectServer::set_server_hash(const unsigned algo, const QByteArray &hash)
{
    this->m_server_hash_algo = algo;
    this->m_server_hash = hash;
}

const char *OpenConnectServer::getProtocol_name() const
{
    QByteArray data{ m_protocol_name.toLatin1() };
    return data.data();
}

void OpenConnectServer::setProtocol_name(const QString &protocol_name)
{
    m_protocol_name = protocol_name;
}

QString OpenConnectServer::getGroupname() const
{
    return m_groupname;
}

void OpenConnectServer::setGroupname(const QString &groupname)
{
    m_groupname = groupname;
}

} // namespace Symlex
