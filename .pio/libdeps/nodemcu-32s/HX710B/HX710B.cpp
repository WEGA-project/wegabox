#include "HX710B.h"

HX710B::HX710B(uint8_t sck_pin , uint8_t do_pin, uint8_t mode)
{
    this->SCK_pin = sck_pin;
    this->DO_pin = do_pin;
    this->mode = mode;
}

HX710B::~HX710B(){}

void HX710B::halt_(unsigned long time_)
{
    if( this->delay_ == NULL )
        delay(time_);
        
    else
        this->delay_(time_);
}

unsigned long HX710B::getTick(void)
{
    if( this->tick != NULL )
        return this->tick();
    else    
        return millis();
}

void HX710B::attachDelay(void (*delay__)(unsigned long ti))
{
    this->delay_ = delay__;
}

void HX710B::attachTick(unsigned long (*tick_)(void))
{
    this->tick = tick_;
}

uint8_t HX710B::init(void)
{

    if (this->SCK_pin == NO_PIN_DEFINED ||
        this->DO_pin == NO_PIN_DEFINED )
        return HX710B_ERROR;
    pinMode(this->DO_pin, INPUT_PULLUP);    digitalWrite(this->DO_pin, HIGH);
    pinMode(this->SCK_pin, OUTPUT);         digitalWrite(this->SCK_pin, HIGH);
    for (uint8_t t_ = 0; t_ < this->mode; t_++)
    {
        NOP NOP
        digitalWrite(this->SCK_pin, LOW);
        NOP NOP
        digitalWrite(this->SCK_pin, HIGH);
    }
    
    read(NULL, 500UL);
    this->halt_(500);
    uint8_t ret = read(NULL, 100);
    return ret;
}

uint8_t HX710B::setMode(uint8_t mode_, unsigned long timeout_)
{
    uint32_t val;
    return read(&val, timeout_);
}

uint8_t HX710B::read(uint32_t *data, unsigned long timeout_)
{
    // exit sleep mode
    digitalWrite(this->SCK_pin, LOW);
    if (this->isReady(timeout_) == HX710B_OK)
    {
        uint8_t i = 0;
        uint32_t raw = 0;
        for (i = 0; i < this->mode; i++)
        {
            NOP NOP
            digitalWrite(this->SCK_pin, HIGH);
            NOP NOP
            digitalWrite(this->SCK_pin, LOW);
            if (i < 24)
            {
                raw = raw << 1;
                if (digitalRead(this->DO_pin))
                    raw = raw | 0x01;
            }
        }
        // force the HX710B to enter the sleep mode
        digitalWrite(this->SCK_pin, HIGH);
        if( data != NULL)
            *data = raw ^ 0x800000;
        return HX710B_OK;
    }
    return HX710B_TIMEOUT;
}

uint8_t HX710B::isReady(unsigned long timeout_)
{
    unsigned long time_s = this->getTick();
    while (digitalRead(this->DO_pin)!= LOW)
    {
        // preventive if counter is roll-over and start from beginning
        unsigned long rollOver = this->getTick();
        if (rollOver < time_s)
            time_s = rollOver;
        if (time_s - this->getTick() > timeout_)
            return HX710B_TIMEOUT;
        // need delay;
        this->halt_(5);
    }
    return HX710B_OK;
}