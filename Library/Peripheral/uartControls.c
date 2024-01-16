/**
 * @file uartControls.c
 * @author Arpit Gupta (arpit@euler)
 * @brief 
 * @version 0.1
 * @date 2022-04-06
 * 
 * @copyright Copyright (c) 2022
 * 
 */

/* Standard C includes -------------------------------------------------------*/
#include <string.h>
/* Private includes ----------------------------------------------------------*/
#include "main.h"
#include "usart.h"
#include "onBoardUARTs.h"
#include "bmsConfiguration.h"
/* Self include --------------------------------------------------------------*/
#include "uartControls.h"
/* Type definiton ------------------------------------------------------------*/

/* External variables definiton ----------------------------------------------*/

/* Private constants ---------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

/* Private custom code -------------------------------------------------------*/

/* Function definition -------------------------------------------------------*/
/**
 * @brief Send data on serial uart
 * @param uartID uart number
 * @param stringData data to be sent
 * @return BMS status
 */
bmsStatus_t sendSerial(uint8_t uartID, char *stringData)
{
    bmsStatus_t status = BMS_ERROR;

	if(!(IS_UART_ID(uartID))  ||\
        (NULL == stringData) )
	{
        status = BMS_UART_FUNCTION_PARAMETER_ERROR;
		return status;
	}

    switch(uartID)
    {
        case debugPort:
            status = HAL_UART_Transmit(&hlpuart1, (uint8_t *)stringData, strlen((char *)stringData), 100);
            break;
        case vcuPort:
            status = HAL_UART_Transmit(&huart1, (uint8_t *)stringData, strlen((char *)stringData), 100);
            break;
        default:
            status = BMS_ERROR;
            break;
    }
    
    return status;
}

/* End of File ---------------------------------------------------------------*/

