
/**
 * @file onBoardCCM2949.c
 * @author Arpit Gupta (arpit@euler)
 * @brief 
 * @version 0.1
 * @date 2022-04-09
 * 
 * @copyright Copyright (c) 2022
 * 
 */

/* Standard C includes -------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>

/* Private includes ----------------------------------------------------------*/
#include "customDelays.h"

#include "timerControls.h"
#include "gpioControls.h"
#include "spiControls.h"
#include "onBoardIOs.h"
#include "onBoardUARTs.h"

#include "onBoardUARTs.h"
#include "usart.h"
#include "main.h"
/* Self include --------------------------------------------------------------*/
#include "onBoardCCM2949.h"
/* Type definiton ------------------------------------------------------------*/
#define LTC2949_ISO_SPI_WAKEUP_TIMEOUT    4500
/* External variables definiton ----------------------------------------------*/

/* Private constants ---------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
// in indirect address mode we always write a burst of 4 bytes to RDCVIADDR which includes a write to REGSCTRL
uint8_t LTC2949_iAddrRegsCtrl[LTC2949_IADDRREGSCTRL_LEN] = {0};
uint8_t LTC2949_gpioCtrl[2] = {0};
uint8_t LTC2949_adcConf = 0; // PasV, NTC register
uint8_t LTC2949_CellMonitorCount = 0;
bool LTC2949_onTopOfDaisychain = false;
bool LTC2949_DebugEnable = false;
bool LTC2949_forceWrRegsCtrl = false;
bool LTC2949_autoForceWrRegsCtrl = false;
unsigned long LTC2949_isoSPI_wakeup_timeoutUs = 0;	// holds the next timestamp when isoSPI timeout duration has elapsed
// double LTC2949_TBFAC = 0.0;
// uint32_t LTC2949_lastTBx = 0; // stores the last TBx value

/* Private function prototypes -----------------------------------------------*/
static inline void LTC2949_ADCConfigAll(uint8_t adcCfg);

/* Private custom code -------------------------------------------------------*/
static inline void LTC2949_ADCConfigAll(uint8_t adcCfg)
{
  bitsMaskApplyToReg(LTC2949_adcConf, adcCfg, LTC2949_BM_ADCCONF_P1ASV | LTC2949_BM_ADCCONF_P2ASV | LTC2949_BM_ADCCONF_NTC1 | LTC2949_BM_ADCCONF_NTC2 | LTC2949_BM_ADCCONF_NTCSLOT1);
}

/* Function definition -------------------------------------------------------*/
/**
 * @brief 
 * 
 */
bmsStatus_t LTC2949Initialize(void)
{
  bmsStatus_t status = BMS_ERROR;

  setSPISlaveSelect(ccmSpi);
	delayMilliSeconds(100);
	LTC2949_init_lib(0, false, false);
	LTC2949_init_device_state();
	//uint8_t error = WakeUpReportStatus();
	//error |= Cont(false); // Cont mode will be enabled later
	//delayMilliSeconds(LTC2949_TIMING_CONT_CYCLE);
	//LTC2949_reset();
  
  status = BMS_OK;
  return status;
}

/* Function definition -------------------------------------------------------*/
/**
 * @brief 
 * 
 */
// use with 
// LTC2949_BM_OCC_DACTHR_xxx for occ_Thr
// LTC2949_BM_OCC_DGLT_xxx   for occ_Dgl
// LTC2949_BM_OCC_POL_xxxx   for occ_Pol
// set occ_Thr to -1 to disable OCC


bmsStatus_t LTC2949_OCCConfig(int8_t occ1Thr, uint8_t occ1Dgl, uint8_t occ1Pol, 
                              int8_t occ2Thr, uint8_t occ2Dgl, uint8_t occ2Pol)
{
  bmsStatus_t status = BMS_ERROR;
	uint8_t data[2] = { 0, 0 };

	if (occ1Thr >= 0)
  {
    data[0] = (occ1Pol << 6) | (occ1Dgl << 4) | (occ1Thr << 1) | LTC2949_BM_OCC1CTRL_OCC1EN;
  }
		
	if (occ2Thr >= 0)
  {
    data[1] = (occ1Pol << 6) | (occ1Dgl << 4) | (occ1Thr << 1) | LTC2949_BM_OCC2CTRL_OCC2EN;
  }

	status = CheckDeviceReady();

  LTC2949_WRITE(LTC2949_REG_OCC1CTRL, 1, &data[0]);
	LTC2949_WRITE(LTC2949_REG_OCC2CTRL, 1, &data[1]);

  return status;
}

bmsStatus_t LTC2949_init_lib(uint8_t cellMonitorCount, bool ltc2949onTopOfDaisychain, bool debugEnable)
{
  bmsStatus_t status = BMS_ERROR;
	LTC2949_CellMonitorCount = cellMonitorCount;
	LTC2949_DebugEnable = debugEnable;
	LTC2949_onTopOfDaisychain = ltc2949onTopOfDaisychain;
	// make sure the default value adjusted to e.g. BCREN is written with the next transaction
	LTC2949_forceWrRegsCtrl = true;
	LTC2949_autoForceWrRegsCtrl = true;
	LTC2949_iAddrRegsCtrl[LTC2949_REGSCTRL_IX] = LTC2949_BM_REGSCTRL_RDCVCONF;
  status = BMS_OK;
  return status;
}

bmsStatus_t LTC2949_GpioCurrConfigClr(void)
{
  bmsStatus_t status = BMS_ERROR;
  LTC2949_gpioCtrl[0] = 0;
  LTC2949_gpioCtrl[1] = 0;
  status = BMS_OK;
  return status;
}

bmsStatus_t LTC2949_init_device_state(void)
{
  bmsStatus_t status = BMS_ERROR;
  LTC2949_forceWrRegsCtrl = true;
  LTC2949_autoForceWrRegsCtrl = true;
  LTC2949_isoSPI_wakeup_timeoutUs = 0;
  //LTC2949_TBFAC = LTC2949_INTC;
  //LTC2949_ADCConfigUpdate(0);
  LTC2949_adcConf = 0;
  LTC2949_GpioCurrConfigClr();
  // this is the default of REGSCTRL
  LTC2949_iAddrRegsCtrl[LTC2949_REGSCTRL_IX] = LTC2949_BM_REGSCTRL_RDCVCONF;
  //LTC2949_lastTBx = 0; // stores the last TBx value
  status = BMS_OK;
  return status;
}


bmsStatus_t LTC2949_isoSPI_wakeup_timeoutUs_init(uint16_t timeout)
{
  bmsStatus_t status = BMS_ERROR;
  //LTC2949_isoSPI_wakeup_timeoutUs = micros() + (6000);
	LTC2949_isoSPI_wakeup_timeoutUs = micros() + timeout;
  status = BMS_OK;
  return status;
}

bmsStatus_t pec15_calc_init(uint16_t * pec15)
{
  bmsStatus_t status = BMS_ERROR;
  (*pec15) = 16U; // initialize with PEC15 calculator seed value
  status = BMS_OK;
  return status;
}

bmsStatus_t pec15_calc_finalize(uint16_t * pec15)
{
  bmsStatus_t status = BMS_ERROR;
  (*pec15) = (*pec15) << 1U;//The CRC15 has a 0 in the LSB so the pec15 must be multiplied by 2
  status = BMS_OK;
  return status;
}

bmsStatus_t pec15_calc(uint16_t * pec15, uint8_t nextByte)
{
  bmsStatus_t status = BMS_ERROR;
  uint8_t addr = (uint8_t)(((*pec15) >> 7) ^ nextByte);
  (*pec15) = ((*pec15) << 8) ^ _pec15_calc_internal_(addr);
  status = BMS_OK;
  return status;
}

int16_t _pec15_calc_internal_(uint16_t remainder)
{
  remainder <<= 7;
  for (uint8_t i = 0; i < 8; i++) //BIT8 to BIT0
  {
    remainder <<= 1;
    if (remainder & 0x8000U)
      remainder ^= PEC15_POLY;
  }
  return remainder;
}

bmsStatus_t spi_write_calc_pec(uint16_t * pec15, uint8_t data)
{
  bmsStatus_t status = BMS_ERROR;
  pec15_calc(pec15, data);
  status = spi_write(data);
  return status;
}

bmsStatus_t spi_write_pec(uint16_t * pec15)
{
  bmsStatus_t status = BMS_ERROR;
  pec15_calc_finalize(pec15);
  status = spi_write(*pec15 >> 8); // MSB 1st
  //if(status != BMS_OK) return status;
  status = spi_write(*pec15);
  return status;
}

uint8_t spi_read_calc_pec(uint16_t * pec15, uint8_t *datarx)
{
  bmsStatus_t status = BMS_ERROR;
  status = spi_read(datarx);
  pec15_calc(pec15, *datarx);
  return status;
}

uint16_t ltc2949CRCErrorCount = 0;

bmsStatus_t spi_read_chk_pec(uint16_t * pec15, uint8_t * errCnt)
{
  bmsStatus_t status = BMS_ERROR;
  bool ok = false;
  uint16_t pec_slave;
  pec15_calc_finalize(pec15);
  status = spi_read_pec(&pec_slave);
  if(pec_slave == *pec15) ok = true;
  if (!ok && ((errCnt[0] & LTC2949_ERRCODE_PECERR_MASK) < LTC2949_PEC_ERR_CNT_MAX))
  {
    errCnt[0]++;
    ltc2949CRCErrorCount++;
#ifdef DEBUG
    //pec error sending on uart
    HAL_UART_Transmit(&hlpuart1, (uint8_t *)" PEC15 ERROR ", 13, 100);
#endif
  }

  return status;
}

bmsStatus_t spi_read_pec(uint16_t *pec_slave)
{
  bmsStatus_t status = BMS_ERROR;
  uint8_t rxData;
  status = spi_read(&rxData);         //MSB 1st
  //if(status != BMS_OK) return status;
  *pec_slave = rxData;
  status = spi_read(&rxData);        //LSB 2nd
  *pec_slave = *pec_slave << 8 | rxData; 
  return status;
}

bmsStatus_t spi_write(uint8_t datatx)
{
  bmsStatus_t status = BMS_ERROR;
	status = spiWriteBytes(1, &datatx);
  return status;
}

bmsStatus_t spi_read(uint8_t *datarx)
{
  bmsStatus_t status = BMS_ERROR;
  uint8_t dataDumTx = 0xFF;
  *datarx = 0x00;
  status = spiReadWriteByte(&dataDumTx, datarx);
  return status;
}

uint8_t LTC2949_CALC_OPT_NO_PECS(uint16_t len)
{
  // calc optimum number of bytes per PEC
  if (len <= 16)
    return len;
  else
  {
    uint8_t bytesPerPec = 16;
    for (; bytesPerPec > 1; bytesPerPec--)
    {
      if ((len % bytesPerPec) == 0)
        break;
    }
    return bytesPerPec;
  }
}

uint8_t LTC2949_CALC_ID_BYTE(uint8_t bytesPerPec, bool readNotWrite)
{
	uint8_t cmd_id = bytesPerPec - 1;
  // 1st shift cmd_id[3..2] to position ID[4..3] and leave cmd_id[1..0] at ID[1..0]
  cmd_id = ((cmd_id & 0xc) << 1) | (cmd_id & 0x3);
  // calc PEC1 ^ PEC0, place to ID2 and PEC3 ^ PEC2, place to ID5
  cmd_id = (((cmd_id << 1) ^ (cmd_id << 2)) & 0x24) | cmd_id;
  // put all together
  cmd_id |= (readNotWrite ? LTC2949_DC_READ_BIT : LTC2949_DC_WRITE_BIT);
  return cmd_id;
}

bmsStatus_t LTC2949_wakeup_idle(bool force)
{
  bmsStatus_t status = BMS_ERROR;
  if (!(force) && !(LTC_TIMEOUT_CHECK(micros(), LTC2949_isoSPI_wakeup_timeoutUs)))
    return BMS_OK; // isoSPI is still awake, we don't have to do anything

  if (LTC2949_CellMonitorCount)
  {
    for (uint8_t i = 0; i <= LTC2949_CellMonitorCount; i++)
    {
      resetSPISlaveSelect(ccmSpi);
      delayMilliSeconds(1);
      setSPISlaveSelect(ccmSpi);
      delayMilliSeconds(1);
    }
  }
  else
  {
    resetSPISlaveSelect(ccmSpi);
    delayMilliSeconds(1);
    setSPISlaveSelect(ccmSpi);
    delayMilliSeconds(1);
  }
  LTC2949_isoSPI_wakeup_timeoutUs_init(LTC2949_ISO_SPI_WAKEUP_TIMEOUT);
  status = BMS_OK;
  return status;
}

bmsStatus_t LTC2949_ProcessErrors(uint8_t error)
{
  bmsStatus_t status = BMS_ERROR;
  if (error)
  {
    LTC2949_forceWrRegsCtrl = LTC2949_autoForceWrRegsCtrl;
    status = BMS_CCM_PEC15_ERROR;
  }
  else
  {
    status = BMS_OK;
  }
  return status;
}

bmsStatus_t _LTC2949_DcRW_(uint8_t addr, uint16_t len, uint8_t * data, bool readNotWrite)
{
  bmsStatus_t status = BMS_ERROR;
  // calc optimum number of bytes per PEC
  uint8_t bytesPerPec = LTC2949_CALC_OPT_NO_PECS(len);
  uint16_t pec;
  // init the pec calculator
  pec15_calc_init(&pec);

  //This will guarantee that the LTC2949 isoSPI port is awake.
  LTC2949_wakeup_idle(LTC2949_forceWrRegsCtrl); // just to be on the safe side, in case we force write REGSCTRL we also force isoSPI wakeup pulses

  resetSPISlaveSelect(ccmSpi); //spiStart();

  // direct read/write commands are always addressed!!!
  status = spi_write_calc_pec(&pec, LTC2949_ADDR_CMD);
  //if(status != BMS_OK) return status;
  status = spi_write_calc_pec(&pec, addr);
  //if(status != BMS_OK) return status;
  status = spi_write_pec(&pec);
  //if(status != BMS_OK) return status;
  // write cmd_id (there is no PEC for the cmd_id byte as it has intrinsic checksum)
  status = spi_write(LTC2949_CALC_ID_BYTE(bytesPerPec, readNotWrite));
  //if(status != BMS_OK) return status;

  uint8_t error = 0;
  pec15_calc_init(&pec);

  if (readNotWrite)
  {
    for (uint16_t i = 0; i < len;)
    {
      status = spi_read_calc_pec(&pec, &data[i]);
      //if(status != BMS_OK) return status;
      error |= LTC2949_ERRCODE_COMMERR; // this is cleared after successful PEC check later
      i++;
      if (i % bytesPerPec != 0)
        continue;
      // read the PEC bytes and check for PEC error
      status = spi_read_chk_pec(&pec, &error);
      //if(status != BMS_OK) return status;

      pec15_calc_init(&pec);

      error &= ~LTC2949_ERRCODE_COMMERR; // PEC check done, so we clear it
      // Note: this ensures we always return with error LTC2949_ERRCODE_COMMERR
      //       in case we read data bytes without protection by PEC.
      // In other words: the two last bytes read must always be a PEC bytes.
    }
  }
  else
  {
    // WRITE COMMAND
    for (uint16_t i = 0; i < len;)
    {
      status = spi_write_calc_pec(&pec, data[i++]);
      //if(status != BMS_OK) return status;

      if (i % bytesPerPec != 0)
        continue;

      // it's time to calc and send the PEC
      status = spi_write_pec(&pec);
      //if(status != BMS_OK) return status;

      pec15_calc_init(&pec);
    }
  }
  setSPISlaveSelect(ccmSpi);//spiStop();
  LTC2949_isoSPI_wakeup_timeoutUs_init(LTC2949_ISO_SPI_WAKEUP_TIMEOUT);

  //char _data = 0xFFU;
  //while(HAL_SPI_Transmit(&hspi1, (uint8_t *) &_data, 1, 100) != HAL_OK);

  status = LTC2949_ProcessErrors(error);
  return status;
}

// direct write without changing page!
bmsStatus_t _LTC2949_direct_write_(uint8_t addr, uint16_t len, uint8_t* data)
{
  bmsStatus_t status = BMS_ERROR;
  status = _LTC2949_DcRW_(addr, len, data, LTC2949_DcWrite);
  return status;
}

bmsStatus_t _LTC2949_ADJ_REGSCTRL_DC_(uint16_t ADDR, bool readNotWrite)
{
  bmsStatus_t status = BMS_ERROR;
  bool wrRegsCtrl = false;
  // check page
  if ((!LTC2949_IS_ADDR_SPECIAL_ROW(ADDR)) &&
    (bitMaskSetChk(LTC2949_iAddrRegsCtrl[LTC2949_REGSCTRL_IX], LTC2949_BM_REGSCTRL_PAGE) != bitMaskSetChk(ADDR, 0x100)))
  {
    bitMaskSetClr(LTC2949_iAddrRegsCtrl[LTC2949_REGSCTRL_IX], LTC2949_BM_REGSCTRL_PAGE, bitMaskSetChk(ADDR, 0x100));
    wrRegsCtrl = true;
  }
  // check broadcast read enable
  if (readNotWrite && LTC2949_onTopOfDaisychain && bitMaskClrChk(LTC2949_iAddrRegsCtrl[LTC2949_REGSCTRL_IX], LTC2949_BM_REGSCTRL_BCREN))
  {
    bitMaskSet(LTC2949_iAddrRegsCtrl[LTC2949_REGSCTRL_IX], LTC2949_BM_REGSCTRL_BCREN);
    wrRegsCtrl = true;
  }
  if (wrRegsCtrl || LTC2949_forceWrRegsCtrl)
  {
    status = _LTC2949_direct_write_(LTC2949_REG_REGSCTRL, 1, &(LTC2949_iAddrRegsCtrl[LTC2949_REGSCTRL_IX]));
    LTC2949_forceWrRegsCtrl = false;
  }
  return status;
}

int16_t LTC2949_GetRegsCtrlDataOffset(uint8_t addr, uint16_t len)
{
  if (len > 4)
    return -1;

  int16_t i = (int16_t)LTC2949_REG_REGSCTRL - (int16_t)addr;
  if ((i >= 0) && ((uint16_t)i < len))
    return i;
  return -1;
}

bmsStatus_t LTC2949_AdjustRegsCtrlBuffer(uint8_t addr, uint16_t len, uint8_t * data)
{
  bmsStatus_t status = BMS_ERROR;
  int16_t regsCtrlOffs = LTC2949_GetRegsCtrlDataOffset(addr, len);
  if (regsCtrlOffs >= 0)
    LTC2949_iAddrRegsCtrl[LTC2949_REGSCTRL_IX] = data[regsCtrlOffs];

  status = BMS_OK;
  return status;
}

bmsStatus_t LTC2949_READ(uint16_t ADDR, uint16_t LEN, uint8_t * DATAPOINTER)
{
  bmsStatus_t status = BMS_ERROR;
  _LTC2949_ADJ_REGSCTRL_DC_(ADDR, LTC2949_DcRead);
  status = _LTC2949_DcRW_(ADDR, LEN, DATAPOINTER, LTC2949_DcRead);
  //if(status != BMS_OK) return status;
  LTC2949_AdjustRegsCtrlBuffer(ADDR, LEN, DATAPOINTER);
  return status;
}

bmsStatus_t LTC2949_WRITE(uint16_t ADDR, uint16_t LEN, uint8_t * DATAPOINTER)
{
  bmsStatus_t status = BMS_ERROR;
  _LTC2949_ADJ_REGSCTRL_DC_(ADDR, LTC2949_DcWrite);
  status = _LTC2949_direct_write_(ADDR, LEN, DATAPOINTER);
  LTC2949_AdjustRegsCtrlBuffer(ADDR, LEN, DATAPOINTER);
  return status;
}

bmsStatus_t LTC2949_PollReg(uint16_t addr, uint8_t val, uint8_t mask, uint16_t timeout, bool stopOnPecErrors, uint8_t * finalRead)
{
  bmsStatus_t status = BMS_ERROR;
  uint8_t ret = 0;
  bool stop = false;

  if (finalRead == NULL)
    finalRead = &ret;

  unsigned long endTime = millis() + timeout;

  do
  {
	  status = LTC2949_READ(addr, 1, finalRead);
    if (status != BMS_OK)
    {
      if (stopOnPecErrors)
      {
        return status;
      }
    }
    else if ((finalRead[0] & mask) == val)
    {
      status = BMS_OK;
      return status;
    }

    if (stop)
    {
      return BMS_CCM_TIMEOUT; //LTC2949_ERRCODE_TIMEOUT; // timeout
    }

    if (!LTC2949_IS_ADDR_SPECIAL_ROW(addr))
    {
      LTC2949_forceWrRegsCtrl = true; // if polling fails and we are not reading from special row, we force write of REGSCTRL.
    }

    if (LTC_TIMEOUT_CHECK(millis(), endTime))
    {
      stop = true; // timeout, but we try one more time!
    }

  } while(true);

  return status;
}


bmsStatus_t LTC2949_OpctlIdle(void)
{
  bmsStatus_t status = BMS_ERROR;
	status = _LTC2949_DcRW_(LTC2949_REG_OPCTRL, 1, 0, LTC2949_DcWrite);
	//_LTC2949_direct_write_(LTC2949_REG_OPCTRL, 1, 0x00);
  return status;
}

bmsStatus_t LTC2949_OpctlCont(void)
{
  bmsStatus_t status = BMS_ERROR;
	uint8_t _OPCTRL = LTC2949_BM_OPCTRL_CONT;
	status = _LTC2949_DcRW_(LTC2949_REG_OPCTRL, 1,(uint8_t*) &_OPCTRL, LTC2949_DcWrite);
  return status;
}

bmsStatus_t LTC2949_ADCConfigWrite(void)
{
  bmsStatus_t status = BMS_ERROR;
  status = LTC2949_WRITE(LTC2949_REG_ADCCONF, 1, (uint8_t *)&LTC2949_adcConf); // LTC2949_OpctlAdjUpd() should be called by user afterwards
  return status;
}

bmsStatus_t LTC2949_ADCConfigRead(uint8_t * data)
{
  bmsStatus_t status = BMS_ERROR;
  status = LTC2949_READ(LTC2949_REG_ADCCONF, 1, data);
  //if(status != BMS_OK) return status;
  LTC2949_adcConf = *data;
  return status;
}

bmsStatus_t LTC2949_OpctlAdjUpd(bool clear)
{
  bmsStatus_t status = BMS_ERROR;
  // Adjust update must always be done in IDLE mode!
  // Thus if we were in continuous mode the CONT bit will be cleared
  uint8_t data = LTC2949_BM_OPCTRL_ADJUPD;
  if (clear)
  {
    data |= LTC2949_BM_OPCTRL_CLR;
    //LTC2949_lastTBx = 0;
  }
  // note: every time we write to special row, we can directly use _LTC2949_direct_write_ if we make sure to update LTC2949_iAddrRegsCtrl[LTC2949_REGSCTRL_IX] in case we write LTC2949_REG_REGSCTRL
  status = _LTC2949_direct_write_(LTC2949_REG_OPCTRL, 1, (uint8_t *)&data);
  delayMilliSeconds(1);
  status = LTC2949_PollReg(LTC2949_REG_OPCTRL, 0, 0xFFU, LTC2949_POLL_TIMEOUT, true, NULL);
  return status;
}

//// true: TBx was updated
//bool LTC2949_ChkUpdate(uint8_t * error, unsigned long* ms, uint8_t tbAddr)
//{
//  uint32_t tbx;
//  if (ms != NULL)
//    *ms = millis(); // store the timestamp when the check was done
//
//  // here we don't care about little or big endianness
//  // as we just check for equal / unequal.
//  *error = LTC2949_READ(tbAddr, 4, (uint8_t*)&tbx);
//  if (*error)
//    return false;
//  if (tbx == LTC2949_lastTBx)
//    return false; // tbx still not changed
//  // tbx changed
//  LTC2949_lastTBx = tbx;
//  return true;
//}
//
//uint8_t LTC2949_PollUpdate(unsigned long* measStart, uint8_t tbAddr)
//{
//  uint8_t error = 0;
//  bool timeoutInitDone = false;
//  unsigned long _measStart = 0;
//  unsigned long _timeout;
//
//  if (!measStart)
//    measStart = &_measStart;
//
//  while (true)
//  {
//    uint8_t err;
//    if (LTC2949_ChkUpdate(&err, measStart, tbAddr))
//    {
//      error |= err;
//      error &= ~LTC2949_ERRCODE_TIMEOUT;
//      break;
//    }
//    error |= err;
//    if (error)
//      break; // timeout or communication error
//
//    if (!timeoutInitDone)
//    {
//      timeoutInitDone = true;
//      _timeout = *measStart + LTC2949_TIMING_IDLE2CONT2UPDATE;
//    }
//    else if (LTC_TIMEOUT_CHECK(millis(), _timeout))
//    {
//      error |= LTC2949_ERRCODE_TIMEOUT;
//    }
//  }
//  return error;
//}

bmsStatus_t LTC2949_GoCont(uint8_t cfgFast, uint8_t adcCfg, unsigned long* startOfMeas, bool clrAccus)
{
  bmsStatus_t status = BMS_ERROR;
	uint8_t data;
	status = LTC2949_READ(LTC2949_REG_OPCTRL, 1, &data); // check current operation mode
//  if (bitMaskSetChk(data, LTC2949_BM_OPCTRL_CONT))
//  {
//    // clear of FACTRL is not necessary (done automatically internally)
//    // LTC2949_WriteFastCfg(0);     // clear FACTRL
//    // we are in CONT mode, so go IDLE
//    LTC2949_OpctlIdle();       // clear OPCTRL
//  }

//  // configure ADC (PasV, NTC...)
  LTC2949_ADCConfigAll(adcCfg);
//  // write ADC config
  LTC2949_ADCConfigWrite();
//  // clear prev. measurement and make adjust update
  status = LTC2949_OpctlAdjUpd(clrAccus);

  // read & clear status
  // error |= LTC2949_ReadChkStatusFaults(true, reportFaults);

//  if (!clrAccus)
//  {
//    LTC2949_READ(LTC2949_VAL_TB1, 4, (uint8_t*)&LTC2949_lastTBx);
//  }

//  unsigned long _startOfMeas;
//  // start cont. measurement
  LTC2949_OpctlCont(); // write CONT to OPCTRL
//  if (error)
//    return error;
//  // poll for start of measurement (TB1/2/3/4 != 0)
//  error = LTC2949_PollUpdate(&_startOfMeas, LTC2949_VAL_TB1);
//  if (error)
//    return error;
//
//  // one measurement usually takes 100 ms. So the actual
//  // start of measurement was 100 ms before the current millis value.
//  _startOfMeas -= 100;
//
//  if (startOfMeas != NULL)
//    *startOfMeas = _startOfMeas;
//
//  // enable fast continuous or single shot
//  LTC2949_WriteFastCfg(cfgFast); // write FACTRL

  return status;
}

bmsStatus_t LTC2949_WriteFastCfg()
{
  bmsStatus_t status = BMS_ERROR;
  // note: every time we write to special row, we can directly use _LTC2949_direct_write_ if we make sure to update LTC2949_iAddrRegsCtrl[LTC2949_REGSCTRL_IX] in case we write LTC2949_REG_REGSCTRL
  status = _LTC2949_direct_write_(LTC2949_REG_FACTRL, 1, (uint8_t*)(LTC2949_BM_FACTRL_FACONV | LTC2949_BM_FACTRL_FACH2));
  return status;
}

bmsStatus_t Cont(bool enable)
{
  bmsStatus_t status = BMS_ERROR;
  if (enable)
  {
    // fast slot not used, still we configure something
//    LTC2949_SlotFastCfg(3, 2);
    // SLOT1 measures temperature via NTC between V1 and GND.
    // SLOT2 used optionally, see ADD_SLOT2_MEASUREMENT
//    LTC2949_SlotsCfg(1, 0, SLOT2_MEAS_POS, SLOT2_MEAS_NEG);
    // enable NTC temperature measurement via SLOT1
    //NtcCfgWrite(1, NTC_RREF, NTC_STH_A, NTC_STH_B, NTC_STH_C);

    // read & clear status
    //error |= LTC2949_ReadChkStatusFaults(true, false, 10, NULL, NULL, LTC2949_STATFAULTSCHK_DEFAULT_SETTING);
    // enable measurement
    status =  LTC2949_GoCont(LTCDEF_FACTRL_CONFIG, LTCDEF_ADCCFG_CONFIG, NULL, true);
    return status;
  }
  //LTC2949_WriteFastCfg(0);
  status = LTC2949_OpctlIdle();
  return status;
}

/// returns 0 if no error
bmsStatus_t LTC2949_MemLock(bool lock)
{
  bmsStatus_t status = BMS_ERROR;
  // clear all MLK bits
  LTC2949_iAddrRegsCtrl[LTC2949_REGSCTRL_IX] &= ~(LTC2949_BM_REGSCTRL_MLK1 | LTC2949_BM_REGSCTRL_MLK0);

  if (lock)
  {
    LTC2949_iAddrRegsCtrl[LTC2949_REGSCTRL_IX] |= LTC2949_BM_REGSCTRL_MLK0; // set lock request bit
  }

  // request lock or unlock of memory (note: every time we write to special row, we can directly use _LTC2949_direct_write_ if we make sure to update LTC2949_iAddrRegsCtrl[LTC2949_REGSCTRL_IX] in case we write LTC2949_REG_REGSCTRL)
  status = _LTC2949_direct_write_(LTC2949_REG_REGSCTRL, 1, (uint8_t *)&(LTC2949_iAddrRegsCtrl[LTC2949_REGSCTRL_IX]));
  //if(status != BMS_OK) return status;

  if (!lock)
  {
    status = BMS_OK;
    return status; // in case of unlock we immediately return
  }
  // calc timeout for lock request acknowledge
  unsigned long timeout = millis() + LTC2949_TIMING_MLOCK_ACK;


  do
  {
    // read lock state
    status = LTC2949_READ(LTC2949_REG_REGSCTRL, 1, &(LTC2949_iAddrRegsCtrl[LTC2949_REGSCTRL_IX]));
    // check any error except timeout
    if (status & ~LTC2949_ERRCODE_TIMEOUT) // here we ignore timeout
    {
      break;
    }

    // check valid lock acknowledge
    if (((LTC2949_iAddrRegsCtrl[LTC2949_REGSCTRL_IX] & (LTC2949_BM_REGSCTRL_MLK1 | LTC2949_BM_REGSCTRL_MLK0)) == LTC2949_BM_REGSCTRL_MLK1)) // we expect this bit to be set after lock request
    {
      status &= ~LTC2949_ERRCODE_TIMEOUT;
      break;
    }

    // now we also break in case of timeout (as even the last read after timeout failed)
    if (status != BMS_OK)
    {
      break;
    }

    if (LTC_TIMEOUT_CHECK(millis(), timeout))
    {
      status = BMS_CCM_TIMEOUT;
    }

  } while (1);

  // clear all lock bits
  LTC2949_iAddrRegsCtrl[LTC2949_REGSCTRL_IX] &= ~(LTC2949_BM_REGSCTRL_MLK1 | LTC2949_BM_REGSCTRL_MLK0);
  return status;
}

bmsStatus_t LTC2949_ReadChkStatusFaults(bool lockMemAndClr, bool printResult, uint8_t len, uint8_t * statFaultsExpAndRd, bool * expChkFailed, uint8_t expDefaultSet)
{
  len = min(len, 10); // STATUS, 7xALTERTS, FAULTS, EXTFAULTS = 10 is maximum
  // lock memory?
  bmsStatus_t status = BMS_ERROR; 
  status = lockMemAndClr ? LTC2949_MemLock(true) : BMS_OK;
  //if(status != BMS_OK) return status;

  bool  _expChkFailed;
  uint8_t expStatFaultsDefault[10] = {0};
  if (expChkFailed == NULL)
    expChkFailed = &_expChkFailed;
  *expChkFailed = false;

  if (statFaultsExpAndRd == NULL)
    statFaultsExpAndRd = expStatFaultsDefault;
  else
  {
    // default expectations
    if (expDefaultSet & _LTC2949_STATFAULTSCHK_ALL_DEFAULT_MASK)
    {
      // set all to 0
      memset(expStatFaultsDefault, 0, len);

      // after sleep?
      if (bitMaskSetChk(expDefaultSet, LTC2949_STATFAULTSCHK_DFLT_AFTER_SLEEP))
        expStatFaultsDefault[0] =
        LTC2949_BM_STATUS_UVLOA |
        LTC2949_BM_STATUS_PORA |
        LTC2949_BM_STATUS_UVLOD;//0x0B

      // after reset?
      if (bitMaskSetChk(expDefaultSet, LTC2949_STATFAULTSCHK_DFLT_AFTER_RESET))
      {
        expStatFaultsDefault[0] =
          LTC2949_BM_STATUS_UVLOA |
          LTC2949_BM_STATUS_PORA |
          LTC2949_BM_STATUS_UVLOSTBY |
          LTC2949_BM_STATUS_UVLOD;//0x0F
        if (len > 9)
          expStatFaultsDefault[8] = LTC2949_BM_EXTFAULTS_HWMBISTEXEC;
      }

      // when in cont?
      if (bitMaskSetChk(expDefaultSet, LTC2949_STATFAULTSCHK_DFLT_AFTER_CONT))
        expStatFaultsDefault[0] |= LTC2949_BM_STATUS_UPDATE;

      // or finally all cleared (overwrites all above)
      if (bitMaskSetChk(expDefaultSet, LTC2949_STATFAULTSCHK_DFLT_AFTER_CLR))
      {
        expStatFaultsDefault[0] = 0;
        if (len > 9)
          expStatFaultsDefault[8] = 0;
      }
    }
    else
    {
      // use given values
      memcpy(expStatFaultsDefault, statFaultsExpAndRd, len);
    }
  }

  // read all STATUS / ALTERS regs
  status = LTC2949_READ(LTC2949_REG_STATUS, min(8, len), statFaultsExpAndRd);
  //if(status != BMS_OK) return status;

  if (len > 9)
  {
    // read EXTFAULTS & FAULTS reg
    status = LTC2949_READ(LTC2949_REG_EXTFAULTS, 2, (uint8_t *)&(statFaultsExpAndRd[8]));
    //if(status != BMS_OK) return status;
  }
  else if (len > 8)
  {
    // read FAULTS only
    status = LTC2949_READ(LTC2949_REG_FAULTS, 1, (uint8_t *)&(statFaultsExpAndRd[8]));
    //if(status != BMS_OK) return status;
  }

  // clear should only be done if memory is locked!
  if (lockMemAndClr)
  {
    // zeros for clearing of status regs
    uint8_t zeros[8];
    memset(zeros, 0, 8);
    // clear STATUS / ALTERS and FAULTS
    status = LTC2949_WRITE(LTC2949_REG_STATUS, min(8, len), zeros);
    //if(status != BMS_OK) return status;

    if (len > 9)
    {
      status = LTC2949_WRITE(LTC2949_REG_EXTFAULTS, 2, zeros);
      //if(status != BMS_OK) return status;
    }
    else if (len > 8)
    {
      status = LTC2949_WRITE(LTC2949_REG_FAULTS, 1, zeros);
      //if(status != BMS_OK) return status;
    }
      
  }

  // print before modifying read results!
  //if (printResult)
  //  LTC2949_ReportFaults(statFaultsExpAndRd, len);

  if (statFaultsExpAndRd != expStatFaultsDefault) // only if expected values were given, see above
  {
    // check for bits that should be ignored
    if (bitMaskSetChk(expDefaultSet, LTC2949_STATFAULTSCHK_IGNORE_STATUPD))
      bitMaskCpy(expStatFaultsDefault[0], statFaultsExpAndRd[0], LTC2949_BM_STATUS_UPDATE);
    if (bitMaskSetChk(expDefaultSet, LTC2949_STATFAULTSCHK_IGNORE_STAT0F))
      bitMaskCpy(expStatFaultsDefault[0], statFaultsExpAndRd[0], 0x0F);
    if (bitMaskSetChk(expDefaultSet, LTC2949_STATFAULTSCHK_IGNORE_MBISTEXEC) && len > 9)
      bitMaskCpy(expStatFaultsDefault[8], statFaultsExpAndRd[8], LTC2949_BM_EXTFAULTS_HWMBISTEXEC);

    // check regs versus expectations
    for (uint8_t i = 0; i < len; i++)
      *expChkFailed = *expChkFailed || (statFaultsExpAndRd[i] != expStatFaultsDefault[i]);
  }

  if (lockMemAndClr)
  {
    status = LTC2949_MemLock(false);
    // Note: Function LTC2949_ReadChkStatusFaults should not be called faster than one system tick
    //       if clearing should be effective
    //delayMilliSeconds(LTC2949_TIMING_IDLE2CONT2UPDATE);
  }
  return status;
}

bmsStatus_t LTC2949_WakeupAndAck(void)
{
  bmsStatus_t status = BMS_ERROR;
  // write the default of REGSCTRL (Note BCREN will be automatically adjusted by following poll command if necessary!)
  LTC2949_iAddrRegsCtrl[LTC2949_REGSCTRL_IX] = LTC2949_BM_REGSCTRL_RDCVCONF;
  // this will force write of REGSCTRL with above value in any case
  LTC2949_forceWrRegsCtrl = true;
  // let's try to read
  LTC2949_wakeup_idle(true);
  // poll OPCTRL
  // Note: LTC2949_PollReg will always write REGSCTRL in case of indirect commands or in case of PEC error
  // (thus BREN and RDCVCONF will always be adjusted according to communication mode)

  status = LTC2949_PollReg(LTC2949_REG_OPCTRL, 0, LTC2949_BM_OPCTRL_SLEEP, LTC2949_TIMING_BOOTUP, false, NULL);
  //if(status != BMS_OK) return status;
  status = LTC2949_WRITE(LTC2949_REG_WKUPACK, 1, 0x00); // write wake up acknowledge
  return status;
}

bmsStatus_t WakeUpReportStatus(void)
{
  bmsStatus_t status = BMS_ERROR;
  status = LTC2949_WakeupAndAck();
  //if(status != BMS_OK) return status;
  status = LTC2949_ReadChkStatusFaults(true, false, 10, NULL, NULL, LTC2949_STATFAULTSCHK_DEFAULT_SETTING);
  return status;
}

bmsStatus_t CheckDeviceReady(void)
{
  bmsStatus_t status = BMS_ERROR;
  uint8_t data[10]= {'\0'};
  uint8_t data1[10]= {'\0'};
  uint8_t data2[10]= {'\0'};
  uint8_t data3[10]= {'\0'};
  uint8_t error;
  bool expChkFailed;

  if ((error = LTC2949_READ(LTC2949_REG_OPCTRL, 1, data) != 0) || // PEC error (e.g. rebooting)
    (data[0] != LTC2949_BM_OPCTRL_CONT) ||              // not in continuous mode
    (error = LTC2949_READ(LTC2949_REG_FACTRL, 1, data1) != 0) || // PEC error
    (data1[0] != LTCDEF_FACTRL_CONFIG) ||               // not or wrong fast mode
    (error = LTC2949_ADCConfigRead(data2) != 0) ||       // PEC error
    (data2[0] != LTCDEF_ADCCFG_CONFIG) ||        // wrong ADC config
    (error = LTC2949_ReadChkStatusFaults(
      /*boolean lockMemAndClr:    */ false,
      /*boolean printResult:      */ false,
      /*byte len:                 */ 10,
      /*byte * statFaultsExpAndRd:*/ data3,
      /*boolean * expChkFailed:   */ &expChkFailed,
      /*byte expDefaultSet):      */ LTC2949_STATFAULTSCHK_IGNORE_STATUPD | LTC2949_STATFAULTSCHK_DFLT_AFTER_CLR) != 0) ||
    expChkFailed
    )
  {
    error |= WakeUpReportStatus();
    error |= Cont(false); // go IDLE;
    delayMilliSeconds(110);//delayMilliSeconds(LTC2949_TIMING_CONT_CYCLE);
    error |= Cont(true);
    status = BMS_CMU_NOT_READY;
    // return status;
  }
  else
  {
    status = BMS_OK;
  }

  return status;
}

bmsStatus_t LTC2949_reset(void)
{
  bmsStatus_t status = BMS_ERROR;
  status = LTC2949_WRITE(LTC2949_REG_OPCTRL, 1, 0); // force STANDBY
  //if(status != BMS_OK) return status;
  // write unlock reset on 2nd page.
  // note that LTC2949_WRITE(... also changes the page if necessary.
  status = LTC2949_WRITE(LTC2949_REG_RSTUNLCK, 1, (uint8_t *)LTC2949_CODE_RSTUNLCK); // write 0x55 to RSTUNLCK register on 2nd page
  //if(status != BMS_OK) return status;
  delayMilliSeconds(110);//delayMilliSeconds(LTC2949_TIMING_CONT_CYCLE); // delay 105 ms
  status = LTC2949_WRITE(LTC2949_REG_OPCTRL, 1, (uint8_t *)LTC2949_BM_OPCTRL_SLEEP); // go to sleep
  //if(status != BMS_OK) return status;
  delayMilliSeconds(20);//delayMilliSeconds(LTC2949_TIMING_IDLE_CYCLE); // wait 19 ms for sleep
  // now we are ready to make a reset
  status = LTC2949_WRITE(LTC2949_REG_OPCTRL, 1, (uint8_t *)LTC2949_BM_OPCTRL_RST); // make the reset
  // DONE
  //
  // now LTC2949 can be woken up again and STATUS register must report 0x0F
  // any other value means there was no reset (e.g. due to communication error)
  // and the reset has to be repeated
  LTC2949_init_device_state();
  delayMilliSeconds(1);

  return status;
}

double LTC2949_BytesToDouble(uint8_t * bytes, uint8_t length, uint8_t sign, double lsb)
{
  if (length == 0)
		return 0.0;
	else if (length == 1)
		return sign ? (int8_t)(bytes[0]) * lsb : bytes[0] * lsb;
	else if (length == 2)
		return sign ? LTC_2BytesToInt16(bytes) * lsb : LTC_2BytesToUInt16(bytes) * lsb;
	else if (length == 3)
		return sign ? LTC_3BytesToInt32(bytes) * lsb : LTC_3BytesToUInt32(bytes) * lsb;
	else if (length == 4)
		return sign ? LTC_4BytesToInt32(bytes) * lsb : LTC_4BytesToUInt32(bytes) * lsb;
	else if (length == 6)
		return LTC_6BytesToInt64(bytes) * lsb;
	else
		return -1.0;
}

int64_t LTC_6BytesToInt64(uint8_t* bytes)
{
	int64_t ret = (bytes[0] & 0x80)
		? 0xFFFFL
		: 0L;
	for (uint8_t i = 0; i < 6; i++)
	{
		ret = ret << 8;
		ret |= bytes[i];
	}
	return ret;
}

int32_t LTC_4BytesToInt32(uint8_t* bytes)
{
	int32_t ret;

	ret = *bytes;
	bytes++;
	ret = ret << 8;
	ret |= *bytes;
	bytes++;
	ret = ret << 8;
	ret |= *bytes;
	bytes++;
	ret = ret << 8;
	ret |= *bytes;
	return ret;
}

uint32_t LTC_4BytesToUInt32(uint8_t* bytes)
{
	uint32_t ret;

	ret = *bytes;
	bytes++;
	ret = ret << 8;
	ret |= *bytes;
	bytes++;
	ret = ret << 8;
	ret |= *bytes;
	bytes++;
	ret = ret << 8;
	ret |= *bytes;
	return ret;
}

int32_t LTC_3BytesToInt32(uint8_t* bytes)
{
  int32_t ret;
  // sign extension
  if (*bytes & 0x80)
    ret = 0xFF00;
  else
    ret = 0;

  ret |= *bytes;
  bytes++;
  ret = ret << 8;
  ret |= *bytes;
  bytes++;
  ret = ret << 8;
  ret |= *bytes;
  return ret;
}

uint32_t LTC_3BytesToUInt32(uint8_t* bytes)
{
	uint32_t ret;

	ret = *bytes;
	bytes++;
	ret = ret << 8;
	ret |= *bytes;
	bytes++;
	ret = ret << 8;
	ret |= *bytes;
	return ret;
}

int16_t LTC_2BytesToInt16(uint8_t* bytes)
{
  int16_t ret;
  ret = *bytes;
  bytes++;
  ret = ret << 8;
  ret |= *bytes;
  return ret;
}

uint16_t LTC_2BytesToUInt16(uint8_t* bytes)
{
	uint16_t ret;
	ret = *bytes;
	bytes++;
	ret = ret << 8;
	ret |= *bytes;
	return ret;
}




/* End of File ---------------------------------------------------------------*/
