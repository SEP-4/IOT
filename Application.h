/*
 * Application.h
 *
 * Created: 28/04/2021 11.36.57
 *  Author: maria
 */
#pragma once 
#include <stdint.h>
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
#include <event_groups.h>

void Application_initialiseSystem();
EventGroupHandle_t Application_getEventGroup();
void Application_aFunctionToClearBits(EventGroupHandle_t xEventGroup);