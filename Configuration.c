/*
 * Configuration.c
 *
 * Created: 05/05/2021 22.48.07
 *  Author: maria
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
uint16_t windows_settings;

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


uint16_t configuration_get_windows_data(){
	return windows_settings;
}

void configuration_set_windows_data(uint16_t value){
	windows_settings = value;
}

SemaphoreHandle_t get_mutex(){
	return _configurationSemaphore ;
}


