

#include "rh_app.hh"

namespace rh{



/******************************************************************************/
/* [239023] This class handle program shared resource                         */
/* @category:    Resource                                                     */
/* @uniqueID:    239023                                                       */
/******************************************************************************/
AppResource::AppResource( void):
userTick(0),UserTickOverflow(false),time{.bit={.year=1970,.month=1,.date=1,.hour=0,.minute=0,.second=0}},key0(false) {
    const char *hhmmss = __TIME__;
    if( hhmmss!=nullptr && strlen(hhmmss)==8){
        time.bit.hour   = (hhmmss[0]-'0')*10+(hhmmss[1]-'0');
        time.bit.minute = (hhmmss[3]-'0')*10+(hhmmss[4]-'0');
        time.bit.second = (hhmmss[6]-'0')*10+(hhmmss[7]-'0');
    }

    const char *mmmddyyyy = __DATE__;
    if( mmmddyyyy!=nullptr && strlen(mmmddyyyy)==11 ){
        switch( mmmddyyyy[0]+mmmddyyyy[1]+mmmddyyyy[2] ){
            case 281: time.bit.month = 1; break;
            case 269: time.bit.month = 2; break;
            case 288: time.bit.month = 3; break;
            case 291: time.bit.month = 4; break;
            case 295: time.bit.month = 5; break;
            case 301: time.bit.month = 6; break;
            case 299: time.bit.month = 7; break;
            case 285: time.bit.month = 8; break;
            case 296: time.bit.month = 9; break;
            case 294: time.bit.month = 10; break;
            case 307: time.bit.month = 11; break;
            case 268: time.bit.month = 12; break;
            default: break;
        }
        time.bit.date = (mmmddyyyy[4]-'0')*10+(mmmddyyyy[5]-'0');
        time.bit.year = (mmmddyyyy[7]-'0')*1000+(mmmddyyyy[8]-'0')*100+(mmmddyyyy[9]-'0')*10+(mmmddyyyy[10]-'0');
    }
    
}





}