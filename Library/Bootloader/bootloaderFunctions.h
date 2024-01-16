/**
 * @file bootloaderFunctions.h
 * @author Arpit Gupta (arpit@euler)
 * @brief 
 * @version 0.1
 * @date 2022-02-25
 * 
 * @copyright Copyright (c) 2022
 * 
 */

/* Header Guard --------------------------------------------------------------*/
#ifndef BOOTLOADERFUNCTIONS_H_
#define BOOTLOADERFUNCTIONS_H_

/* Standard C includes -------------------------------------------------------*/
#include "stdint.h"
#include "memoryMap.h"

/* Private includes ----------------------------------------------------------*/

/* Private typedef -----------------------------------------------------------*/


//image header contains 32 bytes

typedef struct __attribute__((packed))
{
	uint16_t image_magic;
	uint16_t image_hdr_version;
	uint32_t crc;
	uint32_t data_size;
	uint8_t image_type;
	uint8_t version_major;
	uint8_t version_minor;
	uint8_t version_patch;
	uint32_t active_app;
	uint32_t reserved;
	char git_sha[8];
} image_hdr_t;

#ifdef APP1
#define CURR_APP 1
#endif
#ifdef APP2
#define CURR_APP 2
#endif
/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/
#define VECTOR_OFFSET    (uint32_t)0x200
#define APP_START_OFFSET (uint16_t)32

#define APP1_VECTOR (uint32_t)(VECTOR_OFFSET + (uint32_t)&__app1_start__)
#define APP2_VECTOR (uint32_t)(VECTOR_OFFSET + (uint32_t)&__app2_start__)

/*******************************************************************************/

#define SHARED_MAGIC (uint32_t)0x27182818
#define UART_WAIT    (uint16_t)500

#define IMAGE_MAGIC          0x7777
#define IMAGE_HEADER_VERSION 1

#ifndef IMAGE_CRC
#define IMAGE_CRC 0
#endif

#define BOOTLOOP_RETRY_COUNT (uint8_t)8
#define BL_SWITCH_PARTITION  (uint32_t)(1 << 0)


#define BIT0  (1 << 0)
#define BIT1  (1 << 1)
#define BIT2  (1 << 2)
#define BIT3  (1 << 3)
#define BIT4  (1 << 4)
#define BIT5  (1 << 5)
#define BIT6  (1 << 6)
#define BIT7  (1 << 7)
#define BIT8  (1 << 8)
#define BIT9  (1 << 9)
#define BIT10 (1 << 10)
#define BIT11 (1 << 11)
#define BIT12 (1 << 12)
#define BIT13 (1 << 13)
#define BIT14 (1 << 14)
#define BIT15 (1 << 15)
#define BIT16 (1 << 16)
#define BIT17 (1 << 17)
#define BIT18 (1 << 18)
#define BIT19 (1 << 19)
#define BIT20 (1 << 20)
#define BIT21 (1 << 21)
#define BIT22 (1 << 22)
#define BIT23 (1 << 23)
#define BIT24 (1 << 24)
#define BIT25 (1 << 25)
#define BIT26 (1 << 26)
#define BIT27 (1 << 27)
#define BIT28 (1 << 28)
#define BIT29 (1 << 29)
#define BIT30 (1 << 30)
#define BIT31 (1 << 31)


#define STARK_CAN_TP_RX     BIT0
#define START_OTA 			BIT1




/* External variables --------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Inline functions ----------------------------------------------------------*/

/* Function prototypes -------------------------------------------------------*/
uint8_t switchToPrimaryApp(void);
uint8_t switchToSecondaryApp(void);
uint16_t getAppPartition(void);
uint8_t verifyApp1(void);
uint8_t verifyApp2(void);
void bootloader_flag_handler(uint32_t *flag);
uint8_t setApp1Active(void);
uint8_t setApp2Active(void);
uint32_t crc32_for_byte(uint32_t r);
uint32_t crc32(const void *data, uint32_t n_bytes);

#endif
/* End of File ---------------------------------------------------------------*/




