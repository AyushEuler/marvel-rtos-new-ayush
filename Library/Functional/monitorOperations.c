/**
 * @file monitorOperations.c
 * @author Arpit Gupta (arpit@euler)
 * @brief 
 * @version 0.1
 * @date 2022-02-25
 * 
 * @copyright Copyright (c) 2022
 * 
 */

/* Standard C includes -------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

/* Private includes ----------------------------------------------------------*/
#include "bmsCANMatrix.h"
#include "bmsErrorCodes.h"
#include "customDelays.h"
#include "initializeOperations.h"

#include "onBoardIOs.h"
#include "onBoardUARTs.h"
#include "onBoardCMU6815.h"
#include "onBoardCCM2949.h"
#include "onBoardCANv2b.h"
#include "canControls.h"
#include "onBoardMemory.h"
#include "conversion.h"
/* Self include --------------------------------------------------------------*/
#include "monitorOperations.h"
#include "dataProcessing.h"
#include "threadxMain.h"
/* Type definiton ------------------------------------------------------------*/

#define TIMEBASE_FREQUENCY_1MHZ 	0x07
#define TIMEBASE_FREQUENCY_4MHZ 	0xF2
#define TIMEBASE_FREQUENCY 			TIMEBASE_FREQUENCY_4MHZ
/* External variables definiton ----------------------------------------------*/

/* Private constants ---------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
uint8_t switchRequest = 0;
float estimatedSoH = 100.0;				//HARDCODED

extern uint8_t fullChargeLatch;
extern uint16_t adbms6815CRCErrorCount;
extern uint16_t ltc2949CRCErrorCount;
extern cmuVoltageData_t cmuVoltages;
extern uint8_t isBalancingCommandZero;
extern uint16_t cellsToBalance[2];
extern uint8_t balanceActiveFlag;
extern thresholdData_t thresholdData; 
extern contactorOutput_t cOutputData;
/* Private function prototypes -----------------------------------------------*/
static uint32_t checkOverVoltage(stb_ statusB, uint8_t cmuIndex);
static uint32_t checkUnderVoltage(stb_ statusB, uint8_t cmuIndex);
/* Private custom code -------------------------------------------------------*/

/* Function definition -------------------------------------------------------*/
/**
 * @brief handles Error State of BMS
 * 
 * @param bmsError 
 * @retval BMS status
 */
bmsStatus_t displayErrorState(uint8_t bmsError)
{
	bmsStatus_t status = BMS_ERROR;

	status = BMS_OK;
	return status;
}


/* Function definition -------------------------------------------------------*/
/**
 * @brief read cell voltages
 * @param 
 * @retval BMS status
 */
bmsStatus_t readCellVoltages(bmsMainData_t *mdata)
{
	bmsStatus_t status = BMS_OK;
	if((NULL == mdata))
	{
		status = BMS_CMU_FUNCTION_PARAMETER_ERROR;
		return status;
	}

	memset((void*)&mdata->cmuData.cmuVolatges,0,sizeof(cmuVoltageData_t));

	status = adBms6815_write_read_config(mdata->cmuData.cmuCount, mdata->cmuData.cmuCellData);
	CHECK_STATUS(status);
    status = adBms6815_start_adc_cell_voltage_conversion(mdata->cmuData.cmuCount);
	CHECK_STATUS(status);
    status = adBms6815_read_cell_voltages(mdata->cmuData.cmuCount, mdata->cmuData.cmuCellData);
	CHECK_STATUS(status);
	status = checkPackVoltages(mdata);
	//return status;
	return BMS_ERROR;
}

/* Function definition -------------------------------------------------------*/
/**
 * @brief read cell temperatures
 * @param 
 * @retval BMS status
 */
bmsStatus_t readCellTemperatures(bmsMainData_t *mdata)
{
	bmsStatus_t status = BMS_OK;

	if((NULL == mdata))
	{
		status = BMS_CMU_FUNCTION_PARAMETER_ERROR;
		return status;
	}
	
	memset((void*)&mdata->cmuData.cmuTemperatures,0,sizeof(cmuTemperatureData_t));
	status = adBms6815_start_aux_voltage_conversion(mdata->cmuData.cmuCount, mdata->cmuData.cmuCellData);
	CHECK_STATUS(status);
	status = adBms6815_read_aux_voltages(mdata->cmuData.cmuCount, mdata->cmuData.cmuCellData);
	CHECK_STATUS(status);
	status = checkPackTemperatures(mdata);

	// return status;
	return BMS_ERROR;
}

/* Function definition -------------------------------------------------------*/
/**
 * @brief read cell status
 * @param 
 * @retval BMS status
 */
bmsStatus_t readPackStatus(bmsMainData_t *mdata)
{
	bmsStatus_t status = BMS_OK;

	if((NULL == mdata))
	{
		status = BMS_CMU_FUNCTION_PARAMETER_ERROR;
		return status;
	}

	mdata->cmuData.cmuVolatges.packSumOfCells = 0;

	status = adBms6815_read_status(mdata->cmuData.cmuCount, mdata->cmuData.cmuCellData);
	CHECK_STATUS(status);

	//Pack voltages
	for(uint8_t cmuIndex = 0; cmuIndex < NUMBER_OF_CMU; cmuIndex++)
	{
		mdata->cmuData.cmuVolatges.sumOfCells[cmuIndex] = mdata->cmuData.cmuCellData[cmuIndex].stata.sc * multiplierVolt * 20;
		mdata->cmuData.cmuVolatges.packSumOfCells += mdata->cmuData.cmuVolatges.sumOfCells[cmuIndex];
	}

	//CMU internal Temperature
	for(uint8_t cmuIndex = 0; cmuIndex < NUMBER_OF_CMU; cmuIndex++)
	{
		mdata->cmuData.cmuTemperatures.cmuInternalTemp[cmuIndex] = (mdata->cmuData.cmuCellData[cmuIndex].stata.itmp * multiplierVolt/0.0075) - 273.15;
	}

	uint32_t faultsData[2] = {0};
	//CMU Status B values/ under voltage and over voltage
	for(uint8_t cmuIndex = 0; cmuIndex < NUMBER_OF_CMU; cmuIndex++)
	{

		faultsData[cmuIndex] = checkOverVoltage(mdata->cmuData.cmuCellData[cmuIndex].statb, cmuIndex);
		if(faultsData[cmuIndex])
		{
			mdata->cmuData.cmuFaults.overVoltage[cmuIndex] = 1;
		}

		faultsData[cmuIndex] = checkUnderVoltage(mdata->cmuData.cmuCellData[cmuIndex].statb, cmuIndex);
		if(faultsData[cmuIndex])
		{
			mdata->cmuData.cmuFaults.underVoltage[cmuIndex] = 1;
		}
	}

	// return status;
	return BMS_ERROR;
}

/* Function definition -------------------------------------------------------*/
/**
 * @brief check pack voltages
 * @param 
 * @retval BMS status
 */
bmsStatus_t checkPackVoltages(bmsMainData_t *mdata)
{
	bmsStatus_t status = BMS_ERROR;

	if((NULL == mdata))
	{
		status = BMS_CMU_FUNCTION_PARAMETER_ERROR;
		return status;
	}

	for(uint8_t cmuIndex = 0; cmuIndex < NUMBER_OF_CMU; cmuIndex++)
	{
		for(uint8_t cellIndex = 0; cellIndex < MAX_CELL_PER_SLAVE; cellIndex++)
		{
			mdata->cmuData.cmuVolatges.cellVoltages[cmuIndex][cellIndex] = CONVERT_CMU_VOLTAGE_TO_MILLI_VOLT(mdata->cmuData.cmuCellData[cmuIndex].cell.c_codes[cellIndex]);
		}
	}

	status = BMS_OK;
	return status;
}

/* Function definition -------------------------------------------------------*/
/**
 * @brief check pack temperature
 * @param 
 * @retval BMS status
 */
bmsStatus_t checkPackTemperatures(bmsMainData_t *mdata)
{
	bmsStatus_t status = BMS_ERROR;

	if((NULL == mdata))
	{
		status = BMS_CMU_FUNCTION_PARAMETER_ERROR;
		return status;
	}

	for(uint8_t cmuIndex = 0; cmuIndex < NUMBER_OF_CMU; cmuIndex++)
	{
		mdata->cmuData.cmuVolatges.refGPIOVoltage[cmuIndex] = mdata->cmuData.cmuCellData[cmuIndex].aux.a_codes[GPIOS_VREF2_VREF3] * multiplierVolt;
		mdata->cmuData.cmuVolatges.refPowerVoltage[cmuIndex] = mdata->cmuData.cmuCellData[cmuIndex].aux.a_codes[VREF2_VREF3] * multiplierVolt;
		
		for(uint8_t auxIndex = GPIO1; auxIndex <= GPIO4; auxIndex++)
		{
			float rawAux = mdata->cmuData.cmuCellData[cmuIndex].aux.a_codes[auxIndex] * multiplierVolt;
			float resistorRatio = rawAux/(mdata->cmuData.cmuVolatges.refGPIOVoltage[cmuIndex] - (rawAux));
			mdata->cmuData.cmuTemperatures.externalNTC[cmuIndex][auxIndex - GPIO1] = 1.0/(((log(resistorRatio)) * 0.000251) + 0.003354) - 273.15;
			resistorRatio = 0.0;
		}

		for(uint8_t auxIndex = GPIO5; auxIndex <= GPIO7; auxIndex++)
		{
			float rawAux = mdata->cmuData.cmuCellData[cmuIndex].aux.a_codes[auxIndex] * multiplierVolt;
			float resistorRatio = rawAux/(mdata->cmuData.cmuVolatges.refGPIOVoltage[cmuIndex] - (rawAux));
			mdata->cmuData.cmuTemperatures.onBoardNTC[cmuIndex][auxIndex - GPIO5] = 1.0/(((log(resistorRatio)) * 0.000251) + 0.003354) - 273.15;
			resistorRatio = 0.0;
		}
	}

	status = BMS_OK;
	return status;
}


static uint32_t checkOverVoltage(stb_ statusB, uint8_t cmuIndex)
{
	uint8_t overVoltage 	= 0;

	overVoltage = statusB.c10ov + statusB.c9ov + statusB.c8ov + statusB.c7ov + statusB.c6ov + 
				  statusB.c5ov + statusB.c4ov + statusB.c3ov + statusB.c2ov + statusB.c1ov;			
	
	if(CELL_IN_SERIES == 42)
	{
		if( (cmuIndex == 0) || (cmuIndex == 1) )
		{
			overVoltage += statusB.c11ov;
		}
	}
	else if(CELL_IN_SERIES == 45)
	{
		if( cmuIndex == 0 )
		{
			overVoltage = statusB.c12ov + statusB.c11ov;
		}
		else
		{
			overVoltage += statusB.c11ov;
		}
	}

	return (overVoltage);
}

static uint32_t checkUnderVoltage(stb_ statusB, uint8_t cmuIndex)
{
	uint8_t underVoltage 	= 0;

	underVoltage = statusB.c10uv + statusB.c9uv + statusB.c8uv + statusB.c7uv + statusB.c6uv + 
				  statusB.c5uv + statusB.c4uv + statusB.c3uv + statusB.c2uv + statusB.c1uv;				
	
	if(CELL_IN_SERIES == 42)
	{
		if( (cmuIndex == 0) || (cmuIndex == 1) )
		{
			underVoltage += statusB.c11uv;
		}
	}
	else if(CELL_IN_SERIES == 45)
	{
		if( cmuIndex == 0 )
		{
			underVoltage = statusB.c12uv + statusB.c11uv;
		}
		else
		{
			underVoltage += statusB.c11uv;
		}
	}

	return (underVoltage);
}


/* Function definition -------------------------------------------------------*/
/**
 * @brief read status and fault registers of LTC2949
 * @param 
 * @retval BMS status
 */
bmsStatus_t readStatusFaultReg(bmsMainData_t *mdata)
{
	bmsStatus_t status = BMS_ERROR;
	uint8_t tempStatusReg = 0;
    uint8_t tempOpctrlReg = 0;
    uint8_t tempFactrlReg = 0;
    uint8_t tempAdcconfReg = 0;
    uint8_t tempExtfaultsReg = 0;
    uint8_t tempFaultsReg = 0;
    uint8_t tempTbReg = 0;
    uint8_t tempOcc1Reg = 0;
    uint8_t tempOcc2Reg = 0;

	if((NULL == mdata))
	{
		status = BMS_CCM_FUNCTION_PARAMETER_ERROR;
		return status;
	}

	mdata->ccmData.statusReg 		= tempStatusReg;
	mdata->ccmData.opctrlReg 		= tempOpctrlReg;
	mdata->ccmData.factrlReg 		= tempFactrlReg;
	mdata->ccmData.adcconfReg 		= tempAdcconfReg;
	mdata->ccmData.extfaultsReg 	= tempExtfaultsReg;
	mdata->ccmData.faultsReg 		= tempFaultsReg;

	mdata->ccmData.tbReg 		= tempTbReg;
	mdata->ccmData.occ1Reg 		= tempOcc1Reg;
	mdata->ccmData.occ2Reg 		= tempOcc2Reg;

	LTC2949_READ(LTC2949_REG_STATUS, 1, (uint8_t*)&tempStatusReg);
	LTC2949_READ(LTC2949_REG_OPCTRL, 1, (uint8_t*)&tempOpctrlReg);
	LTC2949_READ(LTC2949_REG_FACTRL, 1, (uint8_t*)&tempFactrlReg);
	LTC2949_READ(LTC2949_REG_ADCCONF, 1, (uint8_t*)&tempAdcconfReg);
	LTC2949_READ(LTC2949_REG_EXTFAULTS, 1, (uint8_t*)&tempExtfaultsReg);
	LTC2949_READ(LTC2949_REG_FAULTS, 1, (uint8_t*)&tempFaultsReg);
	LTC2949_READ(LTC2949_REG_TBCTRL, 1, (uint8_t*)&tempTbReg);
	LTC2949_READ(LTC2949_REG_OCC1CTRL, 1, (uint8_t*)&tempOcc1Reg);
	LTC2949_READ(LTC2949_REG_OCC2CTRL, 1, (uint8_t*)&tempOcc2Reg);

	mdata->ccmData.statusReg 		= tempStatusReg;
	mdata->ccmData.opctrlReg 		= tempOpctrlReg;
	mdata->ccmData.factrlReg 		= tempFactrlReg;
	mdata->ccmData.adcconfReg 		= tempAdcconfReg;
	mdata->ccmData.extfaultsReg 	= tempExtfaultsReg;
	mdata->ccmData.faultsReg 		= tempFaultsReg;

	mdata->ccmData.tbReg 		= tempTbReg;
	mdata->ccmData.occ1Reg 		= tempOcc1Reg;
	mdata->ccmData.occ2Reg 		= tempOcc2Reg;

	status = BMS_OK;
	// return status;
	return BMS_ERROR;
}

/* Function definition -------------------------------------------------------*/
/**
 * @brief read pack voltage and current from LTC2949
 * @param 
 * @retval BMS status
 */
bmsStatus_t readPackVoltageCurrent(bmsMainData_t *mdata)
{
	bmsStatus_t status = BMS_ERROR;

	uint8_t buffI1[3] 		= {'\0'};
	uint8_t buffI2[3] 		= {'\0'};
	uint8_t buffIAVG1[3] 	= {'\0'};
	uint8_t buffP1[3] 		= {'\0'};
	uint8_t buffBAT[2] 		= {'\0'};
	uint8_t buffInTemp[2] 	= {'\0'};

	uint8_t timebaseFrequency 				= TIMEBASE_FREQUENCY;

	if((NULL == mdata))
	{
		status = BMS_CCM_FUNCTION_PARAMETER_ERROR;
		return status;
	}

	mdata->ccmData.packCurrent 				= 0.0;
	mdata->ccmData.packFilteredCurrent	 	= 0.0;
	mdata->ccmData.packVoltage 				= 0.0;
	mdata->ccmData.packPower 				= 0.0;
	mdata->ccmData.ccmInternalTemp 			= 0.0;
	mdata->ccmData.packFastCurrent 			= 0.0;

	status = LTC2949_WRITE(LTC2949_REG_TBCTRL, 1, &timebaseFrequency);
	// CHECK_STATUS(status);

	//reading 100ms current
	status = LTC2949_READ(LTC2949_VAL_I1, 3, (uint8_t*)&buffI1);
	// CHECK_STATUS(status);	
	mdata->ccmData.packCurrent = LTC_3BytesToInt32(buffI1) * LTC2949_LSB_I1;			    	//V
	mdata->ccmData.packCurrent = mdata->ccmData.packCurrent / CONVERT_MICRO_OHM_TO_OHM(SHUNT_RESISTOR_uOhm);	//mA = V/mOhom
	// mdata->ccmData.packCurrent = mdata->ccmData.packCurrent / CONVERT_MICRO_OHM_TO_MILLI_OHM(SHUNT_RESISTOR_uOhm);

	//reading 400ms current
	status = LTC2949_READ(LTC2949_VAL_IAVG1, 3, (uint8_t*)&buffIAVG1);
	// CHECK_STATUS(status);	
	mdata->ccmData.packFilteredCurrent = LTC_3BytesToInt32(buffIAVG1) * LTC2949_LSB_IAVG1;		//A
	mdata->ccmData.packFilteredCurrent = mdata->ccmData.packFilteredCurrent / CONVERT_MICRO_OHM_TO_OHM(SHUNT_RESISTOR_uOhm);


	status = LTC2949_READ(LTC2949_VAL_BAT, 2, (uint8_t*)&buffBAT);
	// CHECK_STATUS(status);
	mdata->ccmData.packVoltage = LTC_2BytesToInt16(buffBAT) * LTC2949_LSB_BAT;			//V
	mdata->ccmData.packVoltage = 101 * mdata->ccmData.packVoltage; 									//MARVEL-1.0 or 3.0	-> R1 = 10K, R2 = 1000K  -> 101*Vo = Vin ////MARVEL-II R1 = 10K, R2 = 560K -> 57*Vo = Vin
	
	status = LTC2949_READ(LTC2949_VAL_P1, 3, (uint8_t*)&buffP1);
	// CHECK_STATUS(status);
	mdata->ccmData.packPower = LTC_3BytesToInt32(buffP1) * LTC2949_LSB_P1;				//W
	mdata->ccmData.packPower = mdata->ccmData.packPower / CONVERT_MICRO_OHM_TO_OHM(SHUNT_RESISTOR_uOhm);								//P = V*V/R
	
	status = LTC2949_READ(LTC2949_VAL_TEMP, 2, (uint8_t*)&buffInTemp);
	// CHECK_STATUS(status);
	mdata->ccmData.ccmInternalTemp = LTC_2BytesToInt16(buffInTemp) * LTC2949_LSB_TEMP;	//C

	//reading 1ms current
	status = LTC2949_READ(LTC2949_VAL_I2, 3, (uint8_t*)&buffI2);	
	mdata->ccmData.packFastCurrent = LTC_3BytesToInt32(buffI2) * LTC2949_LSB_I2;				//A
	mdata->ccmData.packFastCurrent = mdata->ccmData.packFastCurrent / CONVERT_MICRO_OHM_TO_OHM(SHUNT_RESISTOR_uOhm);

	// return status;
	return BMS_ERROR;
}

/* Function definition -------------------------------------------------------*/
/**
 * @brief read charge energy data from LTC2949
 * @param 
 * @retval BMS status
 */
bmsStatus_t readChargeEnergyData(bmsMainData_t *mdata)
{
	bmsStatus_t status = BMS_ERROR;
	uint8_t buffData[16] = {'\0'};
	// uint8_t buffTB1[4] = {'\0'};
	// uint8_t buffC1[6] = {'\0'};
	// uint8_t buffE1[6] = {'\0'};

	uint8_t timebaseFrequency 		= TIMEBASE_FREQUENCY;
	uint8_t accumulationData  		= 0x00;

	if((NULL == mdata))
	{
		status = BMS_CCM_FUNCTION_PARAMETER_ERROR;
		return status;
	}

	mdata->ccmData.packCapacity 	= 0.0;
	mdata->ccmData.packEnergy 		= 0.0;
	mdata->ccmData.timeBase 		= 0.0;

	status = LTC2949_WRITE(LTC2949_REG_TBCTRL, 1, &timebaseFrequency);
	// CHECK_STATUS(status);
	
	status = LTC2949_WRITE(LTC2949_REG_ACCCTRL1, 1, &accumulationData);
	CHECK_STATUS(status);

	//Reading Channel 1 in Single Shot
	status = LTC2949_READ(LTC2949_VAL_C1, 16, buffData);
	CHECK_STATUS(status);
	mdata->ccmData.packCapacity 	= LTC2949_BytesToDouble(buffData + 0 + 0, 6, 1, LTC2949_LSB_C1) / CONVERT_MICRO_OHM_TO_OHM(SHUNT_RESISTOR_uOhm);
	mdata->ccmData.packEnergy 		= LTC2949_BytesToDouble(buffData + 0 + 6, 6, 1, LTC2949_LSB_E1) / CONVERT_MICRO_OHM_TO_OHM(SHUNT_RESISTOR_uOhm);
	mdata->ccmData.timeBase 		= LTC2949_BytesToDouble(buffData + 6 + 6, 4, 1, LTC2949_LSB_TB1);
	HAL_Delay(10);
	status = LTC2949_reset();
	CHECK_STATUS(status);

	if(mdata->ccmData.timeBase == 0.0)
	{
		mdata->ccmData.timeBaseResetCounter++;
		mdata->ccmData.isCCMReset = 1;
	}

	// return status;
	return BMS_ERROR;
}

/* Function definition -------------------------------------------------------*/
/**
 * @brief set over current configuration for LTC2949
 * @param 
 * @retval BMS status
 */
bmsStatus_t setOverCurrentConfiguration(int8_t threshold, uint8_t deglitchTime, uint8_t polarity)
{
	bmsStatus_t status = BMS_ERROR;
	status = LTC2949_OCCConfig(threshold, deglitchTime, polarity, threshold, deglitchTime, polarity);
	return status;
}


/* Function definition -------------------------------------------------------*/
/**
 * @brief send data on CAN
 * @param 
 * @retval BMS status
 */
bmsStatus_t sendDataOnCAN(bmsMainData_t *mdata, protectionOutput_t *pOutput, coulombCountingOutput_t *ccOutput)
{
	bmsStatus_t status = BMS_ERROR;
	uint8_t canDataFrame[8] = {'\0'};

	if((NULL == mdata))
	{
		status = BMS_CAN_FUNCTION_PARAMETER_ERROR;
		return status;
	}

	//BigEndian Format
	// for(uint8_t canDLC = 0; canDLC < CAN_packVoltage.messageDLC; canDLC++)
	// 	canDataFrame[CAN_packVoltage.messageStartByte + canDLC] = ((int) (packVoltage/CAN_packVoltage.messageScaling)  >> (8 * (CAN_packVoltage.messageDLC - canDLC - 1)));
	
	//LittleEndian Format
	//	for(int8_t canDLC = CAN_packCurrent.messageDLC - 1; canDLC >= 0; canDLC--)
	//		canDataFrame[CAN_packCurrent.messageStartByte + canDLC] = ((int) (mdata->ccmData.packCurrent/CAN_packCurrent.messageScaling)  >> (8 * canDLC));
	for(int8_t canDLC = CAN_balancingLimit.messageDLC - 1; canDLC >=0; canDLC --)
		canDataFrame[CAN_balancingLimit.messageStartByte + canDLC] = ((int) (thresholdData.balancingMinThreshold/CAN_balancingLimit.messageScaling) >> (8 * canDLC));

	for(int8_t canDLC = CAN_flagsPreChargeActive.messageDLC - 1; canDLC >= 0; canDLC--)
		canDataFrame[CAN_flagsPreChargeActive.messageStartByte + canDLC] = ((int) (cOutputData.preChargeContactorState/CAN_flagsPreChargeActive.messageScaling) >> (8 * canDLC));

	for(int8_t canDLC = CAN_flagsPreChargeActive.messageID - 1; canDLC >= 0; canDLC--)
		canDataFrame[CAN_flagsBalancingActive.messageStartByte + canDLC] = ((int) (balanceActiveFlag/CAN_flagsBalancingActive.messageScaling) >> (8 * canDLC));

	for(int8_t canDLC = CAN_packCurrent.messageDLC - 1; canDLC >= 0; canDLC--)
		canDataFrame[CAN_packCurrent.messageStartByte + canDLC] = ((int) (mdata->ccmData.packCurrent/CAN_packCurrent.messageScaling)  >> (8 * canDLC));
		
    status = sendCANDataOnQueue(CAN_packCurrent.messageID, canDataFrame, sizeof(canDataFrame));
    CHECK_STATUS(status);
    delayMilliSeconds(CAN_MESSAGE_DELAY);

	for(int8_t canDLC = CAN_stateOfCharge.messageDLC - 1; canDLC >= 0; canDLC--)
		canDataFrame[CAN_stateOfCharge.messageStartByte + canDLC] = ((int) (mdata->socData.estimatedSoC/CAN_stateOfCharge.messageScaling)  >> (8 * canDLC));
	
	for(int8_t canDLC = CAN_stateOfHealth.messageDLC - 1; canDLC >= 0; canDLC--)
		canDataFrame[CAN_stateOfHealth.messageStartByte + canDLC] = ((int) (estimatedSoH/CAN_stateOfHealth.messageScaling)  >> (8 * canDLC));
	
	for(int8_t canDLC = CAN_bmsState.messageDLC - 1; canDLC >= 0; canDLC--)
		canDataFrame[CAN_bmsState.messageStartByte + canDLC] = ((int) (switchRequest/CAN_bmsState.messageScaling)  >> (8 * canDLC));

	for(int8_t canDLC = CAN_flagsFullCharge.messageDLC - 1; canDLC >= 0; canDLC--)
		canDataFrame[CAN_flagsFullCharge.messageStartByte + canDLC] = ((int) (fullChargeLatch/CAN_flagsFullCharge.messageScaling)  >> (8 * canDLC));
	
	for(int8_t canDLC = CAN_packVoltage.messageDLC - 1; canDLC >= 0; canDLC--)
		canDataFrame[CAN_packVoltage.messageStartByte + canDLC] = ((int) (mdata->cmuData.cmuVolatges.packSumOfCells/CAN_packVoltage.messageScaling)  >> (8 * canDLC)); //((int) (mdata->ccmData.packVoltage/CAN_packVoltage.messageScaling)  >> (8 * canDLC));

    status = sendCANDataOnQueue(CAN_packVoltage.messageID, canDataFrame, sizeof(canDataFrame));
    CHECK_STATUS(status);
    delayMilliSeconds(CAN_MESSAGE_DELAY);

		for(int8_t canDLC = CAN_cmu1Temp1.messageDLC - 1; canDLC >= 0; canDLC--)
			canDataFrame[CAN_cmu1Temp1.messageStartByte + canDLC] = ((int) (floor(mdata->cmuData.cmuTemperatures.externalNTC[0][0])/CAN_cmu1Temp1.messageScaling)  >> (8 * canDLC));
		
		for(int8_t canDLC = CAN_cmu1Temp2.messageDLC - 1; canDLC >= 0; canDLC--)
			canDataFrame[CAN_cmu1Temp2.messageStartByte + canDLC] = ((int) (floor(mdata->cmuData.cmuTemperatures.externalNTC[0][1])/CAN_cmu1Temp2.messageScaling)  >> (8 * canDLC));
		
		for(int8_t canDLC = CAN_cmu1Temp3.messageDLC - 1; canDLC >= 0; canDLC--)
			canDataFrame[CAN_cmu1Temp3.messageStartByte + canDLC] = ((int) (floor(mdata->cmuData.cmuTemperatures.externalNTC[0][2])/CAN_cmu1Temp3.messageScaling)  >> (8 * canDLC));
		
		for(int8_t canDLC = CAN_cmu1Temp4.messageDLC - 1; canDLC >= 0; canDLC--)
			canDataFrame[CAN_cmu1Temp4.messageStartByte + canDLC] = ((int) (floor(mdata->cmuData.cmuTemperatures.externalNTC[0][3])/CAN_cmu1Temp4.messageScaling)  >> (8 * canDLC));
		
		for(int8_t canDLC = CAN_cmu2Temp1.messageDLC - 1; canDLC >= 0; canDLC--)
			canDataFrame[CAN_cmu2Temp1.messageStartByte + canDLC] = ((int) (floor(mdata->cmuData.cmuTemperatures.externalNTC[1][0])/CAN_cmu2Temp1.messageScaling)  >> (8 * canDLC));
		
		for(int8_t canDLC = CAN_cmu2Temp2.messageDLC - 1; canDLC >= 0; canDLC--)
			canDataFrame[CAN_cmu2Temp2.messageStartByte + canDLC] = ((int) (floor(mdata->cmuData.cmuTemperatures.externalNTC[1][1])/CAN_cmu2Temp2.messageScaling)  >> (8 * canDLC));
		
		for(int8_t canDLC = CAN_packCapacity.messageDLC - 1; canDLC >= 0; canDLC--)
			canDataFrame[CAN_packCapacity.messageStartByte + canDLC] = ((int) ((ccOutput->totalCapacityRemaining/100)/CAN_packCapacity.messageScaling)  >> (8 * canDLC));
		
    status = sendCANDataOnQueue(CAN_packCapacity.messageID, canDataFrame, sizeof(canDataFrame));
    CHECK_STATUS(status);
    delayMilliSeconds(CAN_MESSAGE_DELAY);

		for(int8_t canDLC = CAN_cmu2Temp3.messageDLC - 1; canDLC >= 0; canDLC--)
			canDataFrame[CAN_cmu2Temp3.messageStartByte + canDLC] = ((int) (floor(mdata->cmuData.cmuTemperatures.externalNTC[1][2])/CAN_cmu2Temp3.messageScaling)  >> (8 * canDLC));
		
		for(int8_t canDLC = CAN_cmu2Temp4.messageDLC - 1; canDLC >= 0; canDLC--)
			canDataFrame[CAN_cmu2Temp4.messageStartByte + canDLC] = ((int) (floor(mdata->cmuData.cmuTemperatures.externalNTC[1][3])/CAN_cmu2Temp4.messageScaling)  >> (8 * canDLC));
		
#ifdef STORM_ONE

		for(int8_t canDLC = CAN_cmu3Temp1.messageDLC - 1; canDLC >= 0; canDLC--)
			canDataFrame[CAN_cmu3Temp1.messageStartByte + canDLC] = ((int) (floor(mdata->cmuData.cmuTemperatures.externalNTC[2][0])/CAN_cmu3Temp1.messageScaling)  >> (8 * canDLC));

		for(int8_t canDLC = CAN_cmu3Temp2.messageDLC - 1; canDLC >= 0; canDLC--)
			canDataFrame[CAN_cmu3Temp2.messageStartByte + canDLC] = ((int) (floor(mdata->cmuData.cmuTemperatures.externalNTC[2][1])/CAN_cmu3Temp2.messageScaling)  >> (8 * canDLC));

		for(int8_t canDLC = CAN_cmu3Temp3.messageDLC - 1; canDLC >= 0; canDLC--)
			canDataFrame[CAN_cmu3Temp3.messageStartByte + canDLC] = ((int) (floor(mdata->cmuData.cmuTemperatures.externalNTC[2][2])/CAN_cmu3Temp3.messageScaling)  >> (8 * canDLC));

		for(int8_t canDLC = CAN_cmu3Temp4.messageDLC - 1; canDLC >= 0; canDLC--)
			canDataFrame[CAN_cmu3Temp4.messageStartByte + canDLC] = ((int) (floor(mdata->cmuData.cmuTemperatures.externalNTC[2][3])/CAN_cmu3Temp4.messageScaling)  >> (8 * canDLC));

		for(int8_t canDLC = CAN_cmu4Temp1.messageDLC - 1; canDLC >= 0; canDLC--)
			canDataFrame[CAN_cmu4Temp1.messageStartByte + canDLC] = ((int) (floor(mdata->cmuData.cmuTemperatures.externalNTC[3][1])/CAN_cmu4Temp1.messageScaling)  >> (8 * canDLC));

		for(int8_t canDLC = CAN_cmu4Temp2.messageDLC - 1; canDLC >= 0; canDLC--)
			canDataFrame[CAN_cmu4Temp2.messageStartByte + canDLC] = ((int) (floor(mdata->cmuData.cmuTemperatures.externalNTC[3][2])/CAN_cmu4Temp2.messageScaling)  >> (8 * canDLC));

		status = sendCANDataOnQueue(CAN_cmu2Temp4.messageID, canDataFrame, sizeof(canDataFrame));
		CHECK_STATUS(status);
		delayMilliSeconds(CAN_MESSAGE_DELAY);
#endif

		for(int8_t canDLC = CAN_cmu1Cell1.messageDLC - 1; canDLC >= 0; canDLC--)
			canDataFrame[CAN_cmu1Cell1.messageStartByte + canDLC] = ((int) (mdata->cmuData.cmuVolatges.cellVoltages[0][0]/CAN_cmu1Cell1.messageScaling)  >> (8 * canDLC));
		
		for(int8_t canDLC = CAN_cmu1Cell2.messageDLC - 1; canDLC >= 0; canDLC--)
			canDataFrame[CAN_cmu1Cell2.messageStartByte + canDLC] = ((int) (mdata->cmuData.cmuVolatges.cellVoltages[0][1]/CAN_cmu1Cell2.messageScaling)  >> (8 * canDLC));
		
		for(int8_t canDLC = CAN_cmu1Cell3.messageDLC - 1; canDLC >= 0; canDLC--)
			canDataFrame[CAN_cmu1Cell3.messageStartByte + canDLC] = ((int) (mdata->cmuData.cmuVolatges.cellVoltages[0][2]/CAN_cmu1Cell3.messageScaling)  >> (8 * canDLC));
		
		for(int8_t canDLC = CAN_cmu1Cell4.messageDLC - 1; canDLC >= 0; canDLC--)
			canDataFrame[CAN_cmu1Cell4.messageStartByte + canDLC] = ((int) (mdata->cmuData.cmuVolatges.cellVoltages[0][3]/CAN_cmu1Cell4.messageScaling)  >> (8 * canDLC));
		
	status = sendCANDataOnQueue(CAN_cmu1Cell4.messageID, canDataFrame, sizeof(canDataFrame));
	CHECK_STATUS(status);
	delayMilliSeconds(CAN_MESSAGE_DELAY);

		for(int8_t canDLC = CAN_cmu1Cell5.messageDLC - 1; canDLC >= 0; canDLC--)
			canDataFrame[CAN_cmu1Cell5.messageStartByte + canDLC] = ((int) (mdata->cmuData.cmuVolatges.cellVoltages[0][4]/CAN_cmu1Cell5.messageScaling)  >> (8 * canDLC));
		
		for(int8_t canDLC = CAN_cmu1Cell6.messageDLC - 1; canDLC >= 0; canDLC--)
			canDataFrame[CAN_cmu1Cell6.messageStartByte + canDLC] = ((int) (mdata->cmuData.cmuVolatges.cellVoltages[0][5]/CAN_cmu1Cell6.messageScaling)  >> (8 * canDLC));
		
		for(int8_t canDLC = CAN_cmu1Cell7.messageDLC - 1; canDLC >= 0; canDLC--)
			canDataFrame[CAN_cmu1Cell7.messageStartByte + canDLC] = ((int) (mdata->cmuData.cmuVolatges.cellVoltages[0][6]/CAN_cmu1Cell7.messageScaling)  >> (8 * canDLC));
		
		for(int8_t canDLC = CAN_cmu1Cell8.messageDLC - 1; canDLC >= 0; canDLC--)
			canDataFrame[CAN_cmu1Cell8.messageStartByte + canDLC] = ((int) (mdata->cmuData.cmuVolatges.cellVoltages[0][7]/CAN_cmu1Cell8.messageScaling)  >> (8 * canDLC));
	
	status = sendCANDataOnQueue(CAN_cmu1Cell8.messageID, canDataFrame, sizeof(canDataFrame));
	CHECK_STATUS(status);
	delayMilliSeconds(CAN_MESSAGE_DELAY);

		for(int8_t canDLC = CAN_cmu1Cell9.messageDLC - 1; canDLC >= 0; canDLC--)
			canDataFrame[CAN_cmu1Cell9.messageStartByte + canDLC] = ((int) (mdata->cmuData.cmuVolatges.cellVoltages[0][8]/CAN_cmu1Cell9.messageScaling)  >> (8 * canDLC));
		
		for(int8_t canDLC = CAN_cmu1Cell10.messageDLC - 1; canDLC >= 0; canDLC--)
			canDataFrame[CAN_cmu1Cell10.messageStartByte + canDLC] = ((int) (mdata->cmuData.cmuVolatges.cellVoltages[0][9]/CAN_cmu1Cell10.messageScaling)  >> (8 * canDLC));
		
		for(int8_t canDLC = CAN_cmu1Cell11.messageDLC - 1; canDLC >= 0; canDLC--)
			canDataFrame[CAN_cmu1Cell11.messageStartByte + canDLC] = ((int) (mdata->cmuData.cmuVolatges.cellVoltages[0][10]/CAN_cmu1Cell11.messageScaling)  >> (8 * canDLC));
		
		for(int8_t canDLC = CAN_cmu1Cell12.messageDLC - 1; canDLC >= 0; canDLC--)
			canDataFrame[CAN_cmu1Cell12.messageStartByte + canDLC] = ((int) (mdata->cmuData.cmuVolatges.cellVoltages[0][11]/CAN_cmu1Cell12.messageScaling)  >> (8 * canDLC));
		
	status = sendCANDataOnQueue(CAN_cmu1Cell12.messageID, canDataFrame, sizeof(canDataFrame));
	CHECK_STATUS(status);
	delayMilliSeconds(CAN_MESSAGE_DELAY);

		for(int8_t canDLC = CAN_cmu2Cell1.messageDLC - 1; canDLC >= 0; canDLC--)
			canDataFrame[CAN_cmu2Cell1.messageStartByte + canDLC] = ((int) (mdata->cmuData.cmuVolatges.cellVoltages[1][0]/CAN_cmu2Cell1.messageScaling)  >> (8 * canDLC));
		
		for(int8_t canDLC = CAN_cmu2Cell2.messageDLC - 1; canDLC >= 0; canDLC--)
			canDataFrame[CAN_cmu2Cell2.messageStartByte + canDLC] = ((int) (mdata->cmuData.cmuVolatges.cellVoltages[1][1]/CAN_cmu2Cell2.messageScaling)  >> (8 * canDLC));
		
		for(int8_t canDLC = CAN_cmu2Cell3.messageDLC - 1; canDLC >= 0; canDLC--)
			canDataFrame[CAN_cmu2Cell3.messageStartByte + canDLC] = ((int) (mdata->cmuData.cmuVolatges.cellVoltages[1][2]/CAN_cmu2Cell3.messageScaling)  >> (8 * canDLC));
		
		for(int8_t canDLC = CAN_cmu2Cell4.messageDLC - 1; canDLC >= 0; canDLC--)
			canDataFrame[CAN_cmu2Cell4.messageStartByte + canDLC] = ((int) (mdata->cmuData.cmuVolatges.cellVoltages[1][3]/CAN_cmu2Cell4.messageScaling)  >> (8 * canDLC));
		
	status = sendCANDataOnQueue(CAN_cmu2Cell4.messageID, canDataFrame, sizeof(canDataFrame));
	CHECK_STATUS(status);
	delayMilliSeconds(CAN_MESSAGE_DELAY);

		for(int8_t canDLC = CAN_cmu2Cell5.messageDLC - 1; canDLC >= 0; canDLC--)
			canDataFrame[CAN_cmu2Cell5.messageStartByte + canDLC] = ((int) (mdata->cmuData.cmuVolatges.cellVoltages[1][4]/CAN_cmu2Cell5.messageScaling)  >> (8 * canDLC));
		
		for(int8_t canDLC = CAN_cmu2Cell6.messageDLC - 1; canDLC >= 0; canDLC--)
			canDataFrame[CAN_cmu2Cell6.messageStartByte + canDLC] = ((int) (mdata->cmuData.cmuVolatges.cellVoltages[1][5]/CAN_cmu2Cell6.messageScaling)  >> (8 * canDLC));
		
		for(int8_t canDLC = CAN_cmu2Cell7.messageDLC - 1; canDLC >= 0; canDLC--)
			canDataFrame[CAN_cmu2Cell7.messageStartByte + canDLC] = ((int) (mdata->cmuData.cmuVolatges.cellVoltages[1][6]/CAN_cmu2Cell7.messageScaling)  >> (8 * canDLC));
		
		for(int8_t canDLC = CAN_cmu2Cell8.messageDLC - 1; canDLC >= 0; canDLC--)
			canDataFrame[CAN_cmu2Cell8.messageStartByte + canDLC] = ((int) (mdata->cmuData.cmuVolatges.cellVoltages[1][7]/CAN_cmu2Cell8.messageScaling)  >> (8 * canDLC));
		
	status = sendCANDataOnQueue(CAN_cmu2Cell8.messageID, canDataFrame, sizeof(canDataFrame));
	CHECK_STATUS(status);
	delayMilliSeconds(CAN_MESSAGE_DELAY);	

		for(int8_t canDLC = CAN_cmu2Cell9.messageDLC - 1; canDLC >= 0; canDLC--)
			canDataFrame[CAN_cmu2Cell9.messageStartByte + canDLC] = ((int) (mdata->cmuData.cmuVolatges.cellVoltages[1][8]/CAN_cmu2Cell9.messageScaling)  >> (8 * canDLC));
		
		for(int8_t canDLC = CAN_cmu2Cell10.messageDLC - 1; canDLC >= 0; canDLC--)
			canDataFrame[CAN_cmu2Cell10.messageStartByte + canDLC] = ((int) (mdata->cmuData.cmuVolatges.cellVoltages[1][9]/CAN_cmu2Cell10.messageScaling)  >> (8 * canDLC));
		
		for(int8_t canDLC = CAN_cmu2Cell11.messageDLC - 1; canDLC >= 0; canDLC--)
			canDataFrame[CAN_cmu2Cell11.messageStartByte + canDLC] = ((int) (mdata->cmuData.cmuVolatges.cellVoltages[1][10]/CAN_cmu2Cell11.messageScaling)  >> (8 * canDLC));
		
		for(int8_t canDLC = CAN_cmu2Cell12.messageDLC - 1; canDLC >= 0; canDLC--)
			canDataFrame[CAN_cmu2Cell12.messageStartByte + canDLC] = ((int) (mdata->cmuData.cmuVolatges.cellVoltages[1][11]/CAN_cmu2Cell12.messageScaling)  >> (8 * canDLC));
		
	status = sendCANDataOnQueue(CAN_cmu2Cell12.messageID, canDataFrame, sizeof(canDataFrame));
	CHECK_STATUS(status);
	delayMilliSeconds(CAN_MESSAGE_DELAY);

	//Dynamic Currents Instantaneous

	for(int8_t canDLC = CAN_InstantaneousLimitIN.messageDLC -1; canDLC >= 0; canDLC--)        
		canDataFrame[CAN_InstantaneousLimitIN.messageStartByte + canDLC] = ((int) (mdata->instaneousLimitIn_A/CAN_InstantaneousLimitIN.messageScaling) >> (8 * canDLC));

	for(int8_t canDLC = CAN_InstantaneousLimitOUT.messageDLC -1; canDLC >= 0; canDLC--)        
		canDataFrame[CAN_InstantaneousLimitOUT.messageStartByte + canDLC] = ((int) (mdata->instaneousLimitOut_A/CAN_InstantaneousLimitOUT.messageScaling) >> (8 * canDLC));

	canDataFrame[5] = pOutput->protectionOutputData.i2t_Calculated_A2sec;

	for(uint8_t canDLC = 0; canDLC < CAN_packVoltage.messageDLC; canDLC++)
			canDataFrame[CAN_packVoltage.messageStartByte + canDLC] = ((int) (mdata->ccmData.packVoltage/CAN_packVoltage.messageScaling)  >> (8 * (CAN_packVoltage.messageDLC - canDLC - 1)));
		
	

	status = sendCANDataOnQueue(CAN_TX_ID_015, canDataFrame, sizeof(canDataFrame));
	CHECK_STATUS(status);
	delayMilliSeconds(CAN_MESSAGE_DELAY);


#ifdef STORM_ONE

		for(int8_t canDLC = CAN_cmu3Cell1.messageDLC - 1; canDLC >= 0; canDLC--)
			canDataFrame[CAN_cmu3Cell1.messageStartByte + canDLC] = ((int) (mdata->cmuData.cmuVolatges.cellVoltages[2][0]/CAN_cmu3Cell1.messageScaling)  >> (8 * canDLC));
		
		for(int8_t canDLC = CAN_cmu3Cell2.messageDLC - 1; canDLC >= 0; canDLC--)
			canDataFrame[CAN_cmu3Cell2.messageStartByte + canDLC] = ((int) (mdata->cmuData.cmuVolatges.cellVoltages[2][1]/CAN_cmu3Cell2.messageScaling)  >> (8 * canDLC));
		
		for(int8_t canDLC = CAN_cmu3Cell3.messageDLC - 1; canDLC >= 0; canDLC--)
			canDataFrame[CAN_cmu3Cell3.messageStartByte + canDLC] = ((int) (mdata->cmuData.cmuVolatges.cellVoltages[2][2]/CAN_cmu3Cell3.messageScaling)  >> (8 * canDLC));
		
		for(int8_t canDLC = CAN_cmu3Cell4.messageDLC - 1; canDLC >= 0; canDLC--)
			canDataFrame[CAN_cmu3Cell4.messageStartByte + canDLC] = ((int) (mdata->cmuData.cmuVolatges.cellVoltages[2][3]/CAN_cmu3Cell4.messageScaling)  >> (8 * canDLC));
		
	status = sendCANDataOnQueue(CAN_cmu3Cell4.messageID, canDataFrame, sizeof(canDataFrame));
	CHECK_STATUS(status);
	delayMilliSeconds(CAN_MESSAGE_DELAY);

		for(int8_t canDLC = CAN_cmu3Cell5.messageDLC - 1; canDLC >= 0; canDLC--)
			canDataFrame[CAN_cmu3Cell5.messageStartByte + canDLC] = ((int) (mdata->cmuData.cmuVolatges.cellVoltages[2][4]/CAN_cmu3Cell5.messageScaling)  >> (8 * canDLC));
		
		for(int8_t canDLC = CAN_cmu3Cell6.messageDLC - 1; canDLC >= 0; canDLC--)
			canDataFrame[CAN_cmu3Cell6.messageStartByte + canDLC] = ((int) (mdata->cmuData.cmuVolatges.cellVoltages[2][5]/CAN_cmu3Cell6.messageScaling)  >> (8 * canDLC));
		
		for(int8_t canDLC = CAN_cmu3Cell7.messageDLC - 1; canDLC >= 0; canDLC--)
			canDataFrame[CAN_cmu3Cell7.messageStartByte + canDLC] = ((int) (mdata->cmuData.cmuVolatges.cellVoltages[2][6]/CAN_cmu3Cell7.messageScaling)  >> (8 * canDLC));
		
		for(int8_t canDLC = CAN_cmu3Cell8.messageDLC - 1; canDLC >= 0; canDLC--)
			canDataFrame[CAN_cmu3Cell8.messageStartByte + canDLC] = ((int) (mdata->cmuData.cmuVolatges.cellVoltages[2][7]/CAN_cmu3Cell8.messageScaling)  >> (8 * canDLC));
	
	status = sendCANDataOnQueue(CAN_cmu3Cell8.messageID, canDataFrame, sizeof(canDataFrame));
	CHECK_STATUS(status);
	delayMilliSeconds(CAN_MESSAGE_DELAY);

		for(int8_t canDLC = CAN_cmu3Cell9.messageDLC - 1; canDLC >= 0; canDLC--)
			canDataFrame[CAN_cmu3Cell9.messageStartByte + canDLC] = ((int) (mdata->cmuData.cmuVolatges.cellVoltages[2][8]/CAN_cmu3Cell9.messageScaling)  >> (8 * canDLC));
		
		for(int8_t canDLC = CAN_cmu3Cell10.messageDLC - 1; canDLC >= 0; canDLC--)
			canDataFrame[CAN_cmu3Cell10.messageStartByte + canDLC] = ((int) (mdata->cmuData.cmuVolatges.cellVoltages[2][9]/CAN_cmu3Cell10.messageScaling)  >> (8 * canDLC));
		
		for(int8_t canDLC = CAN_cmu3Cell11.messageDLC - 1; canDLC >= 0; canDLC--)
			canDataFrame[CAN_cmu3Cell11.messageStartByte + canDLC] = ((int) (mdata->cmuData.cmuVolatges.cellVoltages[2][10]/CAN_cmu3Cell11.messageScaling)  >> (8 * canDLC));
		
		for(int8_t canDLC = CAN_cmu3Cell12.messageDLC - 1; canDLC >= 0; canDLC--)
			canDataFrame[CAN_cmu3Cell12.messageStartByte + canDLC] = ((int) (mdata->cmuData.cmuVolatges.cellVoltages[2][11]/CAN_cmu3Cell12.messageScaling)  >> (8 * canDLC));
		
	status = sendCANDataOnQueue(CAN_cmu3Cell12.messageID, canDataFrame, sizeof(canDataFrame));
	CHECK_STATUS(status);
	delayMilliSeconds(CAN_MESSAGE_DELAY);

		for(int8_t canDLC = CAN_cmu4Cell1.messageDLC - 1; canDLC >= 0; canDLC--)
			canDataFrame[CAN_cmu4Cell1.messageStartByte + canDLC] = ((int) (mdata->cmuData.cmuVolatges.cellVoltages[3][0]/CAN_cmu4Cell1.messageScaling)  >> (8 * canDLC));
		
		for(int8_t canDLC = CAN_cmu4Cell2.messageDLC - 1; canDLC >= 0; canDLC--)
			canDataFrame[CAN_cmu4Cell2.messageStartByte + canDLC] = ((int) (mdata->cmuData.cmuVolatges.cellVoltages[3][1]/CAN_cmu4Cell2.messageScaling)  >> (8 * canDLC));
		
		for(int8_t canDLC = CAN_cmu4Cell3.messageDLC - 1; canDLC >= 0; canDLC--)
			canDataFrame[CAN_cmu4Cell3.messageStartByte + canDLC] = ((int) (mdata->cmuData.cmuVolatges.cellVoltages[3][2]/CAN_cmu4Cell3.messageScaling)  >> (8 * canDLC));
		
		for(int8_t canDLC = CAN_cmu4Cell4.messageDLC - 1; canDLC >= 0; canDLC--)
			canDataFrame[CAN_cmu4Cell4.messageStartByte + canDLC] = ((int) (mdata->cmuData.cmuVolatges.cellVoltages[3][3]/CAN_cmu4Cell4.messageScaling)  >> (8 * canDLC));
		
	status = sendCANDataOnQueue(CAN_cmu4Cell4.messageID, canDataFrame, sizeof(canDataFrame));
	CHECK_STATUS(status);
	delayMilliSeconds(CAN_MESSAGE_DELAY);

		for(int8_t canDLC = CAN_cmu4Cell5.messageDLC - 1; canDLC >= 0; canDLC--)
			canDataFrame[CAN_cmu4Cell5.messageStartByte + canDLC] = ((int) (mdata->cmuData.cmuVolatges.cellVoltages[3][4]/CAN_cmu4Cell5.messageScaling)  >> (8 * canDLC));
		
		for(int8_t canDLC = CAN_cmu4Cell6.messageDLC - 1; canDLC >= 0; canDLC--)
			canDataFrame[CAN_cmu4Cell6.messageStartByte + canDLC] = ((int) (mdata->cmuData.cmuVolatges.cellVoltages[3][5]/CAN_cmu4Cell6.messageScaling)  >> (8 * canDLC));
		
		for(int8_t canDLC = CAN_cmu4Cell7.messageDLC - 1; canDLC >= 0; canDLC--)
			canDataFrame[CAN_cmu4Cell7.messageStartByte + canDLC] = ((int) (mdata->cmuData.cmuVolatges.cellVoltages[3][6]/CAN_cmu4Cell7.messageScaling)  >> (8 * canDLC));
		
		for(int8_t canDLC = CAN_cmu4Cell8.messageDLC - 1; canDLC >= 0; canDLC--)
			canDataFrame[CAN_cmu4Cell8.messageStartByte + canDLC] = ((int) (mdata->cmuData.cmuVolatges.cellVoltages[3][7]/CAN_cmu4Cell8.messageScaling)  >> (8 * canDLC));
		
	status = sendCANDataOnQueue(CAN_cmu4Cell8.messageID, canDataFrame, sizeof(canDataFrame));
	CHECK_STATUS(status);
	delayMilliSeconds(CAN_MESSAGE_DELAY);

		for(int8_t canDLC = CAN_cmu4Cell9.messageDLC - 1; canDLC >= 0; canDLC--)
			canDataFrame[CAN_cmu4Cell9.messageStartByte + canDLC] = ((int) (mdata->cmuData.cmuVolatges.cellVoltages[3][8]/CAN_cmu4Cell9.messageScaling)  >> (8 * canDLC));
		
		for(int8_t canDLC = CAN_cmu4Cell10.messageDLC - 1; canDLC >= 0; canDLC--)
			canDataFrame[CAN_cmu4Cell10.messageStartByte + canDLC] = ((int) (mdata->cmuData.cmuVolatges.cellVoltages[3][9]/CAN_cmu4Cell10.messageScaling)  >> (8 * canDLC));
		
		for(int8_t canDLC = CAN_cmu4Cell11.messageDLC - 1; canDLC >= 0; canDLC--)
			canDataFrame[CAN_cmu4Cell11.messageStartByte + canDLC] = ((int) (mdata->cmuData.cmuVolatges.cellVoltages[3][10]/CAN_cmu4Cell11.messageScaling)  >> (8 * canDLC));
		
		for(int8_t canDLC = CAN_cmu4Cell12.messageDLC - 1; canDLC >= 0; canDLC--)
			canDataFrame[CAN_cmu4Cell12.messageStartByte + canDLC] = ((int) (mdata->cmuData.cmuVolatges.cellVoltages[3][11]/CAN_cmu4Cell12.messageScaling)  >> (8 * canDLC));
		
	status = sendCANDataOnQueue(CAN_cmu4Cell12.messageID, canDataFrame, sizeof(canDataFrame));
	CHECK_STATUS(status);
	delayMilliSeconds(CAN_MESSAGE_DELAY);

#endif
	
	for(int8_t canDLC = CAN_packCapacityDesign.messageDLC - 1; canDLC >= 0; canDLC--)
		canDataFrame[CAN_packCapacityDesign.messageStartByte + canDLC] = ((int) (PACK_MAX_CAPACITY_Ah/CAN_packCapacityDesign.messageScaling)  >> (8 * canDLC));

	for(int8_t canDLC = CAN_packCapacityFull.messageDLC - 1; canDLC >= 0; canDLC--)
		canDataFrame[CAN_packCapacityFull.messageStartByte + canDLC] = ((int) (PACK_MAX_CAPACITY_Ah/CAN_packCapacityFull.messageScaling)  >> (8 * canDLC));
	
	for(int8_t canDLC = CAN_balancingBitMask_CMU1.messageDLC - 1; canDLC >= 0; canDLC--)
		canDataFrame[CAN_balancingBitMask_CMU1.messageStartByte + canDLC] = ((int) (cellsToBalance[0]/CAN_balancingBitMask_CMU1.messageScaling)  >> (8 * canDLC));

	for(int8_t canDLC = CAN_balancingBitMask_CMU2.messageDLC - 1; canDLC >= 0; canDLC--)
		canDataFrame[CAN_balancingBitMask_CMU2.messageStartByte + canDLC] = ((int) (cellsToBalance[1]/CAN_balancingBitMask_CMU2.messageScaling)  >> (8 * canDLC));

	status = sendCANDataOnQueue(CAN_packCapacityDesign.messageID, canDataFrame, sizeof(canDataFrame));
	CHECK_STATUS(status);
	delayMilliSeconds(CAN_MESSAGE_DELAY);

// #ifdef LTC2949
		for(int8_t canDLC = CAN_timeBaseResetCounter.messageDLC - 1; canDLC >= 0; canDLC--)
			canDataFrame[CAN_timeBaseResetCounter.messageStartByte + canDLC] = ((int) (mdata->ccmData.timeBaseResetCounter/CAN_timeBaseResetCounter.messageScaling)  >> (8 * canDLC));
	
		for(int8_t canDLC = CAN_mcuResetCounter.messageDLC - 1; canDLC >= 0; canDLC--)
			canDataFrame[CAN_mcuResetCounter.messageStartByte + canDLC] = ((int) (mdata->mcuResetCounter/CAN_mcuResetCounter.messageScaling)  >> (8 * canDLC));

		for(int8_t canDLC = CAN_2949CRCCounter.messageDLC - 1; canDLC >= 0; canDLC--)
			canDataFrame[CAN_2949CRCCounter.messageStartByte + canDLC] = ((int) (ltc2949CRCErrorCount/CAN_2949CRCCounter.messageScaling)  >> (8 * canDLC));

		for(int8_t canDLC = CAN_6815CRCCounter.messageDLC - 1; canDLC >= 0; canDLC--)
			canDataFrame[CAN_6815CRCCounter.messageStartByte + canDLC] = ((int) (adbms6815CRCErrorCount/CAN_6815CRCCounter.messageScaling)  >> (8 * canDLC));
	status = sendCANDataOnQueue(CAN_6815CRCCounter.messageID, canDataFrame, sizeof(canDataFrame));
	CHECK_STATUS(status);
	delayMilliSeconds(CAN_MESSAGE_DELAY);
// #endif

    //dynamic current data on CAN
        for(int8_t canDLC = CAN_dynamicLimitIN.messageDLC -1; canDLC >= 0; canDLC--)        
            canDataFrame[CAN_dynamicLimitIN.messageStartByte + canDLC] = ((int) (mdata->dynamicLimitIn_A/CAN_dynamicLimitIN.messageScaling) >> (8 * canDLC));

        for(int8_t canDLC = CAN_dynamicLimitOUT.messageDLC -1; canDLC >= 0; canDLC--)        
            canDataFrame[CAN_dynamicLimitOUT.messageStartByte + canDLC] = ((int) (mdata->dynamicLimitOut_A/CAN_dynamicLimitOUT.messageScaling) >> (8 * canDLC));

        canDataFrame[4] = MARVEL_FW_VER;
		canDataFrame[5] = MARVEL_HW_VER;
		canDataFrame[6] = MARVEL_CONFIG;

    status = sendCANDataOnQueue(CAN_TX_ID_011, canDataFrame, sizeof(canDataFrame));
    CHECK_STATUS(status);
    delayMilliSeconds(CAN_MESSAGE_DELAY); 

#ifdef CAN_MATRIX_V_0_3
		for(int8_t canDLC = CAN_maxAllowedCellVoltage.messageDLC - 1; canDLC >= 0; canDLC--)
			canDataFrame[CAN_maxAllowedCellVoltage.messageStartByte + canDLC] = ((int) (MAX_ALLOWED_CELL_VOLTAGE/CAN_maxAllowedCellVoltage.messageScaling)  >> (8 * canDLC));
		
		for(int8_t canDLC = CAN_maxAllowedPackVoltage.messageDLC - 1; canDLC >= 0; canDLC--)
			canDataFrame[CAN_maxAllowedPackVoltage.messageStartByte + canDLC] = ((int) (MAX_ALLOWED_PACK_VOLTAGE/CAN_maxAllowedPackVoltage.messageScaling)  >> (8 * canDLC));
		
		for(int8_t canDLC = CAN_maxAllowedCellTemp.messageDLC - 1; canDLC >= 0; canDLC--)
			canDataFrame[CAN_maxAllowedCellTemp.messageStartByte + canDLC] = ((int) (MAX_ALLOWED_CELL_TEMP/CAN_maxAllowedCellTemp.messageScaling)  >> (8 * canDLC));
		
		for(int8_t canDLC = CAN_maxAllowedCurrent.messageDLC - 1; canDLC >= 0; canDLC--)
			canDataFrame[CAN_maxAllowedCurrent.messageStartByte + canDLC] = ((int) (MAX_ALLOWED_CURRENT/CAN_maxAllowedCurrent.messageScaling)  >> (8 * canDLC));
		
	status = sendCANDataOnQueue(CAN_maxAllowedCurrent.messageID, canDataFrame, sizeof(canDataFrame));
	CHECK_STATUS(status);
	delayMilliSeconds(CAN_MESSAGE_DELAY);
#endif




//publishing MARVEL SW,HW, partNumber on CAN
// uint8_t dataBuffSW[4] = {0};
// uint8_t dataBuffHW[4] = {0};

// floatToByte(dataBuffSW, MARVEL_FW_VER);
// floatToByte(dataBuffHW, MARVEL_HW_VER);

// for(uint8_t i = 0; i < 8; i++)
// {
// 	if(i < 4)
// 	{
// 		canDataFrame[i] = dataBuffSW[i];
// 	}
// 	else
// 	{
// 		canDataFrame[i] = dataBuffHW[i - 4];
// 	}
// }

canDataFrame[0] = MARVEL_FW_VER;
canDataFrame[1] = MARVEL_HW_VER;

canDataFrame[2] = MARVEL_CONFIG;

status = sendCANDataOnQueue(CAN_TX_ID_024, canDataFrame, sizeof(canDataFrame));


		
















	return status;
}


/* End of File ---------------------------------------------------------------*/
