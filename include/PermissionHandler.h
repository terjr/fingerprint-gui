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

#ifndef _PERMISSIONHANDLER_H
#define	_PERMISSIONHANDLER_H

#include <stdio.h>
#include <string>

#include "Globals.h"

using namespace std;

class PermissionHandler {
public:
    bool setPermissions(string *path,string *user);
    bool checkPermissions(string *path,string *user);
};

#endif	/* _PERMISSIONHANDLER_H */

