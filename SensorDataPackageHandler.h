/*
 * SensorDataPackageHandler.h
 *
 * Created: 28/04/2021 10.18.21
 *  Author: maria
 */ 
#pragma once

#include <stdint.h>
#include <hih8120.h>
#include <stddef.h>
#include <stdio.h>

#include <ATMEGA_FreeRTOS.h>

#include <lora_driver.h>
#include <status_leds.h>

void SensorDataPackageHandler_SetHumidity(uint16_t humidity);
void SensorDataPackageHandler_SetTemperature(int16_t temperature);
void SensorDataPackageHandler_SetCO2(uint16_t co2);
lora_driver_payload_t SensorDataPackageHandler_getLoRaPayload();