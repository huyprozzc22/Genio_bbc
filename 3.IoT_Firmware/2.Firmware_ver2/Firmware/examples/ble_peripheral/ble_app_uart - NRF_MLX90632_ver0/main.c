//Code MAXM86146+MLX90632

#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include "nordic_common.h"
#include "nrf.h"
#include "ble_hci.h"
#include "ble_advdata.h"
#include "ble_advertising.h"
#include "ble_conn_params.h"
#include "nrf_sdh.h"
#include "nrf_sdh_soc.h"
#include "nrf_sdh_ble.h"
#include "nrf_ble_gatt.h"
#include "nrf_ble_qwr.h"
#include "app_timer.h"
#include "ble_nus.h"
#include "app_uart.h"
#include "app_util_platform.h"
#include "bsp_btn_ble.h"
#include "nrf_pwr_mgmt.h"
#include "nrf_delay.h"
#include "nrf_gpio.h"
#include "nrf_drv_twi.h"
#include "ble_app_saadc.h"
#include "twi_nrf52.h"
#include "mlx90632.h"
#include "MAX30208.h"

#if defined (UART_PRESENT)
#include "nrf_uart.h"
#endif
#if defined (UARTE_PRESENT)
#include "nrf_uarte.h"
#endif

#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"

#define APP_BLE_CONN_CFG_TAG            1                                           /**< A tag identifying the SoftDevice BLE configuration. */

#define DEVICE_NAME                     "BBC 20"                               /**< Name of device. Will be included in the advertising data. */
#define NUS_SERVICE_UUID_TYPE           BLE_UUID_TYPE_VENDOR_BEGIN                  /**< UUID type for the Nordic UART Service (vendor specific). */

#define APP_BLE_OBSERVER_PRIO           3                                           /**< Application's BLE observer priority. You shouldn't need to modify this value. */

#define APP_ADV_INTERVAL                800                                          /**< The advertising interval (in units of 0.625 ms. This value corresponds to 40 ms). */

#define APP_ADV_DURATION                0                                       /**< The advertising duration (180 seconds) in units of 10 milliseconds. */

#define MIN_CONN_INTERVAL               MSEC_TO_UNITS(400, UNIT_1_25_MS)             /**< Minimum acceptable connection interval (20 ms), Connection interval uses 1.25 ms units. */
#define MAX_CONN_INTERVAL               MSEC_TO_UNITS(650, UNIT_1_25_MS)             /**< Maximum acceptable connection interval (75 ms), Connection interval uses 1.25 ms units. */
#define SLAVE_LATENCY                   0                                           /**< Slave latency. */
#define CONN_SUP_TIMEOUT                MSEC_TO_UNITS(4000, UNIT_10_MS)             /**< Connection supervisory timeout (4 seconds), Supervision Timeout uses 10 ms units. */
#define FIRST_CONN_PARAMS_UPDATE_DELAY  APP_TIMER_TICKS(5000)                       /**< Time from initiating event (connect or start of notification) to first time sd_ble_gap_conn_param_update is called (5 seconds). */
#define NEXT_CONN_PARAMS_UPDATE_DELAY   APP_TIMER_TICKS(30000)                      /**< Time between each call to sd_ble_gap_conn_param_update after the first call (30 seconds). */
#define MAX_CONN_PARAMS_UPDATE_COUNT    3                                           /**< Number of attempts before giving up the connection parameter negotiation. */

#define DEAD_BEEF                       0xDEADBEEF                                  /**< Value used as error code on stack dump, can be used to identify stack location on stack unwind. */

#define UART_TX_BUF_SIZE                256                                         /**< UART TX buffer size. */
#define UART_RX_BUF_SIZE                256                                         /**< UART RX buffer size. */

#define HEART_RATE_MEAS_INTERVAL            APP_TIMER_TICKS(4000)

#define LED          25
#define LEDEN        3
#define VIN_DETECT   2
#define CHG          11
//#define APP_PA_LAN
//#ifdef APP_PA_LAN
//#define APP_PA_PIN              	13
//#define APP_LNA_PIN              	11

//#define APP_CHL_PIN                     15
//#define APP_CPS_PIN			6

//#define APP_AMP_PPI_CH_ID_SET   0
//#define APP_AMP_PPI_CH_ID_CLR   1
//#define APP_AMP_GPIOTE_CH_ID    0

//static void pa_lna_setup(void)
//{
//    uint32_t err_code;
//    nrf_gpio_cfg_output(APP_CPS_PIN);
//		nrf_gpio_pin_clear(APP_CPS_PIN); //enable
//	  	nrf_gpio_cfg_output(APP_PA_PIN);
//		nrf_gpio_pin_clear(APP_PA_PIN); //
//	  	nrf_gpio_cfg_output(APP_LNA_PIN);
//		nrf_gpio_pin_clear(APP_LNA_PIN); //

//                nrf_gpio_cfg_output(APP_CHL_PIN);
//		nrf_gpio_pin_clear(APP_CHL_PIN); //		

//    static ble_opt_t pa_lna_opts = {
//        .common_opt = {
//            .pa_lna = {
							
//                .pa_cfg = {
//                    .enable = 1,
//                    .active_high = 1,
//                    .gpio_pin = APP_PA_PIN
//                },
							
								
//                .lna_cfg = {
//                    .enable = 1,
//                    .active_high = 1,
//                    .gpio_pin = APP_LNA_PIN
//                },
								
//                .ppi_ch_id_set = APP_AMP_PPI_CH_ID_SET,
//                .ppi_ch_id_clr = APP_AMP_PPI_CH_ID_CLR,
//                .gpiote_ch_id = APP_AMP_GPIOTE_CH_ID
//            }
//        }
//    };
//    NRF_GPIO->DIRSET |= (1 << APP_PA_PIN) | (1 << APP_LNA_PIN) ;
//    err_code = sd_ble_opt_set(BLE_COMMON_OPT_PA_LNA, &pa_lna_opts);
//    APP_ERROR_CHECK(err_code);

//}
//#endif

//
char snhietdo[]="000";
char nhietdo[30];
char zeronhietdo1[]="C0";
char zeronhietdo2[]="C00";
char zeronhietdo[]="C";

char sbat[]="000";
char bat[]="B000";
char zero1[]="B0";
char zero2[]="B00";
char zero[]="B";
//
char sheart[]="000";
char heart[]="H000";
char zeroheart1[]="H0";
char zeroheart2[]="H00";
char zeroheart[]="H";
//
char sSpo2[]="000"; 
char spo20[]="O000";
char zerospo21[]="O0";
char zerospo22[]="O00";
char zerospo2[]="O";  
char ID[]="I0019";
//
char spower[]="0";
char power[]="S0";
char power2[]="S0";
char IDandHeart[13];
//uint32_t IDandPower[16];
uint32_t IDandHeartandSpo2[18];
uint32_t IDandHeartandSPo2andBat[22];
uint32_t IDandHeartandSPo2andBatandTemp[27];

double tempC;
double object_tempC;
double ambient_tempC;
uint16_t temp10=0;
uint64_t temp_ambian10=0;


int32_t        heart_rate=0;
int32_t        spo2_ob=0;
 char cmd[8], i, j;
 char rsp[256];
 int32_t ppg[12];

 int32_t status, opmode, hr, hr_conf, ibi, ibi_conf, act, r, spo2, spo2_conf, scd;
APP_TIMER_DEF(m_heart_rate_timer_id);                               /**< Heart rate measurement timer. */
static void heart_rate_meas_timeout_handler();
static void read_ppg();
static void config_maxm86146();
static void stop_maxm86146();


//

BLE_NUS_DEF(m_nus, NRF_SDH_BLE_TOTAL_LINK_COUNT);                                   /**< BLE NUS service instance. */
NRF_BLE_GATT_DEF(m_gatt);                                                           /**< GATT module instance. */
NRF_BLE_QWR_DEF(m_qwr);                                                             /**< Context for the Queued Write module.*/
BLE_ADVERTISING_DEF(m_advertising);  
/* TWI instance ID. */
#if TWI0_ENABLED
#define TWI_INSTANCE_ID     0
#endif
/* TWI instance. */                                               /**< Advertising module instance. */
static const nrf_drv_twi_t m_twi = NRF_DRV_TWI_INSTANCE(TWI_INSTANCE_ID);
static uint16_t   m_conn_handle          = BLE_CONN_HANDLE_INVALID;                 /**< Handle of the current connection. */
static uint16_t   m_ble_nus_max_data_len = BLE_GATT_ATT_MTU_DEFAULT - 3;            /**< Maximum length of data (in bytes) that can be transmitted to the peer by the Nordic UART service module. */
static ble_uuid_t m_adv_uuids[]          =                                          /**< Universally unique service identifier. */
{
    {BLE_UUID_NUS_SERVICE, NUS_SERVICE_UUID_TYPE}
};

const int addr =0x55; //0xAA;
int32_t Time_to_Read_PPG = 0;
#define MAXM86146_CFG 1
#ifdef MAXM86146_CFG
  #define PPG_SZ 36  //maxm86146
#else
  #define PPG_SZ 18  //maxm86161, max86141
#endif
#define ACCEL_SZ 6  // accel
#define SENSOR_SZ (PPG_SZ+ACCEL_SZ)
//#define ALGO_SZ 20  // 24 bytes is the algo normal size for 3x.12.0
#define ALGO_SZ 24  // 24 bytes is the algo normal size for 3x.13.x
#define ALGO_ONLY 1
#ifdef ALGO_ONLY
  #define TTL_SZ (ALGO_SZ)
#else
  #define TTL_SZ (PPG_SZ+ACCEL_SZ+ALGO_SZ)
#endif


//#define MFIO_PIN  31//4 Layers
//#define MFIO_PIN  5//2 layers
#define MFIO_PIN  10//BBC_V02

void MFIO_init_output(uint8_t value)
{
    nrf_gpio_cfg_output(MFIO_PIN);                      //GPIO config
  
    if(value == 0)
    {
        nrf_gpio_pin_clear(MFIO_PIN);
    }
    else
    {
        nrf_gpio_pin_set(MFIO_PIN);
    }
}
/**@brief Function for assert macro callback.
 *
 * @details This function will be called in case of an assert in the SoftDevice.
 *
 * @warning This handler is an example only and does not fit a final product. You need to analyse
 *          how your product is supposed to react in case of Assert.
 * @warning On assert from the SoftDevice, the system can only recover on reset.
 *
 * @param[in] line_num    Line number of the failing ASSERT call.
 * @param[in] p_file_name File name of the failing ASSERT call.
 */
void assert_nrf_callback(uint16_t line_num, const uint8_t * p_file_name)
{
    app_error_handler(DEAD_BEEF, line_num, p_file_name);
}

/**@brief Function for initializing the timer module.
 */
static void timers_init(void)
{
    ret_code_t err_code = app_timer_init();
    APP_ERROR_CHECK(err_code);

    err_code = app_timer_create(&m_heart_rate_timer_id,
                                APP_TIMER_MODE_REPEATED,
                                heart_rate_meas_timeout_handler);
    APP_ERROR_CHECK(err_code);
}

/**@brief Function for the GAP initialization.
 *
 * @details This function will set up all the necessary GAP (Generic Access Profile) parameters of
 *          the device. It also sets the permissions and appearance.
 */
static void gap_params_init(void)
{
    uint32_t                err_code;
    ble_gap_conn_params_t   gap_conn_params;
    ble_gap_conn_sec_mode_t sec_mode;

    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&sec_mode);

    err_code = sd_ble_gap_device_name_set(&sec_mode,
                                          (const uint8_t *) DEVICE_NAME,
                                          strlen(DEVICE_NAME));
    APP_ERROR_CHECK(err_code);

    memset(&gap_conn_params, 0, sizeof(gap_conn_params));

    gap_conn_params.min_conn_interval = MIN_CONN_INTERVAL;
    gap_conn_params.max_conn_interval = MAX_CONN_INTERVAL;
    gap_conn_params.slave_latency     = SLAVE_LATENCY;
    gap_conn_params.conn_sup_timeout  = CONN_SUP_TIMEOUT;

    err_code = sd_ble_gap_ppcp_set(&gap_conn_params);
    APP_ERROR_CHECK(err_code);
}


/**@brief Function for handling Queued Write Module errors.
 *
 * @details A pointer to this function will be passed to each service which may need to inform the
 *          application about an error.
 *
 * @param[in]   nrf_error   Error code containing information about what went wrong.
 */
static void nrf_qwr_error_handler(uint32_t nrf_error)
{
    APP_ERROR_HANDLER(nrf_error);
}


/**@brief Function for handling the data from the Nordic UART Service.
 *
 * @details This function will process the data received from the Nordic UART BLE Service and send
 *          it to the UART module.
 *
 * @param[in] p_evt       Nordic UART Service event.
 */
/**@snippet [Handling the data received over BLE] */
static void nus_data_handler(ble_nus_evt_t * p_evt)
{

    if (p_evt->type == BLE_NUS_EVT_RX_DATA)
    {
        uint32_t err_code;

        NRF_LOG_DEBUG("Received data from BLE NUS. Writing data on UART.");
        NRF_LOG_HEXDUMP_DEBUG(p_evt->params.rx_data.p_data, p_evt->params.rx_data.length);

        for (uint32_t i = 0; i < p_evt->params.rx_data.length; i++)
        {
            do
            {
                err_code = app_uart_put(p_evt->params.rx_data.p_data[i]);
                if ((err_code != NRF_SUCCESS) && (err_code != NRF_ERROR_BUSY))
                {
                    NRF_LOG_ERROR("Failed receiving NUS message. Error 0x%x. ", err_code);
                    APP_ERROR_CHECK(err_code);
                }
            } while (err_code == NRF_ERROR_BUSY);
        }
        if (p_evt->params.rx_data.p_data[p_evt->params.rx_data.length - 1] == '\r')
        {
            while (app_uart_put('\n') == NRF_ERROR_BUSY);
        }
    }

}
/**@snippet [Handling the data received over BLE] */


/**@brief Function for initializing services that will be used by the application.
 */
static void services_init(void)
{
    uint32_t           err_code;
    ble_nus_init_t     nus_init;
    nrf_ble_qwr_init_t qwr_init = {0};

    // Initialize Queued Write Module.
    qwr_init.error_handler = nrf_qwr_error_handler;

    err_code = nrf_ble_qwr_init(&m_qwr, &qwr_init);
    APP_ERROR_CHECK(err_code);

    // Initialize NUS.
    memset(&nus_init, 0, sizeof(nus_init));

    nus_init.data_handler = nus_data_handler;

    err_code = ble_nus_init(&m_nus, &nus_init);
    APP_ERROR_CHECK(err_code);
}


/**@brief Function for handling an event from the Connection Parameters Module.
 *
 * @details This function will be called for all events in the Connection Parameters Module
 *          which are passed to the application.
 *
 * @note All this function does is to disconnect. This could have been done by simply setting
 *       the disconnect_on_fail config parameter, but instead we use the event handler
 *       mechanism to demonstrate its use.
 *
 * @param[in] p_evt  Event received from the Connection Parameters Module.
 */
static void on_conn_params_evt(ble_conn_params_evt_t * p_evt)
{
    uint32_t err_code;

    if (p_evt->evt_type == BLE_CONN_PARAMS_EVT_FAILED)
    {
        err_code = sd_ble_gap_disconnect(m_conn_handle, BLE_HCI_CONN_INTERVAL_UNACCEPTABLE);
        APP_ERROR_CHECK(err_code);
    }
}


/**@brief Function for handling errors from the Connection Parameters module.
 *
 * @param[in] nrf_error  Error code containing information about what went wrong.
 */
static void conn_params_error_handler(uint32_t nrf_error)
{
    APP_ERROR_HANDLER(nrf_error);
}


/**@brief Function for initializing the Connection Parameters module.
 */
static void conn_params_init(void)
{
    uint32_t               err_code;
    ble_conn_params_init_t cp_init;

    memset(&cp_init, 0, sizeof(cp_init));

    cp_init.p_conn_params                  = NULL;
    cp_init.first_conn_params_update_delay = FIRST_CONN_PARAMS_UPDATE_DELAY;
    cp_init.next_conn_params_update_delay  = NEXT_CONN_PARAMS_UPDATE_DELAY;
    cp_init.max_conn_params_update_count   = MAX_CONN_PARAMS_UPDATE_COUNT;
    cp_init.start_on_notify_cccd_handle    = BLE_GATT_HANDLE_INVALID;
    cp_init.disconnect_on_fail             = false;
    cp_init.evt_handler                    = on_conn_params_evt;
    cp_init.error_handler                  = conn_params_error_handler;

    err_code = ble_conn_params_init(&cp_init);
    APP_ERROR_CHECK(err_code);
}


/**@brief Function for putting the chip into sleep mode.
 *
 * @note This function will not return.
 */
static void sleep_mode_enter(void)
{
    uint32_t err_code = bsp_indication_set(BSP_INDICATE_IDLE);
    APP_ERROR_CHECK(err_code);

    // Prepare wakeup buttons.
    //err_code = bsp_btn_ble_sleep_mode_prepare();
    //APP_ERROR_CHECK(err_code);

    // Go to system-off mode (this function will not return; wakeup will cause a reset).
    err_code = sd_power_system_off();
    //APP_ERROR_CHECK(err_code);
}


/**@brief Function for handling advertising events.
 *
 * @details This function will be called for advertising events which are passed to the application.
 *
 * @param[in] ble_adv_evt  Advertising event.
 */
static void on_adv_evt(ble_adv_evt_t ble_adv_evt)
{
    uint32_t err_code;

    switch (ble_adv_evt)
    {
        case BLE_ADV_EVT_FAST:
            err_code = bsp_indication_set(BSP_INDICATE_ADVERTISING);
            APP_ERROR_CHECK(err_code);
            break;
        case BLE_ADV_EVT_IDLE:
            sleep_mode_enter();
            break;
        default:
            break;
    }
}


/**@brief Function for handling BLE events.
 *
 * @param[in]   p_ble_evt   Bluetooth stack event.
 * @param[in]   p_context   Unused.
 */
static void ble_evt_handler(ble_evt_t const * p_ble_evt, void * p_context)
{
    uint32_t err_code;

    switch (p_ble_evt->header.evt_id)
    {
        case BLE_GAP_EVT_CONNECTED:
            NRF_LOG_INFO("BLE Connected");
            err_code = bsp_indication_set(BSP_INDICATE_CONNECTED);
            APP_ERROR_CHECK(err_code);
            m_conn_handle = p_ble_evt->evt.gap_evt.conn_handle;
            err_code = nrf_ble_qwr_conn_handle_assign(&m_qwr, m_conn_handle);
            APP_ERROR_CHECK(err_code);
            sd_ble_gap_tx_power_set(BLE_GAP_TX_POWER_ROLE_CONN, m_conn_handle, 4);
            config_maxm86146();
            NRF_LOG_INFO("HR start");

            break;

        case BLE_GAP_EVT_DISCONNECTED:
            NRF_LOG_INFO("BLE Disconnected");
            // LED indication will be changed when advertising starts.
            m_conn_handle = BLE_CONN_HANDLE_INVALID;
            //Stop alg
            stop_maxm86146();
            NRF_LOG_INFO("HR stop");
            app_timer_stop(m_heart_rate_timer_id);
    
            break;

        case BLE_GAP_EVT_PHY_UPDATE_REQUEST:
        {
            NRF_LOG_DEBUG("PHY update request.");
            ble_gap_phys_t const phys =
            {
                .rx_phys = BLE_GAP_PHY_AUTO,
                .tx_phys = BLE_GAP_PHY_AUTO,
            };
            err_code = sd_ble_gap_phy_update(p_ble_evt->evt.gap_evt.conn_handle, &phys);
            APP_ERROR_CHECK(err_code);
        } break;

        case BLE_GAP_EVT_SEC_PARAMS_REQUEST:
            // Pairing not supported
            err_code = sd_ble_gap_sec_params_reply(m_conn_handle, BLE_GAP_SEC_STATUS_PAIRING_NOT_SUPP, NULL, NULL);
            APP_ERROR_CHECK(err_code);
            break;

        case BLE_GATTS_EVT_SYS_ATTR_MISSING:
            // No system attributes have been stored.
            err_code = sd_ble_gatts_sys_attr_set(m_conn_handle, NULL, 0, 0);
            APP_ERROR_CHECK(err_code);
            break;

        case BLE_GATTC_EVT_TIMEOUT:
            // Disconnect on GATT Client timeout event.
            err_code = sd_ble_gap_disconnect(p_ble_evt->evt.gattc_evt.conn_handle,
                                             BLE_HCI_REMOTE_USER_TERMINATED_CONNECTION);
            APP_ERROR_CHECK(err_code);
            break;

        case BLE_GATTS_EVT_TIMEOUT:
            // Disconnect on GATT Server timeout event.
            err_code = sd_ble_gap_disconnect(p_ble_evt->evt.gatts_evt.conn_handle,
                                             BLE_HCI_REMOTE_USER_TERMINATED_CONNECTION);
            APP_ERROR_CHECK(err_code);
            break;

        default:
            // No implementation needed.
            break;
    }
}


/**@brief Function for the SoftDevice initialization.
 *
 * @details This function initializes the SoftDevice and the BLE event interrupt.
 */
static void ble_stack_init(void)
{
    ret_code_t err_code;

    err_code = nrf_sdh_enable_request();
    APP_ERROR_CHECK(err_code);

    // Configure the BLE stack using the default settings.
    // Fetch the start address of the application RAM.
    uint32_t ram_start = 0;
    err_code = nrf_sdh_ble_default_cfg_set(APP_BLE_CONN_CFG_TAG, &ram_start);
    APP_ERROR_CHECK(err_code);

    // Enable BLE stack.
    err_code = nrf_sdh_ble_enable(&ram_start);
    APP_ERROR_CHECK(err_code);

    // Register a handler for BLE events.
    NRF_SDH_BLE_OBSERVER(m_ble_observer, APP_BLE_OBSERVER_PRIO, ble_evt_handler, NULL);
}


/**@brief Function for handling events from the GATT library. */
void gatt_evt_handler(nrf_ble_gatt_t * p_gatt, nrf_ble_gatt_evt_t const * p_evt)
{
    if ((m_conn_handle == p_evt->conn_handle) && (p_evt->evt_id == NRF_BLE_GATT_EVT_ATT_MTU_UPDATED))
    {
        m_ble_nus_max_data_len = p_evt->params.att_mtu_effective - OPCODE_LENGTH - HANDLE_LENGTH;
        NRF_LOG_INFO("Data len is set to 0x%X(%d)", m_ble_nus_max_data_len, m_ble_nus_max_data_len);
    }
    NRF_LOG_DEBUG("ATT MTU exchange completed. central 0x%x peripheral 0x%x",
                  p_gatt->att_mtu_desired_central,
                  p_gatt->att_mtu_desired_periph);
}


/**@brief Function for initializing the GATT library. */
void gatt_init(void)
{
    ret_code_t err_code;

    err_code = nrf_ble_gatt_init(&m_gatt, gatt_evt_handler);
    APP_ERROR_CHECK(err_code);

    err_code = nrf_ble_gatt_att_mtu_periph_set(&m_gatt, NRF_SDH_BLE_GATT_MAX_MTU_SIZE);
    APP_ERROR_CHECK(err_code);
}


/**@brief Function for handling events from the BSP module.
 *
 * @param[in]   event   Event generated by button press.
 */
void bsp_event_handler(bsp_event_t event)
{
    uint32_t err_code;
    switch (event)
    {
        case BSP_EVENT_SLEEP:
            sleep_mode_enter();
            break;

        case BSP_EVENT_DISCONNECT:
            err_code = sd_ble_gap_disconnect(m_conn_handle, BLE_HCI_REMOTE_USER_TERMINATED_CONNECTION);
            if (err_code != NRF_ERROR_INVALID_STATE)
            {
                APP_ERROR_CHECK(err_code);
            }
            break;

        case BSP_EVENT_WHITELIST_OFF:
            if (m_conn_handle == BLE_CONN_HANDLE_INVALID)
            {
                err_code = ble_advertising_restart_without_whitelist(&m_advertising);
                if (err_code != NRF_ERROR_INVALID_STATE)
                {
                    APP_ERROR_CHECK(err_code);
                }
            }
            break;

        default:
            break;
    }
}


/**@brief   Function for handling app_uart events.
 *
 * @details This function will receive a single character from the app_uart module and append it to
 *          a string. The string will be be sent over BLE when the last character received was a
 *          'new line' '\n' (hex 0x0A) or if the string has reached the maximum data length.
 */
/**@snippet [Handling the data received over UART] */
void uart_event_handle(app_uart_evt_t * p_event)
{
    static uint8_t data_array[BLE_NUS_MAX_DATA_LEN];
    static uint8_t index = 0;
    uint32_t       err_code;

    switch (p_event->evt_type)
    {
        case APP_UART_DATA_READY:
            UNUSED_VARIABLE(app_uart_get(&data_array[index]));
            index++;

            if ((data_array[index - 1] == '\n') ||
                (data_array[index - 1] == '\r') ||
                (index >= m_ble_nus_max_data_len))
            {
                if (index > 1)
                {
                    NRF_LOG_DEBUG("Ready to send data over BLE NUS");
                    NRF_LOG_HEXDUMP_DEBUG(data_array, index);

                    do
                    {
                        uint16_t length = (uint16_t)index;
                        err_code = ble_nus_data_send(&m_nus, data_array, &length, m_conn_handle);
                        if ((err_code != NRF_ERROR_INVALID_STATE) &&
                            (err_code != NRF_ERROR_RESOURCES) &&
                            (err_code != NRF_ERROR_NOT_FOUND))
                        {
                            APP_ERROR_CHECK(err_code);
                        }
                    } while (err_code == NRF_ERROR_RESOURCES);
                }

                index = 0;
            }
            break;

        case APP_UART_COMMUNICATION_ERROR:
            APP_ERROR_HANDLER(p_event->data.error_communication);
            break;

        case APP_UART_FIFO_ERROR:
            APP_ERROR_HANDLER(p_event->data.error_code);
            break;

        default:
            break;
    }
}
/**@snippet [Handling the data received over UART] */




/**@brief Function for initializing the Advertising functionality.
 */
static void advertising_init(void)
{
    uint32_t               err_code;
    ble_advertising_init_t init;

    memset(&init, 0, sizeof(init));

    init.advdata.name_type          = BLE_ADVDATA_FULL_NAME;
    init.advdata.include_appearance = false;
    init.advdata.flags              = BLE_GAP_ADV_FLAGS_LE_ONLY_GENERAL_DISC_MODE;

    init.srdata.uuids_complete.uuid_cnt = sizeof(m_adv_uuids) / sizeof(m_adv_uuids[0]);
    init.srdata.uuids_complete.p_uuids  = m_adv_uuids;

    init.config.ble_adv_fast_enabled  = true;
    init.config.ble_adv_fast_interval = APP_ADV_INTERVAL;
    init.config.ble_adv_fast_timeout  = APP_ADV_DURATION;
    init.evt_handler = on_adv_evt;

    err_code = ble_advertising_init(&m_advertising, &init);
    APP_ERROR_CHECK(err_code);

    ble_advertising_conn_cfg_tag_set(&m_advertising, APP_BLE_CONN_CFG_TAG);
}


/**@brief Function for initializing buttons and leds.
 *
 * @param[out] p_erase_bonds  Will be true if the clear bonding button was pressed to wake the application up.
 */
static void buttons_leds_init(bool * p_erase_bonds)
{
    bsp_event_t startup_event;

    uint32_t err_code = bsp_init(BSP_INIT_LEDS | BSP_INIT_BUTTONS, bsp_event_handler);
    APP_ERROR_CHECK(err_code);

    err_code = bsp_btn_ble_init(NULL, &startup_event);
    APP_ERROR_CHECK(err_code);

    *p_erase_bonds = (startup_event == BSP_EVENT_CLEAR_BONDING_DATA);
}


/**@brief Function for initializing the nrf log module.
 */
static void log_init(void)
{
    ret_code_t err_code = NRF_LOG_INIT(NULL);
    APP_ERROR_CHECK(err_code);

    NRF_LOG_DEFAULT_BACKENDS_INIT();
}


/**@brief Function for initializing power management.
 */
static void power_management_init(void)
{
    ret_code_t err_code;
    err_code = nrf_pwr_mgmt_init();
    APP_ERROR_CHECK(err_code);
}


/**@brief Function for handling the idle state (main loop).
 *
 * @details If there is no pending log operation, then sleep until next the next event occurs.
 */
static void idle_state_handle(void)
{
    if (NRF_LOG_PROCESS() == false)
    {
        nrf_pwr_mgmt_run();
    }
}


/**@brief Function for starting advertising.
 */
static void advertising_start(void)
{
    uint32_t err_code = ble_advertising_start(&m_advertising, BLE_ADV_MODE_FAST);
    APP_ERROR_CHECK(err_code);
}





void twi_init (void)
{
    ret_code_t err_code;

    const nrf_drv_twi_config_t twi_config = {
    
     //  .scl                = 30,
      // .sda                = 29, //BBC 
       .scl                = 29,// BBC_V2
       .sda                = 20,
       .frequency          = NRF_DRV_TWI_FREQ_400K,
       .interrupt_priority = APP_IRQ_PRIORITY_HIGH,
       .clear_bus_init     = false
    };

    err_code = nrf_drv_twi_init(&m_twi, &twi_config, NULL, NULL);
    APP_ERROR_CHECK(err_code);

    nrf_drv_twi_enable(&m_twi);
}
/**@brief Application main function.
 */
 void read_ppg(void) {
    char cmd[8], i, j, samples;
    char rsp[3000];
    int32_t ppg[12];
    int32_t accel[3];
   
    int32_t scnt = 0;
    int32_t ptr = 0;
    int32_t sptr = 0;
     MFIO_init_output(0); nrf_delay_us(300);
    Time_to_Read_PPG = 0;
#if 0
// 2.1
    cmd[0] = 0x00;
    cmd[1] = 0x00;
    nrf_drv_twi_tx(&m_twi,addr, cmd, 2);
    nrf_delay_us(100);
     nrf_drv_twi_rx(&m_twi,addr, rsp, 2);
//    NRF_LOG_INFO("2.1 Status: %x %x\n\r", rsp[0], rsp[1]);
#endif
// 2.2
    cmd[0] = 0x12;
    cmd[1] = 0x00;
    nrf_drv_twi_tx(&m_twi,addr, cmd, 2,false);
    nrf_delay_us(100);
     nrf_drv_twi_rx(&m_twi,addr, rsp, 2);
//    NRF_LOG_INFO("2.2 Status: %x %x\n\r", rsp[0], rsp[1]);
    samples = rsp[1];
//    NRF_LOG_INFO("num samples %d, (num*ttl)+1 %d\n\r",  rsp[1], TTL_SZ*samples+1);
//    NRF_LOG_INFO("num smpls %d \n\r",  samples);
    scnt = rsp[1];
// 2.3
    cmd[0] = 0x12;
    cmd[1] = 0x01;
    nrf_drv_twi_tx(&m_twi,addr, cmd, 2,false);
    nrf_delay_us(100);
//    nrf_delay_ms(1);
     nrf_drv_twi_rx(&m_twi,addr, rsp, 1+(TTL_SZ*samples));
        status = rsp[0];
        
        sptr = 1;
        for (i = 0; i < scnt; i++) {
#ifdef ALGO_ONLY
            ptr = sptr;
#else
            ptr = sptr;
            ppg[0] = (rsp[ptr+0] << 16) | (rsp[ptr+1] << 8) | (rsp[ptr+2]);
            ppg[1] = (rsp[ptr+3] << 16) | (rsp[ptr+4] << 8) | (rsp[ptr+5]);
            ppg[2] = (rsp[ptr+6] << 16) | (rsp[ptr+7] << 8) | (rsp[ptr+8]);
            ppg[3] = (rsp[ptr+9] << 16) | (rsp[ptr+10] << 8) | (rsp[ptr+11]);
            NRF_LOG_INFO("%d,%d,%d,%d,", ppg[0], ppg[1], ppg[2], ppg[3]);
            ptr = sptr + SENSOR_SZ;
#endif
//            NRF_LOG_INFO("ptr %d ttlsiz %d ", ptr, TTL_SZ);
            opmode = rsp[ptr];
            hr =  (rsp[ptr+1] << 8) + rsp[ptr+2];
            hr_conf = rsp[ptr+3];
            ibi = rsp[ptr+4];
            ibi_conf = (rsp[ptr+5] << 8) + rsp[ptr+6];
            act = rsp[ptr+7];
            r = (rsp[ptr+8] << 8) + rsp[ptr+9];
            spo2_conf = rsp[ptr+10];
            spo2 = (rsp[ptr+11] << 8) + rsp[ptr+12];
            scd = rsp[ptr+19];

            sptr += (TTL_SZ);

            NRF_LOG_INFO("%d,%d,%d,%d,%d", hr, hr_conf, spo2, spo2_conf, scd);
            NRF_LOG_INFO("\n\r");
        }
         MFIO_init_output(1);
} 
void config_max30208(){
  max30208_init();
  enableDATARDY();
  startConvert();
}
void read_temp_max30208(){
    uint8_t counter = 5;
    while(counter--){
        uint8_t mask = getINT();
        if (mask & MAX30208_INT_STATUS_TEMP_RDY) {
            uint8_t samplesTaken = 0;
            check();
            tempC = readTemperature();

            startConvert();
        }
        else{
          startConvert();
        }
        nrf_delay_ms(200);
    }

}
//
int count=0;
int count_temp=0;
bool flag_maxm=0;
static void heart_rate_meas_timeout_handler()
{
count=count+1;
NRF_LOG_INFO("COUNT=:%d",count);
 //mlx90632_get_object_temp(&object_tempC);
 temp10=(object_tempC)*10;
  NRF_LOG_INFO("Temp=:%d",temp10);
if(temp10<280)
{
count_temp++;
if(count_temp==3)
{
NRF_LOG_INFO("Temp <320.OFF HR");
//stop_maxm86146();
flag_maxm=0;
count_temp=0;
}
}else
{
if(flag_maxm==0)
{
flag_maxm=1;
config_maxm86146();
NRF_LOG_INFO("Temp >320.ON HR AGAIN");

}

  }

 read_ppg();



    heart_rate =(hr/10);   
    spo2_ob =(spo2/10);
         if(scd==1|scd==2)
         
        {
        heart_rate=0;
        }
        //if(hr_conf==0)
        //{
        //heart_rate=0;
        //}

        if(count<5)
        {
          heart_rate=0;
          }

        if(heart_rate==0)
        {
        spo2_ob=0;
        }
       
  //  NRF_LOG_INFO("HR2 = %d",heart_rate);
   // NRF_LOG_INFO("SpO22 = %d",spo2_ob);



   //Battery

           nrf_drv_saadc_sample();
           itoa(percentage_batt_lvl, sbat, 10);  
      
            if( percentage_batt_lvl==0 || percentage_batt_lvl<10)
           {
            strcpy(bat,zero2);
            strcat(bat, sbat);
            }
            else  if( percentage_batt_lvl>=10 && percentage_batt_lvl <100 )
           {
            strcpy(bat,zero1);
            strcat(bat, sbat);
            }
            else
            {
            strcpy(bat,zero);
            strcat(bat, sbat);
            }
  //SPo2

            itoa(spo2_ob, sSpo2, 10);
            if( spo2_ob>=0 && spo2_ob<10)
           {
            strcpy(spo20,zerospo22);
            strcat(spo20, sSpo2);
         
            }         
            else  if( spo2_ob>10 && spo2_ob<100 )
           {
            strcpy(spo20,zerospo21);
            strcat(spo20, sSpo2);
            }
            //

  //HR
            itoa(heart_rate, sheart, 10);
            if( heart_rate>=0 && heart_rate<10)
           {
            strcpy(heart,zeroheart2);
            strcat(heart, sheart);
         
            }
            else  if( heart_rate>10 && heart_rate <100 )
           {
            strcpy(heart,zeroheart1);
            strcat(heart, sheart);
            }
            else  if( heart_rate>=100 )
            {
            strcpy(heart,zeroheart);
            strcat(heart, sheart);
            }
        //Nhietdo
            read_temp_max30208();
            sprintf(nhietdo,"%s%d.%02d",zeronhietdo,(int)tempC,(int)((tempC - (int)tempC)*100));


            NRF_LOG_INFO("Nhiet do %s",nhietdo);
            NRF_LOG_FLUSH();
        //SAC
            uint32_t charge_state = nrf_gpio_pin_read(CHG);
            uint32_t power_state = nrf_gpio_pin_read(VIN_DETECT);
            itoa(power_state, spower, 10);
            if(power_state == 1 && charge_state == 0){
              strcpy(power,power2);
              strcat(power, spower);
            }
            else if(power_state == 0 && charge_state == 1){
              strcpy(power,power2);
              strcat(power, spower);
            }
             NRF_LOG_INFO("power: %s", power);
             NRF_LOG_FLUSH();
           // ID and Heart
            strcpy(IDandHeart,ID);
            strcat(IDandHeart, power);

            strcat(IDandHeart,heart);

            
            // ID and Heart and Spo2
            strcpy(IDandHeartandSpo2,IDandHeart);
            strcat(IDandHeartandSpo2, spo20);
           // NRF_LOG_INFO("IN IDandHeartandSpo2 : %s \n", IDandHeartandSpo2);  
            //
            strcpy(IDandHeartandSPo2andBat,IDandHeartandSpo2);
            strcat(IDandHeartandSPo2andBat,bat );

            NRF_LOG_INFO("bat%s\n", bat);  
            NRF_LOG_FLUSH();

           // NRF_LOG_INFO("%s \n", IDandHeartandSPo2andBat);  
         
         	
            strcpy(IDandHeartandSPo2andBatandTemp,IDandHeartandSPo2andBat);
            strcat(IDandHeartandSPo2andBatandTemp,nhietdo );
            NRF_LOG_INFO("%s\n", IDandHeartandSPo2andBatandTemp);  
            NRF_LOG_FLUSH();
            uint16_t length = 27;
            ble_nus_data_send(&m_nus, IDandHeartandSPo2andBatandTemp, &length, m_conn_handle);
          
            //ble_nus_data_send(&m_nus, "01234567890123456789012345678901234567890", &length, m_conn_handle);
}
//
void config_maxm86146(void)
{
    MFIO_init_output(1);
    nrf_delay_ms(10);
    //rst = 1;


    MFIO_init_output(0);; nrf_delay_us(300);
    cmd[0] = 0x02;
    cmd[1] = 0x00;
    nrf_drv_twi_tx(&m_twi,addr, cmd, 2,false);
    MFIO_init_output(1); nrf_delay_ms(2); MFIO_init_output(0);; nrf_delay_us(300);
    nrf_drv_twi_rx(&m_twi,addr, rsp, 2);
    MFIO_init_output(1); MFIO_init_output(0);; nrf_delay_us(300);
    NRF_LOG_INFO("02 00 Status, Operating Mode 1: %x %x\n\r", rsp[0], rsp[1]);
     NRF_LOG_FLUSH();
//
    cmd[0] = 0x01;
    cmd[1] = 0x00;
    cmd[2] = 0x00;
    nrf_drv_twi_tx(&m_twi,addr, cmd, 3,false);
    nrf_delay_ms(1500);
    MFIO_init_output(1); nrf_delay_ms(2); MFIO_init_output(0);; nrf_delay_us(300);
    nrf_drv_twi_rx(&m_twi,addr, rsp, 2);
    MFIO_init_output(1); MFIO_init_output(0);; nrf_delay_us(300);
    NRF_LOG_INFO("Exit bootloader" );
     NRF_LOG_FLUSH();
     //
    cmd[0] = 0x02;
    cmd[1] = 0x00;
    nrf_drv_twi_tx(&m_twi,addr, cmd, 2,false);
    MFIO_init_output(1); nrf_delay_ms(2); MFIO_init_output(0);; nrf_delay_us(300);
    nrf_drv_twi_rx(&m_twi,addr, rsp, 2);
    MFIO_init_output(1); MFIO_init_output(0);; nrf_delay_us(300);
    NRF_LOG_INFO("02 00 Status, Operating Mode 2: %x %x\n\r", rsp[0], rsp[1]);
     NRF_LOG_FLUSH();
     // rd ver    
    cmd[0] = 0xFF;
    cmd[1] = 0x03;
    nrf_drv_twi_tx(&m_twi,addr, cmd, 2,false);
     MFIO_init_output(1); nrf_delay_ms(2);  MFIO_init_output(0); nrf_delay_us(300);
    nrf_delay_us(300);
     nrf_drv_twi_rx(&m_twi,addr, rsp, 4);
     MFIO_init_output(1);  MFIO_init_output(0); nrf_delay_us(300);
    NRF_LOG_INFO("Ver: %d %d %d %d\n\r", rsp[0], rsp[1], rsp[2], rsp[3]);
     NRF_LOG_FLUSH();
// 1.3 sensor and algo data
    cmd[0] = 0x10;
    cmd[1] = 0x00;
#ifdef ALGO_ONLY
    cmd[2] = 0x02;  // algo data
#else
    cmd[2] = 0x03;  // sensor + algo data
#endif
    nrf_drv_twi_tx(&m_twi,addr, cmd, 3,false);
     MFIO_init_output(1); nrf_delay_ms(2);  MFIO_init_output(0); nrf_delay_us(300);
     nrf_drv_twi_rx(&m_twi,addr, rsp, 1);
     MFIO_init_output(1);  MFIO_init_output(0); nrf_delay_us(300);
    NRF_LOG_INFO("1.3 Status: %x\n\r", rsp[0]);
     NRF_LOG_FLUSH();
// 1.7 cont hr, spo2
    cmd[0] = 0x50;
    cmd[1] = 0x07;
    cmd[2] = 0x0A;
    cmd[3] = 0x00;
    nrf_drv_twi_tx(&m_twi,addr, cmd, 4,false);
     MFIO_init_output(1); nrf_delay_ms(2);  MFIO_init_output(0); nrf_delay_us(300);
     nrf_drv_twi_rx(&m_twi,addr, rsp, 1);
     MFIO_init_output(1);  MFIO_init_output(0); nrf_delay_us(300);
    NRF_LOG_INFO("1.7 Status: %x\n\r", rsp[0]);
     NRF_LOG_FLUSH();
    //// 1.5 table 8 (Khanh)
    cmd[0] = 0x10;
    cmd[1] = 0x02;
    //cmd[2] = 0xF0; // 40ms *240 =6s respone data 
    cmd[2] = 0x64; // 40ms *100 =4s respone data 
   //cmd[2] = 0x14; // 40ms *14 =1s respone data 
    nrf_drv_twi_tx(&m_twi,addr, cmd, 3,false);
     MFIO_init_output(1); nrf_delay_ms(2);  MFIO_init_output(0); nrf_delay_us(300);
     nrf_drv_twi_rx(&m_twi,addr, rsp, 1);
     MFIO_init_output(1);  MFIO_init_output(0); nrf_delay_us(300);
    NRF_LOG_INFO("1.5 Table 8: %x\n\r", rsp[0]);
     NRF_LOG_FLUSH();
//1.11 rd AFE part id
    cmd[0] = 0x41;
    cmd[1] = 0x00;
    cmd[2] = 0xFF;
    nrf_drv_twi_tx(&m_twi,addr, cmd, 3,false);
     MFIO_init_output(1); nrf_delay_ms(2);  MFIO_init_output(0); nrf_delay_us(300);
     nrf_drv_twi_rx(&m_twi,addr, rsp, 2);
     MFIO_init_output(1);  MFIO_init_output(0); nrf_delay_us(300);
    NRF_LOG_INFO("1.11 part id afe %x %x\n\r", rsp[0], rsp[1]);
     NRF_LOG_FLUSH();
//1.13 rd accel who
    cmd[0] = 0x41;
    cmd[1] = 0x04;
    cmd[2] = 0x0F;
    nrf_drv_twi_tx(&m_twi,addr, cmd, 3,false);
     MFIO_init_output(1); nrf_delay_ms(2);  MFIO_init_output(0); nrf_delay_us(300);
     nrf_drv_twi_rx(&m_twi,addr, rsp, 2);
     MFIO_init_output(1);  MFIO_init_output(0); nrf_delay_us(300);
    NRF_LOG_INFO("1.12 who accel %x %x\n\r", rsp[0], rsp[1]);
     NRF_LOG_FLUSH();
#ifdef MAXM86146_CFG
#if 1
//1.20 Sec 4.1  map leds to slots for MAXM86146
    cmd[0] = 0x50;
    cmd[1] = 0x07;
    cmd[2] = 0x19;
    cmd[3] = 0x13;
    cmd[4] = 0x56;
    cmd[5] = 0x00;
    nrf_drv_twi_tx(&m_twi,addr, cmd, 6,false);
     MFIO_init_output(1); nrf_delay_ms(2);  MFIO_init_output(0); nrf_delay_us(300);
     nrf_drv_twi_rx(&m_twi,addr, rsp, 1);
     MFIO_init_output(1);  MFIO_init_output(0); nrf_delay_us(300);
    NRF_LOG_INFO("map leds to slots%x\n\r", rsp[0]);
     NRF_LOG_FLUSH();
//1.21  map HR inputs to slots
    cmd[0] = 0x50;
    cmd[1] = 0x07;
    cmd[2] = 0x17;
    cmd[3] = 0x00;
    cmd[4] = 0x11;
    nrf_drv_twi_tx(&m_twi,addr, cmd, 5,false);
     MFIO_init_output(1); nrf_delay_ms(2);  MFIO_init_output(0); nrf_delay_us(300);
     nrf_drv_twi_rx(&m_twi,addr, rsp, 1);
     MFIO_init_output(1);  MFIO_init_output(0); nrf_delay_us(300);
    NRF_LOG_INFO("map HR to slots/PDs %x\n\r", rsp[0]);
     NRF_LOG_FLUSH();
//1.22  map SpO2 inputs to slots
    cmd[0] = 0x50;
    cmd[1] = 0x07;
    cmd[2] = 0x18;
    cmd[3] = 0x30;
    cmd[4] = 0x20;
    nrf_drv_twi_tx(&m_twi,addr, cmd, 5,false);
     MFIO_init_output(1); nrf_delay_ms(2);  MFIO_init_output(0); nrf_delay_us(300);
     nrf_drv_twi_rx(&m_twi,addr, rsp, 1);
     MFIO_init_output(1);  MFIO_init_output(0); nrf_delay_us(300);


   
#endif
#if 0
/*
//1.20 Sec 4.1  map leds to slots for MAXM86146
    cmd[0] = 0x50;
    cmd[1] = 0x07;
    cmd[2] = 0x19;
    cmd[3] = 0x15;
    cmd[4] = 0x60;
    cmd[5] = 0x00;
    nrf_drv_twi_tx(&m_twi,addr, cmd, 6);
     MFIO_init_output(1); nrf_delay_ms(2);  MFIO_init_output(0); nrf_delay_us(300);
     nrf_drv_twi_rx(&m_twi,addr, rsp, 1);
     MFIO_init_output(1);  MFIO_init_output(0); nrf_delay_us(300);
    NRF_LOG_INFO("map leds to slots%x\n\r", rsp[0]);
//1.21  map HR inputs to slots
    cmd[0] = 0x50;
    cmd[1] = 0x07;
    cmd[2] = 0x17;
    cmd[3] = 0x00;
    cmd[4] = 0x01;
    nrf_drv_twi_tx(&m_twi,addr, cmd, 5);
     MFIO_init_output(1); nrf_delay_ms(2);  MFIO_init_output(0); nrf_delay_us(300);
     nrf_drv_twi_rx(&m_twi,addr, rsp, 1);
     MFIO_init_output(1);  MFIO_init_output(0); nrf_delay_us(300);
    NRF_LOG_INFO("map HR to slots/PDs %x\n\r", rsp[0]);
//1.22  map SpO2 inputs to slots
    cmd[0] = 0x50;
    cmd[1] = 0x07;
    cmd[2] = 0x18;
    cmd[3] = 0x20;
    cmd[4] = 0x10;
    nrf_drv_twi_tx(&m_twi,addr, cmd, 5);
     MFIO_init_output(1); nrf_delay_ms(2);  MFIO_init_output(0); nrf_delay_us(300);
     nrf_drv_twi_rx(&m_twi,addr, rsp, 1);
     MFIO_init_output(1);  MFIO_init_output(0); nrf_delay_us(300);
    NRF_LOG_INFO("map SpO2 to slots/PDs %x\n\r", rsp[0]);
    */
#endif
#endif // MAXM86146_CFG

// 1.3
    cmd[0] = 0x52;
    cmd[1] = 0x07;
    cmd[2] = 0x01;
    nrf_drv_twi_tx(&m_twi,addr, cmd, 3,false);
    nrf_delay_ms(465);
     nrf_drv_twi_rx(&m_twi,addr, rsp, 1);
    NRF_LOG_INFO("1.14 status: %x\n\r", rsp[0]);
     NRF_LOG_FLUSH();
     MFIO_init_output(1);
    //
 app_timer_start(m_heart_rate_timer_id, HEART_RATE_MEAS_INTERVAL, NULL);
}
//Stop HR Sensor
void stop_maxm86146(void)
{
count=0;
  MFIO_init_output(1);  MFIO_init_output(0); nrf_delay_us(300);
      cmd[0] = 0x52;
      cmd[1] = 0x07;
      cmd[2] = 0x00;
   nrf_drv_twi_tx(&m_twi,addr, cmd, 3,false);
   MFIO_init_output(1); nrf_delay_ms(120);  MFIO_init_output(0); nrf_delay_us(300);
}

int main(void)
{

    bool erase_bonds;
    

    // Initialize.
     //nrf_drv_gpiote_init();
     // nrf_gpio_cfg_output(4); 
     //nrf_gpio_cfg_output(SKY66112_CDS); 
     //nrf_gpio_pin_set(4);
     //nrf_gpio_pin_set(SKY66112_CHL);
     //nrf_delay_ms(500);
     //nrf_gpio_pin_set(19);
     //nrf_delay_ms(500);
    ret_code_t err_code;
   






    log_init();
    timers_init();

    power_management_init();

    ble_stack_init();

    gap_params_init();
    gatt_init();
    services_init();
    advertising_init();
    conn_params_init();
    NRF_LOG_INFO("BBC Start");
    NRF_LOG_INFO("Please connect to BLE Gateway to start HR sensor");
    //nrf_gpio_cfg_output(LEDEN);
    //nrf_gpio_cfg_output(LED);
    nrf_gpio_cfg_input(CHG,NRF_GPIO_PIN_NOPULL);
    nrf_gpio_cfg_input(VIN_DETECT,NRF_GPIO_PIN_NOPULL);
    //nrf_gpio_pin_write(LEDEN, 1);
    //nrf_gpio_pin_write(LED, 1);
    //while(1){
    //}
    //pa_lna_setup();
    sd_ble_gap_tx_power_set(BLE_GAP_TX_POWER_ROLE_ADV, m_advertising.adv_handle ,4);
    advertising_start();
    twi_init();
 
    //mlx90632_init();
    config_max30208();
    stop_maxm86146();
    adc_configure();
    //while(1){
    //     nrf_drv_saadc_sample();
    //     NRF_LOG_INFO("adc:%d", percentage_batt_lvl);
    //     NRF_LOG_FLUSH();
    //     nrf_delay_ms(1000);
    //}
   // config_maxm86146();
    
////Sets which LED index is to be used by the SCD

//  MFIO_init_output(1); MFIO_init_output(0);; nrf_delay_us(300);
//    cmd[0] = 0xE5;
//    cmd[1] = 0x00;
//    nrf_drv_twi_tx(&m_twi,addr, cmd, 2,false);
//    MFIO_init_output(1); nrf_delay_ms(2); MFIO_init_output(0);; nrf_delay_us(300);
//    nrf_drv_twi_rx(&m_twi,addr, rsp, 2);
//    MFIO_init_output(1); MFIO_init_output(0);; nrf_delay_us(300);
//    NRF_LOG_INFO("Sets which LED index is to be used by the SCD: %x %x\n\r", rsp[0], rsp[1]);
//    NRF_LOG_FLUSH();
// // Set output mode to algorithm data (SCD Only when used with 1.7).    
//    cmd[0] = 0x10;
//    cmd[1] = 0x00;
//    cmd[2] = 0x02;
//    nrf_drv_twi_tx(&m_twi,addr, cmd, 3,false);
//     MFIO_init_output(1); nrf_delay_ms(2);  MFIO_init_output(0); nrf_delay_us(300);
//    nrf_delay_us(300);
//     nrf_drv_twi_rx(&m_twi,addr, rsp, 2);
//     MFIO_init_output(1);  MFIO_init_output(0); nrf_delay_us(300);
//    NRF_LOG_INFO("Set output mode to algorithm data  %x %x\n\r", rsp[0], rsp[1]);
//     NRF_LOG_FLUSH();
//// Set sensor hub interrupt threshold.
//    cmd[0] = 0x10;
//    cmd[1] = 0x01;
//    cmd[2] = 0x01;

//    nrf_drv_twi_tx(&m_twi,addr, cmd, 3,false);
//     MFIO_init_output(1); nrf_delay_ms(2);  MFIO_init_output(0); nrf_delay_us(300);
//     nrf_drv_twi_rx(&m_twi,addr, rsp, 2);
//     MFIO_init_output(1);  MFIO_init_output(0); nrf_delay_us(300);
//    NRF_LOG_INFO("Set sensor hub interrupt threshold :%x %x\n\r", rsp[0], rsp[1]);
//     NRF_LOG_FLUSH();
//// Set the samples report period to 40ms.Samples report rate to be one report per every sensor sample
//    cmd[0] = 0x10;
//    cmd[1] = 0x02;
//    cmd[2] = 0x01;
//    nrf_drv_twi_tx(&m_twi,addr, cmd, 3,false);
//     MFIO_init_output(1); nrf_delay_ms(2);  MFIO_init_output(0); nrf_delay_us(300);
//     nrf_drv_twi_rx(&m_twi,addr, rsp, 2);
//     MFIO_init_output(1);  MFIO_init_output(0); nrf_delay_us(300);
//    NRF_LOG_INFO("Set the samples report:%x %x\n\r", rsp[0], rsp[1]);
//     NRF_LOG_FLUSH();
//// Enable AFE (e.g., MAX86141) with sensor hub samples..    
//    cmd[0] = 0x44;
//    cmd[1] = 0x00;
//    cmd[2] = 0x01;
//    cmd[3] = 0x00;
//    nrf_drv_twi_tx(&m_twi,addr, cmd, 4,false);
//     MFIO_init_output(1); nrf_delay_ms(2);  MFIO_init_output(0); nrf_delay_us(300);
//    nrf_delay_us(300);
//     nrf_drv_twi_rx(&m_twi,addr, rsp, 2);
//     MFIO_init_output(1);  MFIO_init_output(0); nrf_delay_us(300);
//    NRF_LOG_INFO("Enable AFE %x %x\n\r", rsp[0], rsp[1]);
//     NRF_LOG_FLUSH();
//// Enable accelerometer with sensor hub or hostside accelerometer.    
//    cmd[0] = 0x44;
//    cmd[1] = 0x04;
//    cmd[2] = 0x01;
//    cmd[3] = 0x00;
//    nrf_drv_twi_tx(&m_twi,addr, cmd, 4,false);
//    nrf_delay_ms(20);
//     MFIO_init_output(1); nrf_delay_ms(2);  MFIO_init_output(0); nrf_delay_us(300);
//    nrf_delay_us(300);
//     nrf_drv_twi_rx(&m_twi,addr, rsp, 2);
//     MFIO_init_output(1);  MFIO_init_output(0); nrf_delay_us(300);
//    NRF_LOG_INFO("Enable accelerometer with sensor hub or hostside accelerometer. %x %x\n\r", rsp[0], rsp[1]);
//     NRF_LOG_FLUSH();
//// Enable SCD Only algorithm.The format of samples report is shown in Table
//    cmd[0] = 0x52;
//    cmd[1] = 0x07;
//    cmd[2] = 0x03;

//    nrf_drv_twi_tx(&m_twi,addr, cmd,3,false);
//    nrf_delay_ms(465);
//     MFIO_init_output(1); nrf_delay_ms(2);  MFIO_init_output(0); nrf_delay_us(300);
//    nrf_delay_us(300);
//     nrf_drv_twi_rx(&m_twi,addr, rsp, 2);
//     NRF_LOG_INFO("Enable accelerometer with sensor hub or hostside accelerometer. %x %x\n\r", rsp[0], rsp[1]);
//     NRF_LOG_FLUSH();    






    // Enter main loop.
    for (;;)
    {
        idle_state_handle();
    }
}
/**
 * @}
 */
