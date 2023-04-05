#ifndef COMMON_H
#define	COMMON_H

#include "wiiu_common.h"

#define HBL_VERSION                 "v3.0"
#define HBL_VERSION_INT             300

#ifdef PC
#define APPS_ROOT             "./sdroot/wiiu/apps"
#else
#define APPS_ROOT             "fs:/vol/external01/wiiu/apps/"
#endif

#define EXIT_RELAUNCH_ON_LOAD       0xFFFFFFFD

#endif	/* COMMON_H */

