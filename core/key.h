#ifndef KEY_H
#define KEY_H

#include <QByteArray>
#include <QString>
#include <QTemporaryFile>
extern "C" {
#include <gnutls/x509.h>
}

namespace Start {

class Key
{
public:
    Key();
    ~Key();

    /* functions return zero on success */
    int import_file(const QString& File);
    int import_pem(const QByteArray& data);
    void set(const gnutls_x509_privkey_t privkey);

    void set_window(QWidget* w);

    int data_export(QByteArray& data);
    int tmpfile_export(QString& File);

    bool is_ok() const;
    void get_url(QString& url) const;

    void clear();

    QString last_err;

private:
    gnutls_x509_privkey_t privkey;
    QTemporaryFile tmpfile;
    QString url;
    QWidget* w;
    bool imported;
};

} // namespace Symlex

#endif // KEY_H
