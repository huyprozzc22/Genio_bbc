#include <stdint.h>
#include <string.h>

#include "pa_lna.h"
#include "ble.h"
#include "app_error.h"
#include "nrf_drv_gpiote.h"
#include "nrf_drv_ppi.h"

void pa_lna_init(uint32_t gpio_pa_pin, uint32_t gpio_lna_pin)
{
    ble_opt_t opt;
    uint32_t gpiote_ch = NULL;
    ret_code_t err_code;        

    memset(&opt, 0, sizeof(ble_opt_t));
    
    err_code = nrf_drv_gpiote_init();
    if(err_code != NRF_ERROR_INVALID_STATE)
        APP_ERROR_CHECK(err_code);
    
    err_code = nrf_drv_ppi_init();
   // if(err_code != MODULE_ALREADY_INITIALIZED)
        APP_ERROR_CHECK(err_code);
    
    nrf_ppi_channel_t ppi_set_ch;
    nrf_ppi_channel_t ppi_clr_ch;
    
    err_code = nrf_drv_ppi_channel_alloc(&ppi_set_ch);
    APP_ERROR_CHECK(err_code);
    
    err_code = nrf_drv_ppi_channel_alloc(&ppi_clr_ch);
    APP_ERROR_CHECK(err_code);

    nrf_drv_gpiote_out_config_t config = GPIOTE_CONFIG_OUT_TASK_TOGGLE(false);
    
    if((gpio_pa_pin == NULL) && (gpio_lna_pin == NULL))
    {
        err_code = NRF_ERROR_INVALID_PARAM;
        APP_ERROR_CHECK(err_code);
    }    

    if(gpio_pa_pin != NULL)
    {
        if(gpiote_ch == NULL)
        {
            err_code = nrf_drv_gpiote_out_init(gpio_pa_pin, &config);
            APP_ERROR_CHECK(err_code);
            
            gpiote_ch = nrf_drv_gpiote_out_task_addr_get(gpio_pa_pin); 
        }
        
        // PA config
        opt.common_opt.pa_lna.pa_cfg.active_high = 1;   // Set the pin to be active high
        opt.common_opt.pa_lna.pa_cfg.enable      = 1;   // Enable toggling
        opt.common_opt.pa_lna.pa_cfg.gpio_pin    = gpio_pa_pin; // The GPIO pin to toggle tx  
    }
    
    if(gpio_lna_pin != NULL)
    {
        if(gpiote_ch == NULL)
        {
            err_code = nrf_drv_gpiote_out_init(gpio_lna_pin, &config);
            APP_ERROR_CHECK(err_code);        
            
            gpiote_ch = nrf_drv_gpiote_out_task_addr_get(gpio_lna_pin); 
        }
        
        // LNA config
        opt.common_opt.pa_lna.lna_cfg.active_high  = 1; // Set the pin to be active high
        opt.common_opt.pa_lna.lna_cfg.enable       = 1; // Enable toggling
        opt.common_opt.pa_lna.lna_cfg.gpio_pin     = gpio_lna_pin;  // The GPIO pin to toggle rx
    }

    // Common PA/LNA config
    opt.common_opt.pa_lna.gpiote_ch_id  = (gpiote_ch - NRF_GPIOTE_BASE) >> 2;   // GPIOTE channel used for radio pin toggling
    opt.common_opt.pa_lna.ppi_ch_id_clr = ppi_clr_ch;   // PPI channel used for radio pin clearing
    opt.common_opt.pa_lna.ppi_ch_id_set = ppi_set_ch;   // PPI channel used for radio pin setting
    
    err_code = sd_ble_opt_set(BLE_COMMON_OPT_PA_LNA, &opt);
    APP_ERROR_CHECK(err_code);    
}
