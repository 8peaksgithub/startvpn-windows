win32 {
 !contains(QMAKE_TARGET.arch, x86_64) {

        message("Win 32 build")

        INCLUDEPATH += $$PWD/openconnect/windows/include
        DEPENDPATH += $$PWD/openconnect/windows/include

        win32: LIBS += -L$$PWD/openconnect/windows/lib/ -llibgmp.dll -llibgnutls.dll -llibhogweed.dll -llibiconv.dll -llibidn2.dll -lliblz4.dll -lliblzma.dll
        win32: LIBS += -L$$PWD/openconnect/windows/lib/ -llibspdlog -llibstoken.dll -llibunistring.dll -llibxml2.dll -llibz.dll -llibnettle.dll -llibopenconnect.dll -llibp11-kit.dll
        # TODO: Re-enable when Qt 6 compatible version is available
        # win32: LIBS += -L$$PWD/openconnect/windows/lib/ -llibqtsingleapplication
        win32: LIBS += -L$$PWD/openconnect/windows/lib/ -llibproxy.dll
        win32: LIBS += -lws2_32 -lwbemuuid -lole32 -loleaut32

    } else {

        message("Win 64 build")

        INCLUDEPATH += $$PWD/openconnect/windows/include
        DEPENDPATH += $$PWD/openconnect/windows/include

        win32: LIBS += -L$$PWD/openconnect/windows/lib/ -llibgmp.dll -llibgnutls.dll -llibhogweed.dll -llibiconv.dll -llibidn2.dll -lliblz4.dll -lliblzma.dll
        win32: LIBS += -L$$PWD/openconnect/windows/lib/ -llibspdlog -llibstoken.dll -llibunistring.dll -llibxml2.dll -llibz.dll -llibnettle.dll -llibopenconnect.dll -llibp11-kit.dll
        # TODO: Re-enable when Qt 6 compatible version is available
        # win32: LIBS += -L$$PWD/openconnect/windows/lib/ -llibqtsingleapplication
        win32: LIBS += -L$$PWD/openconnect/windows/lib/ -llibproxy.dll
        win32: LIBS += -lws2_32 -lwbemuuid -lole32 -loleaut32

    }
}

macx {

    message("macos build")

    DEFINES += PROJ_ADMIN_PRIV_ELEVATION
    INCLUDEPATH += $$PWD/openconnect/macos/include
    DEPENDPATH += $$PWD/openconnect/macos/include
    PRE_TARGETDEPS += $$PWD/openconnect/macos/lib/libqtsingleapplication.a

    LIBS += -L$$PWD/openconnect/macos/lib/ -lopenconnect.5
    LIBS += -L$$PWD/openconnect/macos/lib/ -lgnutls.30
    LIBS += -L$$PWD/openconnect/macos/lib/ -lspdlog.1.9.2
    LIBS += -L$$PWD/openconnect/macos/lib/ -lfmt.8.0.1
    LIBS += -L$$PWD/openconnect/macos/lib/ -lqtsingleapplication
    LIBS += -framework Security

}
