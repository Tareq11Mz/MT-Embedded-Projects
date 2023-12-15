/******************************************************************************
 *
 * Temperature Sensor
 *
 * File Name: lm35_sensor.h
 *
 * Description: source file for the LM35 Temperature Sensor driver
 *
 *******************************************************************************/
#include "lm35sensor.h"

#include "adc.h"
#include <util/delay.h>



uint8 LM35_getTemperature(ADC_ConfigType *ptr_ConfigType)
{
    uint8 temp_value = 0;

    uint16 adc_value = 0;

    /* Reading ADC channel  */
    adc_value = ADC_readChannel(SENSOR_CHANNEL_ID);

    /* Calculate the temperature from the ADC value - configurable*/
    if (ptr_ConfigType->ref_volt == INTERNAL)
        temp_value = (uint8)(((float)adc_value * SENSOR_MAX_TEMPERATURE * 2.56) / (ADC_MAXIMUM_VALUE * SENSOR_MAX_VOLT_VALUE));
    else if (ptr_ConfigType->ref_volt == AREF)
        temp_value = (uint8)(((uint32)adc_value * SENSOR_MAX_TEMPERATURE * 5) / (ADC_MAXIMUM_VALUE * SENSOR_MAX_VOLT_VALUE));
    else if (ptr_ConfigType->ref_volt == AVCC)
        temp_value = (uint8)(((uint32)adc_value * SENSOR_MAX_TEMPERATURE * ADC_REF_VOLT_VALUE) / (ADC_MAXIMUM_VALUE * SENSOR_MAX_VOLT_VALUE));

    return temp_value;
}
