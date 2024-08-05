#include <stdint.h>
#include <math.h>
#include <errno.h>

#include "twi_nrf52.h"
#include "twi_types.h"
#include "mlx90632.h"

#include <string.h>

static mlx90632_error_t mlx90632_i2c_read16(int16_t register_address, uint16_t *value);
static mlx90632_error_t mlx90632_i2c_read32(int16_t register_address, uint32_t *value);
static mlx90632_error_t mlx90632_i2c_write(int16_t register_address, uint16_t value);

static mlx90632_error_t mlx90632_get_status(uint16_t* reg_status);
static mlx90632_error_t mlx90632_eeprom_status(bool* status);
static mlx90632_error_t mlx90632_clear_new_data(void);
static mlx90632_error_t mlx90632_set_mode(uint16_t mode);
static mlx90632_error_t mlx90632_get_cycle_position(uint8_t* cyc_pos);
static mlx90632_error_t mlx90632_read_eeprom(void);

//Declare variables for the calibration values
double P_R;
double P_G;
double P_T;
double P_O;
double Ea;
double Eb;
double Fa;
double Fb;
double Ga;
double Gb;
double Ka;
double Ha;
double Hb;

static double TOdut = 30.0; //Assume 25C for first iteration
static double TO0 = 25.0; //object temp from previous calculation
static double TA0 = 25.0; //ambient temp from previous calculation

/* HAL_I2C_Mem_Read()/Write() are used instead of Master_Transmit()/Receive() because repeated start condition is needed */
/* Implementation of I2C read for 16-bit values */
static mlx90632_error_t mlx90632_i2c_read16(int16_t register_address, uint16_t* value)
{
    i2c_error_t err = I2C_E_OK;

    uint8_t twi_cmd[2]={0};
    uint8_t twi_data[2]={0};
	memset(twi_cmd,0, sizeof(twi_cmd));
	memset(twi_data,0, sizeof(twi_data));

	twi_cmd[0] = register_address >> 8;
    twi_cmd[1] = register_address & 0xFF;

    twi_master_write(MLX90632_ADDR, twi_cmd, sizeof(twi_cmd), true);
    twi_master_read(MLX90632_ADDR, twi_data, sizeof(twi_data));
    *value = (twi_data[0] << 8) | twi_data[1];

    if(err != I2C_E_OK) return MLX90632_I2C_ERR;
    return MLX90632_E_OK;

}

/* Implementation of I2C read for 32-bit values */
static mlx90632_error_t mlx90632_i2c_read32(int16_t register_address, uint32_t* value)
{
    i2c_error_t err = I2C_E_OK;

    uint8_t twi_cmd[2]={0};
    uint8_t twi_data[4]={0};

	twi_cmd[0] = register_address >> 8;
    twi_cmd[1] = register_address & 0xFF;

    twi_master_write(MLX90632_ADDR, twi_cmd, sizeof(twi_cmd), true);
    twi_master_read(MLX90632_ADDR, twi_data, sizeof(twi_data));
    *value = (twi_data[2] << 24)|(twi_data[3] << 16)|(twi_data[0] << 1)|twi_data[1];

    if(err != I2C_E_OK) return MLX90632_I2C_ERR;
    return err;
}

/* Implementation of I2C write for 16-bit values */
static mlx90632_error_t mlx90632_i2c_write(int16_t register_address, uint16_t value)
{
    i2c_error_t err = I2C_E_OK;

    uint8_t twi_cmd[2]={0};
    uint8_t twi_data[2]={0};

	twi_cmd[0] = register_address >> 8;
    twi_cmd[1] = register_address & 0xFF;

    twi_data[0] = value >> 8;
    twi_data[1] = value & 0xFF;

    twi_master_write(MLX90632_ADDR, twi_cmd, sizeof(twi_cmd), true);
    twi_master_write(MLX90632_ADDR, twi_data, sizeof(twi_cmd), false);

    if(err != I2C_E_OK) return MLX90632_I2C_ERR;
    return err;
}

static mlx90632_error_t mlx90632_get_status(uint16_t* reg_status)
{
    // bit 10 : device_busy
    // bit 9  : eeprom_busy
    // bit 8  : brown_out
    // bit 6:2: cycle_position
    // bit 0  : new_data
    mlx90632_error_t err = mlx90632_i2c_read16(MLX90632_REG_STATUS, reg_status);
    if(err != MLX90632_E_OK) return err;
    return err;
}

//Returns true if eeprom is busy
//EEPROM is busy during boot up and during EEPROM write/erase
static mlx90632_error_t mlx90632_eeprom_status(bool* ee_status)
{
    uint16_t reg_status;
    mlx90632_error_t err = mlx90632_get_status(&reg_status);
    if(err != MLX90632_E_OK) return err;

    if(reg_status & MLX90632_STAT_EE_BUSY){
        *ee_status = true;
    }
    else {
        *ee_status = false;
    }
    return err;
}

//Sets the sensing mode (3 modes availabe)
static mlx90632_error_t mlx90632_set_mode(uint16_t mode)
{
    uint16_t reg_status;
    mlx90632_error_t err;
    err = mlx90632_i2c_read16(MLX90632_REG_CTRL, &reg_status); //Get current bits
    if(err != MLX90632_E_OK) return err;

    reg_status &= ~(0x0003 << 1); //Clear the mode bits
    reg_status |= (mode << 1); //Set the bits

    err = mlx90632_i2c_write(MLX90632_REG_CTRL, reg_status); //Set the mode bits
    if(err != MLX90632_E_OK) return err;

    return err;
}

mlx90632_error_t mlx90632_get_mode(uint16_t* mode)
{
    mlx90632_error_t err;
    err = mlx90632_i2c_read16(MLX90632_REG_CTRL, mode); //Get current register settings
    if(err != MLX90632_E_OK) return err;
    *mode = (*mode >> 1) & 0x0003; //Clear all other bits
    return err;
}

//This reads all the temperature calibration factors for the sensor itself
//This is needed so that it can be called from getObjectTemp *and* users can call getSensorTemp
//without causing a double read
// static mlx90632_error_t MLX90632::mlx90632_pre_calculation_temp(double* base_temp)
// {
//     mlx90632_error_t err;
//
//     //Get RAM_6 and RAM_9
//     int16_t RAM_6_value;
//     err = mlx90632_i2c_read16(MLX90632_RAM_6, (uint16_t&)RAM_6_value);
//     if(err != MLX90632_E_OK) return err;
//
//     int16_t RAM_9_value;
//     err = mlx90632_i2c_read16(MLX90632_RAM_9, (uint16_t&)RAM_9_value);
//     if(err != MLX90632_E_OK) return err;
//
//     double VRta = RAM_9_value + Gb * (RAM_6_value / 12.0);
//
//     double AMB = (RAM_6_value / 12.0) / VRta * pow(2, 19);
//
//     return err;
// }

//Returns the cycle_pos from status register. cycle_pos is 0 to 31
static mlx90632_error_t mlx90632_get_cycle_position(uint8_t* cyc_pos)
{
    uint16_t reg_status;
    mlx90632_error_t err = mlx90632_get_status(&reg_status);
    if(err != MLX90632_E_OK) return err;

    *cyc_pos = reg_status >> 2 ; // //6:2 = 5 bits, Shave off last two bits
    *cyc_pos &= 0x1F; //Get lower 5 bits.

    return err;
}

//Clear the new_data bit. This is done after a measurement is complete
static mlx90632_error_t mlx90632_clear_new_data(void)
{
    uint16_t reg_status;
    mlx90632_error_t err = mlx90632_get_status(&reg_status); //Get current bits
    if(err != MLX90632_E_OK) return err;

    reg_status &= ~(1 << 0); //Clear the bit
    mlx90632_i2c_write(MLX90632_REG_STATUS, reg_status); //Set the mode bits
    if(err != MLX90632_E_OK) return err;

    return err;
}

//Returns true if new data is available
static mlx90632_error_t mlx90632_data_available(bool* isAvail)
{
    uint16_t reg_status;
    mlx90632_error_t err = mlx90632_get_status(&reg_status); //Get current bits
    if(err != MLX90632_E_OK) return err;

    if (reg_status & ((uint16_t)1 << 0)){
        *isAvail = true;
    }
    else {
        *isAvail = false;
    }
    return err;
}

//Sets the Start of Conversion (SOC) bit
mlx90632_error_t mlx90632_set_SOC(void)
{
    mlx90632_error_t err;
    uint16_t reg_status;
    err = mlx90632_i2c_read16(MLX90632_REG_CTRL, &reg_status); //Get current bits
    if(err != MLX90632_E_OK) return err;

    reg_status |= (1 << 3); //Set the bit

    err = mlx90632_i2c_write(MLX90632_REG_CTRL, reg_status); //Set the bit
    if(err != MLX90632_E_OK) return err;

    return err;
}

mlx90632_error_t mlx90632_get_object_temp(double* object_tempC)
{
    double TOdut = 25.0; //Assume 25C for first iteration
    double TO0 = 25.0; //object temp from previous calculation
    double TA0 = 25.0; //ambient temp from previous calculation

    mlx90632_error_t err = MLX90632_E_OK;
    uint16_t mode;
    mlx90632_get_mode(&mode);
    if(mode != MLX90632_CONT_MODE) mlx90632_set_SOC();

    // follow temperature calculation sequence which is described in datasheet
    // 1. Write new_data = 0
    err = mlx90632_clear_new_data();

    // 2. Check when new_data = 1
    uint16_t counter = 0;
    bool is_data_avail = false;
    while (is_data_avail == false)
    {
        mlx90632_data_available(&is_data_avail);
        nrf_delay_ms(100);
        counter++;
        if (counter == 10)
        {
            return MLX90632_TIMEOUT_ERR;
        }
    }

    // 3. Read cycle_pos to get measurement pointer

    // the choice between [RAM_4 and RAM_5] or [RAM_7 and RAM_8]
    int16_t RAM_lower_value;
    int16_t RAM_upper_value;
    int16_t RAM_6_value;
    int16_t RAM_9_value;

    //Get RAM_6 and RAM_9
    err = mlx90632_i2c_read16(MLX90632_RAM_6, (uint16_t*)&RAM_6_value);
    if(err != MLX90632_E_OK) return err;

    err = mlx90632_i2c_read16(MLX90632_RAM_9, (uint16_t*)&RAM_9_value);
    if(err != MLX90632_E_OK) return err;

    //Read cycle_pos to get measurement pointer
    uint8_t cyc_pos = 0; //initializing
    err = mlx90632_get_cycle_position(&cyc_pos);
    if(err != MLX90632_E_OK) return err;

    //If cycle_pos = 1
    //Calculate TA and TO based on RAM_4, RAM_5, RAM_6, RAM_9
    if (cyc_pos == 1)
    {
        err = mlx90632_i2c_read16(MLX90632_RAM_4, (uint16_t*)&RAM_lower_value);
        if(err != MLX90632_E_OK) return err;

        err = mlx90632_i2c_read16(MLX90632_RAM_5, (uint16_t*)&RAM_upper_value);
        if(err != MLX90632_E_OK) return err;
    }
    //If cycle_pos = 2
    //Calculate TA and TO based on RAM_7, RAM_8, RAM_6, RAM_9
    else if (cyc_pos == 2)
    {
        err = mlx90632_i2c_read16(MLX90632_RAM_7, (uint16_t*)&RAM_lower_value);
        if(err != MLX90632_E_OK) return err;

        err = mlx90632_i2c_read16(MLX90632_RAM_8, (uint16_t*)&RAM_upper_value);
        if(err != MLX90632_E_OK) return err;
    }
    else
    {
        return MLX90632_CYCLE_POS_ERR;
    }

    //Object temp requires 3 iterations
    for (uint8_t i = 0 ; i < 3; i++)
    {
        double VRta = (double)RAM_9_value + Gb * ((double)RAM_6_value / 12.0);

        double AMB = (double)(((double)RAM_6_value / 12.0) / VRta) * pow(2, 19);

        double S = ((double)RAM_lower_value + (double)RAM_upper_value) / 2.0;
        double VRto = ((double)RAM_9_value + Ka * ((double)RAM_6_value / 12.0));
        double Sto = (double)((S / 12.0) / VRto) * pow(2, 19);

        double TAdut = (AMB - Eb) / Ea + 25.0;

        double ambientTempK = TAdut + 273.15;

        double bigFraction = Sto / (1 * Fa * Ha * (1 + Ga * (TOdut - TO0) + Fb * (TAdut - TA0)));
        double obj_temp = bigFraction + pow(ambientTempK, 4);
        obj_temp = pow(obj_temp, 0.25); //Take 4th root
        obj_temp = obj_temp - 273.15 - Hb;

        TOdut = obj_temp;
    }

    // magic calib
 //   if ((TOdut < 34.0) && (TOdut >= 33.0)) {
 //       TOdut += 3.1;
 //   }
	//else if ((TOdut < 35.0) && (TOdut >= 34.0)) {
	//	TOdut += 2.0;
	//}
	//else if ((TOdut < 36.0) && (TOdut >= 35.0)) {
	//	TOdut += 2.0;
	//}
	//else if ((TOdut < 37.0) && (TOdut >= 36.0)) {
	//	TOdut += 2.0;
	//}
 //   else if ((TOdut < 38.0) && (TOdut >= 37.0)) {
	//	TOdut += 2.0;
	//}
 //   else if ((TOdut < 39.0) && (TOdut >= 38.0)) {
	//	TOdut += 2.0;
	//}
 //   else if ((TOdut < 40.0) && (TOdut >= 39.0)) {
	//	TOdut += 2.0;
	//}
 //   else {
 //       // do nothing
 //   }

    *object_tempC = TOdut-1;

    return err;
}

/**
 * Implementation of reading all calibration parameters for calucation of Ta and To
 */
static mlx90632_error_t mlx90632_read_eeprom(void)
{
    mlx90632_error_t err = MLX90632_E_OK;
    //Load all the static calibration factors
    int16_t tempValue16=0;
    int32_t tempValue32=0;

	err = mlx90632_i2c_read32(MLX90632_EE_P_R, (uint32_t*)&tempValue32);
	if (err != MLX90632_E_OK) return err;
    P_R = (double)tempValue32 * pow(2, -8);

	err = mlx90632_i2c_read32(MLX90632_EE_P_G, (uint32_t*)&tempValue32);
	if (err != MLX90632_E_OK) return err;
    P_G = (double)tempValue32 * pow(2, -20);

	err = mlx90632_i2c_read32(MLX90632_EE_P_O, (uint32_t*)&tempValue32);
	if (err != MLX90632_E_OK) return err;
    P_O = (double)tempValue32 * pow(2, -8);

	err = mlx90632_i2c_read32(MLX90632_EE_P_T, (uint32_t*)&tempValue32);
	if (err != MLX90632_E_OK) return err;
    P_T = (double)tempValue32 * pow(2, -44);

	err = mlx90632_i2c_read32(MLX90632_EE_Ea, (uint32_t*)&tempValue32);
	if (err != MLX90632_E_OK) return err;
    Ea = (double)tempValue32 * pow(2, -16);

	err = mlx90632_i2c_read32(MLX90632_EE_Eb, (uint32_t*)&tempValue32);
	if (err != MLX90632_E_OK) return err;
    Eb = (double)tempValue32 * pow(2, -8);

	err = mlx90632_i2c_read32(MLX90632_EE_Fa, (uint32_t*)&tempValue32);
    if (err != MLX90632_E_OK) return err;
    Fa = (double)tempValue32 * pow(2, -46);

	err = mlx90632_i2c_read32(MLX90632_EE_Fb, (uint32_t*)&tempValue32);
    if (err != MLX90632_E_OK) return err;
    Fb = (double)tempValue32 * pow(2, -36);

	err = mlx90632_i2c_read32(MLX90632_EE_Ga, (uint32_t*)&tempValue32);
    if (err != MLX90632_E_OK) return err;
    Ga = (double)tempValue32 * pow(2, -36);

	err = mlx90632_i2c_read16(MLX90632_EE_Gb, (uint16_t*)&tempValue16);
    if (err != MLX90632_E_OK) return err;
    Gb = (double)tempValue16 * pow(2, -10);

	err = mlx90632_i2c_read16(MLX90632_EE_Ha, (uint16_t*)&tempValue16);
    if (err != MLX90632_E_OK) return err;
    Ha = (double)tempValue16 * pow(2, -14);

	err = mlx90632_i2c_read16(MLX90632_EE_Hb, (uint16_t*)&tempValue16);
    if (err != MLX90632_E_OK) return err;
    Hb = (double)tempValue16 * pow(2, -10);

	err = mlx90632_i2c_read16(MLX90632_EE_Ka, (uint16_t*)&tempValue16);
    if (err != MLX90632_E_OK) return err;
    Ka = (double)tempValue16 * pow(2, -10);

	return err;
}


mlx90632_error_t mlx90632_init(void)
{
    mlx90632_error_t err;
    uint16_t mlx90632_addr;

    err = mlx90632_i2c_read16(MLX90632_EE_I2C_ADDRESS, &mlx90632_addr);
    if (err != MLX90632_E_OK) return err;

    if (mlx90632_addr != (0x003A>>1)) return MLX90632_ID_ERR;

    //Wait for eeprom_busy to clear
    uint16_t counter = 0;
    bool ee_is_busy = true;
    while (ee_is_busy)
    {
        mlx90632_eeprom_status(&ee_is_busy);
        nrf_delay_ms(1);
        counter++;
        if (counter == 10)
        {
            return MLX90632_TIMEOUT_ERR; //Error
        }
    }

    err = mlx90632_set_mode(MLX90632_SLEEP_MODE);
    if (err != MLX90632_E_OK) return err;

    err = mlx90632_read_eeprom();
    if (err != MLX90632_E_OK) return err;

    return err;
}
