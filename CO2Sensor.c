/*
 * CO2Sensor.c
 *
 * Created: 05/05/2021 15.26.01
 *  Author: maria
 */ 
#include <mh_z19.h>

#include <stddef.h>
#include <stdio.h>
#include "SensorDataPackageHandler.h"
#include "Application.h"
#include <ATMEGA_FreeRTOS.h>
#include <event_groups.h>


uint16_t CO2ppm;
mh_z19_returnCode_t rc;
mh_z19_returnCode_t getCO2return;


void CO2Sensor_handler_task( void *pvParameters );

void CO2Sensor_handler_initialise(UBaseType_t CO2_task_priority){
	
	xTaskCreate(
	CO2Sensor_handler_task
	,  "CO2Hand"  // A name just for humans
	,  configMINIMAL_STACK_SIZE  // This stack size can be checked & adjusted by reading the Stack Highwater
	,  NULL
	,  CO2_task_priority  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
	,  NULL );
}

void CO2Sensor_handler_task(void *pvParameters)
{
	printf("startCO2");
	(void)pvParameters;
	mh_z19_injectCallBack(NULL);
	
	TickType_t xLastWakeTime;
	const TickType_t xFrequency = pdMS_TO_TICKS(300000UL); // Upload message every 5 minutes (300000 ms)
	xLastWakeTime = xTaskGetTickCount();
	
	for(;;)
	{
		xTaskDelayUntil( &xLastWakeTime, xFrequency );
		rc = mh_z19_takeMeassuring();
		if (rc == MHZ19_OK)
		{
			vTaskDelay(50);
			if(getCO2return == MHZ19_OK){
				getCO2return = mh_z19_getCo2Ppm(&CO2ppm);
				CO2Sensor_aFunctionToSetBits(Application_getEventGroup());
			}
			else if(getCO2return == MHZ19_NO_MEASSURING_AVAILABLE){
				printf("Didn't get the measurements");
			}
		}	
	}
}


uint16_t CO2Sensor_getCO2InUint16(){
	return CO2ppm;
}


#define BIT_0	( 1 << 0 )

void CO2Sensor_aFunctionToSetBits( EventGroupHandle_t xEventGroup )
{
EventBits_t uxBits;

  /* Set bit 0  in xEventGroup. */
  uxBits = xEventGroupSetBits(
                              xEventGroup,    /* The event group being updated. */
                              BIT_0  );/* The bits being set. */

  if( ( uxBits & ( BIT_0  ) ) == ( BIT_0 ) )
  {
      /* bit 0 remained set when the function returned. */
  }
}