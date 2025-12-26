#include "openconnectconnection.h"
#include "openconnectserver.h"

#include <QFuture>
#include <QNetworkProxy>
#include <QNetworkProxyQuery>
#include <QtConcurrent>

extern "C" {
#include <gnutls/pkcs11.h>
#include <openconnect.h>
}

#include "common/common.h"
#include "common/openconnectapi.h"
#include "common/logger.h"

#include <QTcpServer>
#include <QTcpSocket>

namespace Start {

OpenConnectConnection::OpenConnectConnection()
    : QObject(nullptr)
    , m_server(nullptr)
    , m_status(IDLE)
    , form_attempt(0)
    , form_pass_attempt(0)
{
    this->cmd_fd = INVALID_SOCKET;
    QObject::connect(this,&OpenConnectConnection::statusChanged,this, &OpenConnectConnection::onStatusChanged, Qt::QueuedConnection);
}

OpenConnectConnection::~OpenConnectConnection()
{
    int counter = 10;
    if (this->futureWatcher.isRunning() == true) {
        this->disconnect();
    }

    while (this->futureWatcher.isRunning() == true && counter > 0) {
        ms_sleep(200);
        counter--;
    }
}

void OpenConnectConnection::init()
{
    gnutls_global_init();
    openconnect_init_ssl();
}

void OpenConnectConnection::release()
{

}

std::shared_ptr<OpenConnectServer> OpenConnectConnection::server() const
{
    return m_server;
}

void OpenConnectConnection::setServer(std::shared_ptr<OpenConnectServer> server)
{
    m_server = server;
}

void OpenConnectConnection::setType(const int type)
{
    m_server_type = type;
}

void OpenConnectConnection::setOpenConfig(const QString config, const QString address)
{
    QString configTemp = QString::fromLatin1(QByteArray::fromBase64(config.toLatin1()));

    QStringList list = address.split(":");
    QString ip = list.value(0);
    QString port = list.value(1);

    QString remoteIP("remoteIP");
    QString remotePort("remotePort");
    QString remoteServerIP("remoteServerIP");

    configTemp.replace(configTemp.indexOf(remoteIP), remoteIP.size(), ip);
    configTemp.replace(configTemp.indexOf(remotePort), remotePort.size(), port);
    configTemp.replace(configTemp.indexOf(remoteServerIP), remoteServerIP.size(), ip);

    open_config = configTemp;
}

QString OpenConnectConnection::username() const
{
    return m_username;
}

void OpenConnectConnection::setUsername(const QString &username)
{
    qDebug()<<"set username:::"<<username;
    m_username = username;

}

QString OpenConnectConnection::password() const
{
    return m_password;
}

void OpenConnectConnection::setPassword(const QString &password)
{
    m_password = password;
}

OpenConnectConnection::ConnectionError OpenConnectConnection::connect()
{
    // Validate server type
    if(m_server_type != 1 && m_server_type != 2){
        Logger::instance().addMessage(QString("Unsupported server type: %1. Only OpenVPN (type 1) and OpenConnect (type 2) are supported.").arg(m_server_type));
        return SERVER_INVALID;
    }
    
    if(m_server_type == 2){
        // OpenConnect
        qDebug() << "Connecting Via Open Connect";

        if(m_server->serverAddress().isEmpty())
            return SERVER_INVALID;

        if(m_username.isEmpty() || m_password.isEmpty())
            return CREDENTIAL_INVALID;

        QFuture<void> future;
        QList<QNetworkProxy> proxies;
        QNetworkProxyQuery query;

        if (this->cmd_fd != INVALID_SOCKET) {
            return SOCKET_ACTIVE;
        }

        if (this->futureWatcher.isRunning() == true) {
            return INSTANCE_ACTIVE;
        }

        this->vpninfo = openconnect_vpninfo_new(Common::appName().toLatin1().data(), OpenConnectApi::validate_peer_cert, nullptr,
                                                OpenConnectApi::process_auth_form, OpenConnectApi::progress_vfn, this);

        if (this->vpninfo == nullptr) {
            throw std::runtime_error("initial setup fails");
        }

        this->cmd_fd = openconnect_setup_cmd_pipe(vpninfo);
        if (this->cmd_fd == INVALID_SOCKET) {
            Logger::instance().addMessage(QObject::tr("invalid socket"));
            throw std::runtime_error("pipe setup fails");
        }

        OpenConnectApi::set_sock_block(static_cast<int>(this->cmd_fd));

        openconnect_set_stats_handler(this->vpninfo, OpenConnectApi::stats_vfn);

        server()->setProtocol_name("anyconnect");

        openconnect_set_protocol(vpninfo, server()->getProtocol_name());
        openconnect_set_setup_tun_handler(vpninfo, OpenConnectApi::setupTunDevice);
        openconnect_parse_url(this->vpninfo, const_cast<char*>(server()->serverAddress().toLocal8Bit().data()));

        future = QtConcurrent::run(OpenConnectApi::main_loop, this, this);

        this->futureWatcher.setFuture(future);
    }else if(m_server_type == 1){
        // OpenVPN
        qDebug() << "Connecting Via Open VPN";
        if (open_config.length() == 0)
            return SERVER_INVALID;
        if(m_username.isEmpty() || m_password.isEmpty()){
            return CREDENTIAL_INVALID;
        }

        runner = new OpenVpnRunner(this);

        QObject::connect(runner, &OpenVpnRunner::transfer, this, &OpenConnectConnection::setStats);

        QObject::connect(runner, &OpenVpnRunner::disconnected, this, [=]() {
            emit statusChanged(IDLE);
        });
        QObject::connect(runner, &OpenVpnRunner::connected, this, [=]() {
            emit statusChanged(CONNECTED);
        });
        QObject::connect(runner, &OpenVpnRunner::connecting, this, [=]() {
            emit statusChanged(CONNECTING);
        });

        bool success = runner->connect(open_config, m_username, m_password);
        if (success) {
            Logger::instance().addMessage("OpenVPN Connect Success");
        }else{
            Logger::instance().addMessage("OpenVPN Connect Failed");
        }
    }

    return NONE;
}

void OpenConnectConnection::disconnect()
{
    // Clear sensitive data from memory
    if(!m_username.isEmpty()) {
        m_username.fill('\0');
        m_username.clear();
    }
    if(!m_password.isEmpty()) {
        m_password.fill('\0');
        m_password.clear();
    }
    
    if(m_server_type == 2){
        // Open Connect
        qDebug() << "Disconnecting Open Connect";
        char cmd = OC_CMD_CANCEL;

        if (this->cmd_fd != INVALID_SOCKET) {
            emit statusChanged(IDLING);
            int ret = pipe_write(this->cmd_fd, &cmd, 1);
            if (ret < 0) {
                Logger::instance().addMessage(QObject::tr("term_thread: IPC error: ") + QString::number(net_errno));
            }
            this->cmd_fd = INVALID_SOCKET;
            ms_sleep(200);
        } else {
            emit statusChanged(IDLE);
        }
    }else if(m_server_type == 1){
        // Open VPN
        qDebug() << "Disconnecting OpenVPN";

        if (runner->m_managementServer->isListening())
            runner->m_managementServer->close();
        if (runner->m_managementConnection && runner->m_managementConnection->isOpen())
            runner->m_managementConnection->abort();

        if (!runner->m_hasDisconnected) {
            runner->m_hasDisconnected = true;
            emit statusChanged(IDLE);
        }

        if(runner->m_process->state() == QProcess::Running){
            runner->m_process->close();
        }

//        if (runner->m_process->state() == QProcess::NotRunning)
//            deleteLater();
    }
}

void OpenConnectConnection::onStatusChanged(OpenConnectConnection::Status status)
{
    m_status = status;

    if(m_status == IDLE)
        cmd_fd = INVALID_SOCKET;
}

int OpenConnectConnection::ctspConnect()
{
    int ret;
    QString cert_file, key_file;
    QString ca_file;

    // Add CERT logic here

#ifdef Q_OS_WIN32
    const QString osName{ "win" };
#elif defined Q_OS_OSX
    const QString osName{ "mac-intel" };
#elif defined Q_OS_LINUX
    const QString osName = QString("linux%1").arg(QSysInfo::buildCpuArchitecture() == "i386" ? "" : "-64").toStdString().c_str();
#elif defined Q_OS_FREEBSD
    const QString osName = QString("freebsd%1").arg(QSysInfo::buildCpuArchitecture() == "i386" ? "" : "-64").toStdString().c_str();
#else
#error Define OS string of other platforms...
#endif
    openconnect_set_reported_os(vpninfo, osName.toStdString().c_str());

    ret = openconnect_obtain_cookie(vpninfo);
    if (ret != 0) {
        this->setLastError(tr("Authentication error; cannot obtain cookie"));
        return ret;
    }

    ret = openconnect_make_cstp_connection(vpninfo);
    if (ret != 0) {
        this->setLastError(tr("Error establishing the CSTP channel"));
        return ret;
    }

    return 0;
}

int OpenConnectConnection::dtls_connect()
{
    if (false != true) {
        int ret = openconnect_setup_dtls(vpninfo,
                                         SYMLEX_DTLS_TIMEOUT);
        if (ret != 0) {
            this->setLastError(tr("Error setting up DTLS (%1)").arg(ret));
            return ret;
        }
    }

    return 0;
}

void OpenConnectConnection::fetchInfo()
{
    const struct oc_ip_info* info;
    int ret = openconnect_get_ip_info(this->vpninfo, &info, nullptr, nullptr);
    if (ret == 0) {
        if (info->addr) {
            m_ip = info->addr;
            if (info->netmask) {
                m_ip += "/";
                m_ip += info->netmask;
            }
        }
        if (info->addr6) {
            m_ip6 = info->addr6;
            if (info->netmask6) {
                m_ip6 += "/";
                m_ip6 += info->netmask6;
            }
        }

        m_dns = info->dns[0];
        if (info->dns[1]) {
            m_dns += ", ";
            m_dns += info->dns[1];
        }
        if (info->dns[2]) {
            m_dns += " ";
            m_dns += info->dns[2];
        }
    }
    return;
}

void OpenConnectConnection::fetchCipherInfo()
{
    const char* cipher = openconnect_get_cstp_cipher(this->vpninfo);
    if (cipher != nullptr) {
        m_cstp = QLatin1String(cipher);
    }
    cipher = openconnect_get_dtls_cipher(this->vpninfo);
    if (cipher != nullptr) {
        m_dtls = QLatin1String(cipher);
    }
}

QString OpenConnectConnection::lastError() const
{
    return m_lastError;
}

void OpenConnectConnection::setLastError(const QString &lastError)
{
    m_lastError = lastError;
}

void OpenConnectConnection::setStats(uint64_t tx, uint64_t rx)
{
    emit statsChanged(tx,rx);
}

void OpenConnectConnection::logOpenConnect()
{
    /* now read %temp%\\vpnc.log and post it to our log */
    QString tfile = QDir::tempPath() + QDir::separator() + QLatin1String("vpnc.log");
    QFile file(tfile);
    if (file.open(QIODevice::ReadOnly) == true) {
        QTextStream in(&file);

        QString bannerMessage;
        bool processBannerMessage = false;

        while (!in.atEnd()) {
            const QString line{ in.readLine() };
            Logger::instance().addMessage(line);

            if (line == QLatin1String("--------------------- BANNER ---------------------")) {
                processBannerMessage = true;
                continue;
            }
            if (line == QLatin1String("------------------- BANNER end -------------------")) {
                processBannerMessage = false;
                continue;
            }
            if (processBannerMessage) {
                bannerMessage += line + "\n";
            }
        }
        file.close();
        if (file.remove() != true) {
            Logger::instance().addMessage(QLatin1String("Could not remove ") + tfile + ": " + QString::number(static_cast<int>(file.error())));
        }
    } else {
        Logger::instance().addMessage(QLatin1String("Could not open ") + tfile + ": " + QString::number(static_cast<int>(file.error())));
    }
}

int OpenConnectConnection::requestStats()
{
    char cmd = OC_CMD_STATS;
    if (this->cmd_fd != INVALID_SOCKET) {
        int ret = pipe_write(this->cmd_fd, &cmd, 1);
        if (ret < 0) {
            Logger::instance().addMessage(QObject::tr("update_stats: IPC error: ") + QString::number(net_errno));
            return -1;
        }
    } else {
        Logger::instance().addMessage(QObject::tr("update_stats: invalid socket"));
        return -1;
    }

    return 0;
}

OpenConnectConnection::Status OpenConnectConnection::status() const
{
    return m_status;
}

void OpenConnectConnection::setStatus(const Status &status)
{
    m_status = status;
}

} // namespace Symlex
