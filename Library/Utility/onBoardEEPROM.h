/**
 * @file onBoardEEPROM.h
 * @author Lalit Ahuja
 * @brief 
 * @version 0.1
 * @date 2023-01-11
 * 
 * @copyright Copyright (c) 2023
 * 
 */

/* Header Guard --------------------------------------------------------------*/
#ifndef ONBOARDEEPROM_H_
#define ONBOARDEEPROM_H_

/* Standard C includes -------------------------------------------------------*/
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "inttypes.h"
/* Private includes ----------------------------------------------------------*/
#include "bmsConfiguration.h"
/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Private typedef -----------------------------------------------------------*/

/* External variables --------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Inline functions ----------------------------------------------------------*/

/* Function prototypes -------------------------------------------------------*/


/* Configure the size and address of the eeprom*/
#define EEPROM_SIZE												EEPROM_SIZE_4_KBIT
#define EEPROM_DEVICE_ADDRESS	 								0xA0
#define EEPROM_DEVICE_ADDRESS_1 								0xA2
#define EEPROM_DEVICE_ADDRESS_2 								0xA4
#define EEPROM_DEVICE_ADDRESS_3 								0xA8

/* Configure the delay values for the eeprom*/
#define DELAY_5MS												5
#define DELAY_10MS												10
#define EEPROM_DELAY 											DELAY_10MS

#define EEPROM_START_ADDRESS 									0

/* values to select for eeprom size*/
#define EEPROM_SIZE_1_KBIT   									1
#define EEPROM_SIZE_2_KBIT   									2
#define EEPROM_SIZE_4_KBIT   									4
#define EEPROM_SIZE_8_KBIT   									8
#define EEPROM_SIZE_16_KBIT   									16
#define EEPROM_SIZE_32_KBIT   									32
#define EEPROM_SIZE_64_KBIT    									64
#define EEPROM_SIZE_128_KBIT   									128
#define EEPROM_SIZE_256_KBIT   									256
#define EEPROM_SIZE_512_KBIT 				  					512
#define EEPROM_SIZE_1_MBIT   									1024
#define EEPROM_SIZE_2_MBIT   									2048

/* values to select for eeprom page size*/
#define EEPROM_PAGE_SIZE_16_BYTE 								16
#define EEPROM_PAGE_SIZE_32_BYTE 								32
#define EEPROM_PAGE_SIZE_64_BYTE 								64
#define EEPROM_PAGE_SIZE_128_BYTE 								128
#define EEPROM_PAGE_SIZE_256_BYTE 								256

/* values to select for eeprom memory address size*/
#define EEPROM_MEMORY_ADDRESS_SIZE_1_BYTE						1
#define EEPROM_MEMORY_ADDRESS_SIZE_2_BYTE						2


#define EEPROM_SIZE_4_KBIT_MEMORY_ADDRESS_MASK 					0x0100
#define EEPROM_SIZE_8_KBIT_MEMORY_ADDRESS_MASK 					0x0300
#define EEPROM_SIZE_16_KBIT_MEMORY_ADDRESS_MASK 				0x0700
#define EEPROM_SIZE_1_MBIT_MEMORY_ADDRESS_MASK 					0x010000
#define EEPROM_SIZE_2_MBIT_MEMORY_ADDRESS_MASK 					0x030000

#define EEPROM_MEMORY_ADDRESS_SIZE_1_BYTE_SHIFT_POSITION		7
#define EEPROM_MEMORY_ADDRESS_SIZE_2_BYTE_SHIFT_POSITION		15

#define NUMBER_OF_BIT_IN_ONE_BYTE 								8
#define NUMBER_OF_BIT_IN_ONE_KBIT 								1024
#define CONVERT_KBIT_TO_BYTE(x)									((((x)*NUMBER_OF_BIT_IN_ONE_KBIT)/NUMBER_OF_BIT_IN_ONE_BYTE))


#if (EEPROM_SIZE == EEPROM_SIZE_1_KBIT) || (EEPROM_SIZE == EEPROM_SIZE_2_KBIT) || \
	(EEPROM_SIZE == EEPROM_SIZE_4_KBIT) || (EEPROM_SIZE == EEPROM_SIZE_8_KBIT) || \
	(EEPROM_SIZE == EEPROM_SIZE_16_KBIT)
	#define EEPROM_PAGE_SIZE     			EEPROM_PAGE_SIZE_16_BYTE
	#define EEPROM_MEMORY_ADDRESS_SIZE     	EEPROM_MEMORY_ADDRESS_SIZE_1_BYTE

#elif (EEPROM_SIZE == EEPROM_SIZE_32_KBIT) || (EEPROM_SIZE == EEPROM_SIZE_64_KBIT)
	#define EEPROM_PAGE_SIZE     			EEPROM_PAGE_SIZE_32_BYTE
	#define EEPROM_MEMORY_ADDRESS_SIZE     	EEPROM_MEMORY_ADDRESS_SIZE_2_BYTE

#elif (EEPROM_SIZE == EEPROM_SIZE_128_KBIT) || (EEPROM_SIZE == EEPROM_SIZE_256_KBIT)
	#define EEPROM_PAGE_SIZE     			EEPROM_PAGE_SIZE_64_BYTE
	#define EEPROM_MEMORY_ADDRESS_SIZE     	EEPROM_MEMORY_ADDRESS_SIZE_2_BYTE

#elif (EEPROM_SIZE == EEPROM_SIZE_512_KBIT)
	#define EEPROM_PAGE_SIZE     			EEPROM_PAGE_SIZE_128_BYTE
	#define EEPROM_MEMORY_ADDRESS_SIZE     	EEPROM_MEMORY_ADDRESS_SIZE_2_BYTE

#elif (EEPROM_SIZE == EEPROM_SIZE_1_MBIT) || (EEPROM_SIZE == EEPROM_SIZE_2_MBIT)
	#define EEPROM_PAGE_SIZE     			EEPROM_PAGE_SIZE_256_BYTE
	#define EEPROM_MEMORY_ADDRESS_SIZE     	EEPROM_MEMORY_ADDRESS_SIZE_2_BYTE
#endif


#define IS_EEPROM_ADDRESS(ADDRESS)	    (((ADDRESS) == EEPROM_DEVICE_ADDRESS)  || ((ADDRESS) == EEPROM_DEVICE_ADDRESS_1) ||\
                                    	((ADDRESS) == EEPROM_DEVICE_ADDRESS_2) || ((ADDRESS) == EEPROM_DEVICE_ADDRESS_3)  )

bmsStatus_t eepromRead (uint16_t deviceAddress, uint32_t memoryAddress, uint8_t *dataBuffer, uint16_t dataLength);
bmsStatus_t eepromWrite(uint16_t deviceAddress, uint32_t memoryAddress, uint8_t *dataBuffer, uint16_t dataLength);
bmsStatus_t eepromEraseAllPage(void);


#endif
/* End of File ---------------------------------------------------------------*/

