// $Id: $
#include "share.h"
#include "icus221_drv.h"
#include "dummy_master_drv.h"
#include "dummy_peripheral_drv.h"

extern void pass(void);
extern void fail(void);
extern void delay(unsigned int time);

main()
{
    DPeripheral_SetCLKfreq(PCLK, 1000000);
    delay(50);

    delay(200);
    pass();
}

void pass(void){
    DPeripheral_Pass();
    while(1);
}

void fail(void){
    DPeripheral_Fail();
    while(1);
}

