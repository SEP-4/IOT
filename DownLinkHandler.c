/*
 * DownLinkHandler.c
 *
 * Created: 05/05/2021 22.31.32
 *  Author: maria
 */ 
#include <stddef.h>
#include <stdio.h>
#include <stdbool.h>

#include <ATMEGA_FreeRTOS.h>

#include <lora_driver.h>
#include <status_leds.h>
#include <message_buffer.h>
#include "Configuration.h"

void DownLinkHandler_lora_handler_task( void *pvParameters );

static lora_driver_payload_t _downlink_payload;
uint16_t windowDataSetting; // Windows Data setting
int16_t maxTempSetting; // Max Temperature
MessageBufferHandle_t _downLinkMessageBufferHandle;



void DownLinkHandler_lora_handler_initialise(UBaseType_t DownLinkHandler_lora_handler_task_priority, MessageBufferHandle_t downLinkMessageBufferHandle)
{
	_downLinkMessageBufferHandle = downLinkMessageBufferHandle;
	xTaskCreate(
	DownLinkHandler_lora_handler_task
	,  "DownLinkHandler"  // A name just for humans
	,  configMINIMAL_STACK_SIZE+200  // This stack size can be checked & adjusted by reading the Stack Highwater
	,  NULL
	,  DownLinkHandler_lora_handler_task_priority  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
	,  NULL );
}

void DownLinkHandler_lora_handler_task( void *pvParameters )
{
	/*TickType_t xLastWakeTime;
	const TickType_t xFrequency = pdMS_TO_TICKS(300000UL); // Download message every 5 minutes (300000 ms)
	const TickType_t xConfigurationDelay = pdMS_TO_TICKS(50000);
	xLastWakeTime = xTaskGetTickCount();
	*/
	configuration_create();
	SemaphoreHandle_t semaphore_mutex = get_mutex();
	xSemaphoreGive(semaphore_mutex);
	
	for(;;)
	{
		//xTaskDelayUntil( &xLastWakeTime, xFrequency );
		printf("startLoraDownlinkTask\n");
		status_leds_shortPuls(led_ST4);  // OPTIONAL
		xMessageBufferReceive(_downLinkMessageBufferHandle, &_downlink_payload, sizeof(lora_driver_payload_t), portMAX_DELAY);
		printf("DOWN LINK: from port: %d with %d bytes received!", _downlink_payload.portNo, _downlink_payload.len); // Just for Debug
		if (4 == _downlink_payload.len) // Check that we have got the expected 4 bytes
		{
			// decode the payload into our variables
			windowDataSetting = (_downlink_payload.bytes[0] << 8) + _downlink_payload.bytes[1];
			//printf(windowDataSetting + " Hex code");
			maxTempSetting = (_downlink_payload.bytes[2] << 8) + _downlink_payload.bytes[3];
			printf("%d Hex code 2", maxTempSetting);
			for(;;){
				if(xSemaphoreTake(semaphore_mutex, portMAX_DELAY)){
					configuration_set_windows_data(windowDataSetting);
					xSemaphoreGive(semaphore_mutex);
					break;
				}else{
					//xTaskDelayUntil(&xLastWakeTime, xConfigurationDelay);
				}
			}
		}
	}
}