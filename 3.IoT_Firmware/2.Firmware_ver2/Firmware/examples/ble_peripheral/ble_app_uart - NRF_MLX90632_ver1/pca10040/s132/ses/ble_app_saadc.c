#include "ble_app_saadc.h"

#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"

// STATIC VARIABLE
uint8_t nrf_saadc_avail = 0;
static nrf_saadc_value_t adc_buf[1] = {0};
extern uint8_t m_manuf_info[1];

// STATIC FUNCTION
static void saadc_event_handler(nrf_drv_saadc_evt_t const * p_event);

uint32_t batt_lvl_in_milli_volts = 0;
uint8_t percentage_batt_lvl = 0;
uint8_t ble_low_level_indicate = 0;

/**@brief Function for handling the ADC interrupt.
 *
 * @details  This function will fetch the conversion result from the ADC, convert the value into
 *           percentage and send it to peer.
 */
static void saadc_event_handler(nrf_drv_saadc_evt_t const * p_event)
{
    if (p_event->type == NRF_DRV_SAADC_EVT_DONE)
    {
    	uint32_t          err_code;
        nrf_saadc_value_t adc_result;

        adc_result = p_event->data.done.p_buffer[0];

        err_code = nrf_drv_saadc_buffer_convert(p_event->data.done.p_buffer, 1);
        APP_ERROR_CHECK(err_code);

        batt_lvl_in_milli_volts = ADC_RESULT_IN_MILLI_VOLTS(adc_result);
        //NRF_LOG_INFO("milli_volts: %d",batt_lvl_in_milli_volts);
        //NRF_LOG_FLUSH();
        percentage_batt_lvl = (batt_lvl_in_milli_volts - BATT_LOW_THRESHOLD)*100/(BATT_HIG_THRESHOLD - BATT_LOW_THRESHOLD);

        /**< Battery info advertised by the Beacon. */
      //  m_manuf_info[0] = percentage_batt_lvl;
        if((percentage_batt_lvl <= BATTERY_LOW_LEVEL_PERCENT) | (batt_lvl_in_milli_volts <= BATTERY_LOW_LEVEL_MILLIVOLTS)) {
            ble_low_level_indicate = 1;
        }
        else {
            ble_low_level_indicate = 0;
        }
    }
}


/**@brief Function for configuring ADC to do battery level conversion.
 */
void adc_configure(void)
{
    ret_code_t err_code;

    // VDD as input.
    nrf_saadc_channel_config_t config =
        NRF_DRV_SAADC_DEFAULT_CHANNEL_CONFIG_SE(NRF_SAADC_INPUT_AIN4); // P0.28/AIN4

    err_code = nrf_drv_saadc_init(NULL, saadc_event_handler);
    APP_ERROR_CHECK(err_code);

    err_code = nrf_drv_saadc_channel_init(0, &config);
    APP_ERROR_CHECK(err_code);

    err_code = nrf_drv_saadc_buffer_convert(&adc_buf[0], 1);
    APP_ERROR_CHECK(err_code);
    //
    // err_code = nrf_drv_saadc_buffer_convert(&adc_buf[1], 1);
    // APP_ERROR_CHECK(err_code);
}
