#include "mainwindow.h"

#include <QApplication>
#include "common/common.h"
#include <QSystemTrayIcon>
#include <QMenu>
#include <QAction>
#include<QMessageBox>
// TODO: Re-enable single instance with Qt 6 compatible solution
// #include <QtSingleApplication>

#ifdef __MACH__
#include <Security/Security.h>
#include <mach-o/dyld.h>
#endif

#if defined(Q_OS_MACOS) && defined(PROJ_ADMIN_PRIV_ELEVATION)
bool relaunch_as_root()
{
    QMessageBox msgBox;
    char appPath[2048];
    uint32_t size = sizeof(appPath);
    AuthorizationRef authRef;
    OSStatus status;

    /* Get the path of the current program */
    if (_NSGetExecutablePath(appPath, &size) != 0) {
        msgBox.setText(QObject::tr("Could not get program path to elevate privileges."));
        return false;
    }

    status = AuthorizationCreate(NULL, kAuthorizationEmptyEnvironment,
        kAuthorizationFlagDefaults, &authRef);

    if (status != errAuthorizationSuccess) {
        msgBox.setText(QObject::tr("Failed to create authorization reference."));
        return false;
    }
    status = AuthorizationExecuteWithPrivileges(authRef, appPath,
        kAuthorizationFlagDefaults, NULL, NULL);
    AuthorizationFree(authRef, kAuthorizationFlagDestroyRights);

    if (status == errAuthorizationSuccess) {
        /* We've successfully re-launched with root privs. */
        return true;
    }
    return false;
}
#endif

int main(int argc, char *argv[])
{

#if defined(Q_OS_MACOS) && defined(PROJ_ADMIN_PRIV_ELEVATION)
    /* Re-launching with root privs on OS X needs Qt to allow setsuid */
    // TODO: Re-enable when single instance is restored
    // QtSingleApplication::setSetuidAllowed(true);
    QApplication::setSetuidAllowed(true);
#endif

    QApplication a(argc, argv);
    // TODO: Re-enable single instance check when Qt 6 compatible solution is available
    // if (a.isRunning()) {
    //     a.sendMessage("Wake up!");
    //     return 0;
    // }
    a.setQuitOnLastWindowClosed(false);
#if defined(Q_OS_MACOS) && defined(PROJ_ADMIN_PRIV_ELEVATION)
    if (geteuid() != 0) {
        if (relaunch_as_root()) {
            /* We have re-launched with root privs. Exit this process. */
            return 0;
        }

        QMessageBox msgBox;
        msgBox.setText(QObject::tr("This program requires root privileges to fully function."));
        msgBox.setInformativeText(QObject::tr("VPN connection establishment would fail."));
        msgBox.exec();
        return -1;
    }
#endif

    qRegisterMetaType<Start::OpenConnectConnection::Status>("Status");
    MainWindow w;
    // TODO: Re-enable when single instance is restored
    // QObject::connect(&a, &QtSingleApplication::messageReceived,
    //                  [&w](const QString&) {
    //                      w.show();
    //                  });

    // a.setActivationWindow(&w);
    w.setFixedSize(850, 620);
    w.show();
    return a.exec();
}
