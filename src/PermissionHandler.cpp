/*
 * Project "Fingerprint GUI": Services for fingerprint authentication on Linux
 * Module: PermissionHandler.cpp, PermissionHandler.h
 * Purpose: Set and check file- and directory permissions for config files and bir data.
 *          Directories and files must be owned by the given user.
 *          Permission of directories must be 0700.
 *          Permission of files must be 0600.
 *          Files and directories violating these rules are ignored by all modules.
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

#include <string>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <pwd.h>

#include "PermissionHandler.h"

// Set owner of "path" to "user" and set correct mode
// If "user" is NULL, the current user is assumed
bool PermissionHandler::setPermissions(string *path,string *user){
    struct stat bStat;
    struct passwd *bPwd;
    mode_t mode;
    uid_t owner;
    gid_t group;

    if(stat(path->data(),&bStat)!=0)        //file doesn't exist or could not be stated
        return false;
    if(S_ISDIR(bStat.st_mode)){             // is a directory
        mode=USER_DIR_MODE;
    }
    else{
        mode=USER_FILE_MODE;
    }
    if(user==NULL){
        bPwd=getpwuid(getuid());
    }
    else{
        bPwd=getpwnam(user->data());
    }
    if(!bPwd){
        syslog(LOG_ERR,"Could not get password entry for %s.",user->data());
        return false;
    }
    owner=bPwd->pw_uid;
    group=bPwd->pw_gid;
    if(chown(path->data(),owner,group)){
        syslog(LOG_ERR,"Could not set owner/group of %s to %d/%d.",path->data(),owner,group);
        return false;
    }
    if(chmod(path->data(),mode)){
        syslog(LOG_ERR,"Could not set mode of %s to %o.",path->data(),mode);
        return false;
    }
    return true;
}

// Check "path" for being owned by "user" and having correct mode
bool PermissionHandler::checkPermissions(string *path,string *user){
    struct stat bStat;
    struct passwd *bPwd;
    mode_t mode;

    if(stat(path->data(),&bStat)!=0)        //file doesn't exist or could not be stated
        return false;
    if(S_ISDIR(bStat.st_mode)){             // is a directory
        mode=USER_DIR_MODE;
    }
    else{
        mode=USER_FILE_MODE;
    }

    bPwd=getpwuid(bStat.st_uid);
    if(bPwd==NULL){
        syslog(LOG_ERR,"Could not get owner of %s -- ignoring.",path->data());
        return false;
    }
    if(strcmp(bPwd->pw_name,user->data())){ // file is not owned by this user
        syslog(LOG_ERR,"User %s is not owner of %s -- ignoring.",user->data(),path->data());
        return false;
    }
    syslog(LOG_DEBUG,"File %s is owned by %s(%d).",path->data(),bPwd->pw_name,bStat.st_uid);
    if((bStat.st_mode&MODE_MASK)!=mode){ //Wrong file permissions
        syslog(LOG_ERR,"Mode of %s is \"%o\", must be \"%o\" -- ignoring.",path->data(),bStat.st_mode&MODE_MASK,mode);
        return false;
    }
    syslog(LOG_DEBUG,"%s has mode \"%o\" -- OK.",path->data(),bStat.st_mode&MODE_MASK);
    return true;
}
