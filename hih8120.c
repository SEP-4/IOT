/*
 * hih8120.c
 *
 * Created: 21/04/2021 17.40.26
 *  Author: maria
 */ 

#include <hih8120.h>
#include <stddef.h>
#include <stdio.h>

#include <ATMEGA_FreeRTOS.h>




float humidity = 0.0; 
float temperature = 0.0; 
uint16_t humidityIn16 = 0;
uint16_t temperatureIn16 = 0;

TaskHandle_t x1Handle = NULL;
static char _out_buf[100];

void hih820_handler_task( void *pvParameters );

void hih8120_handler_initialise(UBaseType_t hih8120_task_priority){
	xTaskCreate(
	hih820_handler_task
	,  "Hih820Hand"  // A name just for humans
	,  configMINIMAL_STACK_SIZE  // This stack size can be checked & adjusted by reading the Stack Highwater
	,  NULL
	,  hih8120_task_priority  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
	,  &x1Handle );
}

void hih820_handler_task(void *pvParameters)
{
	(void)pvParameters;
	
	TickType_t xLastWakeTime;
	const TickType_t xFrequency = pdMS_TO_TICKS(300000UL); // Upload message every 5 minutes (300000 ms)
	xLastWakeTime = xTaskGetTickCount();
	
	for(;;)
	{
		xTaskDelayUntil( &xLastWakeTime, xFrequency );
		if ( HIH8120_OK == hih8120_initialise() ){
			if(1 == hih8120_isReady()){
				if (HIH8120_OK == hih8120_wakeup())
				{
					vTaskDelay(50);
					if (HIH8120_OK == hih8120_measure())
					{
						vTaskDelay(1);
						humidity = hih8120_getHumidity();
						temperature = hih8120_getTemperature();
					}
				}
			}
		}
		if(HIH8120_OK == hih8120_destroy()){
			printf(HIH8120_OK);
		}
		printf(HIH8120_DRIVER_NOT_INITIALISED);
		if ( HIH8120_OK != hih8120_initialise() )
		{
			printf(HIH8120_OUT_OF_HEAP);
		}
	}
}

uint16_t hih820_getHumidityInUint16(){
	humidityIn16 = humidity;
	return humidityIn16;
}

uint16_t hih820_getTemperatureInUint16(){
	temperatureIn16 = temperature;
	return temperatureIn16;
}