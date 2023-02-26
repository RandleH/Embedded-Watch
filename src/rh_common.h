
#include <stdint.h>
#include <stdbool.h>

#ifndef RH_COMMON_H
#define RH_COMMON_H

typedef uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef volatile u8  vu8;
typedef volatile u16 vu16;
typedef volatile u32 vu32;


#define RH_API
#define RH_STATIC


#define RH_ANSI_DEFAULT               "" //"\033[0m"
#define RH_ANSI_COLOR_RED             "\033[0;31m"
#define RH_ANSI_COLOR_GREEN           "\033[0;32m"
#define RH_ANSI_COLOR_YELLOW          "\033[0;33m"
#define RH_ANSI_COLOR_BLUE            "\033[0;34m"

#define RH_ANSI_BOLD                  "\033[1m"
#define RH_ANSI_ULINE                 "\033[4m"


// Reference: https://cloud.tencent.com/developer/article/1800680


#endif
