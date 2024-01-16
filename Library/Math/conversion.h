/**
 * @file conversion.h
 * @author Lalit Ahuja 
 * @brief 
 * @version 0.1
 * @date 2023-01-29
 * 
 * @copyright Copyright (c) 2022
 * 
 */

/* Header Guard --------------------------------------------------------------*/
#ifndef CONVERSION_H_
#define CONVERSION_H_

/* Standard C includes -------------------------------------------------------*/
#include <inttypes.h>
#include "bmsConfiguration.h"
/* Private includes ----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

#define CONVERT_CMU_VOLTAGE_TO_MILLI_VOLT(voltage) 		                ((voltage) * 0.1)
#define CONVERT_MILLI_VOLT_TO_VOLT(voltage) 		                    ((voltage) * 0.001)

#define CONVERT_TO_MILLI(x)                                             ((x) * 1000)
#define CONVERT_AMPERE_TO_MILLI_AMPERE(ampere)                          CONVERT_TO_MILLI((ampere))  
#define CONVERT_AMPERE_HOUR_TO_MILLI_AMPERE_HOUR(ampereHour)            CONVERT_TO_MILLI((ampereHour))     
#define CONVERT_HOUR_TO_MILLI_SECONDS(hours)                            CONVERT_TO_MILLI(((hours) * 3600))

#define CONVERT_MICRO_OHM_TO_OHM(microOhm)                              ((microOhm) * 0.000001)
#define CONVERT_MICRO_OHM_TO_MILLI_OHM(microOhm)                        ((microOhm) * 0.001)

#define CHAR_SIZE     1
#define UINT8_SIZE    1
#define UINT16_SIZE   2
#define UINT32_SIZE   4

#define INT8_SIZE     1
#define INT16_SIZE    2
#define INT32_SIZE    4

#define FLOAT_SIZE    4
#define DOUBLE_SIZE   8

/* Private macro -------------------------------------------------------------*/

/* Private typedef -----------------------------------------------------------*/

/* External variables --------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Inline functions ----------------------------------------------------------*/

/* Function prototypes -------------------------------------------------------*/

typedef union
{
    float floatValue;
    uint8_t byteValue[FLOAT_SIZE];
}convertByteToFloat_t;

typedef union
{
    float floatValue;
    uint8_t byteValue[FLOAT_SIZE];
}convertFloatToByte_t;

typedef union
{
    double doubleValue;
    uint8_t byteValue[DOUBLE_SIZE];
}convertByteToDouble_t;

typedef union
{
    double doubleValue;
    uint8_t byteValue[DOUBLE_SIZE];
}convertDoubleToByte_t;

bmsStatus_t floatToByte(uint8_t *dataBuffer, float variable);
float byteToFloat(uint8_t *dataBuffer);
bmsStatus_t doubleToByte(uint8_t *dataBuffer, double variable);
double byteToDouble(uint8_t *dataBuffer);
#endif
/* End of File ---------------------------------------------------------------*/

