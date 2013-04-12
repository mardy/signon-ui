include(../../common-project-config.pri)
include($${TOP_SRC_DIR}/common-vars.pri)
include($${TOP_SRC_DIR}/common-installs-config.pri)

TARGET = signon-ui-unittest

CONFIG += \
    build_all \
    debug \
    link_pkgconfig \
    qtestlib

QT += \
    core \
    dbus \
    gui \
    widgets \
    network \
    quick \
    webkitwidgets \
    webkit 

PKGCONFIG += \
    accounts-qt5 \
    signon-plugins-common \
    libnotify \
    libsignon-qt5

SOURCES += \
    fake-libnotify.cpp \
    fake-libsignon.cpp \
    test.cpp \
    $$TOP_SRC_DIR/src/animation-label.cpp \
    $$TOP_SRC_DIR/src/browser-request.cpp \
    $$TOP_SRC_DIR/src/cookie-jar-manager.cpp \
    $$TOP_SRC_DIR/src/debug.cpp \
    $$TOP_SRC_DIR/src/dialog.cpp \
    $$TOP_SRC_DIR/src/i18n.cpp \
    $$TOP_SRC_DIR/src/network-access-manager.cpp \
    $$TOP_SRC_DIR/src/reauthenticator.cpp \
    $$TOP_SRC_DIR/src/request.cpp
HEADERS += \
    fake-libnotify.h \
    test.h \
    $$TOP_SRC_DIR/src/animation-label.h \
    $$TOP_SRC_DIR/src/browser-request.h \
    $$TOP_SRC_DIR/src/debug.h \
    $$TOP_SRC_DIR/src/cookie-jar-manager.h \
    $$TOP_SRC_DIR/src/dialog.h \
    $$TOP_SRC_DIR/src/network-access-manager.h \
    $$TOP_SRC_DIR/src/reauthenticator.h \
    $$TOP_SRC_DIR/src/request.h

lessThan(QT_MAJOR_VERSION, 5) {
    SOURCES += $$TOP_SRC_DIR/src/embed-manager.cpp
    HEADERS += $$TOP_SRC_DIR/src/embed-manager.h
}

INCLUDEPATH += \
    . \
    $$TOP_SRC_DIR/src

QMAKE_CXXFLAGS += \
    -fno-exceptions \
    -fno-rtti

DEFINES += \
    DEBUG_ENABLED \
    UNIT_TESTS

RESOURCES += $$TOP_SRC_DIR/src/animationlabel.qrc
RESOURCES += $$TOP_SRC_DIR/src/qml.qrc

check.commands = "xvfb-run -a dbus-test-runner -t ./signon-ui-unittest"
QMAKE_EXTRA_TARGETS += check