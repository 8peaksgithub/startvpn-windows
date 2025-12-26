#include "config.h"
#include "crypto.h"
#include <QSettings>
#include <QDebug>

namespace Start {

const QString USERNAME_KEY = "ServerHash";
const QString PASSWORD_KEY = "ConnectionHash";
const QString ACTIVE_SERVER_KEY = "ActiveServer";
const QString FAVOURITES_KEY = "Favourites";
const QString NOTIFICATION_KEY = "Notify";
const QString LOGIN_KEY = "AutoLogin";

const QString CRENDENTIAL_KEY = "2VHGR";

Config::Config()
{
    m_activeServerIndex = 0;
    m_loggedIn = false;
}

void Config::load()
{
    QSettings settings("startvpn", "Kolpolok Limited");
    m_cachedUsername = settings.value(USERNAME_KEY,QString()).toString();
    m_cachedPassword = settings.value(PASSWORD_KEY,QString()).toString();
    m_activeServerIndex = settings.value(ACTIVE_SERVER_KEY,0).toInt();
    m_notification = settings.value(NOTIFICATION_KEY,false).toBool();
    m_loggedIn = settings.value(LOGIN_KEY,false).toBool();


}

void Config::save()
{
    QSettings settings("startvpn", "Kolpolok Limited");
    settings.setValue(USERNAME_KEY,m_cachedUsername);
    settings.setValue(PASSWORD_KEY,m_cachedPassword);
    settings.setValue(ACTIVE_SERVER_KEY,m_activeServerIndex);
    settings.setValue(NOTIFICATION_KEY, m_notification);
    settings.setValue(LOGIN_KEY, m_loggedIn);

}

void Config::reset()
{
    m_activeServerIndex = 0;
    m_favourites.clear();
    m_notification = false;
    m_loggedIn = false;
    qDebug()<<"is_loging:::reset:::"<<m_loggedIn;

    save();
}

QString Config::cachedUsername() const
{
    if(m_cachedUsername.isEmpty())
        return QString();

    return decodeString(m_cachedUsername,CRENDENTIAL_KEY);
}

void Config::setCachedUsername(const QString &cachedUsername)
{
    m_cachedUsername = encodeString(cachedUsername,CRENDENTIAL_KEY);
}

QString Config::cachedPassword() const
{
    if(m_cachedPassword.isEmpty())
        return QString();

    return decodeString(m_cachedPassword,CRENDENTIAL_KEY);
}

void Config::setCachedPassword(const QString &cachedPassword)
{
    m_cachedPassword = encodeString(cachedPassword,CRENDENTIAL_KEY);
}

int Config::activeServerIndex() const
{
    return m_activeServerIndex;
}

void Config::setActiveServerIndex(int activeServerIndex)
{
    m_activeServerIndex = activeServerIndex;
}

QVector<int> Config::favourites() const
{
    return m_favourites;
}

void Config::addFavourite(int server)
{
    if(!m_favourites.contains(server))
        m_favourites.append(server);
}

void Config::removeFavourite(int server)
{
    if(m_favourites.contains(server))
        m_favourites.removeAll(server);
}

void Config::parseFavourites(QString favouriteString)
{
    m_favourites.clear();

    auto list = favouriteString.split(":", Qt::SkipEmptyParts);

    for(auto i = 0; i < list.count(); i++)
    {
        m_favourites.append(list.at(i).toInt());
    }
}

QString Config::compressFavourites()
{
    QString string;
    for(auto i = 0; i < m_favourites.count(); i++)
    {
        string += QString::number(m_favourites.at(i)) + ":";
    }

    return string;
}

bool Config::loggedIn() const
{
    return m_loggedIn;
}

void Config::setLoggedIn(bool loggedIn)
{
    m_loggedIn = loggedIn;

    if(m_loggedIn)
    {
        QSettings settings;
        settings.beginGroup(m_cachedUsername);
        parseFavourites(settings.value(FAVOURITES_KEY,QString()).toString());
        settings.endGroup();
    }
    else
    {
        QSettings settings;
        settings.beginGroup(m_cachedUsername);
        settings.setValue(FAVOURITES_KEY,compressFavourites());
        settings.endGroup();
    }
}

bool Config::notification() const
{
    return m_notification;
}

void Config::setNotification(bool notification)
{
    m_notification = notification;
}

} // namespace Start
