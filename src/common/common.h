#ifndef COMMON_H
#define	COMMON_H

#include "wiiu_common.h"

#define HBL_VERSION                 "v3.0"
#define HBL_VERSION_INT             300

#define CAFE_OS_SD_PATH             "/vol/external01"
#define SD_PATH                     "fs:"
#define WIIU_PATH                   "/wiiu"

#define HBL_TEMP_RPX_PATH           "fs:/vol/external01/wiiu/apps/homebrew_launcher/temp"
#define HBL_TEMP_RPX_FILE           "fs:/vol/external01/wiiu/apps/homebrew_launcher/temp/temp.rpx"

#ifndef EXIT_SUCCESS
#define EXIT_SUCCESS                0
#endif

#define EXIT_RELAUNCH_ON_LOAD       0xFFFFFFFD

#endif	/* COMMON_H */

