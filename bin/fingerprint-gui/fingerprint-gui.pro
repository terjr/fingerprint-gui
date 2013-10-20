TEMPLATE = app
CONFIG += qt thread crypto
TARGET = fingerprint-gui
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
BINDIR     = $${PREFIX}/bin
LIBEXECDIR = $${PREFIX}/$${LIBEXEC}/fingerprint-gui
SHAREDIR   = $${PREFIX}/share
MAN1DIR    = $${PREFIX}/share/man/man1
DOCDIR     = $${PREFIX}/share/doc/fingerprint-gui

target.path     = $${BINDIR}
desktop.path    = $${SHAREDIR}/applications
desktop.files   = fingerprint-gui.desktop
manpage.path    = $${MAN1DIR}
manpage.files   = ../../man/$${TARGET}.1
help.path       = $${DOCDIR}
help.files      = ../../doc/Manual_??.html \
		  ../../doc/*.png
directory.path  = /var/lib/fingerprint-gui
directory.extra = :
INSTALLS += target desktop manpage help directory

DEFINES +=LIBEXECDIR=\\\"$${LIBEXECDIR}\\\"
QMAKE_CXXFLAGS+=-fno-strict-aliasing

LIBS += -lusb-1.0 -lfprint -lpam -ldl

# Input
HEADERS += ../../include/DeviceHandler.h \
           ../../include/PermissionHandler.h \
           ../../include/Globals.h \
           ../../include/AboutImpl.h \
           ../../include/ExistDialogImpl.h \
           ../../include/Fingercodes.h \
           ../../include/Fingerprint.h \
           ../../include/FingerprintDevice.h \
           ../../include/FingerprintData.h \
           ../../src/drivers/UpekDevice.h \
           ../../src/drivers/GenericDevice.h \
           ../../include/MainWindowImpl.h \
           ../../include/SavedDialogImpl.h \
           ../../include/MessageDialogImpl.h \
           ../../include/PamTester.h \
           ../../include/xmlwriter.h \
           ../../include/UserSettings.h \
           ../../include/UsbDevice.h

FORMS += ../../src/About.ui \
         ../../src/ExistDialog.ui \
         ../../src/FingerprintGUI.ui \
         ../../src/SavedDialog.ui \
         ../../src/Message.ui

SOURCES += ../../src/DeviceHandler.cpp \
           ../../src/PermissionHandler.cpp \
           ../../src/AboutImpl.cpp \
           ../../src/ExistDialogImpl.cpp \
           ../../src/Fingerprint.cpp \
           ../../src/FingerprintData.cpp \
           ../../src/drivers/UpekDevice.cpp \
           ../../src/drivers/GenericDevice.cpp \
           ../../src/FingerprintGUI.cpp \
           ../../src/MainWindowImpl.cpp \
           ../../src/SavedDialogImpl.cpp \
           ../../src/MessageDialogImpl.cpp \
           ../../src/PamTester.cpp \
           ../../src/xmlwriter.cpp \
           ../../src/UserSettings.cpp \
           ../../src/UsbDevice.cpp

RESOURCES += ../../src/GUI-res.qrc\
             ../../src/About-res.qrc
