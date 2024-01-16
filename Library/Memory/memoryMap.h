/**
 * @file memoryMap.h
 * @author Arpit Gupta (arpit@euler)
 * @brief 
 * @version 0.1
 * @date 2022-02-25
 * 
 * @copyright Copyright (c) 2022
 * 
 */

/* Header Guard --------------------------------------------------------------*/
#ifndef MEMORYMAP_H_
#define MEMORYMAP_H_

/* Standard C includes -------------------------------------------------------*/
#include <stdint.h>

/* Private includes ----------------------------------------------------------*/

/* Private typedef -----------------------------------------------------------*/

typedef struct __attribute__((packed))
{
    uint16_t    imageMagic;
    uint16_t    imageHDRversion;
    uint32_t    crc;
    uint32_t    dataSize;
    uint8_t     imageType;
    uint8_t     versionMajor;
    uint8_t     versionMinor;
    uint8_t     versionPatch;
    uint32_t    activeApp;
    uint32_t    reserved;
    char        gitSHA[8];
} imageHDR_type;

/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/
#define projectID (uint32_t)0x66778351 //BMS3

/* External variables --------------------------------------------------------*/

extern uint32_t __sharedram_start__;
extern uint32_t __sharedram_size__;
extern uint32_t __bootloader_start__;
extern uint32_t __bootloader_size__;
extern uint32_t __app1_start__;
extern uint32_t __app1_size__;
extern uint32_t __app2_start__;
extern uint32_t __app2_size__;
extern uint32_t __commondata_start__;

/* Private variables ---------------------------------------------------------*/

/* Inline functions ----------------------------------------------------------*/

/* Function prototypes -------------------------------------------------------*/


#endif
/* End of File ---------------------------------------------------------------*/





