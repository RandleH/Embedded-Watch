

#include <stdarg.h>
#include "rh_common.h"
#include "rh_config.h"

#ifndef RH_DEBUG_H
#define RH_DEBUG_H



#define CFG_DEBUG__ENABLE       1

#define CFG_DEBUG__LOG          1
#define CFG_DEBUG__WARNING      0
#define CFG_DEBUG__ERROR        0
#define CFG_DEBUG__BAUDRATE     (115200U)
 
#ifdef __cplusplus
extern "C"{
#endif

int rh_debug__init  ( void);
int rh_debug__printf( const char * fmt, ...);


#ifdef __cplusplus
}
#endif





#define rh_debug__wprintf( fmt, ...)        do{\
                                                rh_debug__printf(RH_CFG_DEBUG_TITLE_WARNING " @%s ln%d:\t", __FILE__, __LINE__);\
                                                rh_debug__printf(fmt, ## __VA_ARGS__);}while(0)

#define rh_debug__eprintf( fmt, ...)        do{\
                                                rh_debug__printf(RH_CFG_DEBUG_TITLE_ERROR " @%s ln%d:\t", __FILE__, __LINE__);\
                                                rh_debug__printf(fmt, ## __VA_ARGS__);}while(0)

#define rh_debug__log( fmt, ...)            do{\
                                                rh_debug__printf(RH_CFG_DEBUG_TITLE_LOG " @%s ln%d:\t", __FILE__, __LINE__);\
                                                rh_debug__printf(fmt, ## __VA_ARGS__);}while(0)



#endif