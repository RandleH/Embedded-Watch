



#include <cstdint>
#include <cstddef>
#include "rh_common.h"

class Bone{
public:
    int print( void);
    
};


class Time : public Bone{

public:
    volatile u32 *pTickMs;
    u8 day;
    u8 hour;
    u8 minute;
    u8 second;
    
    int update ( void);
    int setTime( u8 dd, u8 hh, u8 mm, u8 ss);

};

class UI : public Bone{
public:


};

class Application{


public:

    Time time;
    UI   ui;
    

};





