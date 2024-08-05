#ifndef __MAX30208_H_
#define __MAX30208_H_

#include "stdio.h"
#include <stdint.h>
#include <string.h>
 /// MAX30208 Register Addresses/*SENSOR SLAVE ADDRESS*/
#define MAX30208_DEFAULT_ADDRESS         (0x50)
#define MAX30208_DEFAULT_PART_IDENTIFIER (0x30)
/*INTERRUPT AND STATUS*/
#define MAX30208_INTERRUPT_STATUS        (0x00)
#define MAX30208_INTERRUPT_ENABLE        (0x01)


#define MAX30208_INT_STATUS_AFULL        (0x80)
#define MAX30208_INT_STATUS_TEMP_LOW     (0x04)
#define MAX30208_INT_STATUS_TEMP_HIGH    (0x02)
#define MAX30208_INT_STATUS_TEMP_RDY     (0x01)


/*FIFO*/
#define MAX30208_FIFO_WRITE_POINTER      (0x04)
#define MAX30208_FIFO_READ_POINTER       (0x05)
#define MAX30208_FIFO_OVERFLOW_COUNTER   (0x06)
#define MAX30208_FIFO_DATA_COUNTER       (0x07)
#define MAX30208_FIFO_DATA               (0x08)
#define MAX30208_FIFO_CONFIGURATION1     (0x09)
#define MAX30208_FIFO_CONFIGURATION2     (0x0A)

/*SYSTEM*/
#define MAX30208_SYSTEM_REG              (0x0C)

/*TEMPERATURE*/
#define MAX30208_ALARM_HIGH_MSB          (0x10)
#define MAX30208_ALARM_HIGH_LSB          (0x11)
#define MAX30208_ALARM_LOW_MSB           (0x12)
#define MAX30208_ALARM_LOW_LSB           (0x13)
#define MAX30208_TEMP_SENSOR_SETUP       (0x14)

/*GPIO*/
#define MAX30208_GPIO_SETUP_REG          (0x20)
#define MAX30208_GPIO_CONTROL            (0x21)

/*IDENTIFIERS*/
#define MAX30208_PART_ID1                (0x31)
#define MAX30208_PART_ID2                (0x32)
#define MAX30208_PART_ID3                (0x33)
#define MAX30208_PART_ID4                (0x34)
#define MAX30208_PART_ID5                (0x35)
#define MAX30208_PART_ID6                (0x36)
#define MAX30208_PART_IDENTIFIER         (0xFF)
#define MAX30208_PART_IDENTIFIER_VAL     (0x30)


#define STORAGE_SIZE        32

typedef struct Record {
    uint8_t buffer[STORAGE_SIZE];
    uint8_t head;
    uint8_t tail;
} sense_struct; //This is our circular buffer of readings from the sensor



uint8_t max30208_init(void);
void softReset(void);
void enableAFULL(void);
void disableAFULL(void);
uint8_t readPartID(void);

void enableTEMPHIGH(void);
void disableTEMPHIGH(void);
void enableTEMPLOW(void);
void disableTEMPLOW(void);
void enableDATARDY(void);
void disableDATARDY(void);
void enableInterrup();
void configureFifo(uint8_t mask);
uint8_t fifoAvailable(void);
uint8_t getINT(void);
uint8_t readRegister8(uint8_t reg);
void writeRegister8(uint8_t reg, uint8_t value);
void clearFIFO(void);
void bitMask(uint8_t reg, uint8_t mask, uint8_t thing);
uint8_t updateFifo(uint8_t *data, uint8_t len);
void startConvert(void);
uint8_t getPartID(void);
uint8_t readPartID(void);
void setAlarmHigh(float high);
void setAlarmLow(float low);
uint8_t getWritePointer(void);
uint8_t getReadPointer(void);
uint8_t check(void);
uint8_t available(void);
void nextSample(void);
float readTemperature(void);
float readTemperatureF(void);
float getTemp(uint8_t msb, uint8_t lsb);

#endif /* __MAX30208_H_ */