
/*
 * WindowController.h
 *
 * Created: 27/05/2021 16.59.22
 *  Author: Jannik
 */ 
#pragma once
#include <stddef.h>
#include <stdio.h>
#include <ATMEGA_FreeRTOS.h>
#include <rc_servo.h>

void WindowController_handler_initialise(UBaseType_t WindowController_task_priority);
void WindowController_init(void *pvParameters);
void WindowController_run(void);
