#include<conncectionpage.h>
#include<QDebug>
#include "connectpage.h"
#include "common/common.h"
#include <QPainter>
#include <QStyleOption>
#include <QTimer>
#include "core/openconnectserver.h"
#include "common/config.h"


namespace Start {
void Connectionvpn::onStatusChanged(OpenConnectConnection::Status status)
{
    qDebug()<<"on conncetion change";
    auto opcState = static_cast<OpenConnectConnection::Status>(status);

    if(opcState == OpenConnectConnection::CONNECTING)
    {
        update();
    }
    if(opcState == OpenConnectConnection::IDLE)
    {
        update();
    }
    if(opcState == OpenConnectConnection::CONNECTED)
    {
        Common::globalConfig()->save();
        update();
    }
}

Connectionvpn::Connectionvpn(QWidget *parent):
     QWidget(parent)
{
    connect(Common::globalConnection().get(), &OpenConnectConnection::statusChanged, this, &Connectionvpn::onStatusChanged, Qt::QueuedConnection);
}

Connectionvpn::~Connectionvpn()
{

}

void Connectionvpn::Onction(int &ipID, QString &config, int &type, QString &ip)
{

    Common::globalConfig()->setActiveServerIndex(ipID);
    Common::globalConnection()->setType(type);
    Common::globalConnection()->setOpenConfig(config, ip);

    if(!Common::globalConnection()->server())
    {
        Common::globalConnection()->setServer(std::make_shared<OpenConnectServer>(ip));
    }
    else
        Common::globalConnection()->server()->setServerAddress(ip);

    auto ret = Common::globalConnection()->connect();
    switch (ret) {
    case OpenConnectConnection::SOCKET_ACTIVE: {
        qDebug()<<"Connection already active";
        return;
    }
    case OpenConnectConnection::INSTANCE_ACTIVE: {
        qDebug()<<"VPN already running";
        return;
    }
    case OpenConnectConnection::SERVER_INVALID: {
        qDebug()<<"Invalid server";
        return;
    }
    case OpenConnectConnection::CREDENTIAL_INVALID: {
        qDebug()<<"Invalid credentials";
        return;
    }
    default:
        return;

    }
}

}



