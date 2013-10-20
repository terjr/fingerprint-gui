TEMPLATE = app
CONFIG += qt thread crypto
TARGET = fingerprint-helper
QT += xml
DEPENDPATH += . ../../include ../../src
INCLUDEPATH += . ../../include ../../upek/include

isEmpty(PREFIX) {
	PREFIX = /usr/local
}
isEmpty(LIB) {
	LIB = lib
}
isEmpty(LIBEXEC) {
	LIBEXEC = lib
}
LIBEXECDIR = $${PREFIX}/$${LIBEXEC}/fingerprint-gui

target.path = $${LIBEXECDIR}
INSTALLS += target

DEFINES +=LIBEXECDIR=\\\"$${LIBEXECDIR}\\\"
QMAKE_CXXFLAGS+=-fno-strict-aliasing

LIBS += -lusb-1.0 -lfprint -lfakekey -lX11 -ldl

# Input
HEADERS += \
            ../../include/DeviceHandler.h \
            ../../include/PermissionHandler.h \
            ../../include/UsbDevice.h \
            ../../include/FingerprintDevice.h \
            ../../src/drivers/UpekDevice.h \
            ../../src/drivers/GenericDevice.h \
            ../../include/FingerprintData.h \
            ../../include/FingerprintDiscoverer.h \
            ../../include/xmlwriter.h \
            ../../include/UserSettings.h \
            ../../include/PamGUI.h \
            ../../include/PamNonGUI.h \
            ../../include/Globals.h

FORMS += ../../src/FingerprintPlugin.ui

SOURCES += \
            ../../src/DeviceHandler.cpp \
            ../../src/PermissionHandler.cpp \
            ../../src/UsbDevice.cpp \
            ../../src/drivers/UpekDevice.cpp \
            ../../src/drivers/GenericDevice.cpp \
            ../../src/FingerprintData.cpp \
            ../../src/FingerprintDiscoverer.cpp \
            ../../src/xmlwriter.cpp \
            ../../src/UserSettings.cpp \
            ../../src/PamGUI.cpp \
            ../../src/PamNonGUI.cpp \
            ../../src/FingerprintHelper.cpp

RESOURCES += ../../src/widget-res.qrc
