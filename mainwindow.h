#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QNetworkAccessManager>
#include <QTimer>
#include <QElapsedTimer>
#include <QTreeWidgetItem>
#include <QStackedWidget>
#include <QDebug>
#include <QPushButton>
#include <userinformation.h>
#include <QSystemTrayIcon>
#include <QMap>
#include "core/openconnectconnection.h"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:


    void backBtton_clicked();

    void btnforgetPassword_clicked();

    void btnBackLogin_clicked();

    void btnregitstation_clicked();

    void btnEmailVerification_clicked();

    void btnSendEmail_clicked();

    void btnLogin_clicked();

    void on_btnCollapse_clicked();

    void on_btnConnectBack_clicked();

    void btnLogout_clicked();

    void btnConnect_clicked();

    void btnSubmitEmailVerificationCode_clicked();

    void expandItem(const QModelIndex &index);

    void on_lineEdit_textChanged(const QString &text);

    void update_ui();
    void on_btnResendCode_clicked();

    void on_btnDisconnect_clicked();


    void on_btnLoginPasswordShowlogo_clicked();

    void on_btnSignUpPassshow_clicked();

    void on_btnSignUpConfirmPassShow_clicked();

    void btnUpgradeNow_clicked();


    void loadTray();



    void on_btnPrivacyPolicy_clicked();

    void on_btnDeleteAccount_clicked();

    void on_btnCopy_clicked();

    void on_btnUpgradeNow_2_clicked();

private:
    Ui::MainWindow *ui;
    QNetworkAccessManager *manager;
    QString userEmail;
    QTimer* timer;
    QTimer* serverRefreshTimer;
    QElapsedTimer *eltimer;
    int Time = 26;
    bool is_Login = false;
    QString base_url = "https://vpnstart.net/startvpn_api/app-api-v1";
    UserInformation *info = new UserInformation();

signals:
    void readyToShutdown();

public:
    static QString currentTimeZone();
    void saveServer(QByteArray  &res, QString &username, QString &password);
    void refreshServerList();
    void createServerTreeWidget();
    void addRoot(QString &name,QString &child,int &code,bool &is_lock);
    void addChildServer(QTreeWidgetItem *parent,QString name, bool is_lock);

    void setSelectedLocation(int &index);
    void OnConnection();

    void onStatusChanged(Start::OpenConnectConnection::Status status);

    void customMessage(QString &msg);
    void messageBox(QString &msg);
    void requestConnection(bool reconnect = false);


    void setipID(int &ipID);
    int getipID();

    void setConfig(QString &config);
    QString getConfig();

    void setType(int &type);
    int getType();

    void setIP(QString &ip);
    QString getIP();

    void saveBundil();
    void loadBundil();
    void resetBundil();
    void createFreeBundil(QString text=NULL);
    void createHighspeedBudile(QString text=NULL);
    void createGamingBundil(QString text=NULL);
    void createStreamingBundil(QString text=NULL);
    void createBundil(QString name,QString key,int countryCode,QString text);
    bool bundileIsExist(QString Ikey,QString text);


private:
    QSystemTrayIcon* m_trayIcon;
    QAction* m_connectAction;
    int m_state;
    int ipID;
    QString config;
    int type;
    QString ip;
    bool m_scheduleConnect;
    QMovie *movie;
    QMap<QString, bool> map;


};
#endif // MAINWINDOW_H
