
/*
 * Configuration.h
 *
 * Created: 15/05/2021 15.19.43
 *  Author: Jannik
 */ 
#include <stdbool.h>

void configuration_create();
uint16_t configuration_get_windows_data();
void configuration_set_windows_data(uint16_t value);
bool configuraiton_take();
void configuration_give();
