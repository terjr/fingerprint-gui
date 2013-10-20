/*
 * Project "Fingerprint GUI": Services for fingerprint authentication on Linux
 * Module: FingerprintSuid.cpp
 * Purpose: Helper process running suid root and will be called from "fingerprint-gui" only.
 *          Creates DATA_DIR and DATA_DIR/<username>/ if reuired.
 *          Checks ownership and mode:
 *          DATA_DIR root/root 0755
 *          DATA_DIR/<username>/ user/user 0700
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

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <string.h>
#include <sys/stat.h>
#include <pwd.h>
#include <unistd.h>
#include "Globals.h"

using namespace std;

static string syslogIdent=string(SUID_NAME);

int main(int argc, char** argv) {
    struct stat bStat;
    string userdir(DATA_DIR);
    char* user=NULL;

    openlog(syslogIdent.data(),LOG_NDELAY|LOG_PID,LOG_AUTH);
    setlogmask(LOG_UPTO(LOG_ERR));
    for(int i=0;i<argc;i++){
        if((strcmp(argv[i],"-u")==0)){
            user=argv[++i];
        }

        if((strcmp(argv[i],ARG_DEBUG1)==0)
                | (strcmp(argv[i],ARG_DEBUG2)==0)
                | (strcmp(argv[i],ARG_DEBUG3)==0)){
            setlogmask(-1);
            syslog(LOG_INFO,"Got \"debug\" argument.");
        }
    }
    syslog(LOG_ERR,"Started.");

    // Do we have a username?
    if(user==NULL){
        syslog(LOG_ERR,"Username unknown. Aborting!");
        return (EXIT_FAILURE);
    }

    // Are we running suid root?
    if(geteuid()!=0){
        syslog(LOG_ERR,"%s is not running as root. Must be owned by root and executed suid. Aborting!",SUID_NAME);
        return (EXIT_FAILURE);
    }

    // Check sanity of DATA_DIR first
    if(stat(DATA_DIR,&bStat)!=0){           // could not be stated, try to create it
        syslog(LOG_WARNING,"%s doesn't exist. Creating.",DATA_DIR);
        if(mkdir(DATA_DIR,DATA_DIR_MODE)){
            syslog(LOG_ERR,"Could not create %s. Aborting!",DATA_DIR);
            return (EXIT_FAILURE);
        }
        if(chown(DATA_DIR,0,0)){
            syslog(LOG_ERR,"Could not chown %s to 0/0. Aborting!",DATA_DIR);
            return (EXIT_FAILURE);
        }
        stat(DATA_DIR,&bStat);              // Stat again
    }
    if(bStat.st_uid!=0||bStat.st_gid!=0){   // check ownership
        syslog(LOG_ERR,"%s not owned by root/root. Aborting!",DATA_DIR);
        return (EXIT_FAILURE);
    }
    syslog(LOG_DEBUG,"%s owned by root/root.",DATA_DIR);
    if((bStat.st_mode&MODE_MASK)!=DATA_DIR_MODE){    // check permissions
        syslog(LOG_ERR,"%s has not mode %o. This directory will be ignored!",DATA_DIR,DATA_DIR_MODE);
        return (EXIT_FAILURE);
    }
    syslog(LOG_DEBUG,"%s has mode %o.",DATA_DIR,bStat.st_mode);

    // Check existance of userdir
    userdir.append(user);
    if(stat(userdir.data(),&bStat)!=0){     // could not be stated, try to create it
        syslog(LOG_DEBUG,"%s doesn't exist. Creating.",userdir.data());
        if(mkdir(userdir.data(),S_IRWXU)){
            syslog(LOG_ERR,"Could not create %s. Aborting!",userdir.data());
            return (EXIT_FAILURE);
        }
        if(chown(userdir.data(),getuid(),getgid())){
            syslog(LOG_ERR,"Could not chown %s to %d/%d. Aborting!",userdir.data(),getuid(),getgid());
            return (EXIT_FAILURE);
        }
        stat(userdir.data(),&bStat);        // Stat again
    }

    // Check ownership and permissions
    if(bStat.st_uid!=getuid()){                 // check ownership
        syslog(LOG_ERR,"%s not owned by %s. This directory will be ignored!",userdir.data(),user);
        return (EXIT_FAILURE);
    }
    if((bStat.st_mode&MODE_MASK)!=USER_DIR_MODE){                // check permissions
        syslog(LOG_ERR,"%s has not mode %o. This directory will be ignored!",userdir.data(),USER_DIR_MODE);
        return (EXIT_FAILURE);
    }
    syslog(LOG_DEBUG,"%s OK.",userdir.data());
    return (EXIT_SUCCESS);
}

