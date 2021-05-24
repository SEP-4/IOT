/*
 * main.c
 *
 * Created: 28/04/2021 11.30.46
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
#include <rc_servo.h>
#include <message_buffer.h>

// define semaphore handle

MessageBufferHandle_t downLinkMessageBufferHandle = NULL;


// Prototype for LoRaWAN handler without down link buffer
void UpLinkHandler_lora_handler_initialise(UBaseType_t UpLinkHandler_lora_handler_task_priority);
// Prototype for LoRaWan handler with down link buffer
void DownLinkHandler_lora_handler_initialise(UBaseType_t DownLinkHandler_lora_handler_task_priority, MessageBufferHandle_t downLinkMessageBufferHandle);
// Prototype for hih8120 handler
void hih8120_handler_initialise(UBaseType_t hih8120_handler_task_priority);
// Prototype for Application handler
void Application_handler_initialise(UBaseType_t Application_task_priority);

/*-----------------------------------------------------------*/
int main(void)
{
	// Must be done as the very first thing!!
	// Make it possible to use stdio on COM port 0 (USB) on Arduino board - Setting 57600,8,N,1
	stdio_initialise(ser_USART0);
	// vvvvvvvvvvvvvvvvv BELOW IS LoRaWAN initialisation vvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
	// Status Leds driver
	status_leds_initialise(6); // Priority 5 for internal task
	//Initialize Application
	Application_handler_initialise(5);
	//initialize CO2Sensor
	mh_z19_initialise(ser_USART3);
	//create CO2 task
	CO2Sensor_handler_initialise(4);
	//Create humidity temperature task and start with priority 3
	hih8120_handler_initialise(3);
	//initialize rc_servo
	//rc_servo_initialise();
	// Initialise the LoRaWAN driver with down-link buffer
	//downLinkMessageBufferHandle = xMessageBufferCreate(sizeof(lora_driver_payload_t)*2);
	lora_driver_initialise(1, NULL);
	// Create LoRaWAN task and start it up with priority 2
	UpLinkHandler_lora_handler_initialise(2);
	//Create LoRaWAN task and start with priority 1
	//DownLinkHandler_lora_handler_initialise(1,downLinkMessageBufferHandle);
	//Create Configuration for Downlink_handler and WindowController
	
	printf("Program Started!!\n");
	vTaskStartScheduler(); // Initialize and run the freeRTOS scheduler. Execution should never return from here.
	/* Replace with your application code */
	while (1)
	{
	}
}
