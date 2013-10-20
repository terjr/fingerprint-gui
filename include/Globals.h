/*
 * Project "Fingerprint GUI": Services for fingerprint authentication on Linux
 * Module: Globals.h
 * Purpose: some global defines
 *
 * @author  Wolfgang Ullrich
 * Copyright (C) 2008-2012  Wolfgang Ullrich
 */

/*
    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifndef _GLOBALS_H
#define _GLOBALS_H

#include <syslog.h>

#define VERSION "1.04"
#define COPYRIGHT "(C) 2008-2012  Wolfgang Ullrich"
#define HOMEPAGE "http://www.pdfserver.net/fingerprint/"
#define FORUM_ONLINE "http://darkblue.homeip.net/fingerprint/Forum/"
#define DONATE "contact.php"
#define BITCOINADDRESS "1LsaAggcE8rgB3Hkrxv2adEYEqDkbVSHFd"
#define LICENSE "http://www.gnu.org/licenses/gpl.html"

extern "C" {
#include <libfprint/fprint.h>
}

#define _MULTI_THREADED

#ifndef LIBEXECDIR
#define LIBEXECDIR "/usr/local/lib/fingerprint-gui"
#endif //LIBEXECDIR

#ifndef DOCDIR
#define DOCDIR "/usr/share/doc/fingerprint-gui"
#endif //DOCDIR

#define MODPROBE_COMMAND "/sbin/modprobe"

// Modulenames:
#define GUI_NAME "fingerprint-gui"
#define SUID_NAME "fingerprint-suid"
#define HELPER_NAME "fingerprint-helper"
#define IDENTIFIER_NAME "fingerprint-identifier"
#define AGENT_NAME "fingerprint-polkit-agent"
#define POLKIT_LISTENER_ID "/fingerprintgui/PolicyKit1/AuthenticationAgent"
#define PLUGIN_NAME "fingerprint-plugin"
#define PAM_NAME "pam_fingerprint-gui"

// Helpfiles:
#define HELP_BASE "/Manual_"
#define HELP_EXT ".html"

#define DATA_DIR "/var/lib/fingerprint-gui/"    // configdirectory on local machine
#define PASSWORD_DIR "/.fingerprints/"          // directory for password on external media
#define DATA_EXT ".bir"                         //file extension for fingerprint bir data

// Arguments for modules
#define ARG_DEBUG1 "-d"
#define ARG_DEBUG2 "--debug"
#define ARG_DEBUG3 "debug"
#define ARG_USER "-u"
#define ARG_DECORATED "decorated"
#define ARG_TRY_FIRST_IDENT "try_first_identified"

// File and directory modes:
#define MODE_MASK (S_IRWXO|S_IRWXG|S_IRWXU)
#define DATA_DIR_MODE (S_IRWXU|S_IXGRP|S_IRGRP|S_IXOTH|S_IROTH)
#define USER_DIR_MODE (S_IRUSR|S_IWUSR|S_IXUSR)
#define USER_FILE_MODE (S_IRUSR|S_IWUSR)
#define SUID_MODE (S_IEXEC|S_ISUID)

#define ID_FILE_NAME "/usb.ids"                 //file with USB-IDs in program directory
//This might change in later versions of "usb.ids" from http://www.linux-usb.org/usb.ids
#define END_OF_DEVICE_IDS "# List of known device classes, subclasses and protocols";

#define BACKUP_FILENAME "/Fingerprints.tar.gz"

#define OPERATION_TIMEOUT 60000     //Timeout (usecs) for user action after requesting a swipe
#define SHOW_DELAY 500000           //delay (usecs) for showing results before exiting the GUI

#define SUID_COMMAND LIBEXECDIR "/" SUID_NAME
#define HELPER_COMMAND LIBEXECDIR "/" HELPER_NAME
#define HELPER_PID "/tmp/" HELPER_NAME ".pid"

#define PLUGIN_FIFO "/tmp/" PLUGIN_NAME
// Messages for controlling fingerprint-plugin
#define MSG_ALIVE "_aLiVe_"
#define MSG_BOLD "_bOlD_"
#define MSG_NORMAL "_nOrMaL_"
#define MSG_LABEL "_lAbEl_"
#define MSG_STOP "_sToP_pLuGiN_"
#define MSG_AUTH "_aUtH_"

#define UNDECORATED_MODAL_WINDOW_CENTERED \
    setWindowFlags(Qt::X11BypassWindowManagerHint); \
    setFocusPolicy(Qt::StrongFocus); \
    setFocus(); \
    raise(); \
    activateWindow(); \
    int h=height(); \
    int w=width(); \
    int x=(int)(QApplication::desktop()->width()-w)/2; \
    /* Some applications place their prompt to the center of the screen, so we place our dialog below */ \
    int y=(int)(QApplication::desktop()->height()-h)*3/4; \
    setGeometry(x,y,w,h);

#define ANIMATION_MOVIE \
    movie=new QMovie(QString::fromUtf8(":/new/prefix1/res/Animation.gif")); \
    movie->start(); \
    animationLabel->setMovie(movie);

#define START_TIMER \
    timer=new QTimer(this); \
    connect(timer,SIGNAL(timeout()),this,SLOT(timerTick())); \
    timer->start(500);

#endif /* _GLOBALS_H */
