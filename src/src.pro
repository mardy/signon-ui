include(../common-project-config.pri)

TEMPLATE = subdirs
SUBDIRS = signon-ui.pro

CONFIG(no-widgets) {
    lessThan(QT_MAJOR_VERSION, 5) {
        error('no-widgets' option is supported only with Qt5)
    }
    SUBDIRS += browser-process
}

desktop.path = $${INSTALL_PREFIX}/share/applications
desktop.files += signon-ui.desktop

INSTALLS += desktop
