gpioteManager.c



//Functions intention is to trip when Pin# reaches a high level.
void LABHandler(void){
  
  nrfx_gpiote_out_config_t out_config = NRFX_GPIOTE_CONFIG_OUT_TASK_TOGGLE(true);
  APP_ERROR_CHECK(nrfx_gpiote_out_init(24, &out_config));
  nrfx_gpiote_out_task_enable(24);
    // Configure input pin
  nrfx_gpiote_in_config_t in_config = NRFX_GPIOTE_CONFIG_IN_SENSE_HITOLO(true);
  in_config.pull = NRF_GPIO_PIN_PULLUP;
  APP_ERROR_CHECK(nrfx_gpiote_in_init(28, &in_config, in_pin_handler));
    nrfx_gpiote_in_event_enable(28, true);

  printf("Lab_Handled\n\n");
}