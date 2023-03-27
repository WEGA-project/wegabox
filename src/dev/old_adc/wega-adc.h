// #include "esp32-hal-adc.h"
// #include "freertos/FreeRTOS.h"
// #include "freertos/task.h"
// #include "rom/ets_sys.h"
// #include "esp_attr.h"
// #include "esp_intr.h"
// #include "soc/rtc_io_reg.h"
// #include "soc/rtc_cntl_reg.h"
#include "soc/sens_reg.h"
#include "soc/soc.h"
#include "soc/rtc_io_reg.h"


 
// #include "dev/ec/old_adc/wega-sens-reg.h"
// #include "dev/ec/old_adc/wega-rtc-io-reg.h"


#define GET_PERI_REG_MASK(reg, mask) ({                                                                                \
            ASSERT_IF_DPORT_REG((reg), GET_PERI_REG_MASK);                                                             \
            (READ_PERI_REG(reg) & (mask));                                                                             \
        })

static uint8_t __wega_analogAttenuation = 3;//11db
static uint8_t __wega_analogWidth = 3;//12 bits
static uint8_t __wega_analogCycles = 8;
static uint8_t __wega_analogSamples = 0;//1 sample
static uint8_t __wega_analogClockDiv = 1;

// Width of returned answer ()
static uint8_t __wega_analogReturnedWidth = 12;

void __wega_analogSetWidth(uint8_t bits){
    if(bits < 9){
        bits = 9;
    } else if(bits > 12){
        bits = 12;
    }
    __wega_analogReturnedWidth = bits;
    __wega_analogWidth = bits - 9;
    SET_PERI_REG_BITS(SENS_SAR_START_FORCE_REG, SENS_SAR1_BIT_WIDTH, __wega_analogWidth, SENS_SAR1_BIT_WIDTH_S);
    SET_PERI_REG_BITS(SENS_SAR_READ_CTRL_REG, SENS_SAR1_SAMPLE_BIT, __wega_analogWidth, SENS_SAR1_SAMPLE_BIT_S);

    SET_PERI_REG_BITS(SENS_SAR_START_FORCE_REG, SENS_SAR2_BIT_WIDTH, __wega_analogWidth, SENS_SAR2_BIT_WIDTH_S);
    SET_PERI_REG_BITS(SENS_SAR_READ_CTRL2_REG, SENS_SAR2_SAMPLE_BIT, __wega_analogWidth, SENS_SAR2_SAMPLE_BIT_S);
}

void __wega_analogSetCycles(uint8_t cycles){
    __wega_analogCycles = cycles;
    SET_PERI_REG_BITS(SENS_SAR_READ_CTRL_REG, SENS_SAR1_SAMPLE_CYCLE, __wega_analogCycles, SENS_SAR1_SAMPLE_CYCLE_S);
    SET_PERI_REG_BITS(SENS_SAR_READ_CTRL2_REG, SENS_SAR2_SAMPLE_CYCLE, __wega_analogCycles, SENS_SAR2_SAMPLE_CYCLE_S);
}

void __wega_analogSetSamples(uint8_t samples){
    if(!samples){
        return;
    }
    __wega_analogSamples = samples - 1;
    SET_PERI_REG_BITS(SENS_SAR_READ_CTRL_REG, SENS_SAR1_SAMPLE_NUM, __wega_analogSamples, SENS_SAR1_SAMPLE_NUM_S);
    SET_PERI_REG_BITS(SENS_SAR_READ_CTRL2_REG, SENS_SAR2_SAMPLE_NUM, __wega_analogSamples, SENS_SAR2_SAMPLE_NUM_S);
}

void __wega_analogSetClockDiv(uint8_t clockDiv){
    if(!clockDiv){
        return;
    }
    __wega_analogClockDiv = clockDiv;
    SET_PERI_REG_BITS(SENS_SAR_READ_CTRL_REG, SENS_SAR1_CLK_DIV, __wega_analogClockDiv, SENS_SAR1_CLK_DIV_S);
    SET_PERI_REG_BITS(SENS_SAR_READ_CTRL2_REG, SENS_SAR2_CLK_DIV, __wega_analogClockDiv, SENS_SAR2_CLK_DIV_S);
}

void __wega_analogSetAttenuation(uint8_t attenuation)
{
    __wega_analogAttenuation = attenuation & 3;
    uint32_t att_data = 0;
    int i = 10;
    while(i--){
        att_data |= __wega_analogAttenuation << (i * 2);
    }
    WRITE_PERI_REG(SENS_SAR_ATTEN1_REG, att_data & 0xFFFF);//ADC1 has 8 channels
    WRITE_PERI_REG(SENS_SAR_ATTEN2_REG, att_data);
}

void IRAM_ATTR __wega_analogInit(){
    static bool initialized = false;
    if(initialized){
        return;
    }

    __wega_analogSetAttenuation(__wega_analogAttenuation);
    __wega_analogSetCycles(__wega_analogCycles);
    __wega_analogSetSamples(__wega_analogSamples + 1);//in samples
    __wega_analogSetClockDiv(__wega_analogClockDiv);
    __wega_analogSetWidth(__wega_analogWidth + 9);//in bits

    SET_PERI_REG_MASK(SENS_SAR_READ_CTRL_REG, SENS_SAR1_DATA_INV);
    SET_PERI_REG_MASK(SENS_SAR_READ_CTRL2_REG, SENS_SAR2_DATA_INV);

    SET_PERI_REG_MASK(SENS_SAR_MEAS_START1_REG, SENS_MEAS1_START_FORCE_M); //SAR ADC1 controller (in RTC) is started by SW
    SET_PERI_REG_MASK(SENS_SAR_MEAS_START1_REG, SENS_SAR1_EN_PAD_FORCE_M); //SAR ADC1 pad enable bitmap is controlled by SW
    SET_PERI_REG_MASK(SENS_SAR_MEAS_START2_REG, SENS_MEAS2_START_FORCE_M); //SAR ADC2 controller (in RTC) is started by SW
    SET_PERI_REG_MASK(SENS_SAR_MEAS_START2_REG, SENS_SAR2_EN_PAD_FORCE_M); //SAR ADC2 pad enable bitmap is controlled by SW

    CLEAR_PERI_REG_MASK(SENS_SAR_MEAS_WAIT2_REG, SENS_FORCE_XPD_SAR_M); //force XPD_SAR=0, use XPD_FSM
    SET_PERI_REG_BITS(SENS_SAR_MEAS_WAIT2_REG, SENS_FORCE_XPD_AMP, 0x2, SENS_FORCE_XPD_AMP_S); //force XPD_AMP=0

    CLEAR_PERI_REG_MASK(SENS_SAR_MEAS_CTRL_REG, 0xfff << SENS_AMP_RST_FB_FSM_S);  //clear FSM
    SET_PERI_REG_BITS(SENS_SAR_MEAS_WAIT1_REG, SENS_SAR_AMP_WAIT1, 0x1, SENS_SAR_AMP_WAIT1_S);
    SET_PERI_REG_BITS(SENS_SAR_MEAS_WAIT1_REG, SENS_SAR_AMP_WAIT2, 0x1, SENS_SAR_AMP_WAIT2_S);
    SET_PERI_REG_BITS(SENS_SAR_MEAS_WAIT2_REG, SENS_SAR_AMP_WAIT3, 0x1, SENS_SAR_AMP_WAIT3_S);
    while (GET_PERI_REG_BITS2(SENS_SAR_SLAVE_ADDR1_REG, 0x7, SENS_MEAS_STATUS_S) != 0); //wait det_fsm==

    initialized = true;
}

void __wega_analogSetPinAttenuation(uint8_t pin, adc_attenuation_t attenuation)
{
    int8_t channel = digitalPinToAnalogChannel(pin);
    if(channel < 0 || attenuation > 3){
        return ;
    }
    __wega_analogInit();
    if(channel > 7){
        SET_PERI_REG_BITS(SENS_SAR_ATTEN2_REG, 3, attenuation, ((channel - 10) * 2));
    } else {
        SET_PERI_REG_BITS(SENS_SAR_ATTEN1_REG, 3, attenuation, (channel * 2));
    }
}

bool IRAM_ATTR __wega_adcAttachPin(uint8_t pin){

    int8_t channel = digitalPinToAnalogChannel(pin);
    if(channel < 0){
        return false;//not adc pin
    }

    int8_t pad = digitalPinToTouchChannel(pin);
    if(pad >= 0){
        uint32_t touch = READ_PERI_REG(SENS_SAR_TOUCH_ENABLE_REG);
        if(touch & (1 << pad)){
            touch &= ~((1 << (pad + SENS_TOUCH_PAD_OUTEN2_S))
                    | (1 << (pad + SENS_TOUCH_PAD_OUTEN1_S))
                    | (1 << (pad + SENS_TOUCH_PAD_WORKEN_S)));
            WRITE_PERI_REG(SENS_SAR_TOUCH_ENABLE_REG, touch);
        }
    } else if(pin == 25){
        CLEAR_PERI_REG_MASK(RTC_IO_PAD_DAC1_REG, RTC_IO_PDAC1_XPD_DAC | RTC_IO_PDAC1_DAC_XPD_FORCE);//stop dac1
    } else if(pin == 26){
        CLEAR_PERI_REG_MASK(RTC_IO_PAD_DAC2_REG, RTC_IO_PDAC2_XPD_DAC | RTC_IO_PDAC2_DAC_XPD_FORCE);//stop dac2
    }

    pinMode(pin, ANALOG);

    __wega_analogInit();
    return true;
}

bool IRAM_ATTR __wega_adcStart(uint8_t pin){

    int8_t channel = digitalPinToAnalogChannel(pin);
    if(channel < 0){
        return false;//not adc pin
    }

    if(channel > 9){
        channel -= 10;
        CLEAR_PERI_REG_MASK(SENS_SAR_MEAS_START2_REG, SENS_MEAS2_START_SAR_M);
        SET_PERI_REG_BITS(SENS_SAR_MEAS_START2_REG, SENS_SAR2_EN_PAD, (1 << channel), SENS_SAR2_EN_PAD_S);
        SET_PERI_REG_MASK(SENS_SAR_MEAS_START2_REG, SENS_MEAS2_START_SAR_M);
    } else {
        CLEAR_PERI_REG_MASK(SENS_SAR_MEAS_START1_REG, SENS_MEAS1_START_SAR_M);
        SET_PERI_REG_BITS(SENS_SAR_MEAS_START1_REG, SENS_SAR1_EN_PAD, (1 << channel), SENS_SAR1_EN_PAD_S);
        SET_PERI_REG_MASK(SENS_SAR_MEAS_START1_REG, SENS_MEAS1_START_SAR_M);
    }
    return true;
}

bool IRAM_ATTR __wega_adcBusy(uint8_t pin){

    int8_t channel = digitalPinToAnalogChannel(pin);
    if(channel < 0){
        return false;//not adc pin
    }

    if(channel > 7){
        return (GET_PERI_REG_MASK(SENS_SAR_MEAS_START2_REG, SENS_MEAS2_DONE_SAR) == 0);
    }
    return (GET_PERI_REG_MASK(SENS_SAR_MEAS_START1_REG, SENS_MEAS1_DONE_SAR) == 0);
}

uint16_t IRAM_ATTR __wega_adcEnd(uint8_t pin)
{

    uint16_t value = 0;
    int8_t channel = digitalPinToAnalogChannel(pin);
    if(channel < 0){
        // Serial.print(0);
        return 0;//not adc pin
    }
    if(channel > 7){
        while (GET_PERI_REG_MASK(SENS_SAR_MEAS_START2_REG, SENS_MEAS2_DONE_SAR) == 0); //wait for conversion
        value = GET_PERI_REG_BITS2(SENS_SAR_MEAS_START2_REG, SENS_MEAS2_DATA_SAR, SENS_MEAS2_DATA_SAR_S);
    } else {
        while (GET_PERI_REG_MASK(SENS_SAR_MEAS_START1_REG, SENS_MEAS1_DONE_SAR) == 0); //wait for conversion
        value = GET_PERI_REG_BITS2(SENS_SAR_MEAS_START1_REG, SENS_MEAS1_DATA_SAR, SENS_MEAS1_DATA_SAR_S);
    }

    // Serial.print(value);

    // Shift result if necessary
    uint8_t from = __wega_analogWidth + 9;
    if (from == __wega_analogReturnedWidth) {
        return value;
    }
    if (from > __wega_analogReturnedWidth) {
        return value >> (from - __wega_analogReturnedWidth);
    }
    return value << (__wega_analogReturnedWidth - from);
}

uint16_t IRAM_ATTR __wega_analogRead(uint8_t pin)
{
    if(!__wega_adcAttachPin(pin) || !__wega_adcStart(pin)){
        return 0;
    }
    return __wega_adcEnd(pin);
}

void __wega_analogReadResolution(uint8_t bits)
{
    if(!bits || bits > 16){
        return;
    }
    __wega_analogSetWidth(bits);         // hadware from 9 to 12
    __wega_analogReturnedWidth = bits;   // software from 1 to 16
}

int __wega_hallRead()    //hall sensor without LNA
{
    int Sens_Vp0;
    int Sens_Vn0;
    int Sens_Vp1;
    int Sens_Vn1;

    pinMode(36, ANALOG);
    pinMode(39, ANALOG);
    SET_PERI_REG_MASK(SENS_SAR_TOUCH_CTRL1_REG, SENS_XPD_HALL_FORCE_M);     // hall sens force enable
    SET_PERI_REG_MASK(RTC_IO_HALL_SENS_REG, RTC_IO_XPD_HALL);               // xpd hall
    SET_PERI_REG_MASK(SENS_SAR_TOUCH_CTRL1_REG, SENS_HALL_PHASE_FORCE_M);   // phase force
    CLEAR_PERI_REG_MASK(RTC_IO_HALL_SENS_REG, RTC_IO_HALL_PHASE);           // hall phase
    Sens_Vp0 = __wega_analogRead(36);
    Sens_Vn0 = __wega_analogRead(39);
    SET_PERI_REG_MASK(RTC_IO_HALL_SENS_REG, RTC_IO_HALL_PHASE);
    Sens_Vp1 = __wega_analogRead(36);
    Sens_Vn1 = __wega_analogRead(39);
    SET_PERI_REG_BITS(SENS_SAR_MEAS_WAIT2_REG, SENS_FORCE_XPD_SAR, 0, SENS_FORCE_XPD_SAR_S);
    CLEAR_PERI_REG_MASK(SENS_SAR_TOUCH_CTRL1_REG, SENS_XPD_HALL_FORCE);
    CLEAR_PERI_REG_MASK(SENS_SAR_TOUCH_CTRL1_REG, SENS_HALL_PHASE_FORCE);
    return (Sens_Vp1 - Sens_Vp0) - (Sens_Vn1 - Sens_Vn0);
}