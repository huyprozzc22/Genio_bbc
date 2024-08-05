00001 /*******************************************************************************
00002 * Copyright (C) Maxim Integrated Products, Inc., All Rights Reserved.
00003 *
00004 * Permission is hereby granted, free of charge, to any person obtaining a
00005 * copy of this software and associated documentation files (the "Software"),
00006 * to deal in the Software without restriction, including without limitation
00007 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
00008 * and/or sell copies of the Software, and to permit persons to whom the
00009 * Software is furnished to do so, subject to the following conditions:
00010 *
00011 * The above copyright notice and this permission notice shall be included
00012 * in all copies or substantial portions of the Software.
00013 *
00014 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
00015 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
00016 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
00017 * IN NO EVENT SHALL MAXIM INTEGRATED BE LIABLE FOR ANY CLAIM, DAMAGES
00018 * OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
00019 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
00020 * OTHER DEALINGS IN THE SOFTWARE.
00021 *
00022 * Except as contained in this notice, the name of Maxim Integrated
00023 * Products, Inc. shall not be used except as stated in the Maxim Integrated
00024 * Products, Inc. Branding Policy.
00025 *
00026 * The mere transfer of this software does not imply any licenses
00027 * of trade secrets, proprietary technology, copyrights, patents,
00028 * trademarks, maskwork rights, or any other form of intellectual
00029 * property whatsoever. Maxim Integrated Products, Inc. retains all
00030 * ownership rights.
00031 *******************************************************************************
00032 * @file          MAX30208.cpp
00033 * @brief         This is the C++ file used for the MAX30208 human body temperature sensor library.
00034 * @version       1.0
00035 * @notes         This file needs to be imported along with MAX30208.h for the program to work properly. This is library containing basic functions to be used in conjunction with the MAX30208. This library does not support any other devices. This is an MBed tested library. 
00036 *****************************************************************************/

#include "MAX30208.h"
 
MAX30208::MAX30208(I2C &i2c, uint8_t slaveAddress): 
m_i2c(i2c), m_writeAddress(slaveAddress << 1), 
m_readAddress((slaveAddress << 1) | 1)
{
}
 
 
 //******************************************************************************
 MAX30208::~MAX30208(void) {
   //empty block
 }
 
//******************************************************************************
int32_t MAX30208::writeInterruptRegister(Configuration_InterruptEnable config) {
    return(writeRegister(MAX30208::Interrupt_Enable,(config.all << 8),2));
}

//******************************************************************************
int32_t MAX30208::readInterruptRegister(Configuration_InterruptEnable &config) {
    uint16_t data;
    int32_t status;
    status = readRegister(MAX30208::Interrupt_Enable, data, 1);
    if(status == 0) {
        config.all = data;
    }
    return(status);
}
 
 //****************************************************************************** 
int32_t MAX30208::readStatus(uint16_t &value) {
    return(readRegister(MAX30208::Status,value, 1));
}
     
 //******************************************************************************
int32_t MAX30208::readWritePointer(uint16_t &value) {
    return (readRegister(MAX30208::FIFO_Write_Pointer,value, 1));
}

 //******************************************************************************
int32_t MAX30208::readReadPointer(uint16_t &value) {
    return (readRegister(MAX30208::FIFO_Read_Pointer,value,1));
}
     
 //******************************************************************************
int32_t MAX30208::writeReadPointer(uint8_t config) {
    return(writeRegister(MAX30208::FIFO_Read_Pointer, (config << 8),2));
}
     
 //******************************************************************************
int32_t MAX30208::readOverflow(uint16_t &value) {
    return(readRegister(MAX30208::FIFO_Overflow_Counter,value, 1));
}
     
 //******************************************************************************
int32_t MAX30208::readDataCounter(uint16_t &value) {
    return(readRegister(MAX30208::FIFO_Data_Counter,value, 1));  
}
     
 //******************************************************************************
int32_t MAX30208::readData(uint16_t &value) {
    return(readRegister(MAX30208::FIFO_Data,value, 2));
}
     
 //******************************************************************************
int32_t MAX30208::takeDataMeasurment() {
    return(writeRegister(MAX30208::Temp_Sensor_Setup,0xFF00,2));
}
     
 //******************************************************************************
int32_t MAX30208::readFIFOConfig1(uint16_t &value) {
    return (readRegister(MAX30208::FIFO_Config1,value, 1));
}
     
 //******************************************************************************
int32_t MAX30208::writeFIFOConfig1(uint8_t config) { 
    return(writeRegister(MAX30208::FIFO_Config1,(config << 8),2));
}
     
 //******************************************************************************
int32_t MAX30208::readFIFOConfig2(Configuration_FIFOConfig2 &config) {
    uint16_t data;
    int32_t status;
    status = readRegister(MAX30208::FIFO_Config2, data, 1);
    if(status == 0){
        config.all = data;
    }
    return(status);
}
     
 //******************************************************************************
int32_t MAX30208::writeFIFOConfig2(Configuration_FIFOConfig2 config) {
    return(writeRegister(MAX30208::FIFO_Config2,(config.all << 8),2));
}
     
 //******************************************************************************
int32_t MAX30208::resetDevice() {
    return(writeRegister(MAX30208::System_Control,(0x01 << 8),2));
}
 
 //******************************************************************************
int32_t MAX30208::readAlarmHigh(uint16_t &temp) {
    return readRegister(MAX30208::Alarm_High_MSB,temp, 2);
}
        
 //******************************************************************************
int32_t MAX30208::writeAlarmHigh(uint16_t temp) {
    return(writeRegister(MAX30208::Alarm_High_MSB, temp, 3));
}
     
 //******************************************************************************
int32_t MAX30208::readAlarmLow(uint16_t &value) {
    return (readRegister(MAX30208::Alarm_Low_MSB,value, 2));
}
     
 //******************************************************************************
int32_t MAX30208::writeAlarmLow(uint16_t temp) {
        return(writeRegister(MAX30208::Alarm_Low_MSB,temp,3));
    }
     
 //******************************************************************************
int32_t MAX30208::readGPIOSetup(Configuration_GPIOSetup &config) {
    uint16_t data;
    int32_t status;
    status = readRegister(MAX30208::GPIO_Setup, data, 1);
    if(status == 0) {
        config.all = data;
    }
    return(status);
}
 
 //******************************************************************************
int32_t MAX30208::writeGPIOSetup(Configuration_GPIOSetup config) {
    return(writeRegister(MAX30208::GPIO_Setup,(config.all << 8),2));
}
     
 //******************************************************************************
int32_t MAX30208::readGPIOControl(Configuration_GPIOControl &config) {
    uint16_t data;
    int32_t status;
    status = readRegister(MAX30208::GPIO_Control, data, 1);
    if(status == 0) {
        config.all = data;
    }
    return(status);
}

 //******************************************************************************
int32_t MAX30208::writeGPIOControl(Configuration_GPIOControl config) {
    return(writeRegister(MAX30208::GPIO_Control,(config.all << 8),2));
}
     
 //******************************************************************************
float MAX30208::toCelsius(uint16_t rawTemp) {
    float celsius;
    celsius = 0.005*rawTemp;
    return celsius;    
}
     
 //******************************************************************************
float MAX30208::toFahrenheit(float temperatureC) {
    float temperatureF;
    temperatureF = (temperatureC * 1.8F) + 32.0f;
    return temperatureF;   
}
     
 //******************************************************************************
int32_t MAX30208::writeRegister(Registers_e reg, uint16_t value, int bytesWritten) {

int32_t ret;

uint8_t hi = ((value >> 8) & 0xFF);
uint8_t lo = (value & 0xFF);
char val[3] = {reg, hi, lo};

ret = m_i2c.write(m_writeAddress, val, bytesWritten, false); 
return (ret);
}

 //******************************************************************************
int32_t MAX30208::readRegister(Registers_e reg, uint16_t &value, int bytesRead) {
    
    int32_t ret;
    char cmdata[1] = {reg};
    char dataRead[2];
        
    ret = m_i2c.write(m_readAddress,cmdata,1,true);
    if (ret == 0) {
        ret = m_i2c.read(m_readAddress,dataRead,bytesRead,false);
        if(ret == 0 && bytesRead == 2){
            value = ((dataRead[0]<<8)+ dataRead[1]);
        }
        else if (ret == 0 && bytesRead == 1){
            value = dataRead[0];    
        }
    }
    return(ret);
}