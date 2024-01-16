/**
 * @file timerControls.h
 * @author Arpit Gupta (arpit@euler)
 * @brief 
 * @version 0.1
 * @date 2022-04-15
 * 
 * @copyright Copyright (c) 2022
 * 
 */

/* Header Guard --------------------------------------------------------------*/
#ifndef TIMERCONTROLS_H_
#define TIMERCONTROLS_H_

/* Standard C includes -------------------------------------------------------*/
#include "stdint.h"
/* Private includes ----------------------------------------------------------*/
/* Private includes ----------------------------------------------------------*/
#include "tim.h"
#include "bmsConfiguration.h"
/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Private typedef -----------------------------------------------------------*/
enum pwmChannelID
{
    pwmCh1 = 0,
    pwmCh2,
    pwmCh3,
    pwmCh4
};

enum timerID
{
    cmuTimer = 0,

};

#define IS_TIMER_ID(ID)                     (((ID) == cmuTimer))

#define IS_PWM_CHANNEL_ID(ID)               (((ID) == pwmCh1) || ((ID) == pwmCh2) ||\
                                                     ((ID) == pwmCh3) || ((ID) == pwmCh4))


/* External variables --------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Inline functions ----------------------------------------------------------*/

/* Function prototypes -------------------------------------------------------*/
bmsStatus_t startTimer(uint8_t timerID);
bmsStatus_t stopTimer(uint8_t timerID);
uint32_t getTimCount(void);
bmsStatus_t writePWM(uint8_t channel, long int value);
uint32_t millis(void);
uint32_t micros(void);



#endif
/* End of File ---------------------------------------------------------------*/