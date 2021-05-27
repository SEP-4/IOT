
/*
 * Configuration.h
 *
 * Created: 15/05/2021 15.19.43
 *  Author: Jannik
 */ 
#pragma once
#include <stdbool.h>
#include <ATMEGA_FreeRTOS.h>
#include <stdio.h>
#include <avr/io.h>
#include <avr/sfr_defs.h>

#include <ATMEGA_FreeRTOS.h>
#include <semphr.h>

#include <FreeRTOSTraceDriver.h>
#include <stdio_driver.h>
#include <serial.h>

void configuration_create();
uint8_t configuration_get_windows_data();
void configuration_set_windows_data(uint8_t value);
uint8_t configuration_get_humidity_data();
void configuration_set_humidity_data(uint8_t value);
SemaphoreHandle_t get_mutex();