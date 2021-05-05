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

// define semaphore handle
SemaphoreHandle_t xTestSemaphore;

// Prototype for LoRaWAN handler
void lora_handler_initialise(UBaseType_t lora_handler_task_priority);
// Prototype for hih8120 handler
void hih8120_handler_initialise(UBaseType_t hih8120_handler_task_priority);
// Prototype for Application handler
void Application_handler_initialise(UBaseType_t Application_task_priority);

void create_semaphores(void)
{
	// Semaphores are useful to stop a Task proceeding, where it should be paused to wait,
	// because it is sharing a resource, such as the Serial port.
	// Semaphores should only be used whilst the scheduler is running, but we can set it up here.
	if ( xTestSemaphore == NULL )  // Check to confirm that the Semaphore has not already been created.
	{
		xTestSemaphore = xSemaphoreCreateMutex();  // Create a mutex semaphore.
		if ( ( xTestSemaphore ) != NULL )
		{
			xSemaphoreGive( ( xTestSemaphore ) );  // Make the mutex available for use, by initially "Giving" the Semaphore.
		}
	}
}

/*-----------------------------------------------------------*/
int main(void)
{
	// Must be done as the very first thing!!
	// Make it possible to use stdio on COM port 0 (USB) on Arduino board - Setting 57600,8,N,1
	stdio_initialise(ser_USART0);
	// Let's create some semaphores
	create_semaphores();
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
	// Initialise the LoRaWAN driver without down-link buffer
	lora_driver_initialise(1, NULL);
	// Create LoRaWAN task and start it up with priority 2
	lora_handler_initialise(2);
	
	
	printf("Program Started!!\n");
	vTaskStartScheduler(); // Initialize and run the freeRTOS scheduler. Execution should never return from here.

	/* Replace with your application code */
	while (1)
	{
	}
}
