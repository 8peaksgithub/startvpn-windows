#ifndef COMMON_H
#define COMMON_H

#include <QObject>
#include <memory>

#undef PROJ_GNUTLS_DEBUG

#define TMP_CERT_PREFIX "tmp-certXXXXXX"
#define TMP_KEY_PREFIX "tmp-keyXXXXXX"

#ifdef _WIN32
#define DEFAULT_VPNC_SCRIPT "vpnc-script.js"
#else
#define DEFAULT_VPNC_SCRIPT "../Resources/vpnc-script"
#endif

#define SYMLEX_RECONNECT_INTERVAL 300
#define SYMLEX_DTLS_TIMEOUT 25

#ifdef _WIN32
#define pipe_write(x, y, z) send(x, y, z, 0)
#else
#define pipe_write(x, y, z) write(x, y, z)
#endif

#ifdef _WIN32
#define net_errno WSAGetLastError()
#define ms_sleep Sleep
#else
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#define ms_sleep(x) usleep(1000 * x)
#define INVALID_SOCKET -1
#define SOCKET int
#define closesocket close
#define net_errno errno
#endif

namespace Start {

class OpenConnectConnection;
class User;
class Config;
class ApiRequest;

class Common
{
public:
    static QString appName();
    static QString appVersion();
    static QString appOrganisation();
    static bool is_url(const QString& str);
    static std::shared_ptr<OpenConnectConnection> globalConnection();
    static std::shared_ptr<User> currentUser();
    static std::shared_ptr<Config> globalConfig();
    static QString currentTimeZone();
    static int currentCountryCallingCode();
    static QPair<QString,QString> manufacturerAndModel();
    static int statsInterval();
    static QString flag(int countryCode);
    static QByteArray updateFontSizes(QByteArray styles, int fontsize1, int fontsize2, int fontWeight = 500);
    static std::shared_ptr<ApiRequest> symlexApi();
    static int minorFontSize();
    static int majorFontSize();
    static int fontWeight();
};

} // namespace Symlex

#endif // COMMON_H
