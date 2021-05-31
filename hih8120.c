/*
 * hih8120.c
 *
 * Created: 21/04/2021 17.40.26
 *  Author: maria
 */ 

#include <hih8120.h>
#include <stddef.h>
#include <stdio.h>
#include "SensorDataPackageHandler.h"
#include "Application.h"
#include <ATMEGA_FreeRTOS.h>
#include "hih8120_2.h"
#include <event_groups.h>


uint16_t humidityIn16 = 0;
int16_t temperatureIn16 = 0;
TickType_t xLastWakeTime;
const TickType_t xFrequency = pdMS_TO_TICKS(300000UL); // Upload message every 5 minutes (300000 ms)


void hih820_handler_task( void *pvParameters );

void hih8120_handler_initialise(UBaseType_t hih8120_task_priority){
	if ( HIH8120_OK == hih8120_initialise() ){
			printf("%d\n",HIH8120_OK);
	}
	xTaskCreate(
	hih820_handler_task
	,  "Hih820Hand"  // A name just for humans
	,  configMINIMAL_STACK_SIZE  // This stack size can be checked & adjusted by reading the Stack Highwater
	,  NULL
	,  hih8120_task_priority  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
	,  NULL );
}

inline void hih8120_init(void *pvParameters){
	(void)pvParameters;
	xLastWakeTime = xTaskGetTickCount();
}

inline void hih8120_run(void){
	xTaskDelayUntil( &xLastWakeTime, xFrequency );
	if (HIH8120_OK == hih8120_wakeup())
	{
		vTaskDelay(50);
		if (HIH8120_OK == hih8120_measure())
		{
			vTaskDelay(1);
			humidityIn16 = hih8120_getHumidityPercent_x10();
			temperatureIn16 = hih8120_getTemperature_x10();
			hih820_aFunctionToSetBits(Application_getEventGroup());
		}
	}
}

void hih820_handler_task(void *pvParameters)
{
	hih8120_init(pvParameters);
	for(;;)
	{
		hih8120_run();
	}
}

uint16_t hih820_getHumidityInUint16(){
	return humidityIn16/10;
}

int16_t hih820_getTemperatureInUint16(){
	return temperatureIn16/10;
}

#define TEMP_HUM_READY_BIT	( 1 << 4 )

void hih820_aFunctionToSetBits( EventGroupHandle_t xEventGroup )
{
EventBits_t uxBits;

  /* Set bit 0 and bit 4 in xEventGroup. */
  uxBits = xEventGroupSetBits(
                              xEventGroup,    /* The event group being updated. */
                              TEMP_HUM_READY_BIT );/* The bits being set. */

  if( ( uxBits & (  TEMP_HUM_READY_BIT ) ) == (  TEMP_HUM_READY_BIT ) )
  {
      /* bit 4 remained set when the function returned. */
  }
}