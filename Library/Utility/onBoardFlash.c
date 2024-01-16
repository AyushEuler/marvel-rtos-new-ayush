/**
 * @file onBoardFlash.c
 * @author 
 * @brief 
 * @version 0.1
 * @date 
 * 
 * @copyright Copyright (c) 2022
 * 
 */

/* Standard C includes -------------------------------------------------------*/
#include <stdio.h>
#include <string.h>
/* Private includes ----------------------------------------------------------*/
#include "flashControls.h"
#include "bmsConfiguration.h"
/* Self include --------------------------------------------------------------*/
#include "onBoardFlash.h"
/* Type definiton ------------------------------------------------------------*/

/* External variables definiton ----------------------------------------------*/

/* Private constants ---------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

/* Private custom code -------------------------------------------------------*/
#define FLASH_CONFIG_DATA_RANGE_MIN_INDEX					0
#define FLASH_CONFIG_DATA_RANGE_MAX_INDEX					1

__IO uint64_t defaultFlashConfigData[FLASH_CONFIG_TOTAL_LENGTH] = {0};
uint8_t configWriteFlag = 0;

uint32_t flashConfigDataRange[FLASH_CONFIG_TOTAL_LENGTH][2] = {\
				            {NUMBER_OF_CMU_MIN_VALUE, 	   							NUMBER_OF_CMU_MAX_VALUE},\
			                {CELL_IN_SERIES_MIN_VALUE, 	   							CELL_IN_SERIES_MAX_VALUE},\
			                {MAX_CELL_PER_SLAVE_MIN_VALUE, 	  						MAX_CELL_PER_SLAVE_MAX_VALUE},\
/*1 */		                {CELL_IN_SLAVE_MIN_VALUE, 	   							CELL_IN_SLAVE_MAX_VALUE},\
/*2 */		                {CELL_IN_SLAVE_MIN_VALUE, 	   							CELL_IN_SLAVE_MAX_VALUE},\
/*3 */		                {CELL_IN_SLAVE_MIN_VALUE, 	   							CELL_IN_SLAVE_MAX_VALUE},\
/*4 */		                {CELL_IN_SLAVE_MIN_VALUE, 	   							CELL_IN_SLAVE_MAX_VALUE},\
/*5 */		                {CELL_IN_SLAVE_MIN_VALUE, 	   							CELL_IN_SLAVE_MAX_VALUE},\
/*6 */		                {CELL_IN_SLAVE_MIN_VALUE, 	   							CELL_IN_SLAVE_MAX_VALUE},\
/*7 */		                {CELL_IN_SLAVE_MIN_VALUE, 	   							CELL_IN_SLAVE_MAX_VALUE},\
/*8 */		                {CELL_IN_SLAVE_MIN_VALUE, 	   							CELL_IN_SLAVE_MAX_VALUE},\
/*9 */		                {CELL_IN_SLAVE_MIN_VALUE, 	   							CELL_IN_SLAVE_MAX_VALUE},\
/*10*/		                {CELL_IN_SLAVE_MIN_VALUE, 	   							CELL_IN_SLAVE_MAX_VALUE},\
/*11*/		                {CELL_IN_SLAVE_MIN_VALUE, 	   							CELL_IN_SLAVE_MAX_VALUE},\
/*12*/		                {CELL_IN_SLAVE_MIN_VALUE, 	   							CELL_IN_SLAVE_MAX_VALUE},\
/*ntc*/		                {EXTERNAL_NTC_PER_SLAVE_MIN_VALUE, 	   					EXTERNAL_NTC_PER_SLAVE_MAX_VALUE},\
			                {INTERNAL_NTC_PER_SLAVE_MIN_VALUE, 	   					INTERNAL_NTC_PER_SLAVE_MAX_VALUE},\
			                {NTC_BETA_VALUE_AT_25C_MIN_VALUE, 	   					NTC_BETA_VALUE_AT_25C_MIN_VALUE},\
/*shunt*/	                {SHUNT_RESISTOR_uOhm_MIN_VALUE, 	   					SHUNT_RESISTOR_uOhm_MAX_VALUE},\
/*cell voltage*/			{CELL_MAX_VOLTAGE_THRESHOLD_mV_MIN_VALUE, 	   			CELL_MAX_VOLTAGE_THRESHOLD_mV_MAX_VALUE},\
							{CELL_MIN_VOLTAGE_THRESHOLD_mV_MIN_VALUE, 	   			CELL_MIN_VOLTAGE_THRESHOLD_mV_MAX_VALUE},\
							{CELL_BALANCING_START_VOLTAGE_mV_MIN_VALUE, 	  	 	CELL_BALANCING_START_VOLTAGE_mV_MAX_VALUE},\
							{CELL_IMBALANCE_THRESHOLD_mV_MIN_VALUE, 	   			CELL_IMBALANCE_THRESHOLD_mV_MAX_VALUE},\
/*current*/					{MAX_CHARGE_CURRENT_THRESHOLD_A_MIN_VALUE, 	   			MAX_CHARGE_CURRENT_THRESHOLD_A_MAX_VALUE},\
							{MAX_DISCHARGE_CURRENT_THRESHOLD_A_MIN_VALUE, 	   		MAX_DISCHARGE_CURRENT_THRESHOLD_A_MAX_VALUE},\
/*capacity*/				{PACK_MAX_CAPACITY_Ah_MIN_VALUE, 	   					PACK_MAX_CAPACITY_Ah_MAX_VALUE},\
							{PACK_MIN_CAPACITY_Ah_MIN_VALUE, 	   					PACK_MIN_CAPACITY_Ah_MAX_VALUE},\
/*cell temp*/				{CELL_MAX_TEMP_THRESHOLD_C_MIN_VALUE, 	   				CELL_MAX_TEMP_THRESHOLD_C_MAX_VALUE},\
							{CELL_MIN_TEMP_THRESHOLD_C_MIN_VALUE, 	   				CELL_MIN_TEMP_THRESHOLD_C_MAX_VALUE},\
/*board temp*/				{BOARD_MAX_TEMP_THRESHOLD_C_MIN_VALUE, 	   				BOARD_MAX_TEMP_THRESHOLD_C_MAX_VALUE},\
							{BOARD_MIN_TEMP_THRESHOLD_C_MIN_VALUE, 	   				BOARD_MIN_TEMP_THRESHOLD_C_MAX_VALUE},\
/*timeout*/					{ERROR_TIMEOUT_ms_MIN_VALUE, 	   						ERROR_TIMEOUT_ms_MAX_VALUE},\
							{WARNING_TIMEOUT_ms_MIN_VALUE, 	   						WARNING_TIMEOUT_ms_MAX_VALUE},\
							{RECOVERY_TIMEOUT_ms_MIN_VALUE, 	   					RECOVERY_TIMEOUT_ms_MAX_VALUE},\
/*protection threshold*/	{MAX_ALLOWED_TEMP_CHANGE_C_MIN_VALUE, 	   				MAX_ALLOWED_TEMP_CHANGE_C_MAX_VALUE},\
							{MAX_ALLOWED_TEMP_GRADIENT_C_MIN_VALUE, 	   			MAX_ALLOWED_TEMP_GRADIENT_C_MAX_VALUE},\
							{HIGH_IMBALANCE_ERROR_mV_MIN_VALUE, 	  				HIGH_IMBALANCE_ERROR_mV_MAX_VALUE},\
/*balancing*/				{BALANCING_OFF_TIME_ms_MIN_VALUE, 	   					BALANCING_OFF_TIME_ms_MAX_VALUE},\
							{BALANCING_ON_TIME_ms_MIN_VALUE, 	   					BALANCING_ON_TIME_ms_MAX_VALUE},\
/*contactor*/				{CONTACTOR_CUT_OFF_TIME_ms_MIN_VALUE, 	   				CONTACTOR_CUT_OFF_TIME_ms_MAX_VALUE},\
							{PRE_CHARGE_CURRENT_mA_MIN_VALUE, 	   					PRE_CHARGE_CURRENT_mA_MAX_VALUE},\
/*cc*/						{VEHICLE_REST_TIME_m_MIN_VALUE, 	   					VEHICLE_REST_TIME_m_MAX_VALUE},\
							{VEHICLE_REST_CURRENT_A_MIN_VALUE, 	   					VEHICLE_REST_CURRENT_A_MAX_VALUE},\
/*data pipeline*/			{CHARGE_CURRENT_DETECTION_THRESHOLD_A_MIN_VALUE, 	    CHARGE_CURRENT_DETECTION_THRESHOLD_A_MAX_VALUE},\
};



/* Function definition -------------------------------------------------------*/
/**
 * @brief initialize all the configuration parameters from the #define
 * @param data pointer to the array
 * @retval BMS status
 */
bmsStatus_t initializeFlashConfigData(__IO uint64_t *data)
{
	bmsStatus_t status = BMS_ERROR;

	if(NULL == data)
	{
		status = BMS_FLASH_FUNCTION_PARAMETER_ERROR;
		return status;
	}

	data[NUMBER_OF_CMU_INDEX]                	= NUMBER_OF_CMU;
	data[CELL_IN_SERIES_INDEX]               	= CELL_IN_SERIES;
	data[MAX_CELL_PER_SLAVE_INDEX]              = MAX_CELL_PER_SLAVE;

	//cell per slave
	data[CELL_IN_SLAVE_1_INDEX] 				= CELL_IN_SLAVE_1;
	data[CELL_IN_SLAVE_2_INDEX] 				= CELL_IN_SLAVE_2;
	data[CELL_IN_SLAVE_3_INDEX] 				= CELL_IN_SLAVE_3;
	data[CELL_IN_SLAVE_4_INDEX] 				= CELL_IN_SLAVE_4;
	data[CELL_IN_SLAVE_5_INDEX] 				= CELL_IN_SLAVE_5;
	data[CELL_IN_SLAVE_6_INDEX] 				= CELL_IN_SLAVE_6;
	data[CELL_IN_SLAVE_7_INDEX] 				= CELL_IN_SLAVE_7;
	data[CELL_IN_SLAVE_8_INDEX] 				= CELL_IN_SLAVE_8;
	data[CELL_IN_SLAVE_9_INDEX] 				= CELL_IN_SLAVE_9;
	data[CELL_IN_SLAVE_10_INDEX] 				= CELL_IN_SLAVE_10;
	data[CELL_IN_SLAVE_11_INDEX] 				= CELL_IN_SLAVE_11;
	data[CELL_IN_SLAVE_12_INDEX] 				= CELL_IN_SLAVE_12;

	//ntc
	data[EXTERNAL_NTC_PER_SLAVE_INDEX]        	= EXTERNAL_NTC_PER_SLAVE;
	data[INTERNAL_NTC_PER_SLAVE_INDEX]        	= INTERNAL_NTC_PER_SLAVE;
	data[NTC_BETA_VALUE_AT_25C_INDEX]           = NTC_BETA_VALUE_AT_25C;

	//shunt
	data[SHUNT_RESISTOR_uOhm_INDEX]             = SHUNT_RESISTOR_uOhm;

	//cell voltage
	data[CELL_MAX_VOLTAGE_THRESHOLD_mV_INDEX]   = CELL_MAX_VOLTAGE_THRESHOLD_mV;
	data[CELL_MIN_VOLTAGE_THRESHOLD_mV_INDEX]   = CELL_MIN_VOLTAGE_THRESHOLD_mV;
	data[CELL_BALANCING_START_VOLTAGE_mV_INDEX] = CELL_BALANCING_START_VOLTAGE_mV;
	data[CELL_IMBALANCE_THRESHOLD_mV_INDEX]     = CELL_IMBALANCE_THRESHOLD_mV;

    //current
    data[MAX_CHARGE_CURRENT_THRESHOLD_A_INDEX]		= MAX_CHARGE_CURRENT_THRESHOLD_A;
    data[MAX_DISCHARGE_CURRENT_THRESHOLD_A_INDEX]	= MAX_DISCHARGE_CURRENT_THRESHOLD_A;

	//capacity
	data[PACK_MAX_CAPACITY_Ah_INDEX]      		= PACK_MAX_CAPACITY_Ah;
	data[PACK_MIN_CAPACITY_Ah_INDEX]      		= PACK_MIN_CAPACITY_Ah;

	//cell temp
	data[CELL_MAX_TEMP_THRESHOLD_C_INDEX]       = CELL_MAX_TEMP_THRESHOLD_C;
	data[CELL_MIN_TEMP_THRESHOLD_C_INDEX]       = CELL_MIN_TEMP_THRESHOLD_C;

	//board temp
	data[BOARD_MAX_TEMP_THRESHOLD_C_INDEX]      = BOARD_MAX_TEMP_THRESHOLD_C;
	data[BOARD_MIN_TEMP_THRESHOLD_C_INDEX]      = BOARD_MIN_TEMP_THRESHOLD_C;

	//timeout
	data[ERROR_TIMEOUT_ms_INDEX] 				= ERROR_TIMEOUT_ms;
	data[WARNING_TIMEOUT_ms_INDEX]     			= WARNING_TIMEOUT_ms;
	data[RECOVERY_TIMEOUT_ms_INDEX]      		= RECOVERY_TIMEOUT_ms;

    //protection threshold
	data[MAX_ALLOWED_TEMP_CHANGE_C_INDEX] 		= MAX_ALLOWED_TEMP_CHANGE_C;
	data[MAX_ALLOWED_TEMP_GRADIENT_C_INDEX] 	= MAX_ALLOWED_TEMP_GRADIENT_C;
	data[HIGH_IMBALANCE_ERROR_mV_INDEX] 		= HIGH_IMBALANCE_ERROR_mV;

	//balancing
	data[BALANCING_OFF_TIME_ms_INDEX] 			= BALANCING_OFF_TIME_ms;
	data[BALANCING_ON_TIME_ms_INDEX] 			= BALANCING_ON_TIME_ms;

	//contactor
	data[CONTACTOR_CUT_OFF_TIME_ms_INDEX] 		= CONTACTOR_CUT_OFF_TIME_ms;
	data[PRE_CHARGE_CURRENT_mA_INDEX] 			= PRE_CHARGE_CURRENT_mA;

	//cc
	data[VEHICLE_REST_TIME_m_INDEX] 			= VEHICLE_REST_TIME_m;
	data[VEHICLE_REST_CURRENT_A_INDEX] 			= VEHICLE_REST_CURRENT_A;

	//data pipeline
	data[CHARGE_CURRENT_DETECTION_THRESHOLD_A_INDEX] 	= CHARGE_CURRENT_DETECTION_THRESHOLD_A;

	//retry counts
	data[RETRY_PRIORITY_ZERO_INDEX]     		= RETRY_PRIORITY_ZERO;
	data[RETRY_PRIORITY_ONE_INDEX] 				= RETRY_PRIORITY_ONE;
	data[RETRY_PRIORITY_TWO_INDEX]				= RETRY_PRIORITY_TWO;
	data[RETRY_PRIORITY_THREE_INDEX] 			= RETRY_PRIORITY_THREE;
	data[RETRY_PRIORITY_FOUR_INDEX] 			= RETRY_PRIORITY_FOUR;
	data[RETRY_PRIORITY_FIVE_INDEX] 			= RETRY_PRIORITY_FIVE;

	//balancing derating
	data[BALANCING_DERATING_START_TEMP_C_INDEX] = BALANCING_DERATING_START_TEMP_C;
	data[BALANCING_DERATING_END_TEMP_C_INDEX]	= BALANCING_DERATING_END_TEMP_C;
	data[BALANCING_MAX_ON_TIME_ms_INDEX]		= BALANCING_MAX_ON_TIME_ms;
	data[BALANCING_MIN_ON_TIME_ms_INDEX]		= BALANCING_MIN_ON_TIME_ms;
	data[BALANCING_MAX_OFF_TIME_ms_INDEX]		= BALANCING_MAX_OFF_TIME_ms;
	data[BALANCING_MIN_OFF_TIME_ms_INDEX] 		= BALANCING_MIN_OFF_TIME_ms;

	//protection thresholds
	data[OCC_ERROR_CURRENT_A_INDEX]	 			= OCC_ERROR_CURRENT_A;
	data[OCC_WARNING_CURRENT_A_INDEX]			= OCC_WARNING_CURRENT_A;
	data[OCD_ERROR_CURRENT_A_INDEX]				= OCD_ERROR_CURRENT_A;
	data[OCD_WARNING_CURRENT_A_INDEX]			= OCD_WARNING_CURRENT_A;

	//temp group1
	data[OTC_ERROR_TEMPERATURE_GROUP_1_INDEX]   = OTC_ERROR_TEMPERATURE_GROUP_1;
	data[OTC_WARNING_TEMPERATURE_GROUP_1_INDEX] = OTC_WARNING_TEMPERATURE_GROUP_1;
	data[OTC_RECOVERY_TEMPERATURE_GROUP_1_INDEX]= OTC_RECOVERY_TEMPERATURE_GROUP_1;

	data[OTD_ERROR_TEMPERATURE_GROUP_1_INDEX]   = OTD_ERROR_TEMPERATURE_GROUP_1;
	data[OTD_WARNING_TEMPERATURE_GROUP_1_INDEX] = OTD_WARNING_TEMPERATURE_GROUP_1;
	data[OTD_RECOVERY_TEMPERATURE_GROUP_1_INDEX]= OTD_RECOVERY_TEMPERATURE_GROUP_1;

	data[UTC_ERROR_TEMPERATURE_GROUP_1_INDEX]   = UTC_ERROR_TEMPERATURE_GROUP_1;
	data[UTC_WARNING_TEMPERATURE_GROUP_1_INDEX] = UTC_WARNING_TEMPERATURE_GROUP_1;
	data[UTC_RECOVERY_TEMPERATURE_GROUP_1_INDEX]= UTC_RECOVERY_TEMPERATURE_GROUP_1;

	data[UTD_ERROR_TEMPERATURE_GROUP_1_INDEX]   = UTD_ERROR_TEMPERATURE_GROUP_1;
	data[UTD_WARNING_TEMPERATURE_GROUP_1_INDEX] = UTD_WARNING_TEMPERATURE_GROUP_1;
	data[UTD_RECOVERY_TEMPERATURE_GROUP_1_INDEX]= UTD_RECOVERY_TEMPERATURE_GROUP_1;

	//temp group2
	data[OTC_ERROR_TEMPERATURE_GROUP_2_INDEX]   = OTC_ERROR_TEMPERATURE_GROUP_2;
	data[OTC_WARNING_TEMPERATURE_GROUP_2_INDEX] = OTC_WARNING_TEMPERATURE_GROUP_2;
	data[OTC_RECOVERY_TEMPERATURE_GROUP_2_INDEX]= OTC_RECOVERY_TEMPERATURE_GROUP_2;

	data[OTD_ERROR_TEMPERATURE_GROUP_2_INDEX]   = OTD_ERROR_TEMPERATURE_GROUP_2;
	data[OTD_WARNING_TEMPERATURE_GROUP_2_INDEX] = OTD_WARNING_TEMPERATURE_GROUP_2;
	data[OTD_RECOVERY_TEMPERATURE_GROUP_2_INDEX]= OTD_RECOVERY_TEMPERATURE_GROUP_2;

	data[UTC_ERROR_TEMPERATURE_GROUP_2_INDEX]   = UTC_ERROR_TEMPERATURE_GROUP_2;
	data[UTC_WARNING_TEMPERATURE_GROUP_2_INDEX] = UTC_WARNING_TEMPERATURE_GROUP_2;
	data[UTC_RECOVERY_TEMPERATURE_GROUP_2_INDEX]= UTC_RECOVERY_TEMPERATURE_GROUP_2;

	data[UTD_ERROR_TEMPERATURE_GROUP_2_INDEX]   = UTD_ERROR_TEMPERATURE_GROUP_2;
	data[UTD_WARNING_TEMPERATURE_GROUP_2_INDEX] = UTD_WARNING_TEMPERATURE_GROUP_2;
	data[UTD_RECOVERY_TEMPERATURE_GROUP_2_INDEX]= UTD_RECOVERY_TEMPERATURE_GROUP_2;

	//ARC REACTOR
	data[AR_CARTRAGE_TEMP_INDEX]				= AR_CARTRAGE_TEMP;
	data[AR_COOLING_TEMP_INDEX]					= AR_COOLING_TEMP;
	data[AR_COOLING_TEMP_CHARGING_INDEX]		= AR_COOLING_TEMP_CHARGING;
	data[AR_ENABLE_INDEX]						= AR_ENABLE;
	data[AR_HEATING_TEMP_INDEX]					= AR_HEATING_TEMP;
	data[AR_HEATING_TEMP_CHARGING_INDEX] 		= AR_HEATING_TEMP_CHARGING;
	data[AR_COOLING_TYPE_INDEX]					= AR_COOLING_TYPE;

	//crc
	data[FLASH_CONFIG_CRC_INDEX]     			= calculateFlashCRC(data, FLASH_CONFIG_NUMBER_OF_PARAMETERS);

	status = BMS_OK;
	return status;
}


/* Function definition -------------------------------------------------------*/
/**
 * @brief Read data From Flash memory
 * @param sectionType section of the flash
 * @param flashAddress starting address of the Flash memory
 * @param flashData pointer to data to be read
 * @param dataSize size of data in Bytes
 * @retval BMS status
 */
bmsStatus_t readDataFromFlash(uint8_t sectionType, uint32_t flashAddress, __IO uint64_t *flashData, uint16_t dataSize)
{
	bmsStatus_t status 	= BMS_ERROR;

	if( !(IS_FLASH_SECTION_TYPE(sectionType)) 		||\
		!(IS_FLASH_PROGRAM_ADDRESS(flashAddress)) 	||\
		(NULL == flashData) 						||\
		(dataSize <= 0))
	{
		status = BMS_FLASH_FUNCTION_PARAMETER_ERROR;
		return status;
	}

	if(*(__IO uint64_t *)(flashAddress) == DATA_EMPTY)
	{
		switch(sectionType)
		{
			case FLASH_CONFIG_SECTION:
				//Data is not present
				//1. Initialize the flash config data with default values
				initializeFlashConfigData(flashData);

				//2. Write data to the flash
				status = flashWrite(flashAddress, flashData, dataSize);
				CHECK_STATUS(status);
				break;
		
			case FLASH_EXCEPTION_SECTION:
				status = BMS_FLASH_EXCEPTION_DATA_NOT_FOUND;
				CHECK_STATUS(status);
				break;

			default:
				status 	= BMS_ERROR;
				break;
		}
	}

	status = checkFlashData(sectionType, flashAddress, flashData, dataSize);
	return status;
}

/* Function definition -------------------------------------------------------*/
/**
 * @brief write data to flash
 * @param sectionType section of the flash
 * @param flashAddress starting address of the Flash memory
 * @param flashData pointer to data
 * @param dataSize size of data in Bytes
 * @param receivedData data to be updated
 * @retval BMS status
 */
bmsStatus_t writeDataToFlash(uint8_t sectionType, uint32_t flashAddress, __IO uint64_t *flashData, uint16_t dataSize, uint8_t *receivedData)
{
	bmsStatus_t status 	  	 = BMS_ERROR;
	uint64_t updatedData   = 0;
	uint16_t index 	       = 0;
	uint32_t crcCalculated = 0;


	if( !(IS_FLASH_SECTION_TYPE(sectionType)) 		||\
		!(IS_FLASH_PROGRAM_ADDRESS(flashAddress)) 	||\
		(NULL == flashData) 						||\
		(dataSize <= 0))
	{
		status = BMS_FLASH_FUNCTION_PARAMETER_ERROR;
		return status;
	}

	switch(sectionType)
	{
		case FLASH_CONFIG_SECTION:
			status = updateReceivedData(receivedData, &index, &updatedData); 
			CHECK_STATUS(status);

			status = checkReceivedDataIsSame(flashData, &index, &updatedData); 
			CHECK_STATUS(status);

			status = updateStoredData(flashData, &index, &updatedData); 
			CHECK_STATUS(status);
			break;

		case FLASH_COTA_UPDATE:
			//compare CRC
			crcCalculated = calculateFlashCRC(flashData, FLASH_CONFIG_TOTAL_LENGTH - 1);
			//or I can use the CRC sent by Stark and store it
			flashData[FLASH_CONFIG_CRC_INDEX] = crcCalculated;
			configWriteFlag = 1;
			break;
	
		case FLASH_EXCEPTION_SECTION:
			//nothing specific to execute in case of exception data
			break;

		default:
			status = BMS_ERROR;
			break;
	}

	if(configWriteFlag)
	{
		status = flashWrite(flashAddress, flashData, dataSize);
		CHECK_STATUS(status);

		status = checkFlashData(sectionType, flashAddress, flashData, dataSize);
	}
		return status;
}

/* Function definition -------------------------------------------------------*/
/**
 * @brief check that the data was written properly
 * @param sectionType section of the flash
 * @param flashAddress starting address of the Flash memory
 * @param flashData pointer to data
 * @param dataSize size of data in Bytes
 * @retval BMS status
 */
bmsStatus_t checkFlashData(uint8_t sectionType, uint32_t flashAddress, __IO uint64_t *flashData, uint16_t dataSize)
{
	bmsStatus_t status 			= BMS_ERROR;
	uint32_t 	calculatedCRC 	= 0;
	uint32_t 	storedCRC		= 0;

	if(!(IS_FLASH_PROGRAM_ADDRESS(flashAddress)) 	||\
		(NULL == flashData) 						||\
		(dataSize <= 0))
	{
		status = BMS_FLASH_FUNCTION_PARAMETER_ERROR;
		return status;
	}

	//1. read the data from flash
	status = flashRead(flashAddress, flashData, dataSize);
	CHECK_STATUS(status);

	//2. read the stored CRC value from flash
	storedCRC = flashData[dataSize - CRC_LENGTH]; 

	//3. calculate the CRC for the data read
	calculatedCRC = calculateFlashCRC(flashData, dataSize - CRC_LENGTH);

	if(calculatedCRC != storedCRC)
	{
		//1. Erase the page
		status = flashErase(flashAddress);
		CHECK_STATUS(status);
		
		switch(sectionType)
		{
			case FLASH_CONFIG_SECTION:
				//2. initialize with the default data
				initializeFlashConfigData(flashData);
				status = BMS_FLASH_CRC_NOT_EQUAL;
				break;
		
			case FLASH_EXCEPTION_SECTION:
				status = BMS_FLASH_CRC_NOT_EQUAL;
				break;

			default:
				status = BMS_ERROR;
				break;
		}
	}

	return status;
}

/* Function definition -------------------------------------------------------*/
/**
 * @brief update the rececive data 
 * @param receivedData received data 
 * @param index pointer to index
 * @param updatedData pointer to the updated data
 * @retval BMS status
 */
bmsStatus_t updateReceivedData(uint8_t *receivedData, uint16_t *index, uint64_t *updatedData)
{
	bmsStatus_t status 	= BMS_ERROR;

	if(	(NULL == receivedData) 	||\
		(NULL == index) 		||\
		(NULL == updatedData))
	{
		status = BMS_FLASH_FUNCTION_PARAMETER_ERROR;
		return status;
	}

	*index 	= (receivedData[1] << 8) | (receivedData[0] & 0xFF); //little endian
	if(!((*index >= 0) && ((*index <= (FLASH_CONFIG_TOTAL_LENGTH - FLASH_CONFIG_CRC_LENGTH)) || (*index == 0xFFFF))))
	{
		status = BMS_FLASH_CONFIG_DATA_OUT_OF_INDEX;
		return status;
	}

	if(*index == 0xFFFF)
	{
		status = BMS_OK;
		configWriteFlag = 1;
		return status;
	}

	for (int i = 0; i < 6; i++)
	{
		// 0th and 1st element are the index and data is stored in the rest 6 bytes
		*updatedData |= (receivedData[i + 2] << (8 * i));
	}

	if(!((*updatedData >= flashConfigDataRange[*index][FLASH_CONFIG_DATA_RANGE_MIN_INDEX]) &&\
		  (*updatedData <= flashConfigDataRange[*index][FLASH_CONFIG_DATA_RANGE_MAX_INDEX])))
	{
		status = BMS_FLASH_CONFIG_DATA_OUT_OF_RANGE;
		return status;
	}

	status = BMS_OK;
	return status;
}


/* Function definition -------------------------------------------------------*/
/**
 * @brief check if received data is same as stored data, if yes don't write again on falsh
 * @param flashData pointer to data
 * @param index pointer to index
 * @param updatedData pointer to the updated data
 * @retval BMS status
 */
bmsStatus_t checkReceivedDataIsSame(__IO uint64_t *flashData, uint16_t *index, uint64_t *updatedData)
{
	bmsStatus_t status 	= BMS_ERROR;
	__IO uint64_t storedData = 0;

	if(	(NULL == flashData) ||\
		(NULL == index) 	||\
		(NULL == updatedData))
	{
		status = BMS_FLASH_FUNCTION_PARAMETER_ERROR;
		return status;
	}

	storedData = flashData[*index];

	if(storedData == *updatedData)
	{
		status = BMS_FLASH_CONFIG_RECEIVED_DATA_IS_SAME;
		return status;
	}

	status = BMS_OK;
	return status;
}


/* Function definition -------------------------------------------------------*/
/**
 * @brief update the stored data
 * @param flashData pointer to data
 * @param index pointer to index
 * @param updatedData pointer to the updated data
 * @retval BMS status
 */
bmsStatus_t updateStoredData(__IO uint64_t *flashData, uint16_t *index, uint64_t *updatedData)
{
	bmsStatus_t status 	= BMS_ERROR;

	if(	(NULL == flashData) ||\
		(NULL == index) 	||\
		(NULL == updatedData))
	{
		status = BMS_FLASH_FUNCTION_PARAMETER_ERROR;
		return status;
	}

	flashData[*index] = *updatedData;
	flashData[FLASH_CONFIG_CRC_INDEX] = calculateFlashCRC(flashData, FLASH_CONFIG_NUMBER_OF_PARAMETERS);

	status = BMS_OK;
	return status;
}


/* Function definition -------------------------------------------------------*/
/**
 * @brief calculates CRC for the flash data
 * @param data pointer to data
 * @param size size of data in Bytes
 * @retval crc
 */
uint32_t calculateFlashCRC( __IO uint64_t *data, uint16_t size)
{
	if(	(NULL == data) ||\
		(size <= 0))
	{
		return BMS_FLASH_FUNCTION_PARAMETER_ERROR;
	}

	uint32_t poly = 0xEDB88320;
	uint32_t crc = 0xFFFFFFFF;

	for (uint16_t i = 0; i < size; i++)
	{
		crc ^= (uint32_t)data[i];

		for (int j = 0; j < 32; j++)
		{
			uint32_t mask = -(crc & 1);
			crc = (crc >> 1) ^ (poly & mask);
		}
	}

	return (~crc);
}

/* End of File ---------------------------------------------------------------*/