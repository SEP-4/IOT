/*
 * hih8120_2.h
 *
 * Created: 04/05/2021 22.33.10
 *  Author: maria
 */ 

#pragma once
#include <event_groups.h>

uint16_t hih820_getHumidityInUint16();
int16_t hih820_getTemperatureInUint16();
void hih820_aFunctionToSetBits(EventGroupHandle_t eventGroup_handler);
