#ifndef KEYPAIR_H
#define KEYPAIR_H

#include "cert.h"
#include "key.h"

namespace Start {

class KeyPair
{
public:
    KeyPair();
    ~KeyPair();

    /* functions return zero on success */
    int import_pfx(const QString& File);
    int import_cert(const QString& File);
    int import_key(const QString& File);
    void set_window(QWidget* w);

    int cert_export(QByteArray& data);
    int key_export(QByteArray& data);

    bool is_complete() const;

    QString last_err;

    Key key;
    Cert cert;

private:
    QWidget* w;
};

} // namespace Symlex

#endif // KEYPAIR_H
