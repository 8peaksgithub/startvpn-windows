#include "openconnectapi.h"
#include "core/openconnectconnection.h"
#include "core/openconnectserver.h"
#include "common.h"
#include "core/gtdb.h"
#include "logger.h"

#include <QCoreApplication>
#include <QDir>

extern "C" {
#include <gnutls/pkcs11.h>
#include <openconnect.h>
}

namespace Start {

static int last_form_empty;

int OpenConnectApi::validate_peer_cert(void* privdata, const char* reason)
{
    Q_UNUSED(reason);
    OpenConnectConnection* vpn = static_cast<OpenConnectConnection*>(privdata);
    unsigned char* der = nullptr;

    int der_size = openconnect_get_peer_cert_DER(vpn->vpninfo, &der);
    if (der_size <= 0) {
        Logger::instance().addMessage(QObject::tr("Peer's certificate has invalid size!"));
        return -1;
    }

    const char* hash = openconnect_get_peer_cert_hash(vpn->vpninfo);
    if (hash == nullptr) {
        Logger::instance().addMessage(QObject::tr("Error getting peer's certificate hash"));
        return -1;
    }

    gnutls_datum_t raw;
    raw.data = der;
    raw.size = static_cast<unsigned int>(der_size);

    gtdb tdb(vpn->server());
    int ret = gnutls_verify_stored_pubkey(reinterpret_cast<const char*>(&tdb),
                                          tdb.tdb, "", "", GNUTLS_CRT_X509, &raw, 0);

    char* details = openconnect_get_peer_cert_details(vpn->vpninfo);
    QString dstr;
    if (details != nullptr) {
        dstr = QString::fromUtf8(details);
        free(details);
    }

    bool save = false;
    if (ret == GNUTLS_E_NO_CERTIFICATE_FOUND) {
        Logger::instance().addMessage(QObject::tr("peer is unknown"));
        // Auto insert new key
        save = true;
    } else if (ret == GNUTLS_E_CERTIFICATE_KEY_MISMATCH) {
        Logger::instance().addMessage(QObject::tr("peer's key has changed!"));
        // Auto-update key if changed
        save = true;
    } else if (ret < 0) {
        QString str = QObject::tr("Could not verify certificate: ");
        str += gnutls_strerror(ret);
        Logger::instance().addMessage(str);
        return -1;
    }

    if (save != false) {
        Logger::instance().addMessage(QObject::tr("saving peer's public key"));
        ret = gnutls_store_pubkey(reinterpret_cast<const char*>(&tdb), tdb.tdb,
                                  "", "", GNUTLS_CRT_X509, &raw, 0, 0);
        if (ret < 0) {
            QString str = QObject::tr("Could not store certificate: ");
            str += gnutls_strerror(ret);
            Logger::instance().addMessage(str);
        } else {
            //            vpn->server()->save(); //TODO: Move to CONFIG
        }
    }
    return 0;
}

int OpenConnectApi::process_auth_form(void *privdata, oc_auth_form *form)
{
    OpenConnectConnection* vpn = static_cast<OpenConnectConnection*>(privdata);
    bool ok;
    QString text;
    struct oc_form_opt* opt;
    QStringList gitems;
    QStringList ditems;
    int i, idx;

    if (form->banner) {
        Logger::instance().addMessage(QLatin1String(form->banner));
    }

    if (form->message) {
        Logger::instance().addMessage(QLatin1String(form->message));
    }

    if (form->error) {
        Logger::instance().addMessage(QLatin1String(form->error));
    }

    int empty = 1;
    if (form->authgroup_opt) {
        struct oc_form_opt_select* select_opt = form->authgroup_opt;

        for (i = 0; i < select_opt->nr_choices; i++) {
            ditems << select_opt->choices[i]->label;
            gitems << select_opt->choices[i]->name;
        }

        if (select_opt->nr_choices == 1) {
            openconnect_set_option_value(&select_opt->form,
                                         select_opt->choices[0]->name);
        } else if (gitems.contains(vpn->server()->getGroupname())) {
            openconnect_set_option_value(&select_opt->form,
                                         vpn->server()->getGroupname().toLatin1().data());
        } else {
            // TODO: Input
            goto fail;
        }

        if (vpn->authgroup_set == 0) {
            vpn->authgroup_set = 1;
            return OC_FORM_RESULT_NEWGROUP;
        }
    }

    for (opt = form->opts; opt; opt = opt->next) {
        text.clear();
        if (opt->flags & OC_FORM_OPT_IGNORE)
            continue;

        if (opt->type == OC_FORM_OPT_SELECT) {
            QStringList items;
            struct oc_form_opt_select* select_opt = reinterpret_cast<oc_form_opt_select*>(opt);

            Logger::instance().addMessage(QLatin1String("Select form: ") + QLatin1String(opt->name));

            if (select_opt == form->authgroup_opt) {
                continue;
            }

            for (i = 0; i < select_opt->nr_choices; i++) {
                items << select_opt->choices[i]->label;
            }

            {
                ok = false; // TODO: Fix
            }

            if (!ok)
                goto fail;

            idx = ditems.indexOf(text);
            if (idx == -1)
                goto fail;

            openconnect_set_option_value(opt, select_opt->choices[idx]->name);
            empty = 0;
        } else if (opt->type == OC_FORM_OPT_TEXT) {
            Logger::instance().addMessage(QLatin1String("Text form: ") + QLatin1String(opt->name));

            if (vpn->form_attempt == 0
                && vpn->username().isEmpty() == false
                && strcasecmp(opt->name, "username") == 0) {
                openconnect_set_option_value(opt,
                                             vpn->username().toLatin1().data());
                empty = 0;
                continue;
            }

            goto fail;

        } else if (opt->type == OC_FORM_OPT_PASSWORD) {
            Logger::instance().addMessage(QLatin1String("Password form: ") + QLatin1String(opt->name));

            if (vpn->form_pass_attempt == 0
                && vpn->password().isEmpty() == false
                && strcasecmp(opt->name, "password") == 0) {
                openconnect_set_option_value(opt,
                                             vpn->password().toLatin1().data());
                empty = 0;
                continue;
            }

            goto fail;
        } else {
            Logger::instance().addMessage(QLatin1String("unknown type ") + QString::number(static_cast<int>(opt->type)));
        }
    }

    /* prevent infinite loops if the authgroup requires certificate auth only */
    if (last_form_empty && empty) {
        return OC_FORM_RESULT_CANCELLED;
    }
    last_form_empty = empty;

    return OC_FORM_RESULT_OK;
fail:
    return OC_FORM_RESULT_CANCELLED;
}

void OpenConnectApi::progress_vfn(void* privdata, int level, const char* fmt, ...)
{
    Q_UNUSED(privdata);
    char buf[512];
    size_t len;
    va_list args;

    /* don't spam */
    if (level == PRG_TRACE)
        return;

    buf[0] = 0;
    va_start(args, fmt);
    vsnprintf(buf, sizeof(buf), fmt, args);
    va_end(args);

    len = strlen(buf);
    if (buf[len - 1] == '\n')
        buf[len - 1] = 0;
    Logger::instance().addMessage(buf);
}

int OpenConnectApi::set_sock_block(int fd)
{
#ifdef _WIN32
    unsigned long mode = 0;
    return ioctlsocket(static_cast<SOCKET>(fd), static_cast<long>(FIONBIO), &mode);
#else
    return fcntl(fd, F_SETFL, fcntl(fd, F_GETFL) & ~O_NONBLOCK);
#endif
}

void OpenConnectApi::stats_vfn(void *privdata, const oc_stats *stats)
{
    OpenConnectConnection* vpn = static_cast<OpenConnectConnection*>(privdata);
    const char* cipher;
    QString dtls;

    cipher = openconnect_get_dtls_cipher(vpn->vpninfo);
    if (cipher != nullptr) {
        dtls = QLatin1String(cipher);
    }

     vpn->setStats(stats->tx_bytes, stats->rx_bytes);
}

void OpenConnectApi::setupTunDevice(void *privdata)
{
    OpenConnectConnection* vpn = static_cast<OpenConnectConnection*>(privdata);

    QByteArray vpncScriptFullPath;
    vpncScriptFullPath.append(QCoreApplication::applicationDirPath());
    vpncScriptFullPath.append(QDir::separator());
    vpncScriptFullPath.append(DEFAULT_VPNC_SCRIPT);
    int ret = openconnect_setup_tun_device(vpn->vpninfo, vpncScriptFullPath.constData(), nullptr);
    if (ret != 0) {
        vpn->setLastError(QObject::tr("Error setting up the TUN device"));
    }
    vpn->logOpenConnect();
}

void OpenConnectApi::main_loop(OpenConnectConnection *vpninfo, void *)
{
    emit vpninfo->statusChanged(OpenConnectConnection::CONNECTING);

    QString ip, ip6, dns, cstp, dtls;

    int ret = 0;
    ret = vpninfo->ctspConnect();
    if (ret != 0) {
        goto fail;
    }

    ret = vpninfo->dtls_connect();
    if (ret != 0) {
         Logger::instance().addMessage(vpninfo->lastError());
    }

    vpninfo->fetchInfo();
    vpninfo->fetchCipherInfo();
    emit vpninfo->statusChanged(OpenConnectConnection::CONNECTED);

    while (true) {
        int ret = openconnect_mainloop(vpninfo->vpninfo,
                                       SYMLEX_RECONNECT_INTERVAL,
                                       RECONNECT_INTERVAL_MIN);
        if (ret != 0) {
            vpninfo->setLastError(QObject::tr("Disconnected"));
            vpninfo->logOpenConnect();
            break;
        }
    }

fail: // LCA: drop this 'goto' and optimize values...
    emit vpninfo->statusChanged(OpenConnectConnection::IDLE);
}

} // namespace Symlex
