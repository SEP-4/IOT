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

static char _out_buf[100];
int8_t percent = 0;

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

void WindowController_handler_task(void *pvParameters)
{
	printf("startWindowController");
	(void)pvParameters;
	
	TickType_t xLastWakeTime;
	const TickType_t xFrequency = pdMS_TO_TICKS(300000UL); // Upload message every 5 minutes (300000 ms)
	xLastWakeTime = xTaskGetTickCount();
	
	for(;;)
	{
		xTaskDelayUntil( &xLastWakeTime, xFrequency );
		rc_servo_setPosition(0, percent);
	}
}