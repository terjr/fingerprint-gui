TEMPLATE = app
TARGET = fingerprint-suid
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
suid.path   = $${target.path}
suid.extra  = chmod 4755 $(INSTALL_ROOT)$${target.path}/$${TARGET}
INSTALLS += target suid

DEFINES +=LIBEXECDIR=\\\"$${LIBEXECDIR}\\\"
QMAKE_CXXFLAGS+=-fno-strict-aliasing

HEADERS += \
           ../../include/Globals.h

SOURCES += \
           ../../src/FingerprintSuid.cpp
