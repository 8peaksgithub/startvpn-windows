#include "gtdb.h"
#include "openconnectserver.h"
extern "C" {
#include <gnutls/crypto.h>
}

#define HASH GNUTLS_DIG_SHA1
#define HASH_LEN 20
#define MAX_HASH_LEN 64

namespace Start {

static int store_cb(const char* db_name,
                    const char* host,
                    const char* service,
                    time_t expiration,
                    const gnutls_datum_t* pubkey)
{
    Q_UNUSED(host)
    Q_UNUSED(service)
    Q_UNUSED(expiration)
    char output[MAX_HASH_LEN];
    int ret = gnutls_hash_fast(HASH, pubkey->data, pubkey->size, output);
    if (ret < 0) {
        return -1;
    }

    QByteArray ahash;
    ahash.append(output, HASH_LEN);
    const gtdb* tdb = reinterpret_cast<const gtdb*>(db_name);
    tdb->server->set_server_hash(HASH, ahash);

    return 0;
}

static int verify_cb(const char* db_name,
                     const char* host,
                     const char* service,
                     const gnutls_datum_t* pubkey)
{
    Q_UNUSED(host);
    Q_UNUSED(service);
    const gtdb* tdb = reinterpret_cast<const gtdb*>(db_name);
    QByteArray ahash;
    unsigned algo = tdb->server->get_server_hash(ahash);
    int len = static_cast<int>(gnutls_hash_get_len(static_cast<gnutls_digest_algorithm_t>(algo)));

    char output[MAX_HASH_LEN];
    if (algo == 0 || len > static_cast<int>(sizeof(output))) {
        return -1;
    }

    if (ahash.size() != len) {
        return -1;
    }

    int ret = gnutls_hash_fast((gnutls_digest_algorithm_t)algo, pubkey->data,
                               pubkey->size, output);
    if (ret < 0) {
        return -1;
    }

    if (memcmp(ahash.constData(), output, len) == 0) {
        return 0;
    }
    return GNUTLS_E_CERTIFICATE_KEY_MISMATCH;
}

gtdb::gtdb(std::shared_ptr<OpenConnectServer> s)
    : server(s)
{
    gnutls_tdb_init(&this->tdb);
    gnutls_tdb_set_verify_func(tdb, verify_cb);
    gnutls_tdb_set_store_func(tdb, store_cb);
}

gtdb::~gtdb()
{
    gnutls_tdb_deinit(this->tdb);
}

gnutls_tdb_t gtdb::get_tdb() const
{
    return tdb;
}

} // namespace Symlex
