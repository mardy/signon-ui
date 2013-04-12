include(../common-project-config.pri)
include($${TOP_SRC_DIR}/common-vars.pri)
include($${TOP_SRC_DIR}/common-installs-config.pri)

TEMPLATE = app
TARGET = signon-ui

I18N_DOMAIN = signon-ui

CONFIG += \
    link_pkgconfig \
    qt

QT += \
    core \
    dbus \
    gui \
    widgets \
    network \
    webkitwidgets \
    quick \
    webkit

PKGCONFIG += \
    accounts-qt5 \
    signon-plugins-common \
    libnotify \
    libproxy-1.0 \
    libsignon-qt5

HEADERS = \
    animation-label.h \
    browser-request.h \
    cookie-jar-manager.h \
    debug.h \
    dialog.h \
    errors.h \
    i18n.h \
    inactivity-timer.h \
    network-access-manager.h \
    reauthenticator.h \
    request.h \
    service.h

SOURCES = \
    animation-label.cpp \
    browser-request.cpp \
    cookie-jar-manager.cpp \
    debug.cpp \
    dialog.cpp \
    i18n.cpp \
    inactivity-timer.cpp \
    main.cpp \
    my-network-proxy-factory.cpp \
    network-access-manager.cpp \
    reauthenticator.cpp \
    request.cpp \
    service.cpp

lessThan(QT_MAJOR_VERSION, 5) {
    HEADERS += embed-manager.h
    SOURCES += embed-manager.cpp
}

DEFINES += \
    DEBUG_ENABLED \
    I18N_DOMAIN=\\\"$${I18N_DOMAIN}\\\"

RESOURCES += animationlabel.qrc

include(signonui_dbus_adaptor.pri)

po.target = ../po/signon-ui.pot
po.depends = $${SOURCES}
po.commands = xgettext -o $@ -d $${I18N_DOMAIN} --keyword=_ $^

QMAKE_EXTRA_TARGETS += \
    po

service.path = $${INSTALL_PREFIX}/share/dbus-1/services
service.files = \
    com.nokia.singlesignonui.service
INSTALLS += service

desktop.path = $${INSTALL_PREFIX}/share/applications
desktop.files += signon-ui.desktop

INSTALLS += desktop

OTHER_FILES += \
    webview.qml \
    KeyboardRectangle.qml \
    StandardAnimation.qml

RESOURCES += \
    qml.qrc
