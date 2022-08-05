#ifndef _HX710B_H_
#define _HX710B_H_

#include "Arduino.h"
#include "stdlib.h"
#include "stdio.h"

#define NOP             __asm__("nop\n\t");
#define halt_1micros    NOP NOP NOP NOP NOP NOP NOP NOP NOP NOP NOP NOP NOP NOP NOP NOP

#define HX710B_DIFF1    25      // data rate 10Hz
#define HX710B_TEMP     26      // data rate 40Hz
#define HX710B_DIFF2    27      // data rate 40Hz

#define HX710B_DEFAULT_MODE     HX710B_DIFF2


#define NO_PIN_DEFINED      255
/* Status Reply */

#define HX710B_OK       0x00U
#define HX710B_ERROR    0x01U
#define HX710B_BUSY     0x02U
#define HX710B_TIMEOUT  0x03U

class HX710B{

    private :
        uint8_t SCK_pin = NO_PIN_DEFINED,
                DO_pin  = NO_PIN_DEFINED;
        uint8_t mode;
        void (*delay_)(unsigned long time_) = NULL;
        unsigned long (*tick)(void) = NULL;
        
        void halt_ ( unsigned long time_);
        unsigned long getTick(void);

    public :
        
        HX710B(uint8_t sck_pin = NO_PIN_DEFINED, uint8_t do_pin = NO_PIN_DEFINED, uint8_t mode = HX710B_DEFAULT_MODE);
        ~HX710B();
        uint8_t init(void);
        uint8_t setMode(uint8_t mode_, unsigned long timeout_ = 500UL);
        uint8_t read(uint32_t *data, unsigned long timeout_ = 500UL);
        uint8_t isReady(unsigned long timeout_ = 500UL);

        void attachDelay(void (*delay__)(unsigned long ti));
        void attachTick(unsigned long (*tick_)(void));
};

#endif