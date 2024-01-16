/**
 * @file spiControls.h
 * @author Arpit Gupta (arpit@euler)
 * @brief 
 * @version 0.1
 * @date 2022-04-09
 * 
 * @copyright Copyright (c) 2022
 * 
 */

/* Header Guard --------------------------------------------------------------*/
#ifndef SPICONTROLS_H_
#define SPICONTROLS_H_

/* Standard C includes -------------------------------------------------------*/
#include "stdint.h"
/* Private includes ----------------------------------------------------------*/
#include "bmsConfiguration.h"
/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Private typedef -----------------------------------------------------------*/

/* External variables --------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Inline functions ----------------------------------------------------------*/

/* Function prototypes -------------------------------------------------------*/
bmsStatus_t spiWriteBytes
(
  uint16_t size,                                /*Option: Number of bytes to be written on the SPI port*/
  uint8_t *tx_data                              /*Array of bytes to be written on the SPI port*/
);

bmsStatus_t spiWriteReadBytes
(
  uint8_t *tx_data,                             /*array of data to be written on SPI port*/
  uint8_t *rx_data,                             /*Input: array that will store the data read by the SPI port*/
  uint16_t sizeTx,                              /*Option: number of bytes Tx*/  
  uint16_t sizeRx                               /*Option: number of bytes Rx*/
);

bmsStatus_t spiReadWriteByte
(
  uint8_t *tx_data,                             /*array of data to be written on SPI port*/
  uint8_t *rx_data                              /*Input: array that will store the data read by the SPI port*/
);

bmsStatus_t spiReadBytes
(
  uint16_t size,                                /*Option: number of bytes*/
  uint8_t *rx_data                              /*Input: array that will store the data read by the SPI port*/
);

#endif
/* End of File ---------------------------------------------------------------*/

