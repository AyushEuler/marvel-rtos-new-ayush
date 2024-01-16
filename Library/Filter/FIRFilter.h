/**
 * @file FIRFilter.h
 * @author Lalit Ahuja
 * @brief 
 * @version 0.1
 * @date 2022-12-15
 * 
 * @copyright Copyright (c) 2022
 * 
 */

/* Header Guard --------------------------------------------------------------*/
#ifndef FIRFILTER_H_
#define FIRFILTER_H_

/* Standard C includes -------------------------------------------------------*/
#include <stdint.h>
/* Private includes ----------------------------------------------------------*/
#include "bmsConfiguration.h"
/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Private typedef -----------------------------------------------------------*/

/* External variables --------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Inline functions ----------------------------------------------------------*/

/* Function prototypes -------------------------------------------------------*/

#define FIR_FILTER_LENGTH 10

typedef struct {
	float 	buf[FIR_FILTER_LENGTH];
	uint8_t bufIndex;
	float 	out;
} FIRFilter_t;

bmsStatus_t FIRFilterInit	(FIRFilter_t *fir);
float 		FIRFilterUpdate	(FIRFilter_t *fir, float data);

#endif
/* End of File ---------------------------------------------------------------*/

