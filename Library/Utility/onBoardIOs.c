/**
 * @file onBoardIOs.c
 * @author Arpit Gupta (arpit@euler)
 * @brief 
 * @version 0.1
 * @date 2022-03-31
 * 
 * @copyright Copyright (c) 2022
 * 
 */

/* Standard C includes -------------------------------------------------------*/

/* Private includes ----------------------------------------------------------*/
#include "customDelays.h"

#include "timerControls.h"
#include "gpioControls.h"
/* Self include --------------------------------------------------------------*/
#include "onBoardIOs.h"
/* Type definiton ------------------------------------------------------------*/

#define PWM_CHANNEL_HIGH            65535
#define PWM_CHANNEL_LOW             0
#define PWM_CHANNEL_DELAY           10

/* External variables definiton ----------------------------------------------*/

/* Private constants ---------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
extern TIM_HandleTypeDef htim6;
/* Private function prototypes -----------------------------------------------*/

/* Private custom code -------------------------------------------------------*/

/* Function definition -------------------------------------------------------*/
/**
 * @brief 
 * @param type 
 * @param code 
 * @return BMS status
 */
bmsStatus_t flashErrorOnLEDs(uint8_t type, uint8_t code)
{
    bmsStatus_t status = BMS_ERROR;

    status = BMS_OK;
    return status;
}

/* Function definition -------------------------------------------------------*/
/**
 * @brief 
 * 
 */
bool isLoadRequested(void)
{
    return(readSwitchState(inLoad));
}

/* Function definition -------------------------------------------------------*/
/**
 * @brief 
 * 
 */
bool isChargeRequested(void)
{
    return(readSwitchState(inCharge));
}

/* Function definition -------------------------------------------------------*/
/**
 * @brief 
 * 
 */
bool isBalancingRequested(void)
{
    return(readSwitchState(inBalance));
}

/* Function definition -------------------------------------------------------*/
/**
 * @brief Turn ON the contactors
 * @param contactorID channel number
 * @retval BMS status
 */
bmsStatus_t turnContactorON(uint8_t contactorID)
{
    bmsStatus_t status = BMS_ERROR;

	if(!(IS_CONTACTOR_ID(contactorID)))
	{
		status = BMS_GPIO_FUNCTION_PARAMETER_ERROR;
		return status;
	}

    switch (contactorID)
    {
        case loadNegative:
            status = writePWM(pwmCh1, PWM_CHANNEL_HIGH);
            break;
        case chargeNegative:
            status = writePWM(pwmCh2, PWM_CHANNEL_HIGH);
            break;
        case commonPositive:
            status = writePWM(pwmCh3, PWM_CHANNEL_HIGH);
            break;
        case prechargePositive:
            status = writePWM(pwmCh4, PWM_CHANNEL_HIGH);
            delayMilliSeconds(PWM_CHANNEL_DELAY);
        default:
            status = BMS_ERROR;
            break;
    }

    return status;
}

/* Function definition -------------------------------------------------------*/
/**
 * @brief Turn OFF the contactors
 * @param contactorID channel number
 * @retval BMS status
 */
bmsStatus_t turnContactorOFF(uint8_t contactorID)
{
    bmsStatus_t status = BMS_ERROR;

	if(!(IS_CONTACTOR_ID(contactorID)))
	{
		status = BMS_GPIO_FUNCTION_PARAMETER_ERROR;
		return status;
	}

    switch (contactorID)
    {
        case loadNegative:
            status = writePWM(pwmCh1, PWM_CHANNEL_LOW);
            delayMilliSeconds(PWM_CHANNEL_DELAY);
            break;
        case chargeNegative:
            status = writePWM(pwmCh2, PWM_CHANNEL_LOW);
            delayMilliSeconds(PWM_CHANNEL_DELAY);
            break;
        case commonPositive:
            status = writePWM(pwmCh3, PWM_CHANNEL_LOW);
            delayMilliSeconds(PWM_CHANNEL_DELAY);
            break;
        case prechargePositive:
            status = writePWM(pwmCh4, PWM_CHANNEL_LOW);
            delayMilliSeconds(PWM_CHANNEL_DELAY);
        default:
            status = BMS_ERROR;
            break;
    }

    return status;
}

/**
 * @brief 
 * 
 */
void readAmbientTemperature(void)
{

}

/**
 * @brief 
 * 
 */
void readAmbientHumidity(void)
{

}

/**
 * @brief 
 * 
 */
bmsStatus_t enterCriticalSection(void)
{
    bmsStatus_t status = BMS_ERROR;
    __HAL_TIM_DISABLE_IT(&htim6,TIM_IT_UPDATE);
    __disable_irq();
    status = BMS_OK;
    return status;
}

/**
 * @brief 
 * 
 */
bmsStatus_t exitCriticalSection(void)
{
    bmsStatus_t status = BMS_ERROR;
    __enable_irq();
    __HAL_TIM_ENABLE_IT(&htim6,TIM_IT_UPDATE);
    status = BMS_OK;
    return status;
}


/* End of File ---------------------------------------------------------------*/

