/*
 * Application.c
 *
 * Created: 28/04/2021 10.16.26
 *  Author: maria
 */ 
#include <stdio.h>
#include <avr/io.h>

#include <ATMEGA_FreeRTOS.h>
#include <task.h>
#include <semphr.h>

#include <stdio_driver.h>
#include <serial.h>

// Needed for LoRaWAN
#include <lora_driver.h>
#include <status_leds.h>

#include "Application.h"
#include <hih8120.h>
#include "hih8120_2.h"
#include <event_groups.h>
#include "CO2Sensor.h"
#include "SensorDataPackageHandler.h"

#define CO2_READY_BIT	( 1 << 0 )
#define TEMP_HUM_READY_BIT	( 1 << 4 )


// Declare a variable to hold the created event group.
EventGroupHandle_t xCreatedEventGroup;

EventGroupHandle_t Application_getEventGroup(){
	return xCreatedEventGroup;
}

void Application_aFunctionToWaitBits( EventGroupHandle_t xEventGroup )
{
EventBits_t uxBits;
const TickType_t xTicksToWait = 100 / portTICK_PERIOD_MS;

  /* Wait a maximum of 100ms for either bit 0 or bit 4 to be set within
  the event group.  Clear the bits before exiting. */
  uxBits = xEventGroupWaitBits(
            xEventGroup,   /* The event group being tested. */
            CO2_READY_BIT | TEMP_HUM_READY_BIT, /* The bits within the event group to wait for. */
            pdFALSE,        /* BIT_0 & BIT_4 should not be cleared before returning. */
            pdTRUE,       /* Wait for both bits, either bit will do. */
            xTicksToWait );/* Wait a maximum of 100ms for either bit to be set. */

  if( ( uxBits & ( CO2_READY_BIT | TEMP_HUM_READY_BIT ) ) == ( CO2_READY_BIT | TEMP_HUM_READY_BIT ) )
  {
      /* xEventGroupWaitBits() returned because both bits were set. */
	  printf("setting the bits, they are set \n");
	  SensorDataPackageHandler_SetHumidity(hih820_getHumidityInUint16());
	  SensorDataPackageHandler_SetTemperature(hih820_getTemperatureInUint16());
	  SensorDataPackageHandler_SetCO2(CO2Sensor_getCO2InUint16());
	  Application_aFunctionToClearBits( xEventGroup );
  }
  else if( ( uxBits & CO2_READY_BIT ) != 0 )
  {
      /* xEventGroupWaitBits() returned because just BIT_0 was set. */
  }
  else if( ( uxBits & TEMP_HUM_READY_BIT ) != 0 )
  {
      /* xEventGroupWaitBits() returned because just BIT_4 was set. */
  }
  else
  {
      /* xEventGroupWaitBits() returned because xTicksToWait ticks passed
      without either BIT_0 or BIT_4 becoming set. */
  }
}

void Application_aFunctionToClearBits( EventGroupHandle_t xEventGroup )
{
EventBits_t uxBits;

  /* Clear bit 0 and bit 4 in xEventGroup. */
  uxBits = xEventGroupClearBits(
                                xEventGroup,  /* The event group being updated. */
                                CO2_READY_BIT | TEMP_HUM_READY_BIT );/* The bits being cleared. */

  if( ( uxBits & ( CO2_READY_BIT | TEMP_HUM_READY_BIT ) ) == ( CO2_READY_BIT | TEMP_HUM_READY_BIT ) )
  {
      /* Both bit 0 and bit 4 were set before xEventGroupClearBits()
      was called.  Both will now be clear (not set). */
  }
  else if( ( uxBits & CO2_READY_BIT ) != 0 )
  {
      /* Bit 0 was set before xEventGroupClearBits() was called.  It will
      now be clear. */
  }
  else if( ( uxBits & TEMP_HUM_READY_BIT ) != 0 )
  {
      /* Bit 4 was set before xEventGroupClearBits() was called.  It will
      now be clear. */
  }
  else
  {
      /* Neither bit 0 nor bit 4 were set in the first place. */
  }
}


/*-----------------------------------------------------------*/

void Application_handler_task( void *pvParameters );

void Application_handler_initialise(UBaseType_t Application_task_priority){
	xTaskCreate(
	Application_handler_task
	,  "ApplicationHand"  // A name just for humans
	,  configMINIMAL_STACK_SIZE  // This stack size can be checked & adjusted by reading the Stack Highwater
	,  NULL
	,  Application_task_priority  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
	,  NULL );
}

void Application_handler_task(void *pvParameters)
{
	printf("startApplication \n");
	(void)pvParameters;
	xCreatedEventGroup = xEventGroupCreate();
	
	TickType_t xLastWakeTime;
	const TickType_t xFrequency = pdMS_TO_TICKS(300UL); // Upload message every 1 minutes (300 ms)
	xLastWakeTime = xTaskGetTickCount();
	
	for(;;)
	{
		xTaskDelayUntil( &xLastWakeTime, xFrequency );
		// Was the event group created successfully?
		if( xCreatedEventGroup == NULL )
		{
		// The event group was not created because there was insufficient
		// FreeRTOS heap available.
		}
		else
		{
			Application_aFunctionToWaitBits( xCreatedEventGroup);
		}
	}
}
