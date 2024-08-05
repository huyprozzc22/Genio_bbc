#ifndef _MLX90632_LIB_
#define _MLX90632_LIB_

#ifndef BIT
#define BIT(x)(1UL << (x))
#endif


/* Definition of I2C address of MLX90632 */
// #define CHIP_ADDRESS 0x3a << 1
#define MLX90632_ADDR 0x3a

/* Memory sections addresses */
#define MLX90632_ADDR_RAM       0x4000 /**< Start address of ram */
#define MLX90632_ADDR_EEPROM    0x2480 /**< Start address of user eeprom */

#define MLX90632_CHIP_VERSION   0x2405 /**< Chip version */

/* EEPROM addresses - used at startup */
#define MLX90632_EE_CTRL    0x24d4 /**< Control register initial value */
#define MLX90632_EE_CONTROL MLX90632_EE_CTRL /**< More human readable for Control register */

#define MLX90632_EE_I2C_ADDRESS 0x24d5 /**< I2C address register initial value */
#define MLX90632_EE_VERSION 0x240b /**< EEPROM version reg - assumed 0x101 */

#define MLX90632_EE_P_R     0x240c /**< Calibration constant ambient reference register 32bit */
#define MLX90632_EE_P_G     0x240e /**< Calibration constant ambient gain register 32bit */
#define MLX90632_EE_P_T     0x2410 /**< Calibration constant ambient tc2 register 32bit	*/
#define MLX90632_EE_P_O     0x2412 /**< Calibration constant ambient offset register 32bit */
#define MLX90632_EE_Aa      0x2414 /**< Aa calibration const register 32bit */
#define MLX90632_EE_Ab      0x2416 /**< Ab calibration const register 32bit */
#define MLX90632_EE_Ba      0x2418 /**< Ba calibration const register 32bit */
#define MLX90632_EE_Bb      0x241a /**< Bb calibration const register 32bit */
#define MLX90632_EE_Ca      0x241c /**< Ca calibration const register 32bit */
#define MLX90632_EE_Cb      0x241e /**< Cb calibration const register 32bit */
#define MLX90632_EE_Da      0x2420 /**< Da calibration const register 32bit */
#define MLX90632_EE_Db      0x2422 /**< Db calibration const register 32bit */
#define MLX90632_EE_Ea      0x2424 /**< Ea calibration constant register 32bit */
#define MLX90632_EE_Eb      0x2426 /**< Eb calibration constant register 32bit */
#define MLX90632_EE_Fa      0x2428 /**< Fa calibration constant register 32bit */
#define MLX90632_EE_Fb      0x242a /**< Fb calibration constant register 32bit */
#define MLX90632_EE_Ga      0x242c /**< Ga calibration constant register 32bit */

#define MLX90632_EE_Gb      0x242e /**< Ambient Beta calibration constant 16bit */
#define MLX90632_EE_Ka      0x242f /**< IR Beta calibration constant 16bit */

#define MLX90632_EE_Ha      0x2481 /**< Ha customer calibration value register 16bit */
#define MLX90632_EE_Hb      0x2482 /**< Hb customer calibration value register 16bit */

/* Register addresses - volatile */
#define MLX90632_REG_I2C_ADDR   0x3000 /**< Chip I2C address register */

/* Control register address - volatile */
#define MLX90632_REG_CTRL       0x3001 /**< Control Register address */
#define MLX90632_CFG_SOC_SHIFT  3 /**< Start measurement in step mode */
#define MLX90632_CFG_SOC_MASK   BIT(MLX90632_CFG_SOC_SHIFT)
#define MLX90632_CFG_PWR_MASK   GENMASK(2, 1) /**< PowerMode Mask */
/* PowerModes statuses */
#define MLX90632_PWR_STATUS(ctrl_val) (ctrl_val << 1)
#define MLX90632_PWR_STATUS_HALT MLX90632_PWR_STATUS(0) /**< Pwrmode hold */
#define MLX90632_PWR_STATUS_SLEEP_STEP MLX90632_PWR_STATUS(1) /**< Pwrmode sleep step*/
#define MLX90632_PWR_STATUS_STEP MLX90632_PWR_STATUS(2) /**< Pwrmode step */
#define MLX90632_PWR_STATUS_CONTINUOUS MLX90632_PWR_STATUS(3) /**< Pwrmode continuous*/

/* Device status register - volatile */
#define MLX90632_REG_STATUS     0x3fff /**< Device status register */
#define MLX90632_STAT_BUSY      BIT(10) /**< Device busy indicator */
#define MLX90632_STAT_EE_BUSY   BIT(9) /**< Device EEPROM busy indicator */
#define MLX90632_STAT_BRST      BIT(8) /**< Device brown out reset indicator */
#define MLX90632_STAT_DATA_RDY  BIT(0) /**< Data ready indicator */

//User RAM
#define MLX90632_RAM_1 0x4000
#define MLX90632_RAM_2 0x4001
#define MLX90632_RAM_3 0x4002
#define MLX90632_RAM_4 0x4003
#define MLX90632_RAM_5 0x4004
#define MLX90632_RAM_6 0x4005
#define MLX90632_RAM_7 0x4006
#define MLX90632_RAM_8 0x4007
#define MLX90632_RAM_9 0x4008

/* Timings (ms) */
#define MLX90632_TIMING_EEPROM 100 /**< Time between EEPROM writes */

/* Magic constants */
#define MLX90632_EEPROM_VERSION 0x8505 /**< EEPROM DSP version for constants */
#define MLX90632_EEPROM_WRITE_KEY 0x554C /**< EEPROM write key 0x55 and 0x4c */
#define MLX90632_RESET_CMD  0x0006 /**< Reset sensor (address or global) */
#define MLX90632_MAX_MEAS_NUM   31 /**< Maximum number of measurements in list */
#define MLX90632_EE_SEED    0x3f6d /**< Seed for the CRC calculations */
#define MLX90632_REF_12 12.0 /**< ResCtrlRef value of Channel 1 or Channel 2 */
#define MLX90632_REF_3  12.0 /**< ResCtrlRef value of Channel 3 */

//Three measurement modes available
#define MLX90632_SLEEP_MODE 0b01
#define MLX90632_STEP_MODE  0b10
#define MLX90632_CONT_MODE  0b11

typedef enum {
    MLX90632_E_OK = 0,
    MLX90632_I2C_ERR = -1,
    MLX90632_ID_ERR = -2,
    MLX90632_EE_VERSION_ERR = -3,
    MLX90632_TIMEOUT_ERR = -4,
    MLX90632_CYCLE_POS_ERR = -5,
    MLX90632_BROWN_OUT_ERR = -8,
	MLX90632_BUSY_ERR = -9,
	MLX90632_EE_BUSY_ERR = -10,
} mlx90632_error_t;

mlx90632_error_t mlx90632_get_object_temp(double* object_tempC);
mlx90632_error_t mlx90632_set_SOC(void);

/** Initialize MLX90632 driver and confirm EEPROM version
 *
 * EEPROM version is important to match sensor EEPROM content and calculations.
 * This is why this function checks for correct EEPROM version before it does
 * checksum validation of the EEPROM content.
 *
 * @note EEPROM version can have swapped high and low bytes due to CPU or I2C.
 * Please confirm that i2c read (16bit) is functioning as expected.
 *
 * @retval 0 Successfully initialized MLX90632 driver
 * @retval <0 Something went wrong. Consult errno.h for more details.
 */
mlx90632_error_t mlx90632_init(void);

#endif
