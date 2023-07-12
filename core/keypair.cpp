#include "keypair.h"
#include "common/common.h"
#include <QInputDialog>
#include <QString>
extern "C" {
#include <gnutls/pkcs12.h>
}

namespace Start {

KeyPair::KeyPair()
    : w(nullptr)
{
}

KeyPair::~KeyPair()
{
}

static int load_pkcs12_file(QWidget* w,
                            Key& key,
                            Cert& cert,
                            QString File,
                            QString& last_err)
{
    gnutls_pkcs12_t pkcs12 = nullptr;
    QString pass;
    int pem = 0;
    gnutls_x509_privkey_t xkey;
    gnutls_x509_crt_t* xcert;
    unsigned int xcert_size;

    bool ok = false; // TODO
    char* p = nullptr; // TODO

    if (w == nullptr || Common::is_url(File)) {
        return -1;
    }

    gnutls_datum_t raw = { nullptr, 0 };
    int ret = gnutls_load_file(File.toLatin1().data(), &raw);
    if (ret < 0) {
        last_err = gnutls_strerror(ret);
        goto fail;
    }

    /* check if the file data contain BEGIN PKCS12 */
    /*char* */ p = strstr((char*)raw.data, "--- BEGIN ");
    if (p != nullptr) {
        pem = 1;
        if (strstr(p, "--- BEGIN PKCS12") == 0) {
            return -1;
        }
    }

    ret = gnutls_pkcs12_init(&pkcs12);
    if (ret < 0) {
        last_err = gnutls_strerror(ret);
        goto fail;
    }

    ret = gnutls_pkcs12_import(pkcs12, &raw,
                               (pem != 0) ? GNUTLS_X509_FMT_PEM : GNUTLS_X509_FMT_DER,
                               0);
    if (ret < 0) {
        last_err = gnutls_strerror(ret);
        goto fail;
    }

    //    bool ok = false;
    pass = QInputDialog::getText(w,
                                 QLatin1String("This file requires a password"),
                                 QLatin1String("Please enter your password"),
                                 QLineEdit::Password,
                                 QString(),
                                 &ok);

    if (ok == false) {
        goto fail;
    }

    ret = gnutls_pkcs12_verify_mac(pkcs12, pass.toLatin1().data());
    if (ret < 0) {
        last_err = gnutls_strerror(ret);
        goto fail;
    }

    ret = gnutls_pkcs12_simple_parse(pkcs12, pass.toLatin1().data(), &xkey, &xcert,
                                     &xcert_size, nullptr, nullptr, nullptr, 0);
    if (ret < 0) {
        last_err = gnutls_strerror(ret);
        goto fail;
    }

    if (xkey) {
        key.set(xkey);
    }
    if (xcert_size > 0) {
        cert.set(xcert[0]);
    }

    for (unsigned i = 1; i < xcert_size; ++i) {
        gnutls_x509_crt_deinit(xcert[i]);
    }
    ret = 0;
    goto cleanup;

fail:
    ret = -1;
cleanup:
    gnutls_free(raw.data);
    if (pkcs12) {
        gnutls_pkcs12_deinit(pkcs12);
    }
    return ret;
}

int KeyPair::import_pfx(const QString& File)
{
    return load_pkcs12_file(this->w, this->key, this->cert, File, this->last_err);
}

int KeyPair::import_cert(const QString& File)
{
    int ret = this->cert.import_file(File);
    if (ret != 0) {
        last_err = cert.last_err;
        return -1;
    }
    return 0;
}

int KeyPair::import_key(const QString& File)
{
    int ret = this->key.import_file(File);
    if (ret != 0) {
        last_err = key.last_err;
        return -1;
    }
    return 0;
}

void KeyPair::set_window(QWidget* w)
{
    this->w = w;
    key.set_window(w);
}

int KeyPair::cert_export(QByteArray& data)
{
    return cert.data_export(data);
}

int KeyPair::key_export(QByteArray& data)
{
    return key.data_export(data);
}

bool KeyPair::is_complete() const
{
    if (key.is_ok() == cert.is_ok()) {
        return true;
    }
    return false;
}

} // namespace Symlex
