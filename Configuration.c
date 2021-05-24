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
 
SemaphoreHandle_t _configurationMutex = NULL; 
uint16_t windows_settings;

void configuration_create(){
	//Initialize Semaphore 
	create_semaphores();
}


uint16_t configuration_get_windows_data(){
	return windows_settings;
}

void configuration_set_windows_data(uint16_t value){
	windows_settings = value;
}

bool configuration_take(){
	if(xSemaphoreTake(_configurationMutex, portMAX_DELAY)){
		return true;
	}else{
		return false;
	}
}

void configuration_give(){
	xSemaphoreGive(_configurationMutex);
}

void create_semaphores(void)
{
	// Semaphores are useful to stop a Task proceeding, where it should be paused to wait,
	// because it is sharing a resource, such as the Serial port.
	// Semaphores should only be used whilst the scheduler is running, but we can set it up here.
	if ( _configurationMutex == NULL )  // Check to confirm that the Semaphore has not already been created.
	{
		_configurationMutex = xSemaphoreCreateMutex();  // Create a mutex semaphore.
		if ( ( _configurationMutex ) != NULL )
		{
			xSemaphoreGive( ( _configurationMutex ) );  // Make the mutex available for use, by initially "Giving" the Semaphore.
		}
	}
}


