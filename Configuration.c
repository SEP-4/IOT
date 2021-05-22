/*
 * Configuration.c
 *
 * Created: 05/05/2021 22.48.07
 *  Author: maria
 */
/*
#include "Configuration.h"
#include <stdio.h>
#include <stdbool.h>
 
SemaphoreHandle_t _configurationMutex = NULL;
static bool state;
uint16_t windows_settings;

void configuration_create(){
	//Initialize Semaphore 
	create_semaphores();
	state = true;
}


uint16_t configuration_get_windows_data(){
	return windows_settings;
}

void configuration_set_windows_data(uint16_t value){
	windows_settings = value;
}

bool configuration_take(){
	if(state){
		state = false;
		return true;
	}else{
		return state;
	}
}

void configuration_give(){
	state = true;
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
			xSemaphoreGive( ( xTestSemaphore ) );  // Make the mutex available for use, by initially "Giving" the Semaphore.
		}
	}
}*/


