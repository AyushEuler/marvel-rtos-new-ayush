/**
 * @file onBoardUARTs.c
 * @author Arpit Gupta (arpit@euler)
 * @brief 
 * @version 0.1
 * @date 2022-04-06
 * 
 * @copyright Copyright (c) 2022
 * 
 */

/* Standard C includes -------------------------------------------------------*/

/* Private includes ----------------------------------------------------------*/
#include "uartControls.h"
/* Self include --------------------------------------------------------------*/
#include "onBoardUARTs.h"
/* Type definiton ------------------------------------------------------------*/

/* External variables definiton ----------------------------------------------*/

/* Private constants ---------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

/* Private custom code -------------------------------------------------------*/

/* Function definition -------------------------------------------------------*/
/**
 * @brief Send data on debug port
 * @param outString data to be sent
 * @return BMS status 
 */
bmsStatus_t sendDataOnDebugPort(char *outString)
{
    if((NULL == outString))
	{
		return BMS_UART_FUNCTION_PARAMETER_ERROR;
	}
#ifdef DEBUG
    bmsStatus_t status = BMS_ERROR;
    status = sendSerial(debugPort, outString);
    return status;
#endif
    return BMS_OK;
}
/* Function definition -------------------------------------------------------*/
/**

/* Function definition -------------------------------------------------------*/
/**

/* End of File ---------------------------------------------------------------*/

