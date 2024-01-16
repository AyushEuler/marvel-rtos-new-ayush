/**
 * @file onBoardCCM2949.h
 * @author Arpit Gupta (arpit@euler)
 * @brief 
 * @version 0.1
 * @date 2022-04-09
 * 
 * @copyright Copyright (c) 2022
 * 
 */

/* Header Guard --------------------------------------------------------------*/
#ifndef ONBOARDCCM2949_H_
#define ONBOARDCCM2949_H_

/* Standard C includes -------------------------------------------------------*/
#include "stdint.h"
#include "stdbool.h"
#include "math.h"
/* Private includes ----------------------------------------------------------*/
#include "bmsConfiguration.h"
/* Private define ------------------------------------------------------------*/
#define PEC15_POLY 					0x4599U

#define min(a,b) ((a)<(b)?(a):(b))
#define max(a,b) ((a)>(b)?(a):(b))

#define bitMaskSetChk(value, bitMask) 				(((value) & (bitMask)) == (bitMask))
#define bitMaskClrChk(value, bitMask) 				(((value) & (bitMask)) == 0)
#define bitMaskSet(value, bitMask) 					(value |=  (bitMask))
#define bitMaskClr(value, bitMask) 					(value &= ~(bitMask))
#define bitMaskSetClrChk(value, bitMask, setNotClr)	((setNotClr) ? bitMaskSetChk(value,bitMask) : bitMaskClrChk(value,bitMask))
#define bitMaskSetClr(value, bitMask, setNotClr) 	((setNotClr) ? bitMaskSet(value,bitMask) : bitMaskClr(value,bitMask))
#define bitMaskCpy(des, src, bitMask) 				{des &= ~bitMask; des |= bitMask & src;}
#define bitsMaskApplyToReg(REG, BITS, MASK) 		{BITS &= (MASK); REG &= ~(MASK); REG |= (BITS);}

/* Private macro -------------------------------------------------------------*/
// max. core boot-up time after power-up or reset (ms)
// typical boot-up time is 73ms. 10% margin is more than enough leading to 80ms
#define LTC2949_TIMING_BOOTUP              100U
// Time after power-up or reset to start polling if core is read (boot-up done, ms)
// the max. wakeup time spec is above. But we start polling for device to be ready earlier which is ok.
#define LTC2949_TIMING_START_POLL_CORE_RDY 10U
// max. CONT mode cycle time (ms)
#define LTC2949_TIMING_CONT_CYCLE          105U
// max. time it takes to lock the memory, worst case is twice CONT cycle time plus some margin (ms)
#define LTC2949_TIMING_MLOCK_ACK           (2*LTC2949_TIMING_CONT_CYCLE+15U)
// max. IDLE cycle time (ms)
// typical IDLE cycle time is 17 ms. 10% margin is more than enough leading to 19ms
#define LTC2949_TIMING_IDLE_CYCLE          19U
// max. power-up to auto sleep time (ms)
#define LTC2949_TIMING_AUTO_SLEEP_MAX      1500U
// min. power-up to auto sleep time (ms)
#define LTC2949_TIMING_AUTO_SLEEP          1000U
// max. time from CONT enable to 1st slow channel values UPDATE done
#define LTC2949_TIMING_IDLE2CONT2UPDATE    160U
// max. time from TB4 update to STATUS/ALERTS update
#define LTC2949_TIMING_TB4_TO_STATS        20U

// Reset Unlock code
// this value written to LTC2949_REG_RSTUNLCK unlocks the reset bit in LTC2949_REG_OPCTRL
#define LTC2949_CODE_RSTUNLCK       0x55

#define LTC2949_ERRCODE_OK           0x00
#define LTC2949_ERRCODE_COMMERR      0x10
#define LTC2949_ERRCODE_FIFOWROVR    0x20
#define LTC2949_ERRCODE_FIFOTAGERR   0x40
#define LTC2949_ERRCODE_OTHER        0x80
#define LTC2949_ERRCODE_TIMEOUT      0x08
#define LTC2949_ERRCODE_FAULTS       0x04
#define LTC2949_ERRCODE_PECERR_MASK  0x03
#define LTC2949_ERRCODE_PECERR_0     0x01
#define LTC2949_ERRCODE_PECERR_1     0x02
#define LTC2949_PEC_ERR_CNT_MAX      LTC2949_ERRCODE_PECERR_MASK

#define LTC2949_GPIO_OUTPUT_HIGH 3
#define LTC2949_GPIO_OUTPUT_LOW	 1
#define LTC2949_GPIO_INPUT		 0
#define LTC2949_GPIO_OUTPUT_TGL  2

#define LTC2949_RDFASTDATA_LENGTH 5
// indices within RDFASTDATA for the measurement results and the HS bytes
#define LTC2949_RDFASTDATA_I1  0
#define LTC2949_RDFASTDATA_I2  1
#define LTC2949_RDFASTDATA_BAT 2
#define LTC2949_RDFASTDATA_AUX 3
#define LTC2949_RDFASTDATA_HS  4

// Values for the FCTRL register, for setting to Fast On or Fast Off.
#define LTC2949_FACTRL_FAST_ALLCONTEN  0x0F
#define LTC2949_FACTRL_FAST_ALLEN      0x0E
#define LTC2949_FACTRL_FAST_OFF        0x00
#define LTC2949_FIFO_TAG_RDOVR  0x55U
#define LTC2949_FIFO_TAG_WROVR  0xAAU
#define LTC2949_FIFO_TAG_OK     0x00U

#define LTC2949_STATE_MSB 0U
#define LTC2949_STATE_LSB 1U
#define LTC2949_STATE_TAG 2U

// make a direct command read (to be used with _LTC2949_DcRW_ function)
#define LTC2949_DcRead  true
// make a direct command write (to be used with _LTC2949_DcRW_ function)
#define LTC2949_DcWrite false

#define LTC2949_DC_READ_BIT  0x80
#define LTC2949_DC_WRITE_BIT 0x40

// default register poll timeout
#define LTC2949_POLL_TIMEOUT 2000

// addressed: 0b1111_1110 = 0xFE
#define LTC2949_ADDR_CMD   0xFE

#define LTC2949_ADDRESS  0x1F

// define index of REGSCTRL and RDCVIADDR within LTC2949_iAddrRegsCtrl byte array
#define LTC2949_REGSCTRL_IX 				(LTC2949_REG_REGSCTRL - LTC2949_REG_RDCVIADDR)
#define LTC2949_RDCVIADDR_IX 				0
#define LTC2949_IADDRREGSCTRL_LEN 			(LTC2949_REGSCTRL_IX + 1)
#define LTC2949_IR_RDCV_BYTES_PER_PEC 		6
#define LTC2949_IS_ADDR_SPECIAL_ROW(ADDR)	bitMaskSetChk(ADDR, 0xF0)

// worst case time after ADCV at which conversion results can be read via RDCV
#define LTC2949_FASTSSHT_RDY_TIME_US 	1260U

// for status faults checks
// default status reg after sleep recovery
#define LTC2949_STATFAULTSCHK_DFLT_AFTER_SLEEP      1
// default status/extfaults reg after (power  on) reset
#define LTC2949_STATFAULTSCHK_DFLT_AFTER_RESET     (1+2)
// default status/extfaults reg after cleared
#define LTC2949_STATFAULTSCHK_DFLT_AFTER_CLR        4
// default status reg when in cont mode (UPDATE bit expected, but only if at least 100 ms have elapsed!)
#define LTC2949_STATFAULTSCHK_DFLT_AFTER_CONT       8
// used internal only to check if default values should be used as expected values
#define _LTC2949_STATFAULTSCHK_ALL_DEFAULT_MASK      0x0F

// ignore value of UPDATE bit in STATUS register
#define LTC2949_STATFAULTSCHK_IGNORE_STATUPD        16
// ignore value of low nibble bits in STATUS register
#define LTC2949_STATFAULTSCHK_IGNORE_STAT0F         32
// ignore value of MBISTEXEC in EXTFAULTS register
#define LTC2949_STATFAULTSCHK_IGNORE_MBISTEXEC      64

// Default setting used for LTC2949_ReadChkStatusFaults if parameter is not given
#define LTC2949_STATFAULTSCHK_DEFAULT_SETTING 		(LTC2949_STATFAULTSCHK_IGNORE_STATUPD|LTC2949_STATFAULTSCHK_IGNORE_STAT0F|LTC2949_STATFAULTSCHK_IGNORE_MBISTEXEC|LTC2949_STATFAULTSCHK_DFLT_AFTER_RESET)

// Wake-up acknowledge:
// Write 0x00 to confirm wake-up from sleep.
// Without acknowledge device will go to sleep after 1 second. (register on page 0)
#define LTC2949_REG_WKUPACK 0x070

// Status register (active high) (register on page 0)
#define LTC2949_REG_STATUS 0x080

// SLOT1 multiplexer setting of negative ADC input
// 0: AGND, 1-12: V1-V12
// 13: IMT, 14: IPT
// 15: BATM, 16: BATP
// 17: CFI2M, 18: CFI2P
// 19: CFI1M, 20: CFI1P
// 21,23: Reserved, 22: 2nd VREF (register on page 0)
#define LTC2949_REG_SLOT1MUXN 0x0EB

// SLOT1 multiplexer setting of positive ADC input
// 0: AGND, 1-12: V1-V12
// 13: IMT, 14: IPT
// 15: BATM, 16: BATP
// 17: CFI2M, 18: CFI2P
// 19: CFI1M, 20: CFI1P
// 21,23: Reserved, 22: 2nd VREF (register on page 0)
#define LTC2949_REG_SLOT1MUXP 0x0EC

// SLOT2 multiplexer setting of negative ADC input
// 0: AGND, 1-12: V1-V12
// 13: IMT, 14: IPT
// 15: BATM, 16: BATP
// 17: CFI2M, 18: CFI2P
// 19: CFI1M, 20: CFI1P
// 21,23: Reserved, 22: 2nd VREF (register on page 0)
#define LTC2949_REG_SLOT2MUXN 0x0ED

// SLOT2 multiplexer setting of positive ADC input
// 0: AGND, 1-12: V1-V12
// 13: IMT, 14: IPT
// 15: BATM, 16: BATP
// 17: CFI2M, 18: CFI2P
// 19: CFI1M, 20: CFI1P
// 21,23: Reserved, 22: 2nd VREF (register on page 0)
#define LTC2949_REG_SLOT2MUXP 0x0EE

// Operation Control (register on page 0)
#define LTC2949_REG_OPCTRL 0x0F0

// GPIO and current source control
// 00: GPIO is tristate
// 11: GPIO is high (DVCC)
// 01: GPIO is low (DGND)
// 10: GPIO toggles at 400kHz between DVCC, DGND (register on page 0)
#define LTC2949_REG_FCURGPIOCTRL 0x0F1

// GPIO control
// 00: GPIO is tristate
// 11: GPIO is high (DVCC)
// 01: GPIO is low (DGND)
// 10: GPIO toggles at 400kHz between DVCC, DGND (register on page 0)
#define LTC2949_REG_FGPIOCTRL 0x0F2

// Fast auxilliary multiplexer setting of negative ADC input
// 0: AGND, 1-12: V1-V12
// 13: IMT, 14: IPT
// 15: BATM, 16: BATP
// 17: CFI2M, 18: CFI2P
// 19: CFI1M, 20: CFI1P
// 21,23: Reserved, 22: 2nd VREF (register on page 0)
#define LTC2949_REG_FAMUXN 0x0F3

// Fast auxilliary multiplexer setting of positive ADC input
// 0: AGND, 1-12: V1-V12
// 13: IMT, 14: IPT
// 15: BATM, 16: BATP
// 17: CFI2M, 18: CFI2P
// 19: CFI1M, 20: CFI1P
// 21,23: Reserved, 22: 2nd VREF (register on page 0)
#define LTC2949_REG_FAMUXP 0x0F4

// Fast channel access control (register on page 0)
#define LTC2949_REG_FACTRL 0x0F5

// Address pointer for indirect memory access via RDCV command (for daisy chain operation only) (register on page 0)
#define LTC2949_REG_RDCVIADDR 0x0FC

// Register Control (register on page 0)
#define LTC2949_REG_REGSCTRL 0x0FF

// if 0x55 it unlocks the SPI reset feature (register on page 1)
#define LTC2949_REG_RSTUNLCK 0x1A9

// ADC Configuration Register allows to disable power multiplication on P1ADC and P2ADC, to turn on NTC temperature measurement of SLOT1 / SLOT2 measurements. (register on page 1)
#define LTC2949_REG_ADCCONF 0x1DF

// Extended faults (register on page 0)
#define LTC2949_REG_EXTFAULTS 0x0DC

// Miscellaneous faults (register on page 0)
#define LTC2949_REG_FAULTS 0x0DD

// Time Base Control (register on page 0)
#define LTC2949_REG_TBCTRL 0x0E9

// Overcurrent  Control  Register allow to set the
// overcurrent comparator thresholds and deglitch times (register on page 0)
#define LTC2949_REG_OCC1CTRL 0x0DE

// Overcurrent  Control  Register allow to set the
// overcurrent comparator thresholds and deglitch times (register on page 0)
#define LTC2949_REG_OCC2CTRL 0x0DF

// Accumulation control of Charge1/2, Energy1/2 by Current1/2 polarity.
// 00: Accumulation takes place always, 
// 01: only if the current is positive, 
// 10: only if the current is negative, 
// 11: No accumulation takes place. (register on page 0)
#define LTC2949_REG_ACCCTRL1 0x0E1

// Accumulation control of Charge3 and Energy4
// 00: Accumulation takes place always
// 01: only if the current is positive
// 10: only if the current is negative
// 11: No accumulation takes place (register on page 0)
#define LTC2949_REG_ACCCTRL2 0x0E2

// Deadband of Current1 for accumulation. 
// Current1 is not accumulated if its absolute value is below the deadband. (register on page 0)
#define LTC2949_REG_ACCI1DB 0x0E4

// Deadband of Current2 for accumulation. 
// Current2 is not accumulated if its absolute value is below the deadband. (register on page 0)
#define LTC2949_REG_ACCI2DB 0x0E5

//Internal Timer Control
#define LTC2949_INTC 31e-6

// LSB of I1 in V
#define LTC2949_LSB_I1           9.5e-07
// LSB of P1 in V²
#define LTC2949_LSB_P1           5.8368e-06
// LSB of P1 as Voltage ADC in V
#define LTC2949_LSB_P1ASV        (LTC2949_LSB_SLOT1/8.0)
// LSB of I2 in V
#define LTC2949_LSB_I2           9.5e-07
// LSB of P2 in V²
#define LTC2949_LSB_P2           5.8368e-06
// LSB of P2 as Voltage ADC in V
#define LTC2949_LSB_P2ASV        (LTC2949_LSB_SLOT2/8.0)
// LSB of IAVG1 in V
#define LTC2949_LSB_IAVG1        2.375e-07
// LSB of BAT in V
#define LTC2949_LSB_BAT          3.75e-04
// LSB of TEMP in °C
#define LTC2949_LSB_TEMP         0.2
// LSB of C1 in Vs
#define LTC2949_LSB_C1           377.887e-12	//with 4MHz Crystal
//// LSB of E1 in V²s
#define LTC2949_LSB_E1           2.32175e-09	//with 4MHz Crystal
//// LSB of TB1 in s
#define LTC2949_LSB_TB1          397.777e-06	//with 4MHz Crystal
//// LSB of C2 in Vs
//#define LTC2949_LSB_C2           (1.21899e-05 * LTC2949_TBFAC)
//// LSB of E2 in V²s
//#define LTC2949_LSB_E2           (7.4895e-05 * LTC2949_TBFAC)
//// LSB of TB2 in s
//#define LTC2949_LSB_TB2          (12.8315 * LTC2949_TBFAC)
//// LSB of C3 in Vs
//#define LTC2949_LSB_C3           (1.21899e-05 * LTC2949_TBFAC)
//// LSB of TB3 in s
//#define LTC2949_LSB_TB3          (12.8315 * LTC2949_TBFAC)
//// LSB of E4 in V²s
//#define LTC2949_LSB_E4           (7.4895e-05 * LTC2949_TBFAC)
//// LSB of TB4 in s
//#define LTC2949_LSB_TB4          (12.8315 * LTC2949_TBFAC)

// Current1 (register on page 0)
#define LTC2949_VAL_I1 0x090

// Power1 (register on page 0)
#define LTC2949_VAL_P1 0x093

// Current2 (register on page 0)
#define LTC2949_VAL_I2 0x096

// Power2 (register on page 0)
#define LTC2949_VAL_P2 0x099

// Average of Current1: (n-1)+(n-2)+(n-3)+(n-4) (register on page 0)
#define LTC2949_VAL_IAVG1 0x09C

// Voltage at BAT (BATP - BATM) (register on page 0)
#define LTC2949_VAL_BAT 0x0A0

// Temperature (register on page 0)
#define LTC2949_VAL_TEMP 0x0A2

// Charge1 (register on page 0)
#define LTC2949_VAL_C1 0x000

// Energy1 (register on page 0)
#define LTC2949_VAL_E1 0x006

// Time1 (register on page 0)
#define LTC2949_VAL_TB1 0x00C

// Charge2 (register on page 0)
#define LTC2949_VAL_C2 0x010

// Energy2 (register on page 0)
#define LTC2949_VAL_E2 0x016

// Time2 (register on page 0)
#define LTC2949_VAL_TB2 0x01C

// Charge3 (register on page 0)
#define LTC2949_VAL_C3 0x024

// Time3 (register on page 0)
#define LTC2949_VAL_TB3 0x02C

// Energy4 (register on page 0)
#define LTC2949_VAL_E4 0x034

// Time4 (register on page 0)
#define LTC2949_VAL_TB4 0x03C



/////////////////////////////////////////////////////////
// Definition of all bit masks.                        //
/////////////////////////////////////////////////////////

// STATUS bit masks
// 1: Undervoltage in the analog domain including ADCs
#define LTC2949_BM_STATUS_UVLOA 0x1
// 1: Power On Reset due to undervoltage in the analog domain.
#define LTC2949_BM_STATUS_PORA 0x2
// 1: Undervoltage in the standby domain
#define LTC2949_BM_STATUS_UVLOSTBY 0x4
// 1: Undervoltage in the digital domain
#define LTC2949_BM_STATUS_UVLOD 0x8
// 1: Result registers have been updated
#define LTC2949_BM_STATUS_UPDATE 0x10
// ADC Error
#define LTC2949_BM_STATUS_ADCERR 0x20
// 1: External clock provided does not fit to TBC settings.
// Also LSB values of accumulated registers are wrong.
#define LTC2949_BM_STATUS_TBERR 0x40

// OPCTRL bit masks
// 0: Normal operation, 1: Sleep
// LTC2949 will wake up in SPI mode if CS pulled low and
// in isoSPI mode if a long differential pulse is detected
#define LTC2949_BM_OPCTRL_SLEEP 0x1
// Clear accumulators and tracking registers
#define LTC2949_BM_OPCTRL_CLR 0x2
// Single shot measurement Ix, Px, SLOTx, BAT, TEMP, VCC
#define LTC2949_BM_OPCTRL_SSHOT 0x4
// Continuous measurement. Only in this mode Charge, Energy
// and Time are measured and Fast channel may be activate
#define LTC2949_BM_OPCTRL_CONT 0x8
// Update calibration data.
// Bit is cleared automatically after update was performed.
#define LTC2949_BM_OPCTRL_ADJUPD 0x20
// Write 1 to reset device
// Only possible after reset was unlocked
// by writing 0x55 to RSTUNLCK
#define LTC2949_BM_OPCTRL_RST 0x80

// FACTRL bit masks
// Global enable of continuous fast measurement
#define LTC2949_BM_FACTRL_FACONV 0x1
// Fast auxilliary channel meaurement enable
#define LTC2949_BM_FACTRL_FACHA 0x2
// Fast channel 1 meaurement enable (I1 and P1 as voltage if PASV1=1)
#define LTC2949_BM_FACTRL_FACH1 0x4
// Fast channel 2 meaurement enable (I2 and P2 as voltage if (PASV1=0, PASV2=1)
#define LTC2949_BM_FACTRL_FACH2 0x8

// REGSCTRL bit masks
// Memory Map Page Select.
// 0: PAGE 0 of memory map is selected.
// 1: PAGE1 of memory map is selected.
#define LTC2949_BM_REGSCTRL_PAGE 0x1
// Broadcast read enable
// 0: broadcast read commands are ignored
// 1: device will respond to broadcast read commands
#define LTC2949_BM_REGSCTRL_BCREN 0x4
// Memory lock control bit 0
#define LTC2949_BM_REGSCTRL_MLK0 0x10
// Memory lock control bit 1
#define LTC2949_BM_REGSCTRL_MLK1 0x20
// 0: indirect memory access mode, RDCV will read data starting at address RDCVIADDR
// 1: RDCV command will read fast channel conversion results
#define LTC2949_BM_REGSCTRL_RDCVCONF 0x80

// ADCCONF bit masks
// 1: P1 ADC configured to voltage mode
// 0: P1 ADC configured to power mode
#define LTC2949_BM_ADCCONF_P1ASV 0x0
// 1: P2 ADC configured to voltage mode
// 0: P2 ADC configured to power mode
#define LTC2949_BM_ADCCONF_P2ASV 0x0
// 1: SLOT1 is considered as NTC voltage and therefore the reported quatity, the thresholds and the tracking registers for SLOT1 have to be considered T (LSB = 0.2°C)
#define LTC2949_BM_ADCCONF_NTC1 0x8
// 1: SLOT2 is considered as NTC voltage and therefore the reported quatity, the thresholds and the tracking registers for SLOT2 have to be considered T (LSB = 0.2°C)
#define LTC2949_BM_ADCCONF_NTC2 0x10
// Reserved, set to 0
#define LTC2949_BM_ADCCONF_RES 0x20
// if 1 also the CH2 will be linked to SLOT1 (given that SLOT1 and SLOT2 are set to NTC); this way the SLOT2 is free to be used as GP ADC IN (one shunt scenario)
#define LTC2949_BM_ADCCONF_NTCSLOT1 0x40


// EXTFAULTS bit masks
// Hamming decoder 1-bit error
#define LTC2949_BM_EXTFAULTS_HD1BITERR 0x1
// ROM CRC error
#define LTC2949_BM_EXTFAULTS_ROMERR 0x2
// MEM error
#define LTC2949_BM_EXTFAULTS_MEMERR 0x4
// FCA error
#define LTC2949_BM_EXTFAULTS_FCAERR 0x8
// XRAM error
#define LTC2949_BM_EXTFAULTS_XRAMERR 0x10
// IRAM error
#define LTC2949_BM_EXTFAULTS_IRAMERR 0x20
// HW memory BIST executed
#define LTC2949_BM_EXTFAULTS_HWMBISTEXEC 0x80

// FAULTS bit masks
// Fuse Table Error, more than 1 bit has flipped in the fuse table, no FEC was possible
#define LTC2949_BM_FAULTS_PROMERR 0x1
// Thermal shutdown, the previous reset was due to an over temperature shutdown
#define LTC2949_BM_FAULTS_TSD 0x2
// Internal communication parity check failed
#define LTC2949_BM_FAULTS_INTCOMMERR 0x4
// external communication (SPI / isoSPI) PEC error
#define LTC2949_BM_FAULTS_EXTCOMMERR 0x8
// fast channel error
#define LTC2949_BM_FAULTS_FAERR 0x10
// Hardware BIST result: 1 error, 0 ok
#define LTC2949_BM_FAULTS_HWBIST 0x20
// Hardware config memory BIST result: 1 error, 0 ok
#define LTC2949_BM_FAULTS_CRCCFG 0x40
// Hardware memory BIST result: 1 error, 0 ok
#define LTC2949_BM_FAULTS_CRCMEM 0x80

// timeout checker: true if time has elapsed, false if not
#define LTC_TIMEOUT_CHECK(CURRENT_TIME,TIMEOUT) (((long)((CURRENT_TIME) - (TIMEOUT)) >= 0L))

// fast channel configuration
#define LTCDEF_FACTRL_CONFIG    0x00    //(LTC2949_BM_FACTRL_FACH2 | LTC2949_BM_FACTRL_FACONV)
// ADC configuration
#define LTCDEF_ADCCFG_CONFIG    0x00    //(LTC2949_BM_ADCCONF_NTC1 | LTC2949_BM_ADCCONF_P2ASV)


#define LTC2949_BM_OCCCTRL_OCCEN  		0x01
#define LTC2949_BM_OCC1CTRL_OCC1EN  	LTC2949_BM_OCCCTRL_OCCEN
#define LTC2949_BM_OCC2CTRL_OCC2EN  	LTC2949_BM_OCCCTRL_OCCEN

#define OCC_POLARITY_BOTH_POLARITY		0x00
#define OCC_POLARITY_POSITIVE_CURRENT	0x01
#define OCC_POLARITY_NEGATIVE_CURRENT	0x02

#define OCC_DEGLITCH_TIME_20_us			0x00
#define OCC_DEGLITCH_TIME_80_us			0x01
#define OCC_DEGLITCH_TIME_320_us		0x02
#define OCC_DEGLITCH_TIME_1280_us		0x03

#define OCC_DAC_THRESHOLD_0_mV			0x00
#define OCC_DAC_THRESHOLD_26_mV			0x01
#define OCC_DAC_THRESHOLD_52_mV			0x02
#define OCC_DAC_THRESHOLD_78_mV			0x03
#define OCC_DAC_THRESHOLD_103_mV		0x04
#define OCC_DAC_THRESHOLD_155_mV		0x05
#define OCC_DAC_THRESHOLD_207_mV		0x06
#define OCC_DAC_THRESHOLD_310_mV		0x07
/* Private typedef -----------------------------------------------------------*/

/* External variables --------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Inline functions ----------------------------------------------------------*/

/* Function prototypes -------------------------------------------------------*/
bmsStatus_t LTC2949Initialize(void);
bmsStatus_t LTC2949_OCCConfig(int8_t occ1Thr, uint8_t occ1Dgl, uint8_t occ1Pol, 
                              int8_t occ2Thr, uint8_t occ2Dgl, uint8_t occ2Pol);

bmsStatus_t LTC2949_init_lib(uint8_t cellMonitorCount, bool ltc2949onTopOfDaisychain, bool debugEnable);
bmsStatus_t LTC2949_GpioCurrConfigClr(void);
bmsStatus_t LTC2949_init_device_state(void);
bmsStatus_t LTC2949_isoSPI_wakeup_timeoutUs_init(uint16_t timeout);

bmsStatus_t pec15_calc_init(uint16_t * pec15);
bmsStatus_t pec15_calc_finalize(uint16_t* pec15);
bmsStatus_t pec15_calc(uint16_t* pec15, uint8_t nextByte);
int16_t _pec15_calc_internal_(uint16_t remainder);

bmsStatus_t spi_write_calc_pec(uint16_t * pec15, uint8_t data);
bmsStatus_t spi_write_pec(uint16_t * pec15);
uint8_t spi_read_calc_pec(uint16_t * pec15,uint8_t *datarx);
bmsStatus_t spi_read_chk_pec(uint16_t * pec15, uint8_t * errCnt);
bmsStatus_t spi_read_pec(uint16_t *pec_slave);
bmsStatus_t spi_write(uint8_t datatx);
bmsStatus_t spi_read(uint8_t *datarx);

uint8_t LTC2949_CALC_OPT_NO_PECS(uint16_t len);
uint8_t LTC2949_CALC_ID_BYTE(uint8_t bytesPerPec, bool readNotWrite);

bmsStatus_t LTC2949_wakeup_idle(bool force);
bmsStatus_t LTC2949_ProcessErrors(uint8_t error);

bmsStatus_t _LTC2949_DcRW_(uint8_t addr, uint16_t len, uint8_t* data, bool readNotWrite);
bmsStatus_t _LTC2949_direct_write_(uint8_t addr, uint16_t len, uint8_t* data);
bmsStatus_t _LTC2949_ADJ_REGSCTRL_DC_(uint16_t ADDR, bool readNotWrite);
int16_t LTC2949_GetRegsCtrlDataOffset(uint8_t addr, uint16_t len);
bmsStatus_t LTC2949_AdjustRegsCtrlBuffer(uint8_t addr, uint16_t len, uint8_t * data);
bmsStatus_t LTC2949_READ(uint16_t ADDR, uint16_t LEN, uint8_t * DATAPOINTER);
bmsStatus_t LTC2949_WRITE(uint16_t ADDR, uint16_t LEN, uint8_t * DATAPOINTER);

bmsStatus_t LTC2949_PollReg(uint16_t addr, uint8_t val, uint8_t mask, uint16_t timeout, bool stopOnPecErrors, uint8_t * finalRead);

bmsStatus_t LTC2949_OpctlIdle(void);
bmsStatus_t LTC2949_OpctlCont(void);

bmsStatus_t LTC2949_ADCConfigWrite(void);
bmsStatus_t LTC2949_ADCConfigRead(uint8_t * data);
bmsStatus_t LTC2949_OpctlAdjUpd(bool clear);

// bool LTC2949_ChkUpdate(uint8_t * error, unsigned long* ms, uint8_t tbAddr);
// uint8_t LTC2949_PollUpdate(unsigned long* measStart, uint8_t tbAddr);
bmsStatus_t LTC2949_GoCont(uint8_t cfgFast, uint8_t adcCfg, unsigned long* startOfMeas, bool clrAccus);
bmsStatus_t LTC2949_WriteFastCfg(void); //(uint8_t cfgFast);
bmsStatus_t Cont(bool enable);

bmsStatus_t LTC2949_MemLock(bool lock);
bmsStatus_t LTC2949_ReadChkStatusFaults(bool lockMemAndClr, bool printResult, uint8_t len, uint8_t * statFaultsExpAndRd, bool * expChkFailed, uint8_t expDefaultSet);
bmsStatus_t WakeUpReportStatus(void);

bmsStatus_t CheckDeviceReady(void);
bmsStatus_t LTC2949_reset(void);

double   LTC2949_BytesToDouble	(uint8_t * bytes, uint8_t length, uint8_t sign, double lsb);
int64_t  LTC_6BytesToInt64		(uint8_t* bytes);
int32_t  LTC_4BytesToInt32		(uint8_t* bytes);
uint32_t LTC_4BytesToUInt32		(uint8_t* bytes);
int32_t  LTC_3BytesToInt32		(uint8_t* bytes);
uint32_t LTC_3BytesToUInt32		(uint8_t* bytes);
int16_t  LTC_2BytesToInt16		(uint8_t* bytes);
uint16_t LTC_2BytesToUInt16		(uint8_t* bytes);


#endif
/* End of File ---------------------------------------------------------------*/

