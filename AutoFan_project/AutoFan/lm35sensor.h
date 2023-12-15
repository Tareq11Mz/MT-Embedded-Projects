/******************************************************************************
 *
 *
 * File Name: lm35_sensor.h
 *
 * Description: header file for the LM35 Temperature Sensor driver
 *
 *******************************************************************************/

#ifndef LM35SENSOR_H_
#define LM35SENSOR_H_

#include "std_types.h"
#include "adc.h"

/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/

#define SENSOR_CHANNEL_ID 2 //THIS IS THE adc CHANNEL
#define SENSOR_MAX_VOLT_VALUE 1.5
#define SENSOR_MAX_TEMPERATURE 150

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/

uint8 LM35_getTemperature(ADC_ConfigType *ptr_ConfigType);

#endif /* LM35SENSOR_H_ */
