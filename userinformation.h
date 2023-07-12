#ifndef USERINFORMATION_H
#define USERINFORMATION_H
#include<QString>
#include<QJsonArray>
#include<QJsonDocument>
#include<QByteArray>

class UserInformation
{
public:
    QByteArray ip_array;
    QString username;
    QString password;
    bool login;
    int expire_in_days;
    QString validity_date;
    QString expired_at;
    int user_type;
    QString user_status;
    QString contact_email;
    int notice_type;
    QString notice;
    double start_after;
    double end_after;
    QString lock_servers_id;
    bool allow_referrence_feature;
    int ipID;
    QString config;
    int type;
    QString ip;
    bool isConnected;
    QString ipName;
public:
    UserInformation();
    void setUsername(QString username);
    void setPassword(QString password);
    void setLoggedIn(bool login);
    void setExpireInDays(int expire_in_days);
    void setValidityDate(QString validity_date);
    void setExpiryDate(QString expired_at);
    void setUserType(int user_type);
    void setUserStatus(QString user_status);
    void setContactEmail(QString contact_email);
    void setNoticeType(int notice_type);
    void setNoticeText(QString notice);
    void setNoticeStartTime(double start_after);
    void setNoticeEndTime(double end_after);
    void setNoticeLockServerIds(QString lock_servers_id);
    void setReferenceEnabled(bool allow_referrence_feature);
    void setIPBundle(QByteArray ip_bundle);
    void setCurrentIPBundle(int &ipID, QString &config, int &type, QString &ip, QString &ipName);
    void resetUserInfo();
    void loadIpBundle();
    int getCurrentipID();
    QString getCurrentConfig();
    int getCurrentType();
    QString getCurrentIP();
};

#endif // USERINFORMATION_H
