/*
 * CO2Sensor.h
 *
 * Created: 05/05/2021 15.46.00
 *  Author: maria
 */ 
#pragma once
#include <event_groups.h>

uint16_t CO2Sensor_getCO2InUint16();
void CO2Sensor_aFunctionToSetBits(EventGroupHandle_t eventGroupHandle_t);
void CO2Sensor_handler_initialise(UBaseType_t CO2_task_priority);