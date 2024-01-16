
/* Standard C includes -------------------------------------------------------*/
#include "watchDogControls.h"

/* Private includes ----------------------------------------------------------*/

/* Self include --------------------------------------------------------------*/

/* Type definiton ------------------------------------------------------------*/

/* External variables definiton ----------------------------------------------*/

/* Private constants ---------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

/* Private custom code -------------------------------------------------------*/

/* Function definition -------------------------------------------------------*/
/**
 * @brief IWDG reload value calculation
 * @param resetTime reset time in seconds
 * @param preScaler pre scaler
 * @retval reload value
 */
uint32_t calculateIWDGReloadValue(uint16_t resetTime, uint8_t preScaler)
{
	if((resetTime < 0)  ||\
     (preScaler < 0))
	{
		return BMS_WATCH_DOG_FUNCTION_PARAMETER_ERROR;
	}

  uint32_t reloadValue = ((resetTime*32000)/4*(2^preScaler)*1000)-1;

  return reloadValue;
}

/* Function definition -------------------------------------------------------*/
/**

/* Function definition -------------------------------------------------------*/
/**

/* End of File ---------------------------------------------------------------*/

