/**
 * @file onBoardFlash.h
 * @author
 * @brief 
 * @version 0.1
 * @date
 * 
 * @copyright Copyright (c) 2022
 * 
 */

/* Header Guard --------------------------------------------------------------*/
#ifndef ONBOARDFLASH_H_
#define ONBOARDFLASH_H_

/* Standard C includes -------------------------------------------------------*/
#include <stdio.h>
#include <stdint.h>
/* Private includes ----------------------------------------------------------*/
#include "bmsConfiguration.h"
/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Private typedef -----------------------------------------------------------*/

typedef enum
{
    NUMBER_OF_CMU_INDEX = 0,
    CELL_IN_SERIES_INDEX,

    //cell per slave
    MAX_CELL_PER_SLAVE_INDEX,
    CELL_IN_SLAVE_1_INDEX,
    CELL_IN_SLAVE_2_INDEX,
    CELL_IN_SLAVE_3_INDEX,
    CELL_IN_SLAVE_4_INDEX,
    CELL_IN_SLAVE_5_INDEX,
    CELL_IN_SLAVE_6_INDEX,
    CELL_IN_SLAVE_7_INDEX,
    CELL_IN_SLAVE_8_INDEX,
    CELL_IN_SLAVE_9_INDEX,
    CELL_IN_SLAVE_10_INDEX,
    CELL_IN_SLAVE_11_INDEX,
    CELL_IN_SLAVE_12_INDEX,

    //ntc
    EXTERNAL_NTC_PER_SLAVE_INDEX,
    INTERNAL_NTC_PER_SLAVE_INDEX,
    NTC_BETA_VALUE_AT_25C_INDEX,

    //shunt
    SHUNT_RESISTOR_uOhm_INDEX,

	//cell voltage
    CELL_MAX_VOLTAGE_THRESHOLD_mV_INDEX,
    CELL_MIN_VOLTAGE_THRESHOLD_mV_INDEX,
    CELL_BALANCING_START_VOLTAGE_mV_INDEX,
    CELL_IMBALANCE_THRESHOLD_mV_INDEX,

    //current
    MAX_CHARGE_CURRENT_THRESHOLD_A_INDEX,
    MAX_DISCHARGE_CURRENT_THRESHOLD_A_INDEX,

    //capacity
    PACK_MAX_CAPACITY_Ah_INDEX,
    PACK_MIN_CAPACITY_Ah_INDEX,

    //cell temp
    CELL_MAX_TEMP_THRESHOLD_C_INDEX,
    CELL_MIN_TEMP_THRESHOLD_C_INDEX,

    //board temp
    BOARD_MAX_TEMP_THRESHOLD_C_INDEX,
    BOARD_MIN_TEMP_THRESHOLD_C_INDEX,

    //timeout
    ERROR_TIMEOUT_ms_INDEX,
    WARNING_TIMEOUT_ms_INDEX,
    RECOVERY_TIMEOUT_ms_INDEX,

    //protection threshold
    MAX_ALLOWED_TEMP_CHANGE_C_INDEX,
    MAX_ALLOWED_TEMP_GRADIENT_C_INDEX,
    HIGH_IMBALANCE_ERROR_mV_INDEX,

    //balancing
    BALANCING_OFF_TIME_ms_INDEX,
    BALANCING_ON_TIME_ms_INDEX,

    //contactor
    CONTACTOR_CUT_OFF_TIME_ms_INDEX,
    PRE_CHARGE_CURRENT_mA_INDEX,
	
    //cc
	VEHICLE_REST_TIME_m_INDEX,
    VEHICLE_REST_CURRENT_A_INDEX,

    //data pipeline
    CHARGE_CURRENT_DETECTION_THRESHOLD_A_INDEX,

    //retry counts
    RETRY_PRIORITY_ZERO_INDEX,
    RETRY_PRIORITY_ONE_INDEX,
    RETRY_PRIORITY_TWO_INDEX,
    RETRY_PRIORITY_THREE_INDEX,
    RETRY_PRIORITY_FOUR_INDEX,
    RETRY_PRIORITY_FIVE_INDEX,

    //balancing derating
    BALANCING_DERATING_START_TEMP_C_INDEX,
    BALANCING_DERATING_END_TEMP_C_INDEX,
    BALANCING_MAX_ON_TIME_ms_INDEX,
    BALANCING_MIN_ON_TIME_ms_INDEX,
    BALANCING_MAX_OFF_TIME_ms_INDEX,
    BALANCING_MIN_OFF_TIME_ms_INDEX,

    //protection thresholds
    OCC_ERROR_CURRENT_A_INDEX,
    OCC_WARNING_CURRENT_A_INDEX,
    OCD_ERROR_CURRENT_A_INDEX,
    OCD_WARNING_CURRENT_A_INDEX,

    //temp group1
    OTC_ERROR_TEMPERATURE_GROUP_1_INDEX,
    OTC_WARNING_TEMPERATURE_GROUP_1_INDEX,
    OTC_RECOVERY_TEMPERATURE_GROUP_1_INDEX,

    OTD_ERROR_TEMPERATURE_GROUP_1_INDEX,
    OTD_WARNING_TEMPERATURE_GROUP_1_INDEX,
    OTD_RECOVERY_TEMPERATURE_GROUP_1_INDEX,

    UTC_ERROR_TEMPERATURE_GROUP_1_INDEX,
    UTC_WARNING_TEMPERATURE_GROUP_1_INDEX,
    UTC_RECOVERY_TEMPERATURE_GROUP_1_INDEX,

    UTD_ERROR_TEMPERATURE_GROUP_1_INDEX,
    UTD_WARNING_TEMPERATURE_GROUP_1_INDEX,
    UTD_RECOVERY_TEMPERATURE_GROUP_1_INDEX,

    //temp group2
    OTC_ERROR_TEMPERATURE_GROUP_2_INDEX,
    OTC_WARNING_TEMPERATURE_GROUP_2_INDEX,
    OTC_RECOVERY_TEMPERATURE_GROUP_2_INDEX,

    OTD_ERROR_TEMPERATURE_GROUP_2_INDEX,
    OTD_WARNING_TEMPERATURE_GROUP_2_INDEX,
    OTD_RECOVERY_TEMPERATURE_GROUP_2_INDEX,

    UTC_ERROR_TEMPERATURE_GROUP_2_INDEX,
    UTC_WARNING_TEMPERATURE_GROUP_2_INDEX,
    UTC_RECOVERY_TEMPERATURE_GROUP_2_INDEX,

    UTD_ERROR_TEMPERATURE_GROUP_2_INDEX,
    UTD_WARNING_TEMPERATURE_GROUP_2_INDEX,
    UTD_RECOVERY_TEMPERATURE_GROUP_2_INDEX,

    //temp group3
    OTC_ERROR_TEMPERATURE_GROUP_3_INDEX,
    OTC_WARNING_TEMPERATURE_GROUP_3_INDEX,
    OTC_RECOVERY_TEMPERATURE_GROUP_3_INDEX,

    OTD_ERROR_TEMPERATURE_GROUP_3_INDEX,
    OTD_WARNING_TEMPERATURE_GROUP_3_INDEX,
    OTD_RECOVERY_TEMPERATURE_GROUP_3_INDEX,

    UTC_ERROR_TEMPERATURE_GROUP_3_INDEX,
    UTC_WARNING_TEMPERATURE_GROUP_3_INDEX,
    UTC_RECOVERY_TEMPERATURE_GROUP_3_INDEX,

    UTD_ERROR_TEMPERATURE_GROUP_3_INDEX,
    UTD_WARNING_TEMPERATURE_GROUP_3_INDEX,
    UTD_RECOVERY_TEMPERATURE_GROUP_3_INDEX,

    //temp group4
    OTC_ERROR_TEMPERATURE_GROUP_4_INDEX,
    OTC_WARNING_TEMPERATURE_GROUP_4_INDEX,
    OTC_RECOVERY_TEMPERATURE_GROUP_4_INDEX,

    OTD_ERROR_TEMPERATURE_GROUP_4_INDEX,
    OTD_WARNING_TEMPERATURE_GROUP_4_INDEX,
    OTD_RECOVERY_TEMPERATURE_GROUP_4_INDEX,

    UTC_ERROR_TEMPERATURE_GROUP_4_INDEX,
    UTC_WARNING_TEMPERATURE_GROUP_4_INDEX,
    UTC_RECOVERY_TEMPERATURE_GROUP_4_INDEX,

    UTD_ERROR_TEMPERATURE_GROUP_4_INDEX,
    UTD_WARNING_TEMPERATURE_GROUP_4_INDEX,
    UTD_RECOVERY_TEMPERATURE_GROUP_4_INDEX,

    //temp group5
    OTC_ERROR_TEMPERATURE_GROUP_5_INDEX,
    OTC_WARNING_TEMPERATURE_GROUP_5_INDEX,
    OTC_RECOVERY_TEMPERATURE_GROUP_5_INDEX,

    OTD_ERROR_TEMPERATURE_GROUP_5_INDEX,
    OTD_WARNING_TEMPERATURE_GROUP_5_INDEX,
    OTD_RECOVERY_TEMPERATURE_GROUP_5_INDEX,

    UTC_ERROR_TEMPERATURE_GROUP_5_INDEX,
    UTC_WARNING_TEMPERATURE_GROUP_5_INDEX,
    UTC_RECOVERY_TEMPERATURE_GROUP_5_INDEX,

    UTD_ERROR_TEMPERATURE_GROUP_5_INDEX,
    UTD_WARNING_TEMPERATURE_GROUP_5_INDEX,
    UTD_RECOVERY_TEMPERATURE_GROUP_5_INDEX,

    //ARC REACTOR
    AR_CARTRAGE_TEMP_INDEX,                                    
    AR_COOLING_TEMP_INDEX,                                      
    AR_COOLING_TEMP_CHARGING_INDEX,                             
    AR_ENABLE_INDEX,                                            
    AR_HEATING_TEMP_INDEX,                                      
    AR_HEATING_TEMP_CHARGING_INDEX,                             
    AR_COOLING_TYPE_INDEX,  
    //crc
    FLASH_CONFIG_CRC_INDEX,
} flashConfigDataIndex_t;

#define DATA_EMPTY  		                    0xFFFFFFFFFFFFFFFFUL
#define FLASH_CONFIG_LAST_DATA_INDEX            FLASH_CONFIG_CRC_INDEX

#define CRC_LENGTH                              1
#define FLASH_CONFIG_CRC_LENGTH                 CRC_LENGTH
#define FLASH_CONFIG_NUMBER_OF_PARAMETERS       FLASH_CONFIG_LAST_DATA_INDEX
#define FLASH_CONFIG_TOTAL_LENGTH               ((FLASH_CONFIG_NUMBER_OF_PARAMETERS) + (FLASH_CONFIG_CRC_LENGTH))

#define FLASH_CONFIG_SECTION		            1
#define FLASH_COTA_UPDATE                       2
#define FLASH_EXCEPTION_SECTION 	            3

#define IS_FLASH_SECTION_TYPE(VALUE)            (((VALUE) == FLASH_CONFIG_SECTION) || ((VALUE) == FLASH_EXCEPTION_SECTION) || ((VALUE) == FLASH_COTA_UPDATE))

/* External variables --------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Inline functions ----------------------------------------------------------*/

/* Function prototypes -------------------------------------------------------*/
extern __IO uint64_t defaultFlashConfigData[FLASH_CONFIG_TOTAL_LENGTH];

bmsStatus_t initializeFlashConfigData   (__IO uint64_t *data);
bmsStatus_t readDataFromFlash           (uint8_t sectionType, uint32_t flashAddress, __IO uint64_t *flashData, uint16_t dataSize);
bmsStatus_t writeDataToFlash            (uint8_t sectionType, uint32_t flashAddress, __IO uint64_t *flashData, uint16_t dataSize, uint8_t *receivedData);

bmsStatus_t checkFlashData              (uint8_t sectionType, uint32_t flashAddress, __IO uint64_t *flashData, uint16_t dataSize);
uint32_t    calculateFlashCRC           ( __IO uint64_t *data, uint16_t size);
bmsStatus_t updateReceivedData          (uint8_t *receivedData, uint16_t *index, uint64_t *updatedData);
bmsStatus_t checkReceivedDataIsSame     (__IO uint64_t *flashData, uint16_t *index, uint64_t *updatedData);
bmsStatus_t updateStoredData            (__IO uint64_t *flashData, uint16_t *index, uint64_t *updatedData);
#endif
/* End of File ---------------------------------------------------------------*/