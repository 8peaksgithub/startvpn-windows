#ifndef CONFIG_H
#define CONFIG_H

#include <QString>
#include <QVector>

namespace Start {

class Config
{
public:
    Config();

    void load();
    void save();
    void reset();

    QString cachedUsername() const;
    void setCachedUsername(const QString &cachedUsername);

    QString cachedPassword() const;
    void setCachedPassword(const QString &cachedPassword);

    int activeServerIndex() const;
    void setActiveServerIndex(int activeServerIndex);

    QVector<int> favourites() const;
    void addFavourite(int server);
    void removeFavourite(int server);

    bool notification() const;
    void setNotification(bool notification);

    bool loggedIn() const;
    void setLoggedIn(bool loggedIn);

private:
    void parseFavourites(QString favouriteString);
    QString compressFavourites();

private:
    QString m_cachedUsername;
    QString m_cachedPassword;
    int m_activeServerIndex;
    QVector<int> m_favourites;
    bool m_notification;
    bool m_loggedIn;
};

} // namespace Start

#endif // CONFIG_H
