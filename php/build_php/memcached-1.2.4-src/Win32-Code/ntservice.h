/* ntservice.h
 *
 *  Copyright (c) 2006 Germán Méndez Bravo (Kronuz) <kronuz@users.sf.net>
 *  All rights reserved.
 *
 */

#ifndef SERVICE_H
#define SERVICE_H

typedef void (*svcFunc) ();

int ServiceStart();
int ServiceStop();
int ServiceRestart();
int ServiceUninstall();
int ServiceInstall();
int ServiceRun();

void ServiceSetFunc(svcFunc runFunc, svcFunc pauseFunc, svcFunc continueFunc, svcFunc stopFunc);

#endif
