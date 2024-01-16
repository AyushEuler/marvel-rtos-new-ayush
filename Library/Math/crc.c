/**
 * @file crc.c
 * @author 
 * @brief 
 * @version 0.1
 * @date 
 * 
 * @copyright Copyright (c) 2022
 * 
 */

/* Standard C includes -------------------------------------------------------*/
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
/* Private includes ----------------------------------------------------------*/
#include "bmsConfiguration.h"
/* Self include --------------------------------------------------------------*/
#include "crc.h"
/* Type definiton ------------------------------------------------------------*/

/* External variables definiton ----------------------------------------------*/

/* Private constants ---------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

/* Private custom code -------------------------------------------------------*/

/* Function definition -------------------------------------------------------*/
/**
 * @brief Generate CRC32 of Data with length size
 * @param data pointer to data
 * @param size size of data
 * @retval crc uint32_t
 */
uint32_t calculateCRC(uint8_t *data, uint16_t size)
{
	if(	(NULL == data) ||\
		(size <= 0))
	{
		return BMS_CRC_FUNCTION_PARAMETER_ERROR;
	}

	uint32_t poly = 0xEDB88320;
	uint32_t crc = 0xFFFFFFFF;

	for (uint16_t i = 0; i < size; i++)
	{
		crc ^= (uint32_t)data[i];

		for (int j = 0; j < 8; j++)
		{
			uint32_t mask = -(crc & 1);
			crc = (crc >> 1) ^ (poly & mask);
		}
	}

	return (~crc);
}

/* End of File ---------------------------------------------------------------*/

