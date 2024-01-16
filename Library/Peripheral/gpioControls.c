/**
 * @file gpioControls.c
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
#include "main.h"
#include "onBoardIOs.h"
#include "bmsConfiguration.h"
#include "customDelays.h"
#include "threadxMain.h"
#include "onBoardMemory.h"
/* Self include --------------------------------------------------------------*/
#include "gpioControls.h"
/* Type definiton ------------------------------------------------------------*/

/* External variables definiton ----------------------------------------------*/

/* Private constants ---------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

/* Private custom code -------------------------------------------------------*/

#define CONTACTOR_DEBOUNCE_TIME_ms      500

uint8_t switchOneTimer = 0;
uint8_t switchOnePreviousState = 0;
uint8_t switchOneCurrentState = 0;

uint8_t switchTwoTimer = 0;
uint8_t switchTwoCurrentState = 0;
uint8_t switchTwoPreviousState = 0;


/* Function definition -------------------------------------------------------*/
/**
 * @brief Toggle the selected led
 * @param gpioID led gpio number
 * @retval BMS status
 */
bmsStatus_t toggleLED(uint8_t gpioID)
{
    bmsStatus_t status = BMS_ERROR;

	if(!(IS_LED_ID(gpioID)))
	{
		return BMS_GPIO_FUNCTION_PARAMETER_ERROR;
	}

    switch (gpioID)
    {
        case errorLEDType:
            HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);
            status = BMS_OK;
            break;
        case errorLEDCode:
            HAL_GPIO_TogglePin(LED2_GPIO_Port, LED2_Pin);
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
 * @brief Make the LED state HIGH
 * @param gpioID led gpio number
 * @retval BMS status
 */
bmsStatus_t setLED(uint8_t gpioID)
{
    bmsStatus_t status = BMS_ERROR;

	if(!(IS_LED_ID(gpioID)))
	{
		return BMS_GPIO_FUNCTION_PARAMETER_ERROR;
	}

    switch (gpioID)
    {
        case errorLEDType:
            HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_SET);
            status = BMS_OK;
            break;
        case errorLEDCode:
            HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_SET);
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
 * @brief Make the LED state LOW
 * @param gpioID led gpio number
 * @retval BMS status
 */
bmsStatus_t resetLED(uint8_t gpioID)
{
    bmsStatus_t status = BMS_ERROR;

	if(!(IS_LED_ID(gpioID)))
	{
		return BMS_GPIO_FUNCTION_PARAMETER_ERROR;
	}

    switch (gpioID)
    {
        case errorLEDType:
            HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_RESET);
            status = BMS_OK;           
            break;
        case errorLEDCode:
            HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_RESET);
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
 * @brief Read the state of the switch
 * @param gpioID switch gpio number
 * @retval state of the switch
 */
uint8_t readSwitchState(uint8_t gpioID)
{
    uint8_t switchState = 0;

	if(!(IS_SWITCH_ID(gpioID)))
	{
		return BMS_GPIO_FUNCTION_PARAMETER_ERROR;
	}

    switch(gpioID)
    {
        case inLoad:

            #ifdef HARDWARE_VERSION_3_1_1
                switchOneCurrentState = HAL_GPIO_ReadPin(controlSW_LOAD_GPIO_Port, controlSW_LOAD_Pin);
            #elif HARDWARE_VERSION_3_0
                switchOneCurrentState = HAL_GPIO_ReadPin(SW1_GPIO_Port, SW1_Pin);
            #endif
            
            if(switchOneCurrentState != switchOnePreviousState)
            {
                switchOneTimer++;
                if((switchOneTimer * LOOP_DELAY_CONTACTOR_CONTROL) >= CONTACTOR_DEBOUNCE_TIME_ms)
                {
                    switchOnePreviousState = switchOneCurrentState;
                }
            }
            else
            {
                switchOneTimer = 0;
            }
            return switchOnePreviousState;
            break;

        case inCharge:

            #ifdef HARDWARE_VERSION_3_1_1
                switchTwoCurrentState = HAL_GPIO_ReadPin(controlSW_CHRG_GPIO_Port, controlSW_CHRG_Pin);
            #elif HARDWARE_VERSION_3_0
                switchTwoCurrentState = HAL_GPIO_ReadPin(SW2_GPIO_Port, SW2_Pin);
            #endif
            
            if(switchTwoCurrentState != switchTwoPreviousState)
            {
                switchTwoTimer++;
                if((switchTwoTimer * LOOP_DELAY_CONTACTOR_CONTROL) >= CONTACTOR_DEBOUNCE_TIME_ms)
                {
                    switchTwoPreviousState = switchTwoCurrentState;
                }
            }
            else
            {
                switchTwoTimer = 0;
            }
            return switchTwoPreviousState;
            break;

        case inBalance:
            #ifdef HARDWARE_VERSION_3_1_1
                switchState = HAL_GPIO_ReadPin(controlSW_BLNC_GPIO_Port, controlSW_BLNC_Pin);
            #elif HARDWARE_VERSION_3_0
                switchState = HAL_GPIO_ReadPin(SW3_GPIO_Port, SW3_Pin);
            #endif
            
            break;
        case inReserved:
            switchState = HAL_GPIO_ReadPin(SW4_GPIO_Port, SW4_Pin);
            break;
        default:
            switchState =  2;
            break;
    }

    return switchState;
}

/* Function definition -------------------------------------------------------*/
/**
 * @brief Set the CS pin of SPI
 * @param spiID SPI number
 * @retval BMS status
 */
bmsStatus_t setSPISlaveSelect(uint8_t spiID)
{
    bmsStatus_t status = BMS_ERROR;

	if(!(IS_SPI_ID(spiID)))
	{
		status = BMS_SPI_FUNCTION_PARAMETER_ERROR;
		return status;
	}

    switch(spiID)
    {
        case cmuSpi:
        case ccmSpi:
            HAL_GPIO_WritePin(SPI1_NSS_GPIO_Port, SPI1_NSS_Pin, GPIO_PIN_SET);
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
 * @brief Reset the CS pin of SPI
 * @param spiID SPI number
 * @retval BMS status
 */
bmsStatus_t resetSPISlaveSelect(uint8_t spiID)
{
    bmsStatus_t status = BMS_ERROR;

	if(!(IS_SPI_ID(spiID)))
	{
		status = BMS_SPI_FUNCTION_PARAMETER_ERROR;
		return status;
	}

    switch(spiID)
    {
        case cmuSpi:
        case ccmSpi:
            HAL_GPIO_WritePin(SPI1_NSS_GPIO_Port, SPI1_NSS_Pin, GPIO_PIN_RESET);
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
 * @brief Set the enable pin of the CAN IC
 * @param canID CAN number
 * @retval BMS status
 */
bmsStatus_t setCANEnable(uint8_t canPort)
{
    bmsStatus_t status = BMS_ERROR;
    
    if(!(IS_CAN_PORT(canPort)))
	{
		status = BMS_CAN_FUNCTION_PARAMETER_ERROR;
		return status;
	}

    switch(canPort)
    {
        case communicationCAN:
            HAL_GPIO_WritePin(CAN1_EN_GPIO_Port, CAN1_EN_Pin, GPIO_PIN_SET);
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
 * @brief Reset the enable pin of the CAN IC
 * @param canID CAN number
 * @retval BMS status
 */
bmsStatus_t resetCANEnable(uint8_t canPort)
{
    bmsStatus_t status = BMS_ERROR;
	
    if(!(IS_CAN_PORT(canPort)))
	{
		status = BMS_CAN_FUNCTION_PARAMETER_ERROR;
		return status;
	}

    switch(canPort)
    {
        case communicationCAN:
            HAL_GPIO_WritePin(CAN1_EN_GPIO_Port, CAN1_EN_Pin, GPIO_PIN_RESET);
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
 * @brief When write protection is enabled, the device will not allow any new data to be written to it, or any existing data to be modified. 
 * @param eepromID eeprom ID
 * @retval BMS status
 */
bmsStatus_t eepromEnableWriteProtection(uint8_t eepromID)
{
    bmsStatus_t status = BMS_ERROR;

    if(!(IS_EEPROM_ID(eepromID)))
	{
		status = BMS_CAN_FUNCTION_PARAMETER_ERROR;
		return status;
	}

    switch(eepromID)
    {
        case eeprom:
            HAL_GPIO_WritePin(MEM_WC_GPIO_Port, MEM_WC_Pin, GPIO_PIN_SET);
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
 * @brief When write protection is disabled, the device will allow new data to be written to it.
 * @param eepromID eeprom ID
 * @retval BMS status
 */
bmsStatus_t eepromDisableWriteProtection(uint8_t eepromID)
{
    bmsStatus_t status = BMS_ERROR;

    if(!(IS_EEPROM_ID(eepromID)))
	{
		status = BMS_CAN_FUNCTION_PARAMETER_ERROR;
		return status;
	}

    switch(eepromID)
    {
        case eeprom:
            HAL_GPIO_WritePin(MEM_WC_GPIO_Port, MEM_WC_Pin, GPIO_PIN_RESET);
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
 * @brief Reset the short circuit
 * @param gpioID GPIO number
 * @retval BMS status
 */
bmsStatus_t resetShortCircuit(uint8_t gpioID)
{
    bmsStatus_t status = BMS_ERROR;

	if(!(IS_GPIO_ID(gpioID)))
	{
		status = BMS_GPIO_FUNCTION_PARAMETER_ERROR;
		return status;
	}

    switch(gpioID)
    {
        case shortCircuit:
            HAL_GPIO_WritePin(SC_Reset_GPIO_Port, SC_Reset_Pin, GPIO_PIN_SET);
            delayMilliSeconds(1);
            HAL_GPIO_WritePin(SC_Reset_GPIO_Port, SC_Reset_Pin, GPIO_PIN_RESET);
            status = BMS_OK;
            break;
        default:
            status = BMS_ERROR;
            break;
    }
    
    return status;
}


/* End of File ---------------------------------------------------------------*/
