/**

/* Standard C includes -------------------------------------------------------*/
#include <stdlib.h>
/* Private includes ----------------------------------------------------------*/
#include "bmsConfiguration.h"
/* Self include --------------------------------------------------------------*/
#include "conversion.h"
/* Type definiton ------------------------------------------------------------*/

/* External variables definiton ----------------------------------------------*/

/* Private constants ---------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

/* Private custom code -------------------------------------------------------*/

/* Function definition -------------------------------------------------------*/
/**
 * @brief convert float to byte
 * @param dataBuffer data array which will be convert to float
 * @param variable float variable
 * @retval none
 */
bmsStatus_t floatToByte(uint8_t *dataBuffer, float variable)
{
    convertByteToFloat_t data = {0};

	if((NULL == dataBuffer))
	{
		return BMS_CONVERSION_FUNCTION_PARAMETER_ERROR;
	}

    data.floatValue = variable;

    for (uint8_t i = 0; i < FLOAT_SIZE; i++)
    {
    	dataBuffer[i] = data.byteValue[i];
    }

    return BMS_OK;
}

/* Function definition -------------------------------------------------------*/
/**
 * @brief convert byte to float
 * @param dataBuffer data array which will be convert to float
 * @retval float value
 */
float byteToFloat(uint8_t *dataBuffer)
{
    convertByteToFloat_t data = {0};
	
    if( (NULL == dataBuffer) )
	{
		return BMS_CONVERSION_FUNCTION_PARAMETER_ERROR;
	}

    for (uint8_t i = 0; i < FLOAT_SIZE; i++)
    {
    	data.byteValue[i] = dataBuffer[i];
    }

   float variable =  data.floatValue;
   return variable;
}

/* Function definition -------------------------------------------------------*/
/**
 * @brief convert double to byte
 * @param dataBuffer data array which will be convert to double
 * @param variable double variable
 * @retval none
 */
bmsStatus_t doubleToByte(uint8_t *dataBuffer, double variable)
{
    convertDoubleToByte_t data = {0};

	if((NULL == dataBuffer))
	{
		return BMS_CONVERSION_FUNCTION_PARAMETER_ERROR;
	}

    data.doubleValue = variable;

    for (uint8_t i = 0; i < DOUBLE_SIZE; i++)
    {
    	dataBuffer[i] = data.byteValue[i];
    }

    return BMS_OK;
}

/* Function definition -------------------------------------------------------*/
/**
 * @brief convert byte to double
 * @param dataBuffer data array which will be convert to double
 * @retval double value
 */
double byteToDouble(uint8_t *dataBuffer)
{
    convertByteToDouble_t data = {0};

	if( (NULL == dataBuffer) )
	{
		return BMS_CONVERSION_FUNCTION_PARAMETER_ERROR;
	}

    for (uint8_t i = 0; i < DOUBLE_SIZE; i++)
    {
    	data.byteValue[i] = dataBuffer[i];
    }

    double variable =  data.doubleValue;
    return variable;
}

/* End of File ---------------------------------------------------------------*/
