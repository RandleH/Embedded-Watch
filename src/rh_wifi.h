


#ifndef RH_WIFI_H
#define RH_WIFI_H

#include <stdint.h>
#include "rh_common.h"
#include "rh_config.h"




#ifdef __cplusplus
extern "C"{
#endif





int rh_wifi__init( void);//
int rh_wifi__send( const u8 *data, size_t len);//



#ifdef __cplusplus
}
#endif


#endif




