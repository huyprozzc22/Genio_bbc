+#ifndef __MAX30208_H_
+#define __MAX30208_H_
+
+#include "mbed.h"
+
+/**
+ * @brief Library for the MAX30208
+ *
+ * @code
+ * #include "mbed.h"
+ * #include "max32630fthr.h"
+ * #include "MAX30208.h"
+ * 
+ * MAX32630FTHR pegasus(MAX32630FTHR::VIO_3V3);
+ *
+ * //Get I2C instance
+ * I2C i2cBus(I2C1_SDA, I2C1_SCL);
+ *
+ * //Get temp sensor instance
+ * MAX30208 BodyTempSensor(i2cBus, 0x50); //Constructor takes 7-bit slave adress. 0x50 is default slave address for MAX30208 Ev-Kit
+ *
+ * int main(void) 
+ * {
+ *     //use sensor
+ * }
+ * @endcode
+ */
+
+class MAX30208
+{
+    
+public:
+    /// MAX30208 Register Addresses
+    enum Registers_e {
+        Status = 0x00,
+        Interrupt_Enable = 0x01,
+        FIFO_Write_Pointer = 0x04,
+        FIFO_Read_Pointer = 0x05,
+        FIFO_Overflow_Counter = 0x06,
+        FIFO_Data_Counter = 0x07,
+        FIFO_Data = 0x08,
+        FIFO_Config1 = 0x09,
+        FIFO_Config2 = 0x0A,
+        System_Control = 0x0C,
+        Alarm_High_MSB = 0x10,
+        Alarm_Low_MSB = 0x12,
+        Temp_Sensor_Setup = 0x14,
+        GPIO_Setup = 0x20,
+        GPIO_Control = 0x21
+    };
+    
+    //Intterupt Register Config
+    union Configuration_InterruptEnable{
+        uint8_t all;
+        struct BitField_s{
+            uint8_t TEMP_RDY_EN : 1;
+            uint8_t TEMP_HI_EN  : 1;
+            uint8_t TEMP_LO_EN  : 1;
+            uint8_t             : 4;    //unused bits
+            uint8_t A_FULL_EN   : 1;
+        }config;
+    };
+    
+    //FIFO Config 2 Register
+    union Configuration_FIFOConfig2{
+        uint8_t all;
+        struct BitField_s{
+            uint8_t               : 1;    //unused bit
+            uint8_t FIFO_RO       : 1;
+            uint8_t A_FULL_TYPE   : 1;
+            uint8_t FIFO_STAT_CLR : 1;
+            uint8_t FLUSH_FIFO    : 1;
+            uint8_t               : 0;    //unused bits
+        }config;
+    };
+    
+    //GPIO Setup Register
+    union Configuration_GPIOSetup{
+        uint8_t all;
+        struct BitField_s{
+            uint8_t GPIO0_MODE     : 2;
+            uint8_t                : 4;    //unused bits
+            uint8_t GPIO1_MODE     : 2;
+        }config;
+    };
+    
+    //GPIO Control Register
+    union Configuration_GPIOControl{
+        uint8_t all;
+        struct BitField_s{
+            uint8_t GPIO0_LL   : 1;
+            uint8_t            : 2;    //unused bits
+            uint8_t GPIO1_LL   : 1;
+            uint8_t            : 0;    //unused bits
+        }config;
+    };
+
+    /**
+    * @brief  Constructor using reference to I2C object
+    * @param i2c - Reference to I2C object
+    * @param slaveAddress - 7-bit I2C address
+    */
+    MAX30208(I2C &i2c, uint8_t slaveAddress);
+
+    /** @brief Destructor */
+    ~MAX30208(void);
+    
+    /**
+    * @brief  Write Interrupt Register
+    * @param config - Reference to Configuration type, config.all is written upon succesful register write
+    * @return 0 on success, non-zero on failure
+    */
+    int32_t writeInterruptRegister(Configuration_InterruptEnable config);    
+    
+    /**
+    * @brief  Read Interrupt Register Configuration
+    * @param config - Reference to Configuration type, config.all is updated upon succesful register read
+    * @return 0 on success, non-zero on failure
+    */
+    int32_t readInterruptRegister(Configuration_InterruptEnable &config);
+    
+    /**
+    * @brief  Read Status Register
+    * @param[out] value - Status Register Value on succesful read
+    * @return 0 on success, non-zero on failure
+    */
+    int32_t readStatus(uint16_t &value);
+    
+    /**
+    * @brief  Read FIFO Write Pointer Value
+    * @param[out] value - FIFO Write Pointer value on succesful read
+    * @return 0 on success, non-zero on failure
+    */
+    int32_t readWritePointer(uint16_t &value);
+    
+    /**
+    * @brief  Read FIFO Read Pointer Value
+    * @param[out] value - FIFO Read Pointer value on succesful read
+    * @return 0 on success, non-zero on failure
+    */
+    int32_t readReadPointer(uint16_t &value);
+    
+    /**
+    * @brief  Write FIFO Read Pointer Value
+    * @param config - New FIFO Read Pointer value on succesful write
+    * @return 0 on success, non-zero on failure
+    */
+    int32_t writeReadPointer(uint8_t config);
+    
+    /**
+    * @brief Read FIFO Overflow Register
+    * @param[out] value - Overflow Counter value on succesful read
+    * @return 0 on success, non-zero on failure
+    */
+    int32_t readOverflow(uint16_t &value);
+    
+    /**
+    * @brief Read Data Counter Register
+    * @param[out] value - Data Count register value on succesful read
+    * @return 0 on success, non-zero on failure
+    */
+    int32_t readDataCounter(uint16_t &value);
+    
+    /**
+    * @brief  Read FIFO Data at FIFO Read Pointer
+    * @param[out] value - Temperature value from FIFO data register on succesful read 
+    * @return 0 on success, non-zero on failure
+    */
+    int32_t readData(uint16_t &value);
+    
+    /**
+    * @brief Take a new temperature reading
+    * @return 0 on success, non-zero on failure
+    */
+    int32_t takeDataMeasurment();
+    
+    /**
+    * @brief  Read FIFO Config1 Register
+    * @param[out] value - FIFO Config1 value on succesful read 
+    * @return 0 on success, non-zero on failure
+    */
+    int32_t readFIFOConfig1(uint16_t &value);
+    
+    /**
+    * @brief Write FIFO Config1 register
+    * @param config - FIFO Config1 register data to write 
+    * @return 0 on success, non-zero on failure
+    */
+    int32_t writeFIFOConfig1(uint8_t config);
+    
+    /**
+    * @brief Read FIFO Config2 register
+    * @param[out] config - Reference to Configuration type, config.all is updated upon succesful register read
+    * @return 0 on success, non-zero on failure
+    */
+    int32_t readFIFOConfig2(Configuration_FIFOConfig2 &config);
+    
+    /**
+    * @brief Read FIFO Config2 register
+    * @param config - Reference to Configuration type, config.all is written upon succesful register write
+    * @return 0 on success, non-zero on failure
+    */
+    int32_t writeFIFOConfig2(Configuration_FIFOConfig2 config);
+    
+    /**
+    * @brief Reset Device to factory default
+    * @return 0 on success, non-zero on failure
+    */
+    int32_t resetDevice(); //set bit 0 in system register to 1 to factory reset
+
+    /**
+    * @brief Read High Temperature Alarm Value
+    * @param[out] temp - High Temperature Alarm Value
+    * @return 0 on success, non-zero on failure
+    */
+    int32_t readAlarmHigh(uint16_t &temp);
+    
+    /**
+    * @brief Write High Temperature Alarm Value
+    * @param temp - 16-bit High Temperature Value to Write
+    * @return 0 on success, non-zero on failure
+    */    
+    int32_t writeAlarmHigh(uint16_t temp);
+    
+    /**
+    * @brief Read Low Temperature Alarm Value
+    * @param[out] temp - Low Temperature Alarm Value
+    * @return 0 on success, non-zero on failure
+    */
+    int32_t readAlarmLow(uint16_t &value);
+    
+    /**
+    * @brief Write Low Temperature Alarm Value
+    * @param temp - 16-bit Low Temperature Value to Write
+    * @return 0 on success, non-zero on failure
+    */
+    int32_t writeAlarmLow(uint16_t temp);
+    
+    /**
+    * @brief Read GPIO Setup register
+    * @param config - Reference to Configuration type, config.all is updated upon succesful register read
+    * @return 0 on success, non-zero on failure
+    */
+    int32_t readGPIOSetup(Configuration_GPIOSetup &config);
+    
+    /**
+    * @brief Write GPIO Setup register
+    * @param config - Reference to Configuration type, config.all is written to register upon succesful register write
+    * @return 0 on success, non-zero on failure
+    */
+    int32_t writeGPIOSetup(Configuration_GPIOSetup config);
+    
+    /**
+    * @brief Read GPIO Control register
+    * @param config - Reference to Configuration type, config.all is updated upon succesful register read
+    * @return 0 on success, non-zero on failure
+    */
+    int32_t readGPIOControl(Configuration_GPIOControl &config);
+    
+    /**
+    * @brief Write GPIO Control register
+    * @param config - Reference to Configuration type, config.all is written to register upon succesful register write
+    * @return 0 on success, non-zero on failure
+    */
+    int32_t writeGPIOControl(Configuration_GPIOControl config);
+    
+    /**
+    * @brief Convert Raw Sensor Data to degrees Celisus
+    * @param rawTemp - 16 bit raw temperature data
+    * @return Returns the converted Celsius Temperature
+    */
+    float toCelsius(uint16_t rawTemp);
+    
+    /**
+    * @brief Convert Celsius Temperature to Fahrenheit
+    * @param temperatureC - Temperature in degrees Celsius that will be converted
+    * @return Returns the converted Fahrenheit temperature
+    */
+    float toFahrenheit(float temperatureC);
+    
+protected:
+
+    /** 
+    * @brief Write register of device at slave address
+    * @param reg - char array that contains address of register and write value
+    * @param value - Data written to register on sucessful write
+    * @param bytesWritten - Number of bytes to write
+    * @return 0 on success, non-zero on failure
+    */
+    int32_t writeRegister(Registers_e reg, uint16_t value, int bytesWritten);

    int32_t readRegister(Registers_e reg, uint16_t &value, int bytesRead);

private:
    /// I2C object
    I2C & m_i2c;
    /// Device slave addresses
    uint8_t m_writeAddress, m_readAddress;
};

#endif /* __MAX30208_H_ */