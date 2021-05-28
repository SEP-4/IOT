/*
 * Configuration.c
 *
 * Created: 05/05/2021 22.48.07
 *  Author: Jannik
 */

#include "Configuration.h"

#include <stdbool.h>
#include <ATMEGA_FreeRTOS.h>
#include <stdio.h>
#include <avr/io.h>
#include <avr/sfr_defs.h>

#include <ATMEGA_FreeRTOS.h>
#include <semphr.h>

#include <FreeRTOSTraceDriver.h>
#include <stdio_driver.h>
#include <serial.h>
 
SemaphoreHandle_t _configurationSemaphore  = NULL; 
uint8_t windows_settings;
uint8_t humidity_setting;

void configuration_create(){
	// Semaphores are useful to stop a Task proceeding, where it should be paused to wait,
	// because it is sharing a resource, such as the Serial port.
	// Semaphores should only be used whilst the scheduler is running, but we can set it up here.
	if ( _configurationSemaphore  == NULL )  // Check to confirm that the Semaphore has not already been created.
	{
		_configurationSemaphore = xSemaphoreCreateBinary();  // Create a binary Mutex semaphore.
		if ( ( _configurationSemaphore  ) != NULL )
		{
			xSemaphoreGive( ( _configurationSemaphore  ) );  // Make the mutex available for use, by initially "Giving" the Semaphore.
		}
	}
}

uint8_t configuration_get_windows_data(){
	return windows_settings;
}

void configuration_set_windows_data(uint8_t value){
	printf("Setting windows to: %d\n",value);
	if(value == 0){
		windows_settings = -100;
	}else{
		windows_settings = 100;
	}
}

uint8_t configuration_get_humidity_data(){
	return humidity_setting;
}

void configuration_set_humidity_data(uint8_t value){
	printf("Setting humidity data to: %d\n",value);
	if(value == 0){
		humidity_setting = -100;
	}else{
		humidity_setting = 100;
	}
}

SemaphoreHandle_t get_mutex(){
	return _configurationSemaphore ;
}