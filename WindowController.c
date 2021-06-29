/*
 * WindowController.c
 *
 * Created: 05/05/2021 22.48.28
 *  Author: maria
 */ 
#include <stddef.h>
#include <stdio.h>
#include <ATMEGA_FreeRTOS.h>
#include <rc_servo.h>
#include "Configuration.h"

int8_t percent = 0;
SemaphoreHandle_t semaphore_mutex = NULL;

void WindowController_handler_task( void *pvParameters );

void WindowController_handler_initialise(UBaseType_t WindowController_task_priority){
	
	xTaskCreate(
	WindowController_handler_task
	,  "WindowControllerHand"  // A name just for humans
	,  configMINIMAL_STACK_SIZE  // This stack size can be checked & adjusted by reading the Stack Highwater
	,  NULL
	,  WindowController_task_priority  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
	,  NULL );
}

void WindowController_init(){
	semaphore_mutex = get_mutex();
	xSemaphoreGive(semaphore_mutex);
}

void WindowController_run(){
	if(xSemaphoreTake(semaphore_mutex, portMAX_DELAY)){
		rc_servo_setPosition(0, configuration_get_windows_data());
		percent = configuration_get_windows_data();
		xSemaphoreGive(semaphore_mutex);
	}
}

void WindowController_handler_task(void *pvParameters)
{
	printf("startWindowController \n");
	(void)pvParameters;
	WindowController_init();
	for(;;)
	{
		WindowController_run();	
	}
}