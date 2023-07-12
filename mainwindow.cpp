#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPixmap>
#include <QPainter>
#include <QUrlQuery>
#include <QSysInfo>
#include <QNetworkRequest>
#include <QDebug>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMessageBox>
#include <QTimer>
#include <QSettings>
#include <QJsonArray>
#include <QLabel>
#include <QtCore>
#include <common/common.h>
#include "common/config.h"
#include <QString>
#include <QSize>
#include <core/openconnectconnection.h>

#include "common/common.h"
#include <QPainter>
#include <QStyleOption>
#include <QTimer>
#include "core/openconnectserver.h"
#include <QDialogButtonBox>
#include <QUrl>
#include <QtCore>
#include <QDesktopServices>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QMovie>
#include <QMap>
#include <QClipboard>
#include <QRegExp>

#define IDENTITY_ROLE Qt::UserRole + 1

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow),
      m_state(0)
{

    ui->setupUi(this);
    loadTray();


    ui->loginLabel->setAlignment(Qt::AlignCenter);
    ui->treeWidget->setColumnWidth(0,200);

    ui->treeWidget->setSelectionMode(QAbstractItemView::NoSelection);
    ui->treeWidget->setFocusPolicy(Qt::NoFocus);
    ui->treeWidget->setHeaderHidden(true);
    ui->treeWidget->setExpandsOnDoubleClick(false);
    ui->treeWidget->QAbstractScrollArea::setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
    ui->treeWidget->QAbstractScrollArea::setVerticalScrollBarPolicy( Qt::ScrollBarAlwaysOff ) ;

    QPixmap *p=new QPixmap(":/logo/resources/user icon.png");
    QPixmap userLogo(p->scaled ( 10,15, Qt::IgnoreAspectRatio, Qt::SmoothTransformation ));
    ui->lblUser->setPixmap(userLogo);
    ui->lblUser->setStyleSheet("margin:12px;border:none;");

    QPixmap *password=new QPixmap(":/logo/resources/password.png");
    QPixmap pass(password->scaled ( 10,15, Qt::IgnoreAspectRatio, Qt::SmoothTransformation ));
    ui->lblpassword->setPixmap(pass);
    ui->lblpassword->setStyleSheet("margin:12px;border:none;");

    QPixmap *userEmail=new QPixmap(":/logo/resources/email.png");
    QPixmap email(userEmail->scaled ( 18,12, Qt::IgnoreAspectRatio, Qt::SmoothTransformation ));


    ui->lblSignupUser->setPixmap(email);
    ui->lblSignupUser->setStyleSheet("margin:10px;border:none;");

    ui->lblSignupPass->setPixmap(pass);
    ui->lblSignupPass->setStyleSheet("margin:12px;border:none;");

    ui->lblSignupConfirmPass->setPixmap(pass);
    ui->lblSignupConfirmPass->setStyleSheet("margin:12px;border:none;");

    ui->lblRresetEmail->setPixmap(userLogo);
    ui->lblRresetEmail->setStyleSheet("margin:5px;border:none;");
    ui->username->setAttribute(Qt::WA_MacShowFocusRect,false);
    ui->password->setAttribute(Qt::WA_MacShowFocusRect,false);
    ui->registationConfirmPasswordLineEdit->setAttribute(Qt::WA_MacShowFocusRect,false);
    ui->registitionPasswordLineEdit->setAttribute(Qt::WA_MacShowFocusRect,false);
    ui->registationConfirmPasswordLineEdit->setAttribute(Qt::WA_MacShowFocusRect,false);
    ui->lineEditResetPasswordEmail->setAttribute(Qt::WA_MacShowFocusRect,false);
    ui->emailVerificationCodeLineEdit->setAttribute(Qt::WA_MacShowFocusRect,false);
    ui->editSearch->setAttribute(Qt::WA_MacShowFocusRect,false);



    QObject::connect(
        ui->treeWidget, SIGNAL(clicked(const QModelIndex &)),
        this, SLOT(expandItem(const QModelIndex &))
    );

    connect(ui->editSearch, SIGNAL(textChanged(QString)),this,SLOT(on_lineEdit_textChanged(QString)));


    connect(ui->btnRegitstationSubmit,SIGNAL(clicked(bool)),this,SLOT(btnEmailVerification_clicked()));
    connect(ui->btncreateAccount,SIGNAL(clicked(bool)),this,SLOT(btnregitstation_clicked()));
    connect(ui->btnback,SIGNAL(clicked(bool)),this,SLOT(backBtton_clicked()));
    connect(ui->btnforgetPassword,SIGNAL(clicked(bool)),this,SLOT(btnforgetPassword_clicked()));
    connect(ui->btnBackLogin,SIGNAL(clicked(bool)),this,SLOT(btnBackLogin_clicked()));
    connect(ui->btnSuccessfullyLogin,SIGNAL(clicked(bool)),this,SLOT(btnBackLogin_clicked()));
    connect(ui->btnSendEmail,SIGNAL(clicked(bool)),this,SLOT(btnSendEmail_clicked()));
    connect(ui->btnResetOk,SIGNAL(clicked(bool)),this,SLOT(btnBackLogin_clicked()));
    connect(ui->btnHomeLogin,SIGNAL(clicked(bool)),this,SLOT(btnBackLogin_clicked()));
    connect(ui->btnLogin,SIGNAL(clicked(bool)),this,SLOT(btnLogin_clicked()));
    connect(ui->btnHomeSignUp,SIGNAL(clicked(bool)),this,SLOT(btnregitstation_clicked()));
    connect(ui->btnConnect,SIGNAL(clicked(bool)),this, SLOT(btnConnect_clicked()));
    connect(ui->btnSubmitEmailVerificationCode,SIGNAL(clicked(bool)),this,SLOT(btnSubmitEmailVerificationCode_clicked()));
    connect(ui->btnLogout_2,SIGNAL(clicked(bool)),this,SLOT(btnLogout_clicked()));
    connect(ui->btnChangeEmail,SIGNAL(clicked(bool)),this,SLOT(btnregitstation_clicked()));
    connect(ui->btnUpgradeNow,SIGNAL(clicked(bool)),this,SLOT(btnUpgradeNow_clicked()));
    connect(ui->toolButtonUpgrade,SIGNAL(clicked(bool)),this,SLOT(btnUpgradeNow_clicked()));



    connect(Start::Common::globalConnection().get(), &Start::OpenConnectConnection::statusChanged, this, &MainWindow::onStatusChanged, Qt::QueuedConnection);


    Start::Common::globalConfig()->load();
    bool is_login = Start::Common::globalConfig()->loggedIn();
    if(is_login){
        ui->lblUserName->setText(Start::Common::globalConfig()->cachedUsername());
        Start::Common::globalConnection()->setUsername(Start::Common::globalConfig()->cachedUsername());
        Start::Common::globalConnection()->setPassword(Start::Common::globalConfig()->cachedPassword());
        Start::Common::globalConfig()->save();
        createServerTreeWidget();
        int activeServerIndex = Start::Common::globalConfig()->activeServerIndex();
        setSelectedLocation(activeServerIndex);
        ui->stackedWidget->setCurrentIndex(2);


    }else{
        ui->stackedWidget->setCurrentIndex(0);

        ui->stackedWidget_2->setCurrentIndex(0);
    }
    ui->homeWindowstackedWidget->setStyleSheet("background-image: url(:/logo/resources/backround.png)");

    movie =  new QMovie(":/logo/resources/Splash and login/Spinner-1s-50px.gif");
    ui->btnLoginLoder->setVisible(false);
    ui->btnRegistationLoder->setVisible(false);
    ui->btnResetPasswordLoader->setVisible(false);
    ui->btnEmailVerificationLoader->setVisible(false);
    ui->btnEmailVerificationLoader->setVisible(false);
    ui->btnMenuLoader->setVisible(false);
    map.clear();


}


void MainWindow::expandItem(const QModelIndex &index)
{
    ui->treeWidget->isExpanded(index)? ui->treeWidget->collapse(index) : ui->treeWidget->expand(index);
    QString itemName = index.data().toString();
    auto Items = ui->treeWidget->findItems(
               itemName, Qt::MatchWrap | Qt::MatchWildcard | Qt::MatchRecursive);
    auto itme = Items.first();

    QLabel *label = new QLabel();
    label->resize(10,10);
    if(index.parent().isValid()){
        int ip_id;
        QString config;
        int type;
        QString ip;
        QString Country;
        QString ServerIPName;

        QSettings settings("startvpn", "Kolpolok Limited");
        auto bundils =  settings.value("IPBundle", "").toString();
        QJsonDocument doc = QJsonDocument::fromJson(bundils.toUtf8());
        QJsonArray serverArray = doc.array();

        for(int i = 0; i <serverArray.size(); i ++){
                QJsonObject obj = serverArray.at(i).toObject();
                QString ipName = obj.value("ipName").toString();
                int Itype = obj.value("type").toInt();
                if(itemName==ipName&&Itype==1)
                {
                   ip_id =  obj.value("ip_id").toInt();
                   config =  obj.value("config").toString();
                   type =  obj.value("type").toInt();
                   ip =  obj.value("ip").toString();
                   Country = obj.value("countryName").toString();
                   ServerIPName =  obj.value("ipName").toString();
                   break;
                }

         }
        if(ui->stackedWidget_2->currentIndex()==2){
            ui->stackedWidget_2->setCurrentIndex(1);
        }
        ui->btnConnect->setEnabled(false);
        if(ip.contains(":"))
            ip =  ip.split(":").first();
        ui->lblConnectIP->setText("Protected IP: "+ip);
        ui->lbldefultip->setText("Unprotected IP: "+ip);
        info->ipName = ServerIPName;
        ui->btnConnect->setEnabled(false);
        if(ip=="8.8.8.8"){
            QString msg = "Can't Connect, Try Premium";
            customMessage(msg);
        }else{
            setipID(ip_id);
            config = config;
            setConfig(config);
            setType(type);
            setIP(ip);
            saveBundil();
            if(m_state){
                requestConnection(true);
            }
            requestConnection(false);


        }

    }
    else if(index.data(IDENTITY_ROLE).toInt() == 1){

    }

   if(ui->treeWidget->isExpanded(index)){
        QPixmap pix(":/logo/resources/arrow down.png");
        pix = pix.scaled(label->size(),Qt::KeepAspectRatio);

        label->setPixmap(pix);
        label->setAlignment(Qt::AlignRight);
        label->setAlignment(Qt::AlignVCenter);
        label->setStyleSheet("margin-left:200px");
        ui->treeWidget->setItemWidget(itme,0,label);

    }else{
        QPixmap pix(":/logo/resources/arrow up.png");
        pix = pix.scaled(label->size(),Qt::KeepAspectRatio);

        label->setPixmap(pix);
        label->setAlignment(Qt::AlignRight);
        label->setAlignment(Qt::AlignVCenter);
        label->setStyleSheet("margin-left:200px");
        ui->treeWidget->setItemWidget(itme,0,label);

    }

}



MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::backBtton_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}


void MainWindow::btnforgetPassword_clicked()
{
    ui->stackedWidget->setCurrentIndex(5);
}


void MainWindow::btnBackLogin_clicked()
{
    Start::Common::globalConfig()->load();

    if(Start::Common::globalConfig()->loggedIn()){
        ui->stackedWidget->setCurrentIndex(2);
        ui->stackedWidget_2->setCurrentIndex(0);
    }else{
        ui->stackedWidget->setCurrentIndex(1);
        ui->stackedWidget_2->setCurrentIndex(0);
    }


}


void MainWindow::btnregitstation_clicked()
{

    ui->stackedWidget->setCurrentIndex(4);
}

void MainWindow::btnEmailVerification_clicked() // regitation verifaction & email verification send
{

    auto email = ui->registitionEmailLineEdit->text();
    auto username = email.simplified();
    username.replace( " ", "" );
    userEmail = username; // save user emil for feture use;
    auto password = ui->registitionPasswordLineEdit->text();
    auto confirmPassword = ui->registationConfirmPasswordLineEdit->text();
    if(password.isEmpty()||username.isEmpty()||confirmPassword.isEmpty()){
        QString msg = "Username/Password can't be Empty!";
        messageBox(msg);
        return;
    }
    ui->btnRegistationLoder->setVisible(true);
    ui->btnRegitstationSubmit->setDisabled(true);
    connect(movie, &QMovie::frameChanged, this, [=](int){
        ui->btnRegistationLoder->setText("");
        ui->btnRegistationLoder->setIcon(movie->currentPixmap().scaledToHeight( ui->btnRegistationLoder->height(), Qt::SmoothTransformation));
        ui->btnRegistationLoder->setIconSize(QSize(29,29));
    });
    movie->start();
    QString timeZone = currentTimeZone();
    QUrlQuery requestData;
    requestData.addQueryItem("username",username);
    requestData.addQueryItem("password",password);
    requestData.addQueryItem("password_confirmation",confirmPassword);
    requestData.addQueryItem("country",timeZone);
    requestData.addQueryItem("enable_email_verification","0");

    Start::Common::globalConfig()->setCachedUsername(email);
    Start::Common::globalConfig()->setCachedPassword(password);
    Start::Common::globalConfig()->save();


    QNetworkRequest request(QUrl(base_url+"/sign-up"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    manager = new QNetworkAccessManager;
    auto reply = manager->post(request, requestData.query(QUrl::FullyEncoded).toUtf8());


    QEventLoop eventLoop;

    connect(reply, &QNetworkReply::finished,&eventLoop,&QEventLoop::quit);
    eventLoop.exec();
    ui->btnRegistationLoder->setVisible(false);
    ui->btnRegitstationSubmit->setDisabled(false);
    movie->stop();
    QByteArray response =  reply->readAll();

    QJsonDocument doc = QJsonDocument::fromJson(response);
    auto rootObject = doc.object();
    qDebug()<<"signUp calling response code:::: "<<rootObject.value("response_code").toInt();
    if(rootObject.value("response_code").toInt() == 1)
    {
        ui->stackedWidget->setCurrentIndex(1);
//        this->timer = new QTimer(this); // for timer resend ip
//        this->timer->setInterval(1000);
//        connect(this->timer, SIGNAL(timeout()), this, SLOT(update_ui()));
//        this->timer->start();
    }
   else{
      QString msg = rootObject.value("message").toString();
      messageBox(msg);
    }

}

void MainWindow::btnSendEmail_clicked()
{

    QString username = ui->lineEditResetPasswordEmail->text();

    QUrlQuery requestData;
    requestData.addQueryItem("username",username);
    requestData.addQueryItem("verification_type","1");
    if(username.isEmpty()){
        QString msg = "Username can't be Empty!";
        messageBox(msg);
        return;
    }
    ui->btnResetPasswordLoader->setVisible(true);
    ui->btnSendEmail->setDisabled(true);
    connect(movie, &QMovie::frameChanged, this, [=](int){
        ui->btnResetPasswordLoader->setText("");
        ui->btnResetPasswordLoader->setIcon(movie->currentPixmap().scaledToHeight( ui->btnResetPasswordLoader->height(), Qt::SmoothTransformation));
        ui->btnResetPasswordLoader->setIconSize(QSize(29,29));
    });
    movie->start();

    QNetworkRequest request(QUrl(base_url+"/reset-password-token-request"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    manager = new QNetworkAccessManager;
    auto reply = manager->post(request, requestData.query(QUrl::FullyEncoded).toUtf8());

    QEventLoop eventLoop;

    connect(reply, &QNetworkReply::finished,&eventLoop,&QEventLoop::quit);
    eventLoop.exec();
    ui->btnResetPasswordLoader->setVisible(false);
    ui->btnSendEmail->setDisabled(false);
    movie->stop();
    QByteArray response =  reply->readAll();

    QJsonDocument doc = QJsonDocument::fromJson(response);
    auto rootObject = doc.object();


    if(rootObject.value("response_code").toInt() == 1)
    {
         ui->stackedWidget->setCurrentIndex(8);
    }else{
        QString msg = rootObject.value("message").toString();
        messageBox(msg);
    }

}

void MainWindow::btnLogin_clicked()
{
    QString password = ui->password->text();
    QString username = ui->username->text();
    if(password.isEmpty()||username.isEmpty()){
        QString msg = "Username/Password can't be Empty!";
        messageBox(msg);
        return;
    }
    ui->btnLoginLoder->setVisible(true);
    ui->btnLogin->setDisabled(true);
    connect(movie, &QMovie::frameChanged, this, [=](int){
        ui->btnLoginLoder->setText("");
        ui->btnLoginLoder->setIcon(movie->currentPixmap().scaledToHeight( ui->btnLoginLoder->height(), Qt::SmoothTransformation));
        ui->btnLoginLoder->setIconSize(QSize(29,29));
    });
    movie->start();

    QUrlQuery requestData;
    requestData.addQueryItem("username",username);
    requestData.addQueryItem("pass",password);
    requestData.addQueryItem("udid",QSysInfo::machineUniqueId());
    requestData.addQueryItem("device_type","3");
    QNetworkRequest request;
    request.setUrl(QUrl(base_url+"/login"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    manager = new QNetworkAccessManager;
    auto greply = manager->get(request);
    if(greply->error() != QNetworkReply::NoError)
    {
        qDebug()<<"network error:::::::::"<<greply->error();
    }
    auto reply = manager->post(request, requestData.query(QUrl::FullyEncoded).toUtf8());

    QEventLoop eventLoop;

    connect(reply, &QNetworkReply::finished,&eventLoop,&QEventLoop::quit);
    eventLoop.exec();
    movie->stop();
    ui->btnLoginLoder->setVisible(false);
    ui->btnLogin->setDisabled(false);
    QByteArray respone= reply->readAll();

    QJsonDocument doc = QJsonDocument::fromJson(respone);
    auto rootObject = doc.object();
    qDebug()<<"rootObject:::"<<rootObject;
    int code = rootObject.value("response_code").toInt();
    qDebug()<<"code::::::"<<code;

    if(rootObject.value("response_code").toInt() == 1)
    {
        map.clear();
        saveServer(respone,username,password);
    }
   else{
      QString msg = rootObject.value("message").toString();
      messageBox(msg);
    }

}


void MainWindow::on_btnCollapse_clicked()
{
    ui->stackedWidget->setCurrentIndex(3);
}


void MainWindow::on_btnConnectBack_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
}


void MainWindow::btnLogout_clicked()
{
    ui->btnMenuLoader->setVisible(true);
    ui->btnDeleteAccount->setDisabled(true);
    connect(movie, &QMovie::frameChanged, this, [=](int){
        ui->btnMenuLoader->setText("");
        ui->btnMenuLoader->setIcon(movie->currentPixmap().scaledToHeight( ui->btnRegistationLoder->height(), Qt::SmoothTransformation));
        ui->btnMenuLoader->setIconSize(QSize(29,29));
    });
    movie->start();
    ui->username->setText("");
    ui->password->setText("");
    ui->stackedWidget->setCurrentIndex(0);
    ui->stackedWidget_2->setCurrentIndex(0);
    QSettings settings("startvpn", "Kolpolok Limited");
    settings.clear();
    info->resetUserInfo();
    resetBundil();
    Start::Common::globalConfig()->reset();
    ui->treeWidget->clear();
    if(m_state)
        Start::Common::globalConnection()->disconnect();
    ui->btnMenuLoader->setVisible(false);
    ui->btnDeleteAccount->setDisabled(false);
    map.clear();
    ui->editSearch->clear();
}

void MainWindow::btnConnect_clicked()
{
    ui->btnConnect->setEnabled(false);
    qDebug()<<"btnconnect button click..";
    int active_server = Start::Common::globalConfig()->activeServerIndex();
    qDebug()<<active_server;
    int ip_id =  info->getCurrentipID();
    QString config =  info->getCurrentConfig();
    int type =  info->getCurrentType();
    QString ip =  info->getCurrentIP();
    if(ip.contains(":"))
        ip =  ip.split(":").first();
    ui->lblConnectIP->setText("Protected IP: "+ip);
    ui->lbldefultip->setText("Unprotected IP: "+ip);
    if(ip=="8.8.8.8"){
        QString msg = "Can't Connect, Try Premium";
        customMessage(msg);
    }else{
        setipID(ip_id);
        setConfig(config);
        setType(type);
        setIP(ip);
        saveBundil();
        OnConnection();

    }

}

QString MainWindow::currentTimeZone()
{
    const auto t1 = QDateTime::currentDateTime();
    const auto t2 = QDateTime(t1.date(), t1.time(),Qt::UTC);

    QString prefix = "+";
    if(t1 > t2)
        prefix = "-";

    auto dt = t1.secsTo(t2);

    QTime t(0,0);
    t = t.addSecs(static_cast<int>(dt));

    auto tz = prefix + t.toString("hh:mm");

    return tz;
}



void MainWindow::btnSubmitEmailVerificationCode_clicked()
{
    ui->btnEmailVerificationLoader->setVisible(true);
    ui->btnSubmitEmailVerificationCode->setDisabled(true);
    connect(movie, &QMovie::frameChanged, this, [=](int){
        ui->btnEmailVerificationLoader->setText("");
        ui->btnEmailVerificationLoader->setIcon(movie->currentPixmap().scaledToHeight( ui->btnLoginLoder->height(), Qt::SmoothTransformation));
        ui->btnEmailVerificationLoader->setIconSize(QSize(29,29));
    });
    movie->start();
    auto verifyCode = ui->emailVerificationCodeLineEdit->text();
    QUrlQuery requestData;
    requestData.addQueryItem("username",userEmail);
    requestData.addQueryItem("token",verifyCode);
    requestData.addQueryItem("token_type","1");

    QNetworkRequest request(QUrl(base_url+"/token-verification"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    manager = new QNetworkAccessManager;

    auto reply = manager->post(request, requestData.query(QUrl::FullyEncoded).toUtf8());

    QEventLoop eventLoop;

    connect(reply, &QNetworkReply::finished,&eventLoop,&QEventLoop::quit);
    eventLoop.exec();
    ui->btnEmailVerificationLoader->setVisible(false);
    ui->btnSubmitEmailVerificationCode->setDisabled(false);

    QByteArray response =  reply->readAll();

    QJsonDocument doc = QJsonDocument::fromJson(response);
    auto rootObject = doc.object();


    if(rootObject.value("response_code").toInt() == 1)
    {
        ui->stackedWidget->setCurrentIndex(1);
    }else{
        QString msg = rootObject.value("message").toString();
        messageBox(msg);
    }


}

void MainWindow::update_ui()
{
    Time -= 1;
    if(Time < 0){
        ui->btnResendCode->setEnabled(true);
        return;
    }
    qDebug()<<Time;
    ui->timeSetLineEdit->setPlaceholderText("Email resend available in "+QString::number( Time )+" seconds");
    ui->timeSetLineEdit->setReadOnly(true);
}



void MainWindow::on_btnResendCode_clicked()
{

    Start::Common::globalConfig()->load();
    QString userEmail = Start::Common::globalConfig()->cachedUsername();
    QString password = Start::Common::globalConfig()->cachedPassword();
    QUrlQuery requestData;
    requestData.addQueryItem("username",userEmail);
    requestData.addQueryItem("password",password);

    QNetworkRequest request(QUrl(base_url+"/resend-email-verification"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    manager = new QNetworkAccessManager;

    auto reply = manager->post(request, requestData.query(QUrl::FullyEncoded).toUtf8());

    QEventLoop eventLoop;

    connect(reply, &QNetworkReply::finished,&eventLoop,&QEventLoop::quit);
    eventLoop.exec();

    QByteArray response =  reply->readAll();

    QJsonDocument doc = QJsonDocument::fromJson(response);
    auto rootObject = doc.object();


    if(rootObject.value("response_code").toInt() == 1)
    {
        QString msg = "Send Email";
        messageBox(msg);
        ui->btnResendCode->setEnabled(false);


        ui->stackedWidget->setCurrentIndex(6);

        this->timer = new QTimer(this); // for timer resend ip
        this->timer->setInterval(1000);
        connect(this->timer, SIGNAL(timeout()), this, SLOT(update_ui()));
        this->timer->start();

    }else{
        QString msg = rootObject.value("message").toString();
        messageBox(msg);
    }

}


void MainWindow::saveServer(QByteArray &res ,QString &username, QString &password)
{
    QJsonDocument doc = QJsonDocument::fromJson(res);
    if(doc.isNull() || !doc.isObject()){
        qDebug()<<"Empty data";
    }else{
        QJsonObject response = doc.object();
        int responseCode = response.value("response_code").toInt();
        if(responseCode == 1){

            m_connectAction->setEnabled(true);
            ui->lblUserName->setText(username);
            info->setLoggedIn(true);
            info->setUsername(username);
            info->setPassword(password);
            Start::Common::globalConnection()->setUsername(username);
            Start::Common::globalConnection()->setPassword(password);
            Start::Common::globalConfig()->setCachedUsername(username);
            Start::Common::globalConfig()->setCachedPassword(password);
            Start::Common::globalConfig()->setLoggedIn(true);
            Start::Common::globalConfig()->save();


            info->setExpireInDays(response.value("expire_in_days").toInt());
            info->setValidityDate(response.value("validity_date").toString());
            info->setExpiryDate(response.value("expired_at").toString());
            info->setUserType(response.value("user_type").toInt());
            info->setUserStatus(response.value("user_status").toString());
            info->setContactEmail(response.value("contact_email").toString());
            info->setNoticeType(response.value("notice_type").toInt());
            info->setNoticeText(response.value("notice").toString());
            info->setNoticeStartTime(response.value("start_after").toDouble());
            info->setNoticeEndTime(response.value("end_after").toDouble());
            info->setNoticeLockServerIds(response.value("lock_servers_id").toString());
            info->setReferenceEnabled(response.value("allow_referrence_feature").toBool());

            QJsonDocument array;
            array.setArray(response.value("ip_bundle").toArray());
            QSettings settings("startvpn", "Kolpolok Limited");
            settings.setValue("IPBundle", array.toJson());

            info->setIPBundle(array.toJson());
            // go to root window
            ui->stackedWidget->setCurrentIndex(2);
            ui->stackedWidget_2->setCurrentIndex(1);
            int defultIndex = 0;
            setSelectedLocation(defultIndex);

            createServerTreeWidget();
        }

    }

}


void MainWindow::createServerTreeWidget()
{

    qDebug()<<"createServer treewidget how many time count will be know...";
    QSettings settings("startvpn", "Kolpolok Limited");
    auto bundils =  settings.value("IPBundle", "").toString();
    QJsonDocument doc = QJsonDocument::fromJson(bundils.toUtf8());
    bool TypeOfUser = true;
    QJsonArray serverArray = doc.array();
    createFreeBundil();
    createHighspeedBudile();
    createStreamingBundil();
    createGamingBundil();
    foreach (QJsonValue server  , serverArray)
    {
       auto obj = server.toObject();
       QString ipName =  obj.value("ipName").toString();
       QString country =  obj.value("countryName").toString();
       QString code = obj.value("countryCode").toString();
       QString ip = obj.value("ip").toString();
       int type = obj.value("type").toInt();
       int countryCode = code.toInt();
       bool is_lock = false;
       if(TypeOfUser){
           QString bundleName =  obj.value("bundleName").toString();
           if(bundleName=="Free Bundle"){
               ui->lblTypeOfUser->setText("Free User");
           }else{
               ui->lblTypeOfUser->setText("Premium User");
           }
           ui->lblTypeOfUser->setAlignment(Qt::AlignCenter);

       }
       if(ip=="8.8.8.8"){
           is_lock = true;
       }
       qDebug()<<"before type::"<<type;
       if(type==1){
           addRoot(country,ipName,countryCode,is_lock);
       }

    }

}

void MainWindow::addRoot(QString &name, QString &child,int &code, bool &is_lock)
{
    QString Iname = name;
    bool is_value = map.contains(Iname);
    if(!is_value){
          map.insert(Iname,true);
    }
    auto ItemList = ui->treeWidget->findItems(
                Iname, Qt::MatchWrap | Qt::MatchWildcard | Qt::MatchRecursive);
    if(is_value){
        addChildServer(ItemList.last(),child,is_lock);
    }else{
        QTreeWidgetItem *item = new QTreeWidgetItem(ui->treeWidget);
        item->setData(0,IDENTITY_ROLE,0);

        QFont fnt;
        fnt.setPixelSize(15);
        item->setFont(0,fnt);
        item->setText(0,name);
        item->setTextAlignment(0,Qt::AlignVCenter);
        QPixmap pixmap(Start::Common::flag(code));
        pixmap.scaled(25,30,Qt::KeepAspectRatio, Qt::SmoothTransformation);

        item->setIcon(0,QIcon(pixmap));
        item->treeWidget()->setIconSize(QSize(35,25));
        QLabel *label = new QLabel();

        label->resize(10,10);

        QPixmap pix(":/logo/resources/arrow up.png");
        pix = pix.scaled(label->size(),Qt::KeepAspectRatio);

        label->setPixmap(pix);
        label->setAccessibleName(name);
        label->setStyleSheet("margin-left:200px;");
        label->setAlignment(Qt::AlignRight);
        label->setAlignment(Qt::AlignVCenter);




        ui->treeWidget->setItemWidget(item,0,label);

        ui->treeWidget->addTopLevelItem(item);
        if(!child.isEmpty())
            addChildServer(item,child,is_lock);

    }

}

void MainWindow::addChildServer(QTreeWidgetItem *parent, QString name, bool is_lock)
{
    QTreeWidgetItem *item = new QTreeWidgetItem(parent);
    item->setText(0,name);
    item->setTextAlignment(0,Qt::AlignVCenter);
    item->setData(0,IDENTITY_ROLE,1);

    QPixmap pxm(":/logo/resources/location.png");
    pxm = pxm.scaled(QSize(15, 15), Qt::KeepAspectRatio);
    QFont fnt;
    fnt.setPixelSize(15);
    item->setFont(0,fnt);
    item->setIcon(0,pxm);

    if(is_lock){
        QLabel *label = new QLabel();
        label->resize(15,15);
        QPixmap pix(":/logo/resources/lock_server_icon.png");
        pix = pix.scaled(label->size(),Qt::KeepAspectRatio);

        label->setPixmap(pix);
        label->setAccessibleName(name);
        label->setStyleSheet("margin-left:180px");

        ui->treeWidget->setItemWidget(item,0,label);
    }

    parent->addChild(item);


}

void MainWindow::setSelectedLocation(int &index)
{

    QSettings settings("startvpn", "Kolpolok Limited");
    auto bundils =  settings.value("IPBundle", "").toString();
    QJsonDocument doc = QJsonDocument::fromJson(bundils.toUtf8());
    bool flag = true;
    QJsonArray serverArray = doc.array();
    QLabel *label = new QLabel();

    label->resize(10,10);
    QPixmap pix(":/logo/resources/arrow down.png");
    pix = pix.scaled(label->size(),Qt::KeepAspectRatio);

    ui->lblSelectedServerLogo->setPixmap(pix);

    ui->lblSelectedServerFlag->setFixedWidth(45);
    ui->lblSelectedServerFlag->setFixedHeight(35);
    foreach (QJsonValue server  , serverArray)
    {

       auto obj = server.toObject();
       int ip_id =  obj.value("ip_id").toInt();
       QString country =  obj.value("countryName").toString();
       QString code = obj.value("countryCode").toString();
       int countryCode = code.toInt();
       QString server_city = obj.value("server_city").toString();
       QString ipName = obj.value("ipName").toString();
       QString ip = obj.value("ip").toString();
       QString bundilName = obj.value("bundleName").toString();
       int type = obj.value("type").toInt();
       if(bundilName!="Free Bundle"){
           ui->toolButtonUpgrade->setText("Switch Plan");
           ui->toolButtonUpgrade->setVisible(false);
           ui->btnUpgradeNow_2->setText("Switch Plan");
           ui->btnUpgradeNow->setText("Switch Plan");
       }else{
           ui->toolButtonUpgrade->setText("Get Premium");
           ui->btnUpgradeNow_2->setText("Get Premium");
           ui->btnUpgradeNow->setText("Get Premium");
       }

       if(index==0 && flag&&ip !="8.8.8.8"&&type==1){
           ui->lblSelectedServerText->setText(country+"\n"+server_city);
           QPixmap pixmap(Start::Common::flag(countryCode));
           ui->lblSelectedServerFlag->setPixmap(pixmap);

           flag = false;
           int ip_id = obj.value("ip_id").toInt();
           QString config = obj.value("config").toString();
           int type = obj.value("type").toInt();
           QString ip = obj.value("ip").toString();
           info->setCurrentIPBundle(ip_id,config,type,ip,ipName);
           if(ip.contains(":"))
               ip =  ip.split(":").first();
           ui->lbldefultip->setText("Unprotected IP: "+ip);
           ui->lblConnectIP->setText("Protected IP: "+ip);
           ui->lbldefultCountry->setText(country);
           ui->stackedWidget_2->setCurrentIndex(1);

           break;
       }
       else if((ip_id==index)&(ip !="8.8.8.8")&&(type==1)){
           ui->lblSelectedServerText->setText(country+"\n"+server_city);
           QPixmap pixmap(Start::Common::flag(countryCode));
           ui->lblSelectedServerFlag->setPixmap(pixmap);

           flag = false;
           int ip_id = obj.value("ip_id").toInt();
           QString config = obj.value("config").toString();
           int type = obj.value("type").toInt();
           QString ip = obj.value("ip").toString();
           info->setCurrentIPBundle(ip_id,config,type,ip,ipName);
           if(ip.contains(":"))
               ip =  ip.split(":").first();
           ui->lbldefultip->setText("Unprotected IP: "+ip);
           ui->lblConnectIP->setText("Protected IP: "+ip);
           ui->stackedWidget_2->setCurrentIndex(2);
           break;
       }
    }
}



void MainWindow::on_btnDisconnect_clicked()
{


    Start::Common::globalConnection()->disconnect();
    ui->stackedWidget_2->setCurrentIndex(1);
    Start::Common::globalConfig()->setActiveServerIndex(0);
    Start::Common::globalConfig()->save();
    int defult = 0;
    setSelectedLocation(defult);

}

void MainWindow::on_lineEdit_textChanged(const QString &text)
{
    ui->treeWidget->clear();
    map.clear();
    QSettings settings("startvpn", "Kolpolok Limited");
    auto bundils =  settings.value("IPBundle", "").toString();
    QJsonDocument doc = QJsonDocument::fromJson(bundils.toUtf8());
    QJsonArray serverArray = doc.array();
    createFreeBundil(text);
    createHighspeedBudile(text);
    createStreamingBundil(text);
    createGamingBundil(text);
    foreach (QJsonValue server  , serverArray)
    {
       auto obj = server.toObject();
       QString ipName =  obj.value("ipName").toString();
       QString country =  obj.value("countryName").toString();
       QString ip = obj.value("ip").toString();
       int type = obj.value("type").toInt();
       bool is_lock = false;
       if(ip=="8.8.8.8"){
           is_lock = true;
       }
        if(country.contains(text.toUpper(),Qt::CaseInsensitive) || ipName.contains(text.toUpper(),Qt::CaseInsensitive)){
            QString code = obj.value("countryCode").toString();
            int countryCode = code.toInt();
            if(type==1)
            {
                addRoot(country,ipName,countryCode,is_lock);
            }


        }

    }

}


void MainWindow::onStatusChanged(Start::OpenConnectConnection::Status status)
{
    auto opcState = static_cast<Start::OpenConnectConnection::Status>(status);

    if(opcState == Start::OpenConnectConnection::CONNECTING)
    {
        m_state = 1;
        ui->lbldefultCountry->setText("Connecting...");
        update();
    }
    if(opcState == Start::OpenConnectConnection::IDLE)
    {
        m_state = 0;
        ui->lbldefultCountry->setText("Disconnected");
        ui->btnConnect->setEnabled(true);
        if(m_scheduleConnect)
        {
            m_scheduleConnect = false;
            QTimer::singleShot(0,this,&MainWindow::OnConnection);
        }
        update();
    }
    if(opcState == Start::OpenConnectConnection::CONNECTED)
    {
        m_state = 2;
        info->isConnected = true;
        ui->btnConnect->setEnabled(true);
        Start::Common::globalConfig()->save();
        update();
        int activeServerIndex = Start::Common::globalConfig()->activeServerIndex();
        setSelectedLocation(activeServerIndex);
        ui->stackedWidget_2->setCurrentIndex(2);
        info->loadIpBundle();

    }
}

void MainWindow::requestConnection(bool reconnect)
{
    if(reconnect)
    {
        if(m_state == 0)
            OnConnection();
        else
        {
            OnConnection();
            m_scheduleConnect = true;
        }
    }
    else
            OnConnection();
}

void MainWindow::setipID(int &ipID)
{
    this->ipID = ipID;
}

int MainWindow::getipID()
{
    return this->ipID;
}

void MainWindow::setConfig(QString &config)
{
    this->config = config;
}

QString MainWindow::getConfig()
{
    return this->config;
}

void MainWindow::setType(int &type)
{
    this->type = type;
}

int MainWindow::getType()
{
    return this->type;
}

void MainWindow::setIP(QString &ip)
{
    this->ip = ip;
}

QString MainWindow::getIP()
{
    return this->ip;
}
void MainWindow::resetBundil()
{
    ipID = 0;
    config = "";
    type = 0;
    ip = "";
    saveBundil();
}
void MainWindow::createBundil(QString name, QString key,int countryCode,QString text){
    QString country = name;
    QString nameIP = "";
    QSettings settings("startvpn", "Kolpolok Limited");
    auto bundils =  settings.value("IPBundle", "").toString();
    QJsonDocument doc = QJsonDocument::fromJson(bundils.toUtf8());
    bool TypeOfUser = true;
    QJsonArray serverArray = doc.array();
    foreach (QJsonValue server  , serverArray)
    {
       auto obj = server.toObject();
       QString ipName =  obj.value("ipName").toString();
       QString ip = obj.value("ip").toString();
       int type = obj.value("type").toInt();
       bool is_lock = false;
       int is_value = obj.value(key).toInt();
       if(TypeOfUser){
           QString bundleName =  obj.value("bundleName").toString();
           if(bundleName!="Free Bundle"){
                if(name=="Free")
                    return;
           }
           if(is_value)
                addRoot(country,nameIP,countryCode,is_lock);
           TypeOfUser = false;
       }
       if(ip=="8.8.8.8"){
           is_lock = true;
       }
       if(type==1){
           if(is_value){
                if(country.contains(text.toUpper(),Qt::CaseInsensitive) || ipName.contains(text.toUpper(),Qt::CaseInsensitive))
                    addRoot(country,ipName,countryCode,is_lock);
            }

        }
    }
}
bool MainWindow::bundileIsExist(QString Ikey,QString text)
{
    QSettings settings("startvpn", "Kolpolok Limited");
    auto bundils =  settings.value("IPBundle", "").toString();
    QJsonDocument doc = QJsonDocument::fromJson(bundils.toUtf8());
    QJsonArray serverArray = doc.array();
    foreach (QJsonValue server  , serverArray)
    {
       auto obj = server.toObject();
       QString ipName =  obj.value("ipName").toString();
       QString country =  obj.value("countryName").toString();
       int is_value = obj.value(Ikey).toInt();
       if(is_value){
           if(country.contains(text.toUpper(),Qt::CaseInsensitive) || ipName.contains(text.toUpper(),Qt::CaseInsensitive))
                return true;
        }


    }
    return false;
}

void MainWindow::createFreeBundil(QString text)
{
    QString Ikey = "is_free";
    if(bundileIsExist(Ikey,text))
        createBundil("Free",Ikey,1000,text);
}

void MainWindow::createHighspeedBudile(QString text)
{
    QString Ikey = "is_fast_server";
    if(bundileIsExist(Ikey,text))
        createBundil("High Speed",Ikey,1001,text);
}

void MainWindow::createGamingBundil(QString text)
{
    QString Ikey = "is_gaming";
    if(bundileIsExist(Ikey,text))
        createBundil("Gaming",Ikey,1002,text);
}

void MainWindow::createStreamingBundil(QString text)
{
    QString Ikey = "is_online_stream";
    if(bundileIsExist(Ikey,text))
        createBundil("Streaming",Ikey,1003,text);
}

void MainWindow::saveBundil()
{
    QSettings settings("connectedIPBundil", "Kolpolok Limited");
    settings.setValue("ipID", ipID);
    settings.setValue("config", config);
    settings.setValue("type", type);
    settings.setValue("ip", ip);

}

void MainWindow::loadBundil()
{
    QSettings settings("connectedIPBundil", "Kolpolok Limited");
    this->ipID = settings.value("ipID", ipID).toInt();
    this->config = settings.value("config", config).toString();
    this->type = settings.value("type", type).toInt();
    this->ip = settings.value("ip", ip).toString();
}

void MainWindow::OnConnection()
{

    if(m_state == 0)
    {
        loadBundil();
        Start::Common::globalConfig()->setActiveServerIndex(ipID);
        Start::Common::globalConnection()->setType(type);
        Start::Common::globalConnection()->setOpenConfig(config, ip);

        if(!Start::Common::globalConnection()->server())
        {
            Start::Common::globalConnection()->setServer(std::make_shared<Start::OpenConnectServer>(ip));
        }
        else
            Start::Common::globalConnection()->server()->setServerAddress(ip);

        auto ret = Start::Common::globalConnection()->connect();
        switch (ret) {
        case Start::OpenConnectConnection::SOCKET_ACTIVE: {
            QString msg = "Connection already active";
            messageBox(msg);
            return;
        }
        case Start::OpenConnectConnection::INSTANCE_ACTIVE: {
            QString msg = "VPN already running";
            messageBox(msg);
            return;
        }
        case Start::OpenConnectConnection::SERVER_INVALID: {
            QString msg = "Invalid server";
            messageBox(msg);
            return;
        }
        case Start::OpenConnectConnection::CREDENTIAL_INVALID: {
            QString msg = "Invalid credentials";
            messageBox(msg);
            return;
        }
        default:
            return;

        }
    }
    else
    {
        Start::Common::globalConnection()->disconnect();
    }

}



void MainWindow::on_btnLoginPasswordShowlogo_clicked()
{
   if(ui->password->echoMode()==QLineEdit::Password){
       ui->password->setEchoMode(QLineEdit::Normal);
       QPixmap pixmap(":/logo/resources/view.png");
       pixmap = pixmap.scaled(15,15,Qt::KeepAspectRatio, Qt::SmoothTransformation);
       QIcon ButtonIcon(pixmap);
       ui->btnLoginPasswordShowlogo->setIcon(ButtonIcon);
   }else{
       ui->password->setEchoMode(QLineEdit::Password);
       auto height = ui->btnLoginPasswordShowlogo->height();
       auto wight = ui->btnLoginPasswordShowlogo->width();
       QPixmap pixmap(":/logo/resources/hide.png");
       pixmap = pixmap.scaled(wight,height,Qt::KeepAspectRatio, Qt::SmoothTransformation);
       QIcon ButtonIcon(pixmap);
       ui->btnLoginPasswordShowlogo->setIcon(ButtonIcon);
   }
}

void MainWindow::on_btnSignUpPassshow_clicked()
{
    if(ui->registitionPasswordLineEdit->echoMode()==QLineEdit::Password){
        ui->registitionPasswordLineEdit->setEchoMode(QLineEdit::Normal);
        QPixmap pixmap(":/logo/resources/view.png");
        pixmap = pixmap.scaled(15,15,Qt::KeepAspectRatio, Qt::SmoothTransformation);
        QIcon ButtonIcon(pixmap);
        ui->btnSignUpPassshow->setIcon(ButtonIcon);
    }else{
        ui->registitionPasswordLineEdit->setEchoMode(QLineEdit::Password);
        auto height = ui->btnSignUpPassshow->height();
        auto wight = ui->btnSignUpPassshow->width();
        QPixmap pixmap(":/logo/resources/hide.png");
        pixmap = pixmap.scaled(wight,height,Qt::KeepAspectRatio, Qt::SmoothTransformation);
        QIcon ButtonIcon(pixmap);
        ui->btnSignUpPassshow->setIcon(ButtonIcon);
    }
}


void MainWindow::on_btnSignUpConfirmPassShow_clicked()
{
    if(ui->registationConfirmPasswordLineEdit->echoMode()==QLineEdit::Password){
        ui->registationConfirmPasswordLineEdit->setEchoMode(QLineEdit::Normal);
        QPixmap pixmap(":/logo/resources/view.png");
        pixmap = pixmap.scaled(15,15,Qt::KeepAspectRatio, Qt::SmoothTransformation);
        QIcon ButtonIcon(pixmap);
        ui->btnSignUpConfirmPassShow->setIcon(ButtonIcon);
    }else{
        ui->registationConfirmPasswordLineEdit->setEchoMode(QLineEdit::Password);
        auto height = ui->btnSignUpConfirmPassShow->height();
        auto wight = ui->btnSignUpConfirmPassShow->width();
        QPixmap pixmap(":/logo/resources/hide.png");
        pixmap = pixmap.scaled(wight,height,Qt::KeepAspectRatio, Qt::SmoothTransformation);
        QIcon ButtonIcon(pixmap);
        ui->btnSignUpConfirmPassShow->setIcon(ButtonIcon);
    }
}

void MainWindow::customMessage(QString &msg)
{
    auto msgbox = new QMessageBox(this);
//    msgbox->setGeometry(750, 650, 250, 200);
//    msgbox->setFixedSize(100,10);
    msgbox->setStyleSheet("color: white;background-color:black;");
    msgbox->setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
    msgbox->setIcon(QMessageBox::Information);
    msgbox->setWindowTitle("Success");
    msgbox->setText("<font size = 4 color = white >"+msg+"</font> ");
    msgbox->setStandardButtons(QMessageBox::NoButton);
    msgbox->open();
    auto timer = new QTimer(msgbox);
    QObject::connect(timer, &QTimer::timeout, msgbox, &QMessageBox::deleteLater);
    timer->start(1500);
}

void MainWindow::messageBox(QString &msg)
{
    auto msgbox = new QMessageBox(this);
//    msgbox->setGeometry(1050, 650, 250, 200);
    msgbox->setStyleSheet("color: white;background-color:gray;");
    msgbox->setIcon(QMessageBox::Information);
    msgbox->setWindowTitle("Error");
    msgbox->setText("<font size = 4 color = white >"+msg+"</font> ");
    msgbox->open();
    auto timer = new QTimer(msgbox);
    QObject::connect(timer, &QTimer::timeout, msgbox, &QMessageBox::deleteLater);
    timer->start(5500);
}




void MainWindow::btnUpgradeNow_clicked()
{
    Start::Common::globalConfig()->load();
    QString userEmail = Start::Common::globalConfig()->cachedUsername();
    QString password = Start::Common::globalConfig()->cachedPassword();
    QString url = "https://vpnstart.net/remotelogin?id="+userEmail.toUtf8().toBase64()+"&token="+password.toUtf8().toBase64();
    QDesktopServices::openUrl(QUrl(url, QUrl::TolerantMode));
}

void MainWindow::loadTray()
{
    if (QSystemTrayIcon::isSystemTrayAvailable()) {

        m_trayIcon = new QSystemTrayIcon(this);

        auto trayIconMenu = new QMenu(this);

        m_connectAction = new QAction(tr("Connect"), this);
        m_connectAction->setDisabled(true);
        connect(m_connectAction,&QAction::triggered, this, [=](bool){
           requestConnection();
        });
        trayIconMenu->addAction(m_connectAction);

        auto quitAction = new QAction(tr("Quit"), this);
        connect(quitAction,&QAction::triggered, this, [=](bool){
            if(m_state == 2)
            {
                connect(this, &MainWindow::readyToShutdown, qApp, &QApplication::quit);
                requestConnection();
            }
            else
                qApp->quit();
        });
        trayIconMenu->addAction(quitAction);

        m_trayIcon->setContextMenu(trayIconMenu);

        connect(m_trayIcon, &QSystemTrayIcon::activated, this, [=](QSystemTrayIcon::ActivationReason reason){
            if(reason == QSystemTrayIcon::DoubleClick)
                this->show();
        });

        QIcon icon(":/logo/resources/start_vpn_final_logo.png");
        icon.setIsMask(true);

        m_trayIcon->setIcon(icon);
        m_trayIcon->show();
    } else {
        m_trayIcon = nullptr;
    }
}


void MainWindow::on_btnPrivacyPolicy_clicked()
{
    QDesktopServices::openUrl(QUrl("https://vpnstart.net/file/privacy_policy.pdf", QUrl::TolerantMode));
}


void MainWindow::on_btnDeleteAccount_clicked()
{
    ui->btnMenuLoader->setVisible(true);
    ui->btnDeleteAccount->setDisabled(true);
    connect(movie, &QMovie::frameChanged, this, [=](int){
        ui->btnMenuLoader->setText("");
        ui->btnMenuLoader->setIcon(movie->currentPixmap().scaledToHeight( ui->btnRegistationLoder->height(), Qt::SmoothTransformation));
        ui->btnMenuLoader->setIconSize(QSize(29,29));
    });
    movie->start();
    Start::Common::globalConfig()->load();
    QString userEmail = Start::Common::globalConfig()->cachedUsername();
    QString password = Start::Common::globalConfig()->cachedPassword();
    QUrlQuery requestData;
    requestData.addQueryItem("username",userEmail);
    requestData.addQueryItem("pass",password);
    requestData.addQueryItem("udid",QSysInfo::machineUniqueId());

    QNetworkRequest request(QUrl(base_url+"/remove-user"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    manager = new QNetworkAccessManager;

    auto reply = manager->post(request, requestData.query(QUrl::FullyEncoded).toUtf8());

    QEventLoop eventLoop;

    connect(reply, &QNetworkReply::finished,&eventLoop,&QEventLoop::quit);
    eventLoop.exec();

    QByteArray response =  reply->readAll();

    QJsonDocument doc = QJsonDocument::fromJson(response);
    auto rootObject = doc.object();
    ui->btnMenuLoader->setVisible(false);
    ui->btnDeleteAccount->setDisabled(false);

    if(rootObject.value("response_code").toInt() == 1)
    {

        ui->stackedWidget->setCurrentIndex(0);
        btnLogout_clicked();


    }else{
        QString msg = rootObject.value("message").toString();
        messageBox(msg);
    }
}


void MainWindow::on_btnCopy_clicked()
{
    QClipboard *clipboard = QGuiApplication::clipboard();
    QString text = ui->lblUserName->text();
    clipboard->setText(text);
    QString msg = "Copied";
    messageBox(msg);
}


void MainWindow::on_btnUpgradeNow_2_clicked()
{
    Start::Common::globalConfig()->load();
    QString userEmail = Start::Common::globalConfig()->cachedUsername();
    QString password = Start::Common::globalConfig()->cachedPassword();
    QString url = "https://vpnstart.net/remotelogin?id="+userEmail.toUtf8().toBase64()+"&token="+password.toUtf8().toBase64();
    QDesktopServices::openUrl(QUrl(url, QUrl::TolerantMode));
}

