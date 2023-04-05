#ifndef COMMON_H
#define	COMMON_H

#ifdef __cplusplus
extern "C" {
#endif

#include "wiiu_common.h"

#define HBL_VERSION                 "v3.0"
#define HBL_VERSION_INT             300

#define CAFE_OS_SD_PATH             "/vol/external01"
#define SD_PATH                     "fs:"
#define WIIU_PATH                   "/wiiu"

#ifndef MEM_BASE
#define MEM_BASE                    (0x00800000)
#endif

#define HBL_TEMP_RPX_PATH           "fs:/vol/external01/wiiu/apps/homebrew_launcher/temp"
#define HBL_TEMP_RPX_FILE           "fs:/vol/external01/wiiu/apps/homebrew_launcher/temp/temp.rpx"

#define ELF_DATA_ADDR               (*(volatile unsigned int*)(MEM_BASE + 0x1300 + 0x00))
#define ELF_DATA_SIZE               (*(volatile unsigned int*)(MEM_BASE + 0x1300 + 0x04))
#define HBL_CHANNEL_OFFSET          (0x1300 + 0x08)
#define HBL_CHANNEL                 (*(volatile unsigned int*)(MEM_BASE + HBL_CHANNEL_OFFSET))
#define RPX_MAX_SIZE                (*(volatile unsigned int*)(MEM_BASE + 0x1300 + 0x0C))
#define RPX_MAX_CODE_SIZE           (*(volatile unsigned int*)(MEM_BASE + 0x1300 + 0x10))
#define MAIN_ENTRY_ADDR             (*(volatile unsigned int*)(MEM_BASE + 0x1400 + 0x00))
#define OS_FIRMWARE_OFFSET          (0x1400 + 0x04)
#define OS_FIRMWARE                 (*(volatile unsigned int*)(MEM_BASE + OS_FIRMWARE_OFFSET))

#define OS_SPECIFICS                ((OsSpecifics*)(MEM_BASE + 0x1500))

#define MEM_AREA_TABLE              ((s_mem_area*)(MEM_BASE + 0x1600))
#define APP_BASE_MEM                ((unsigned char*)(MEM_BASE + 0x2000))

#ifndef EXIT_SUCCESS
#define EXIT_SUCCESS                0
#endif
#define EXIT_RELAUNCH_ON_LOAD       0xFFFFFFFD

#ifdef __cplusplus
}
#endif

#endif	/* COMMON_H */

