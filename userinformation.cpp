#include "userinformation.h"
#include <QSettings>

UserInformation::UserInformation()
{

}

void UserInformation::setUsername(QString username)
{
    this->username = username;
}

void UserInformation::setPassword(QString password)
{
    this->password = password;
}

void UserInformation::setLoggedIn(bool login)
{
    this->login = login;
}

void UserInformation::setExpireInDays(int expire_in_days)
{
    this->expire_in_days = expire_in_days;
}

void UserInformation::setValidityDate(QString validity_date)
{
    this->validity_date = validity_date;
}

void UserInformation::setExpiryDate(QString expired_at)
{
    this->expired_at = expired_at;
}

void UserInformation::setUserType(int user_type)
{
    this->user_type = user_type;
}

void UserInformation::setUserStatus(QString user_status)
{
    this->user_status = user_status;
}

void UserInformation::setContactEmail(QString contact_email)
{
    this->contact_email = contact_email;
}

void UserInformation::setNoticeType(int notice_type)
{
    this->notice_type = notice_type;
}

void UserInformation::setNoticeText(QString notice)
{
    this->notice = notice;
}

void UserInformation::setNoticeStartTime(double start_after)
{
    this->start_after = start_after;
}

void UserInformation::setNoticeEndTime(double end_after)
{
    this->end_after = end_after;
}

void UserInformation::setNoticeLockServerIds(QString lock_servers_id)
{
    this->lock_servers_id = lock_servers_id;
}

void UserInformation::setReferenceEnabled(bool allow_referrence_feature)
{
    this->allow_referrence_feature = allow_referrence_feature;
}

void UserInformation::setIPBundle(QByteArray ip_bundle)
{
    this->ip_array = ip_bundle;
}

void UserInformation::setCurrentIPBundle(int &ipID, QString &config, int &type, QString &ip, QString &ipName)
{
    this->ipID = ipID;
    this->config = config;
    this->type = type;
    this->ip = ip;
    this->ipName = ipName;
    QSettings settings("startvpn", "Kolpolok Limited");
    settings.setValue("ipID", ipID);
    settings.setValue("config", config);
    settings.setValue("type", type);
    settings.setValue("ip", ip);
    settings.setValue("ipName",ipName);

}

void UserInformation::loadIpBundle()
{
    QSettings settings("startvpn", "Kolpolok Limited");
    this->ipID = settings.value("ipID", ipID).toInt();
    this->config = settings.value("config", config).toString();
    this->type = settings.value("type", type).toInt();
    this->ip = settings.value("ip", ip).toString();
    this->ipName = settings.value("ipName",ipName).toString();
}
void UserInformation::resetUserInfo()
{

    this->ipID = 0;
    this->config = "";
    this->type = 0;
    this->ip = "";
    this->ipName = "";
    setCurrentIPBundle(this->ipID, this->config, this->type, this->ip,this->ipName);
}
int UserInformation::getCurrentipID()
{
   return this->ipID;
}

QString UserInformation::getCurrentConfig()
{
   return this->config;
}
int UserInformation::getCurrentType()
{
   return this->type;
}

QString UserInformation::getCurrentIP()
{
   return this->ip;
}


