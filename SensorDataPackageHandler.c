/*
 * SensorDataPackageHandler.c
 *
 * Created: 27/04/2021 14.59.09
 *  Author: maria
 */ 

#include <stdint.h>
#include "SensorDataPackageHandler.h"
#include <hih8120.h>
#include <stddef.h>
#include <stdio.h>

#include <ATMEGA_FreeRTOS.h>

#include <lora_driver.h>
#include <status_leds.h>

uint16_t humIn16 = 0.0;
int16_t tempIn16 = 0.0;
uint16_t co2_ppmIn16 = 0.0;
static lora_driver_payload_t _uplink_payload;

void SensorDataPackageHandler_SetTemperatureAndHumidity( float humdata, float tempdata){
	humIn16 = humdata;
	tempIn16 = tempdata;
	printf("this is hum %f", humdata);
	printf("this is tem %f", tempdata);
}
void SensorDataPackageHandler_SetCO2(uint16_t co2){
	co2_ppmIn16 = co2;
}

lora_driver_payload_t SensorDataPackageHandler_getLoRaPayload(){
	float hum = hih8120_getHumidity();
	float tem = hih8120_getTemperature();
	SensorDataPackageHandler_SetTemperatureAndHumidity(tem,hum);
	
	
	_uplink_payload.len = 6;
	_uplink_payload.portNo = 2;
	_uplink_payload.bytes[0] = humIn16 >> 8;
	_uplink_payload.bytes[1] = humIn16 & 0xFF;
	_uplink_payload.bytes[2] = tempIn16 >> 8;
	_uplink_payload.bytes[3] = tempIn16 & 0xFF;
	_uplink_payload.bytes[4] = co2_ppmIn16 >> 8;
	_uplink_payload.bytes[5] = co2_ppmIn16 & 0xFF;
	return _uplink_payload;
}