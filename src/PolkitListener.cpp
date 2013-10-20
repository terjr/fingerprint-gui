/*
 * Project "Fingerprint GUI": Services for fingerprint authentication on Linux
 * Module: PolkitListener.cpp, PolkitListener.h
 * Purpose: Listens for polkit events
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

/*
 * This code is based on the analysis of the "polkit-kde-1-0.95.1" project
 */

#include <polkit-qt-1/PolkitQt1/Agent/Listener>
#include <polkit-qt-1/PolkitQt1/Agent/Session>
#include <polkit-qt-1/PolkitQt1/Subject>
#include <polkit-qt-1/PolkitQt1/Identity>
#include <polkit-qt-1/PolkitQt1/Details>
#include <qt4/QtGui/qdialogbuttonbox.h>

#include "../include/Globals.h"
#include "../include/PolkitListener.h"

PolkitListener::PolkitListener(QObject *parent) : Listener(parent),inProgress(false),selectedUser(0){
}

PolkitListener::~PolkitListener(){
}

// initiateAuthentication message from polkit
#ifdef LIBPOLKIT_QT_1_1
void PolkitListener::initiateAuthentication(
        const QString &actionId,
        const QString &message,
        const QString &iconName,
        const PolkitQt1::Details &details,
        const QString &cookie,
        const PolkitQt1::Identity::List &identities,
        PolkitQt1::Agent::AsyncResult* result){
#else
void PolkitListener::initiateAuthentication(
        const QString &actionId,
        const QString &message,
        const QString &iconName,
        PolkitQt1::Details *details,
        const QString &cookie,
        QList<PolkitQt1::Identity *> identities,
        PolkitQt1::Agent::AsyncResult* result){
#endif // LIBPOLKIT_QT
    syslog(LOG_DEBUG,"Initiating authentication");
    if(inProgress){
        syslog(LOG_ERR,"Another client is already authenticating, please try again later.");
        result->setError("Another client is already authenticating, please try again later.");
        result->setCompleted();
        return;
    }
    this->identities=identities;
    this->cookie=cookie;
    this->result=result;
#ifdef LIBPOLKIT_QT_1_1
    session.clear();
#else
    session=0;
#endif // LIBPOLKIT_QT
    if(identities.length()==1){
        this->selectedUser=identities[0];
    }
    else{
#ifdef LIBPOLKIT_QT_1_1
        selectedUser=PolkitQt1::Identity();
#else
        selectedUser=0;
#endif // LIBPOLKIT_QT
    }

    inProgress=true;
    // Create the polkit dialog
    dialog=new PolkitDialogImpl(actionId,message,iconName,details,identities);
    connect(dialog->okButton,SIGNAL(clicked()),this,SLOT(dialogAccepted()));
    connect(dialog->passwordEdit,SIGNAL(returnPressed()),this,SLOT(dialogAccepted()));
    connect(dialog->cancelButton,SIGNAL(clicked()),this,SLOT(dialogCanceled()));
    connect(dialog,SIGNAL(rejected()),this,SLOT(dialogCanceled()));
#ifdef LIBPOLKIT_QT_1_1
    connect(dialog,SIGNAL(adminUserSelected(PolkitQt1::Identity)),SLOT(userSelected(PolkitQt1::Identity)));
#else
    connect(dialog,SIGNAL(adminUserSelected(PolkitQt1::Identity *)),SLOT(userSelected(PolkitQt1::Identity *)));
#endif // LIBPOLKIT_QT

    dialog->setOptions();
    dialog->show();
    dialog->raise();
    numTries=0;
    wasCancelled=false;
    if(!dialog->preselectUser()){
        tryAgain();
    }
}

void PolkitListener::finishObtainPrivilege(){
    // Number of tries increase only when some user is selected
#ifdef LIBPOLKIT_QT_1_1
    if(selectedUser.isValid()){
#else
    if(selectedUser!=0){
#endif // LIBPOLKIT_QT
        numTries++;
    }
    syslog(LOG_DEBUG,"Finishing obtaining privileges (G:%u, C:%u, D:%u).",gainedAuthorization,wasCancelled,(dialog!=NULL));
    if(!gainedAuthorization&&!wasCancelled&&(dialog!=NULL)){
        dialog->authenticationFailure();
        if(numTries<3){
            session.data()->deleteLater();
            tryAgain();
            return;
        }
    }
    if(!session.isNull()){
        session.data()->result()->setCompleted();
    }
    else{
        result->setCompleted();
    }
#ifdef LIBPOLKIT_QT_1_1
    session.data()->deleteLater();
#endif // LIBPOLKIT_QT
    if(dialog){
        delete(dialog);
        dialog=NULL;
    }
    inProgress=false;
    syslog(LOG_DEBUG,"Finish obtain authorization: %u",gainedAuthorization);
}

void PolkitListener::tryAgain(){
    syslog(LOG_DEBUG,"Trying again.");
    // We will create a new session only when some user is selected
#ifdef LIBPOLKIT_QT_1_1
    if(selectedUser.isValid()){
        session=new Session(selectedUser,cookie,result);
        connect(session.data(),SIGNAL(request(QString,bool)),this,SLOT(request(QString,bool)));
        connect(session.data(),SIGNAL(completed(bool)),this,SLOT(completed(bool)));
        connect(session.data(),SIGNAL(showError(QString)),this,SLOT(showError(QString)));
        session.data()->initiate();
#else
    if(selectedUser!=0){
        session=new Session(selectedUser,cookie,result);
        connect(session,SIGNAL(request(QString,bool)),this,SLOT(request(QString,bool)));
        connect(session,SIGNAL(completed(bool)),this,SLOT(completed(bool)));
        connect(session,SIGNAL(showError(QString)),this,SLOT(showError(QString)));
        session->initiate();
#endif // LIBPOLKIT_QT
    }
    if(dialog){
        dialog->adjustSize();
    }
}

bool PolkitListener::initiateAuthenticationFinish()
{
    syslog(LOG_DEBUG,"initiateAuthenticationFinish.");
    return true;
}

void PolkitListener::cancelAuthentication()
{
    wasCancelled=true;
    syslog(LOG_DEBUG,"cancelAuthentication.");
    finishObtainPrivilege();
}

void PolkitListener::request(const QString &request,bool echo){
    syslog(LOG_DEBUG,"Request \"%s\", echo: %u.",request.toStdString().data(),echo);
    if(dialog){
        dialog->setRequest(request,echo);
        connect(dialog->pluginDialog->fifoReader,SIGNAL(userAuthenticated()),SLOT(dialogAccepted()));
        connect(dialog->pluginDialog->fifoReader,SIGNAL(stopPlugin()),dialog,SLOT(resize()));
    }
}

void PolkitListener::completed(bool gainedAuthorization){
    syslog(LOG_DEBUG,"Completed: %u.",gainedAuthorization);
    this->gainedAuthorization=gainedAuthorization;
    finishObtainPrivilege();
}

void PolkitListener::showError(const QString &text){
    syslog(LOG_DEBUG,"Error: \"%s\".",text.toStdString().data());
    if(dialog){
        dialog->adjustSize();
    }
}

void PolkitListener::dialogAccepted(){
    syslog(LOG_DEBUG,"dialogAccepted.");
    if(!session.isNull()){
#ifdef LIBPOLKIT_QT_1_1
        session.data()->setResponse(dialog->password());
#else
        session->setResponse(dialog->password());
#endif // LIBPOLKIT_QT
    }
//!!!!!!!!!!!!!!!! USE THIS LOG OUTPUT ONLY FOR DEVELOPMENT !!!!!!!!!!!!!!!!!!!!
//syslog(LOG_DEBUG,"Got password \"%s\".",dialog->password().toStdString().data());
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
}

void PolkitListener::dialogCanceled(){
    syslog(LOG_DEBUG,"dialogCanceled.");
    wasCancelled=true;
    if(!session.isNull()){
#ifdef LIBPOLKIT_QT_1_1
        session.data()->cancel();
#else
        session->cancel();
#endif // LIBPOLKIT_QT
    }
    finishObtainPrivilege();
}

#ifdef LIBPOLKIT_QT_1_1
void PolkitListener::userSelected(PolkitQt1::Identity identity){
    syslog(LOG_DEBUG,"userSelected: %s.",identity.toString().remove("unix-user:").toStdString().data());
#else
void PolkitListener::userSelected(PolkitQt1::Identity *identity){
    syslog(LOG_DEBUG,"userSelected: %s.",identity->toString().remove("unix-user:").toStdString().data());
#endif // LIBPOLKIT_QT
    selectedUser=identity;
    // If some user is selected we must destroy existing session
    if(!session.isNull()){
        session.data()->deleteLater();
    }
    tryAgain();
}
