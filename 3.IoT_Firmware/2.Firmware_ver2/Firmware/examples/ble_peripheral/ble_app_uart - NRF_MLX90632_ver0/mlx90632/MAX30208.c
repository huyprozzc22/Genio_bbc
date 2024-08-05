#include "MAX30208.h"

#include "nrf_drv_twi.h"
#include "twi_nrf52.h"
#include "twi_types.h"

#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"

extern const nrf_drv_twi_t m_twi;
sense_struct sense;
float temperature[16];

#define MAX30208_INT_A_FULL_MASK                    (~0x08)
#define MAX30208_INT_TEMP_LOW_MASK                  (~0x04)
#define MAX30208_INT_TEMP_HIGH_MASK                 (~0x02)
#define MAX30208_INT_TEMP_RDY_MASK                  (~0x01)

#define MAX30208_INT_A_FULL_ENABLE                  (0x80)
#define MAX30208_INT_TEMP_LOW_ENABLE                (0x04)
#define MAX30208_INT_TEMP_HIGH_ENABLE               (0x02)
#define MAX30208_INT_TEMP_RDY_ENABLE                (0x01)

#define MAX30208_INT_DISABLE                        (0x00)


#define GPIO0_MODE_SETUP_MASK                       (~0x03)
#define GPIO0_INPUT_MODE_EANBLE                     (0x00)
#define GPIO0_OUTPUT_MODE_EANBLE                    (0x01)
#define GPIO0_INPUT_PULLDOWM_EANBLE                 (0x02)
#define GPIO0_INT_MODE_EANBLE                       (0x03)

uint8_t max30208_init(){
    if(readPartID() != MAX30208_PART_IDENTIFIER_VAL)
    {
      NRF_LOG_INFO("max30208_init false");
      NRF_LOG_FLUSH();
      return 0;
    }
    getINT();

    clearFIFO();
    memset(&sense, 0, sizeof(sense));
    return 1;
}

// Configuration
void softReset(void)
{
    writeRegister8(MAX30208_SYSTEM_REG, 1);
    nrf_delay_ms(50);
}


void enableAFULL(void)
{
    bitMask(MAX30208_INTERRUPT_ENABLE, MAX30208_INT_A_FULL_MASK, MAX30208_INT_A_FULL_ENABLE);
}
void disableAFULL(void)
{
    bitMask(MAX30208_INTERRUPT_ENABLE, MAX30208_INT_A_FULL_MASK, MAX30208_INT_DISABLE);
}
uint8_t readPartID(void){
  return readRegister8(MAX30208_PART_IDENTIFIER);
}

void enableTEMPHIGH(void)
{
    bitMask(MAX30208_INTERRUPT_ENABLE, MAX30208_INT_TEMP_HIGH_MASK, MAX30208_INT_TEMP_HIGH_ENABLE);
}
void disableTEMPHIGH(void)
{
    bitMask(MAX30208_INTERRUPT_ENABLE, MAX30208_INT_TEMP_HIGH_MASK, MAX30208_INT_DISABLE);
}

void enableTEMPLOW(void)
{
    bitMask(MAX30208_INTERRUPT_ENABLE, MAX30208_INT_TEMP_LOW_MASK, MAX30208_INT_TEMP_LOW_ENABLE);
}

void disableTEMPLOW(void)
{
    bitMask(MAX30208_INTERRUPT_ENABLE, MAX30208_INT_TEMP_LOW_MASK, MAX30208_INT_DISABLE);
}
void enableDATARDY(void)
{
    bitMask(MAX30208_INTERRUPT_ENABLE, MAX30208_INT_TEMP_RDY_MASK, MAX30208_INT_TEMP_RDY_ENABLE);
}

void disableDATARDY(void)
{
    bitMask(MAX30208_INTERRUPT_ENABLE, MAX30208_INT_TEMP_RDY_MASK, MAX30208_INT_DISABLE);
}

void enableInterrup()
{
    bitMask(MAX30208_GPIO_SETUP_REG, GPIO0_MODE_SETUP_MASK, GPIO0_INT_MODE_EANBLE);
}

void configureFifo(uint8_t mask)
{
    bitMask(MAX30208_FIFO_CONFIGURATION2, mask, 1);
}

uint8_t fifoAvailable(void)
{
    if (readRegister8(MAX30208_FIFO_OVERFLOW_COUNTER) != 0) {
        return 32;
    }
    return readRegister8(MAX30208_FIFO_DATA_COUNTER);
}

uint8_t getINT(void)
{
    uint8_t val;
    val = readRegister8(MAX30208_INTERRUPT_STATUS);
    return val;
}

uint8_t readRegister8(uint8_t reg) {
    ret_code_t err_code;

    // Begin TWI transmission
    twi_master_write(MAX30208_DEFAULT_ADDRESS, &reg, sizeof(reg),true);

    // Request 1 byte from I2C device
    uint8_t data;
    twi_master_read(MAX30208_DEFAULT_ADDRESS, &data, sizeof(data));

    return data;
}

void writeRegister8(uint8_t reg, uint8_t value)
{
    uint8_t data[2] = {reg, value};
    twi_master_write(MAX30208_DEFAULT_ADDRESS,&data, sizeof(data),false);
    //twi_master_write(MAX30208_DEFAULT_ADDRESS, &reg, sizeof(reg),true);
    //twi_master_write(MAX30208_DEFAULT_ADDRESS,&value, sizeof(value),false);
}

void clearFIFO(void)
{
    // writeRegister8(MAX30208_FIFO_READ_POINTER, 0);
}
//Given a register, read it, mask it, and then set the thing
void bitMask(uint8_t reg, uint8_t mask, uint8_t thing)
{
    // Grab current register context
    uint8_t originalContents = readRegister8(reg);

    // Zero-out the portions of the register we're interested in
    originalContents = originalContents & mask;

    // Change contents
    writeRegister8( reg, originalContents | thing);
}

uint8_t updateFifo(uint8_t *data, uint8_t len)
{
    uint8_t reg = MAX30208_FIFO_DATA;
    twi_master_write(MAX30208_DEFAULT_ADDRESS, &reg, sizeof(reg), true);

    uint8_t i = 0;
    while (len--) {
        twi_master_read(MAX30208_DEFAULT_ADDRESS, &data[i++], sizeof(uint8_t));
    }

    return i;
}
void startConvert(void)
{
    writeRegister8(MAX30208_TEMP_SENSOR_SETUP, 0x01);
}

// Detecting ID/Revision
uint8_t getPartID(void)
{
    // It's useless
    // uint8_t partID[6];
    // partID[0] = readRegister8(MAX30208_PART_ID1);
    // partID[1] = readRegister8(MAX30208_PART_ID2);
    // partID[2] = readRegister8(MAX30208_PART_ID3);
    // partID[3] = readRegister8(MAX30208_PART_ID4);
    // partID[4] = readRegister8(MAX30208_PART_ID5);
    // partID[5] = readRegister8(MAX30208_PART_ID6);
    // for (int i = 0; i < 6; i++) {
    //     DBG("%x:", partID[i]);
    // }
    return 0;
}

void setAlarmHigh(float high)
{
    high *= 200;
    uint8_t msb = (uint32_t)high >> 8;
    uint8_t lsb = (uint32_t)high & 0xFF;
    writeRegister8(MAX30208_ALARM_HIGH_MSB, msb);
    writeRegister8(MAX30208_ALARM_HIGH_LSB, lsb);
}

void setAlarmLow(float low)
{
    low *= 200;
    uint8_t msb = (uint32_t)low >> 8 ;
    uint8_t lsb = (uint32_t)low & 0xFF;
    writeRegister8(MAX30208_ALARM_LOW_MSB, msb);
    writeRegister8(MAX30208_ALARM_LOW_LSB, lsb);
}

//Read the FIFO Write Pointer
uint8_t getWritePointer(void)
{
    return readRegister8(MAX30208_FIFO_WRITE_POINTER) & 0x1F;
}

//Read the FIFO Read Pointer
uint8_t getReadPointer(void)
{
    return readRegister8(MAX30208_FIFO_READ_POINTER) & 0x1F;
}

uint8_t check(void)
{
    uint8_t len = fifoAvailable();
    if ((len % 2) != 0) {
        len -= 1;
    }

    updateFifo(sense.buffer, len);
    for (uint8_t i = 0; i < len / 2; i += 2) {
        sense.head++;       //Advance the head of the storage struct
        sense.head %= 16;   //Wrap condition
        temperature[sense.head] = getTemp(sense.buffer[i], sense.buffer[i + 1]);
        //NRF_LOG_INFO("[%u] head:%u temperature:%d", i, sense.head, temperature[sense.head]);
    }
    return len / 2;
}

uint8_t available(void)
{
    int8_t numberOfSamples = sense.head - sense.tail;
    if (numberOfSamples < 0) {
        numberOfSamples += 16;
    }
    return (numberOfSamples);
}

//Advance the tail
void nextSample(void)
{
    if (available()) { //Only advance the tail if new data is available
        sense.tail++;
        sense.tail %= 16; //Wrap condition
    }
}

float readTemperature(void)
{
    return temperature[sense.head];
}

float readTemperatureF(void)
{
    float temp = readTemperature();
    if (temp == 0) {
        return 0;
    }
    return (temp * 1.8) + 32.0;
}

float getTemp(uint8_t msb, uint8_t lsb)
{
    if (msb & 0x80) {
        return  0 - (((msb << 8) | (lsb)) * 0.005);
    }
    return ((msb << 8) | (lsb)) * 0.005;
}
