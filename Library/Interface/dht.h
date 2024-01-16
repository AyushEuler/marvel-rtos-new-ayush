/**
 * @file analogControls.h
 * @author Arpit Gupta (arpit@euler)
 * @brief 
 * @version 0.1
 * @date 2022-04-09
 * 
 * @copyright Copyright (c) 2022
 * 
 */

/* Header Guard --------------------------------------------------------------*/
#ifndef __DHT_H_
#define __DHT_H_

#ifdef __cplusplus
extern "C" {
#endif


/* Standard C includes -------------------------------------------------------*/

#include "bmsConfiguration.h"
#include "stm32l4xx_hal.h"
#include "main.h"
#include "tim.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Private typedef -----------------------------------------------------------*/

/* External variables --------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Inline functions ----------------------------------------------------------*/
/* Function prototypes -------------------------------------------------------*/
void DHT11_Start (void);
uint8_t DHT11_Check_Response (void);

uint8_t DHT11_Read (void);
void Set_Pin_Input (GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin);
void Set_Pin_Output (GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin);
void delay (uint16_t time);
void dht11(uint8_t *humid, uint8_t *temp);
void Error_Handler(void);

#endif /* __MAIN_H */
/* End of File ---------------------------------------------------------------*/

