/**
 * @file timerControls.c
 * @author Arpit Gupta (arpit@euler)
 * @brief 
 * @version 0.1
 * @date 2022-04-15
 * 
 * @copyright Copyright (c) 2022
 * 
 */

/* Standard C includes -------------------------------------------------------*/

/* Private includes ----------------------------------------------------------*/
#include "tim.h"
#include "main.h"
#include "bmsConfiguration.h"
/* Self include --------------------------------------------------------------*/
#include "timerControls.h"
/* Type definiton ------------------------------------------------------------*/

/* External variables definiton ----------------------------------------------*/

/* Private constants ---------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

/* Private custom code -------------------------------------------------------*/

/* Function definition -------------------------------------------------------*/
/**
 * @brief start timer
 * @param timerID timer ID number
 * @retval BMS status
 */
bmsStatus_t startTimer(uint8_t timerID)
{
  bmsStatus_t status = BMS_ERROR;

	if(!(IS_TIMER_ID(timerID)))
	{
		status = BMS_TIMER_FUNCTION_PARAMETER_ERROR;
		return status;
	}

  switch(timerID)
  {
    case cmuTimer:
      HAL_TIM_Base_Start(&htim1);
      status = BMS_OK;
      break;
    default:
      status = BMS_ERROR;
      break;
  }
  
  return status;
}

/* Function definition -------------------------------------------------------*/
/**
 * @brief stop timer
 * @param timerID timer ID number
 * @retval BMS status
 */
bmsStatus_t stopTimer(uint8_t timerID)
{
  bmsStatus_t status = BMS_ERROR;

	if(!(IS_TIMER_ID(timerID)))
	{
		status = BMS_TIMER_FUNCTION_PARAMETER_ERROR;
		return status;
	}

  switch(timerID)
  {
    case cmuTimer:
      HAL_TIM_Base_Stop(&htim1);
      status = BMS_OK;
      break;
    default:
      status = BMS_ERROR;
      break;
  }
  
  return status;
}

/* Function definition -------------------------------------------------------*/
/**
 * @brief Get the Tim Count object
 * 
 * @return uint32_t 
 */
uint32_t getTimCount(void)
{
  uint32_t count = 0;
  count = __HAL_TIM_GetCounter(&htim1);
  __HAL_TIM_SetCounter(&htim1, 0);
  return(count);
}
/* Function definition -------------------------------------------------------*/
/**
 * @brief send pwm signal to contactors
 * 
 * @param channel contactor channel number
 * @param value pwm value
 * @retval BMS status
 */
bmsStatus_t writePWM(uint8_t channel, long int value)
{
  bmsStatus_t status = BMS_ERROR;

	if(!(IS_PWM_CHANNEL_ID(channel)) ||\
      (value < 0))
	{
		status = BMS_TIMER_FUNCTION_PARAMETER_ERROR;
		return status;
	}

  switch (channel)
  {
    case pwmCh1:
      TIM3 -> CCR1 = value;
      status = BMS_OK;
      break;
    case pwmCh2:
      TIM3 -> CCR2 = value;
      status = BMS_OK;
      break;
    case pwmCh3:
      TIM3 -> CCR3 = value;
      status = BMS_OK;
      break;
    case pwmCh4:
      TIM3 -> CCR4 = value;
      status = BMS_OK;
      break;
    default:
      status = BMS_ERROR;
      break;
  }

  return status;
}

/**
 * @brief 
 * 
 * @return uint32_t 
 */
uint32_t millis(void)
{
	uint32_t tick = HAL_GetTick();
	return tick;
}

/**
 * @brief 
 * 
 * @return uint32_t 
 */
uint32_t micros(void)
{
	uint32_t tick = __HAL_TIM_GET_COUNTER(&htim1);
	return tick;
}
/* Function definition -------------------------------------------------------*/
/**

/* End of File ---------------------------------------------------------------*/