/**
 * @file FIRFIlter.c
 * @author Lalit Ahuja
 * @brief 
 * @version 0.1
 * @date 2022-12-15
 * 
 * @copyright Copyright (c) 2022
 * 
 */

/* Standard C includes -------------------------------------------------------*/
#include <stdlib.h>
/* Private includes ----------------------------------------------------------*/

/* Self include --------------------------------------------------------------*/
#include "FIRFilter.h"
/* Type definiton ------------------------------------------------------------*/

/* External variables definiton ----------------------------------------------*/

/* Private constants ---------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

/* Private custom code -------------------------------------------------------*/

static float FIR_IMPULSE_RESPONSE[FIR_FILTER_LENGTH] = {0.1f, 0.1f, 0.1f, 0.1f, 0.1f, 0.1f, 0.1f, 0.1f, 0.1f, 0.1f};

/* Function definition -------------------------------------------------------*/
/**
 * @brief
 * @param
 * @retval
 */
bmsStatus_t FIRFilterInit(FIRFilter_t *fir)
{
	bmsStatus_t status = BMS_ERROR;
	if((NULL == fir))
	{
		status = BMS_FILTER_FUNCTION_PARAMETER_ERROR;
		return status;
	}

	/* Clear filter buffer */
	for (uint8_t n = 0; n < FIR_FILTER_LENGTH; n++)
	{
		fir->buf[n] = 0.0f;
	}

	/* Reset buffer index */
	fir->bufIndex = 0;

	/* Clear filter output */
	fir->out = 0.0f;

	status = BMS_OK;
	return status;
}

/* Function definition -------------------------------------------------------*/
/**
 * @brief
 * @param
 * @retval
 */
float FIRFilterUpdate(FIRFilter_t *fir, float data)
{

	if((NULL == fir))
	{
		return BMS_FILTER_FUNCTION_PARAMETER_ERROR;
	}

	/* Store latest sample in buffer */
	fir->buf[fir->bufIndex] = data;

	/* Increment buffer index and wrap around if necessary */
	fir->bufIndex++;

	if (fir->bufIndex == FIR_FILTER_LENGTH)
	{
		fir->bufIndex = 0;
	}

	/* Compute new output sample (via convolution) */
	fir->out = 0.0f;

	uint8_t sumIndex = fir->bufIndex;

	for (uint8_t n = 0; n < FIR_FILTER_LENGTH; n++)
	{
		/* Decrement index and wrap if necessary */
		if (sumIndex > 0) 
		{
			sumIndex--;
		} 
		else 
		{
			sumIndex = FIR_FILTER_LENGTH - 1;
		}

		/* Multiply impulse response with shifted input sample and add to output */
		fir->out += FIR_IMPULSE_RESPONSE[n] * fir->buf[sumIndex];
	}

	/* Return filtered output */
	return fir->out;
}

/* End of File ---------------------------------------------------------------*/

