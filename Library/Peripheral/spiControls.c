/**
 * @file spiControls.c
 * @author Arpit Gupta (arpit@euler)
 * @brief 
 * @version 0.1
 * @date 2022-04-09
 * 
 * @copyright Copyright (c) 2022
 * 
 */

/* Standard C includes -------------------------------------------------------*/

/* Private includes ----------------------------------------------------------*/
#include "spi.h"
#include "onBoardIOs.h"
#include "bmsConfiguration.h"
/* Self include --------------------------------------------------------------*/
#include "spiControls.h"
/* Type definiton ------------------------------------------------------------*/

/* External variables definiton ----------------------------------------------*/

/* Private constants ---------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

/* Private custom code -------------------------------------------------------*/

/* Function definition -------------------------------------------------------*/
/**
 * @brief write data
 * 
 * @param size size of data
 * @param tx_data data buffer
 * @retval BMS status
 */
bmsStatus_t spiWriteBytes(uint16_t size, uint8_t *tx_data)
{
    bmsStatus_t status = BMS_ERROR;

	if( (NULL == tx_data) 	||\
        ((size <= 0)))
	{
		status = BMS_SPI_FUNCTION_PARAMETER_ERROR;
		return status;
	}

    enterCriticalSection();
    status = HAL_SPI_Transmit(&hspi1, tx_data, size, 100);
    exitCriticalSection();
    return status;
}

/* Function definition -------------------------------------------------------*/
/**
 * @brief write read data
 * 
 * @param tx_data transmit data buffer
 * @param rx_data receive data buffer
 * @param sizeTx transmit data buffer size
 * @param sizeRx receive data buffer size
 * @retval BMS status
 */
bmsStatus_t spiWriteReadBytes(uint8_t *tx_data, uint8_t *rx_data, uint16_t sizeTx, uint16_t sizeRx)
{
    bmsStatus_t status = BMS_ERROR;

	if( (NULL == tx_data) 	||\
		(NULL == rx_data) 	||\
		(sizeTx <= 0)       ||\
        ((sizeRx <= 0)))
	{
		status = BMS_SPI_FUNCTION_PARAMETER_ERROR;
		return status;
	}

    enterCriticalSection();
    status = HAL_SPI_Transmit(&hspi1, tx_data, sizeTx, 100);
    CHECK_STATUS(status);
    status = HAL_SPI_Receive(&hspi1, rx_data, sizeRx, 100);
    exitCriticalSection();
    return status;
}

/**
 * @brief read write byte
 * 
 * @param tx_data transmit data
 * @param rx_data receive data
 * @retval BMS status
 */

bmsStatus_t spiReadWriteByte(uint8_t *tx_data, uint8_t *rx_data) 
{
    bmsStatus_t status = BMS_ERROR;

	if( (NULL == tx_data) 	||\
		(NULL == rx_data) )
	{
		status = BMS_SPI_FUNCTION_PARAMETER_ERROR;
		return status;
	}

    enterCriticalSection();
    status = HAL_SPI_TransmitReceive(&hspi1, tx_data, rx_data, 1, 100);
    exitCriticalSection();
    return status;
}

/* Function definition -------------------------------------------------------*/
/**
 * @brief write read data
 * 
 * @param size transmit data buffer size
 * @param rx_data receive data buffer
 * @retval BMS status
 */
bmsStatus_t spiReadBytes(uint16_t size, uint8_t *rx_data)
{
    bmsStatus_t status = BMS_ERROR;

	if( (NULL == rx_data) 	||\
        ((size <= 0)))
	{
		status = BMS_SPI_FUNCTION_PARAMETER_ERROR;
		return status;
	}

    enterCriticalSection();
    status = HAL_SPI_Receive(&hspi1, rx_data, size, 100);
    exitCriticalSection();
    return status;
}

/* End of File ---------------------------------------------------------------*/

