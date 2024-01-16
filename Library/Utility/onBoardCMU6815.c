/**
 * @file onBoardCMU6815.c
 * @author Arpit Gupta (arpit@euler)
 * @brief 
 * @version 0.1
 * @date 2022-04-09
 * 
 * @copyright Copyright (c) 2022
 * 
 */

/* Standard C includes -------------------------------------------------------*/
#include "stdio.h"
#include "stdbool.h"
#include "string.h"
/* Private includes ----------------------------------------------------------*/
#include "customDelays.h"
#include "gpioControls.h"
#include "spiControls.h"
#include "uartControls.h"
#include "timerControls.h"
#include "onBoardIOs.h"

#include "onBoardUARTs.h"
#include "usart.h"
/* Self include --------------------------------------------------------------*/
#include "onBoardCMU6815.h"
/* Type definiton ------------------------------------------------------------*/

/* External variables definiton ----------------------------------------------*/

/* Private constants ---------------------------------------------------------*/
/* Precomputed CRC15 Table */
const uint16_t Crc15Table[256] =
{
  0x0000, 0xc599, 0xceab, 0x0b32, 0xd8cf, 0x1d56, 0x1664, 0xd3fd, 
  0xf407, 0x319e, 0x3aac, 0xff35, 0x2cc8, 0xe951, 0xe263, 0x27fa, 
  0xad97, 0x680e, 0x633c, 0xa6a5, 0x7558, 0xb0c1, 0xbbf3, 0x7e6a, 
  0x5990, 0x9c09, 0x973b, 0x52a2, 0x815f, 0x44c6, 0x4ff4, 0x8a6d, 
  0x5b2e, 0x9eb7, 0x9585, 0x501c, 0x83e1, 0x4678, 0x4d4a, 0x88d3, 
  0xaf29, 0x6ab0, 0x6182, 0xa41b, 0x77e6, 0xb27f, 0xb94d, 0x7cd4, 
  0xf6b9, 0x3320, 0x3812, 0xfd8b, 0x2e76, 0xebef, 0xe0dd, 0x2544, 
  0x02be, 0xc727, 0xcc15, 0x098c, 0xda71, 0x1fe8, 0x14da, 0xd143, 
  0xf3c5, 0x365c, 0x3d6e, 0xf8f7, 0x2b0a, 0xee93, 0xe5a1, 0x2038, 
  0x07c2, 0xc25b, 0xc969, 0x0cf0, 0xdf0d, 0x1a94, 0x11a6, 0xd43f, 
  0x5e52, 0x9bcb, 0x90f9, 0x5560, 0x869d, 0x4304, 0x4836, 0x8daf,
  0xaa55, 0x6fcc, 0x64fe, 0xa167, 0x729a, 0xb703, 0xbc31, 0x79a8, 
  0xa8eb, 0x6d72, 0x6640, 0xa3d9, 0x7024, 0xb5bd, 0xbe8f, 0x7b16, 
  0x5cec, 0x9975, 0x9247, 0x57de, 0x8423, 0x41ba, 0x4a88, 0x8f11, 
  0x057c, 0xc0e5, 0xcbd7, 0x0e4e, 0xddb3, 0x182a, 0x1318, 0xd681, 
  0xf17b, 0x34e2, 0x3fd0, 0xfa49, 0x29b4, 0xec2d, 0xe71f, 0x2286, 
  0xa213, 0x678a, 0x6cb8, 0xa921, 0x7adc, 0xbf45, 0xb477, 0x71ee, 
  0x5614, 0x938d, 0x98bf, 0x5d26, 0x8edb, 0x4b42, 0x4070, 0x85e9, 
  0x0f84, 0xca1d, 0xc12f, 0x04b6, 0xd74b, 0x12d2, 0x19e0, 0xdc79, 
  0xfb83, 0x3e1a, 0x3528, 0xf0b1, 0x234c, 0xe6d5, 0xede7, 0x287e, 
  0xf93d, 0x3ca4, 0x3796, 0xf20f, 0x21f2, 0xe46b, 0xef59, 0x2ac0, 
  0x0d3a, 0xc8a3, 0xc391, 0x0608, 0xd5f5, 0x106c, 0x1b5e, 0xdec7, 
  0x54aa, 0x9133, 0x9a01, 0x5f98, 0x8c65, 0x49fc, 0x42ce, 0x8757, 
  0xa0ad, 0x6534, 0x6e06, 0xab9f, 0x7862, 0xbdfb, 0xb6c9, 0x7350, 
  0x51d6, 0x944f, 0x9f7d, 0x5ae4, 0x8919, 0x4c80, 0x47b2, 0x822b, 
  0xa5d1, 0x6048, 0x6b7a, 0xaee3, 0x7d1e, 0xb887, 0xb3b5, 0x762c, 
  0xfc41, 0x39d8, 0x32ea, 0xf773, 0x248e, 0xe117, 0xea25, 0x2fbc, 
  0x0846, 0xcddf, 0xc6ed, 0x0374, 0xd089, 0x1510, 0x1e22, 0xdbbb, 
  0x0af8, 0xcf61, 0xc453, 0x01ca, 0xd237, 0x17ae, 0x1c9c, 0xd905, 
  0xfeff, 0x3b66, 0x3054, 0xf5cd, 0x2630, 0xe3a9, 0xe89b, 0x2d02, 
  0xa76f, 0x62f6, 0x69c4, 0xac5d, 0x7fa0, 0xba39, 0xb10b, 0x7492, 
  0x5368, 0x96f1, 0x9dc3, 0x585a, 0x8ba7, 0x4e3e, 0x450c, 0x8095
};

/* Pre-computed CRC10 Table */
static const uint16_t  Adbms6815_Crc10Table[256] =
{
  0x000, 0x08f, 0x11e, 0x191, 0x23c, 0x2b3, 0x322, 0x3ad, 0x0f7, 0x078, 0x1e9, 0x166, 0x2cb, 0x244, 0x3d5, 0x35a,
  0x1ee, 0x161, 0x0f0, 0x07f, 0x3d2, 0x35d, 0x2cc, 0x243, 0x119, 0x196, 0x007, 0x088, 0x325, 0x3aa, 0x23b, 0x2b4,
  0x3dc, 0x353, 0x2c2, 0x24d, 0x1e0, 0x16f, 0x0fe, 0x071, 0x32b, 0x3a4, 0x235, 0x2ba, 0x117, 0x198, 0x009, 0x086,
  0x232, 0x2bd, 0x32c, 0x3a3, 0x00e, 0x081, 0x110, 0x19f, 0x2c5, 0x24a, 0x3db, 0x354, 0x0f9, 0x076, 0x1e7, 0x168,
  0x337, 0x3b8, 0x229, 0x2a6, 0x10b, 0x184, 0x015, 0x09a, 0x3c0, 0x34f, 0x2de, 0x251, 0x1fc, 0x173, 0x0e2, 0x06d,
  0x2d9, 0x256, 0x3c7, 0x348, 0x0e5, 0x06a, 0x1fb, 0x174, 0x22e, 0x2a1, 0x330, 0x3bf, 0x012, 0x09d, 0x10c, 0x183,
  0x0eb, 0x064, 0x1f5, 0x17a, 0x2d7, 0x258, 0x3c9, 0x346, 0x01c, 0x093, 0x102, 0x18d, 0x220, 0x2af, 0x33e, 0x3b1,
  0x105, 0x18a, 0x01b, 0x094, 0x339, 0x3b6, 0x227, 0x2a8, 0x1f2, 0x17d, 0x0ec, 0x063, 0x3ce, 0x341, 0x2d0, 0x25f,
  0x2e1, 0x26e, 0x3ff, 0x370, 0x0dd, 0x052, 0x1c3, 0x14c, 0x216, 0x299, 0x308, 0x387, 0x02a, 0x0a5, 0x134, 0x1bb,
  0x30f, 0x380, 0x211, 0x29e, 0x133, 0x1bc, 0x02d, 0x0a2, 0x3f8, 0x377, 0x2e6, 0x269, 0x1c4, 0x14b, 0x0da, 0x055,
  0x13d, 0x1b2, 0x023, 0x0ac, 0x301, 0x38e, 0x21f, 0x290, 0x1ca, 0x145, 0x0d4, 0x05b, 0x3f6, 0x379, 0x2e8, 0x267,
  0x0d3, 0x05c, 0x1cd, 0x142, 0x2ef, 0x260, 0x3f1, 0x37e, 0x024, 0x0ab, 0x13a, 0x1b5, 0x218, 0x297, 0x306, 0x389,
  0x1d6, 0x159, 0x0c8, 0x047, 0x3ea, 0x365, 0x2f4, 0x27b, 0x121, 0x1ae, 0x03f, 0x0b0, 0x31d, 0x392, 0x203, 0x28c,
  0x038, 0x0b7, 0x126, 0x1a9, 0x204, 0x28b, 0x31a, 0x395, 0x0cf, 0x040, 0x1d1, 0x15e, 0x2f3, 0x27c, 0x3ed, 0x362,
  0x20a, 0x285, 0x314, 0x39b, 0x036, 0x0b9, 0x128, 0x1a7, 0x2fd, 0x272, 0x3e3, 0x36c, 0x0c1, 0x04e, 0x1df, 0x150,
  0x3e4, 0x36b, 0x2fa, 0x275, 0x1d8, 0x157, 0x0c6, 0x049, 0x313, 0x39c, 0x20d, 0x282, 0x12f, 0x1a0, 0x031, 0x0be
};

/*Command List - ADBMS6815*/
/* Configation registers commands */
uint8_t WRCFGA[2]        = { 0x00, 0x01 };
uint8_t WRCFGB[2]        = { 0x00, 0x24 };
uint8_t RDCFGA[2]        = { 0x00, 0x02 };
uint8_t RDCFGB[2]        = { 0x00, 0x26 };

/* Read cell voltage result registers commands */
uint8_t RDCVA[2]         = { 0x00, 0x04 };
uint8_t RDCVB[2]         = { 0x00, 0x06 };
uint8_t RDCVC[2]         = { 0x00, 0x08 };
uint8_t RDCVD[2]         = { 0x00, 0x0A };
uint8_t RDCVALL[2]       = { 0x00, 0x38 };

/* Read CD voltage result registers commands */
uint8_t RDCDA[2]         = { 0x00, 0x30 };
uint8_t RDCDB[2]         = { 0x00, 0x31 };
uint8_t RDCDC[2]         = { 0x00, 0x32 };
uint8_t RDCDD[2]         = { 0x00, 0x33 };
uint8_t RDCDALL[2]       = { 0x00, 0x3A };

/* Read aux results */
uint8_t RDAUXA[2]        = { 0x00, 0x0C };
uint8_t RDAUXB[2]        = { 0x00, 0x0E };
uint8_t RDAUXC[2]        = { 0x00, 0x0D };

/* Read status registers */
uint8_t RDSTATA[2]       = { 0x00, 0x10 };
uint8_t RDSTATB[2]       = { 0x00, 0x12 };
uint8_t RDSTATC[2]       = { 0x00, 0x13 };
uint8_t RDASALL[2]       = { 0x00, 0x3C };

/* Pwm registers commands */
uint8_t WRPWM[2]        = { 0x00, 0x20 };
uint8_t RDPWM[2]        = { 0x00, 0x22 };

/* Clear commands */
uint8_t CLRCELL[2]       = { 0x07, 0x11 };
uint8_t CLRAUX [2]       = { 0x07, 0x12 };
uint8_t CLRSTAT[2]       = { 0x07, 0x13 };
uint8_t CLRCD[2]         = { 0x07, 0x16 };
uint8_t CLRFLAG[8]       = { 0x07, 0x17 };

/* Poll adc command */
uint8_t PLADC[2]         = { 0x07, 0x14 };

/* Diagn command */
uint8_t DIAGN[2]         = {0x07 , 0x15};

/* GPIOs Comm commands */
uint8_t WRCOMM[2]        = { 0x07, 0x21 };
uint8_t RDCOMM[2]        = { 0x07, 0x22 };
uint8_t STCOMM[13]       = { 0x07, 0x23, 0xB9, 0xE4 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00};

/* Mute and Unmute commands */
uint8_t MUTE[2]          = { 0x00, 0x28 };
uint8_t UNMUTE[2]        = { 0x00, 0x29 };

uint8_t RSTCC[2]         = { 0x00, 0x2E };
uint8_t SRST[2]          = { 0x00, 0x1F };

/* Read SID command */
uint8_t RDSID[2]         = { 0x00, 0x2C };

/* Private variables ---------------------------------------------------------*/
/* ADC Command Configurations */
ADCOPT    ADC_OPT               = ADCOPT_OFF;
ADC_MODE  ADC_CONVERSION_MODE   = MODE_7KHz_TO_3KHz;
DCC_BIT   ADC_DCP               = DCC_BIT_CLR;
CH        CELL_CH_TO_CONVERT    = ALL_CELLS;
CHG       AUX_CH_TO_CONVERT     = GPIOS_VREF2_VREF3;
CHST      STAT_CH_TO_CONVERT    = SC_ITMP_VA_VD;

uint32_t pladc_count;

/* Private function prototypes -----------------------------------------------*/
bmsStatus_t adBmsCsLow(uint8_t spiID);
bmsStatus_t adBmsCsHigh(uint8_t spiID);
bmsStatus_t adBmsWakeupIc(uint8_t total_ic);

uint16_t Pec15_Calc(uint8_t len, uint8_t *data);                                   
uint16_t adBMS6815Pec10Calculate(uint8_t *pDataBuf, bool bIsRxCmd, uint8_t nLength);

uint32_t adBmsPollAdc(uint8_t tx_cmd[2]);


bmsStatus_t adBms6815ParseConfiga(uint8_t tIC, cell_asic *ic, uint8_t *data);
bmsStatus_t adBms6815ParseConfigb(uint8_t tIC, cell_asic *ic, uint8_t *data);
bmsStatus_t adBms6815ParseConfig(uint8_t tIC, cell_asic *ic, GRP grp, uint8_t *data);
bmsStatus_t adBms6815ParseCell(uint8_t tIC, cell_asic *ic, GRP grp, uint8_t *cv_data);
bmsStatus_t adBms6815ParseDCell(uint8_t tIC, cell_asic *ic, GRP grp, uint8_t *fcv_data);
bmsStatus_t adBms6815ParseAux(uint8_t tIC, cell_asic *ic, GRP grp, uint8_t *aux_data);
bmsStatus_t adBms6815ParseStatusA(uint8_t tIC, cell_asic *ic, uint8_t *data);
bmsStatus_t adBms6815ParseStatusB(uint8_t tIC, cell_asic *ic, uint8_t *data);
bmsStatus_t adBms6815ParseStatusC(uint8_t tIC, cell_asic *ic, uint8_t *data);
bmsStatus_t adBms6815ParseStatus(uint8_t tIC, cell_asic *ic, GRP grp, uint8_t *data);


bmsStatus_t spiSendCmd(uint8_t tx_cmd[2]);
bmsStatus_t spiReadData(uint8_t tIC, uint8_t tx_cmd[2], uint8_t *rx_data, uint8_t *pec_error, uint8_t *cmd_cntr, uint8_t regData_size);
bmsStatus_t spiWriteData(uint8_t tIC, uint8_t tx_cmd[2], uint8_t *data, uint8_t data_size);
bmsStatus_t adBmsReadData(uint8_t tIC, cell_asic *ic, uint8_t cmd_arg[2], TYPE type, GRP group);
bmsStatus_t adBmsWriteData(uint8_t tIC, cell_asic *ic, uint8_t cmd_arg[2], TYPE type, GRP group);

bmsStatus_t adBms6815_Adcv(ADC_MODE MD, DCC_BIT DCP, CH CHCELL);
bmsStatus_t adBms6815_Adsc(ADC_MODE MD, DCC_BIT DCP, CH CHCELL);
bmsStatus_t adBms6815_Adleak(ADC_MODE MD, DCC_BIT DCP, CH CHCELL);
bmsStatus_t adBmss6815_Adax(ADC_MODE MD, CHG CHGPIO);
bmsStatus_t adBms6815_Axow(ADC_MODE MD, PUP_BIT PUP, CHG CHGPIO);
bmsStatus_t adBms6815_Adstat(ADC_MODE MD, CHST CHSTAT);
bmsStatus_t adBms6815_Adcvax(ADC_MODE MD);
bmsStatus_t adBms6815_Adcvsc(ADC_MODE MD);
bmsStatus_t adBms6815_Adol(ADC_MODE MD);
bmsStatus_t adBms6815_Cvow(ADC_MODE MD);
bmsStatus_t adBms6815_Cvpg(ADC_MODE MD, MEM_PG PG);
bmsStatus_t adBms6815_Cdpg(ADC_MODE MD, MEM_PG PG);
bmsStatus_t adBms6815_Axpg(ADC_MODE MD, MEM_PG PG);
bmsStatus_t adBms6815_Statpg(ADC_MODE MD, MEM_PG PG);
bmsStatus_t adBms6815_Adow(ADC_MODE MD, PUP_BIT PUP);

uint16_t SetOverVoltageThreshold(float voltage);
uint16_t SetUnderVoltageThreshold(float voltage);

bmsStatus_t SetPwmDutyCycle(uint8_t tIC, cell_asic *ic, PWM_DUTY duty_cycle);
bmsStatus_t adBms6815ParsePwm(uint8_t tIC, cell_asic *ic, uint8_t *data);
bmsStatus_t adBms6815CreatePwm(uint8_t tIC, cell_asic *ic);

/* Private custom code -------------------------------------------------------*/

/* Function definition -------------------------------------------------------*/
/**
 * @brief adBms Cs Low
 * 
 * @param spiID spi ID
 * @retval BMS status
 */
bmsStatus_t adBmsCsLow(uint8_t spiID)
{
  bmsStatus_t status = BMS_ERROR;

	if(!(IS_SPI_ID(spiID)))
	{
		status = BMS_CMU_FUNCTION_PARAMETER_ERROR;
		return status;
	}

  switch(spiID)
  {
    case cmuSpi:
    case ccmSpi:
      status = resetSPISlaveSelect(spiID);
      break;
    default:
      status = BMS_ERROR;
      break;
  }
  
  return status;
}

/* Function definition -------------------------------------------------------*/
/**
 * @brief adBms Cs High
 * 
 * @param spiID spi ID
 * @retval BMS status
 */
bmsStatus_t adBmsCsHigh(uint8_t spiID)
{
  bmsStatus_t status = BMS_ERROR;

  if(!(IS_SPI_ID(spiID)))
	{
		status = BMS_CMU_FUNCTION_PARAMETER_ERROR;
		return status;
	}

  switch(spiID)
  {
    case cmuSpi:
    case ccmSpi:
      status = setSPISlaveSelect(spiID);
      break;
    default:
      status = BMS_ERROR;
      break;
  }
  return status;
}

/* Function definition -------------------------------------------------------*/
/**
 * @brief wakes up adbms6815 IC
 * 
 * @param t_ic total number of adbms6815 ICs
 * @retval BMS status
 */
bmsStatus_t adBmsWakeupIc(uint8_t t_ic)
{
  bmsStatus_t status = BMS_ERROR;

	if((t_ic <= 0))
	{
		status = BMS_CMU_FUNCTION_PARAMETER_ERROR;
		return status;
	}

  for (uint8_t ic = 0; ic < t_ic; ic++)
  {
    status = adBmsCsLow(cmuSpi);
    delayMilliSeconds(4);
    status = adBmsCsHigh(cmuSpi);  
    delayMilliSeconds(4); 
  }

  return status;
}

/* Function definition -------------------------------------------------------*/
/**
 * @brief calculate pec15
 * 
 * @param len length of the data
 * @param data pointer to data buffer
 * @retval uint16_t calculated pec15
 */
uint16_t Pec15_Calc(uint8_t len, uint8_t *data)
{
  if((len <= 0)   ||\
    (NULL == data))
	{
		return BMS_CMU_FUNCTION_PARAMETER_ERROR;
	}

  uint16_t remainder = 0;
  uint16_t addr  = 0;
  remainder = 16; /* initialize the PEC */

  for (uint8_t i = 0; i<len; i++) /* loops for each byte in data array */
  {
    addr = (((remainder>>7)^data[i])&0xff);/* calculate PEC table address */
    remainder = ((remainder<<8)^Crc15Table[addr]);
  }
  return(remainder*2);/* The CRC15 has a 0 in the LSB so the remainder must be multiplied by 2 */
}

/* Function definition -------------------------------------------------------*/
/**
 * @brief calculate pec 10
 * 
 * @param pDataBuf pointer to data buffer
 * @param bIsRxCmd received command
 * @param nLength length of the data
 * @retval uint16_t calculated pec10
 */
uint16_t adBMS6815Pec10Calculate(uint8_t *pDataBuf, bool bIsRxCmd, uint8_t nLength)
{
  if((NULL == pDataBuf)         ||\
    !(IS_CMU_COMMAND(bIsRxCmd)) ||\
    (nLength <= 0) )
	{
		return BMS_CMU_FUNCTION_PARAMETER_ERROR;
	}

  uint16_t   nRemainder = 16u;/* PEC_SEED */
  /* x10 + x7 + x3 + x2 + x + 1 <- the CRC10 polynomial 100 1000 1111 */
  uint16_t   nPolynomial = 0x8Fu;
  uint8_t   nByteIndex, nBitIndex;
  uint16_t   nTableAddr;

  for (nByteIndex = 0u; nByteIndex < nLength; ++nByteIndex)
  {
    /* calculate PEC table address */
    nTableAddr = (uint16_t)(((uint16_t)(nRemainder >> 2) ^ (uint8_t)pDataBuf[nByteIndex]) & (uint8_t)0xff);
    nRemainder = (uint16_t)(((uint16_t)(nRemainder << 8)) ^ Adbms6815_Crc10Table[nTableAddr]);
  }

  /* If array is from received buffer add command counter to crc calculation */
  if (bIsRxCmd == true)
  {
    nRemainder ^= (uint16_t)(((uint16_t)pDataBuf[nLength] & (uint8_t)0xFC) << 2u);
  }

  /* Perform modulo-2 division, a bit at a time */

  for (nBitIndex = 6u; nBitIndex > 0u; --nBitIndex)
  {
    /* Try to divide the current data bit */
    if ((nRemainder & 0x200u) > 0u)
    {
      nRemainder = (uint16_t)((nRemainder << 1u));
      nRemainder = (uint16_t)(nRemainder ^ nPolynomial);
    }
    else
    {
      nRemainder = (uint16_t)((nRemainder << 1u));
    }
  }

  return ((uint16_t)(nRemainder & 0x3FFu));
}

/* Function definition -------------------------------------------------------*/
/**
 * @brief adBms Poll Adc
 * 
 * @param tx_cmd transmit command
 * @return uint32_t adc count
 */
uint32_t adBmsPollAdc(uint8_t tx_cmd[2])
{
  if((NULL == tx_cmd))
	{
		return BMS_CMU_FUNCTION_PARAMETER_ERROR;
	}

  uint32_t pladc_count = 0;
  uint8_t cmd[4];
  uint16_t cmd_pec;
  uint8_t read_data = 0x00;
  uint8_t SDO_Line = 0xFF;
  cmd[0] = tx_cmd[0];
  cmd[1] = tx_cmd[1];
  cmd_pec = Pec15_Calc(2, cmd);
  cmd[2] = (uint8_t)(cmd_pec >> 8);
  cmd[3] = (uint8_t)(cmd_pec);
  startTimer(cmuTimer);
  adBmsCsLow(ccmSpi);
  spiWriteBytes(4, &cmd[0]);
  do{
    spiReadBytes(1, &read_data);
  }while(!(read_data == SDO_Line));
  adBmsCsHigh(ccmSpi);
  pladc_count = getTimCount();
  stopTimer(cmuTimer);
  return(pladc_count);    
}

/* Function definition -------------------------------------------------------*/
/**
 * @brief adBms6815 Create Config for register a
 * 
 * @param tIC total number of ICs
 * @param ic structure to the IC handle
 * @retval BMS status
 */
bmsStatus_t adBms6815CreateConfiga(uint8_t tIC, cell_asic *ic)
{
  bmsStatus_t status = BMS_ERROR;

  if((tIC <= 0)   ||\
    (NULL == ic))
	{
		return BMS_CMU_FUNCTION_PARAMETER_ERROR;
	}

  for(uint8_t curr_ic = 0; curr_ic < tIC; curr_ic++)
  {
    ic[curr_ic].configa.tx_data[0] = (((ic[curr_ic].tx_cfga.refon & 0x01) << 7) | (ic[curr_ic].tx_cfga.adcopt << 6) | (ic[curr_ic].tx_cfga.ps << 4) | (ic[curr_ic].tx_cfga.cvmin << 2) | (ic[curr_ic].tx_cfga.mcal << 1) | ic[curr_ic].tx_cfga.comm_bk);
    ic[curr_ic].configa.tx_data[1] = (ic[curr_ic].tx_cfga.flag_d & 0xFF);
    ic[curr_ic].configa.tx_data[2] = (((ic[curr_ic].tx_cfga.soakon & 0x01) << 7) | ((ic[curr_ic].tx_cfga.owrng & 0x1) << 6) | ((ic[curr_ic].tx_cfga.owa & 0x07) << 3) | (ic[curr_ic].tx_cfga.owc & 0x07));
    ic[curr_ic].configa.tx_data[3] = ((ic[curr_ic].tx_cfga.gpo & 0xFF));
    ic[curr_ic].configa.tx_data[4] = ((ic[curr_ic].tx_cfga.gpi & 0xFF));
    ic[curr_ic].configa.tx_data[5] = ((ic[curr_ic].tx_cfga.rev & 0x0F) << 4);
  }
   
  status = BMS_OK;
  return status;
}

/* Function definition -------------------------------------------------------*/
/**
 * @brief adBms6815 Create Config for register b
 * 
 * @param tIC total number of ICs
 * @param ic structure to the IC handle
 * @retval BMS status
 */
bmsStatus_t adBms6815CreateConfigb(uint8_t tIC, cell_asic *ic)
{
  bmsStatus_t status = BMS_ERROR;

  if((tIC <= 0)   ||\
    (NULL == ic))
	{
		return BMS_CMU_FUNCTION_PARAMETER_ERROR;
	}

  for(uint8_t curr_ic = 0; curr_ic < tIC; curr_ic++)
  {
    ic[curr_ic].configb.tx_data[0] = ((ic[curr_ic].tx_cfgb.vuv ));
    ic[curr_ic].configb.tx_data[1] = (((ic[curr_ic].tx_cfgb.vov & 0x00F) << 4) | ((ic[curr_ic].tx_cfgb.vuv ) >> 8));
    ic[curr_ic].configb.tx_data[2] = ((ic[curr_ic].tx_cfgb.vov >>4)&0x0FF);
    ic[curr_ic].configb.tx_data[3] = (((ic[curr_ic].tx_cfgb.dtmen & 0x01) << 7) | ((ic[curr_ic].tx_cfgb.dtrng & 0x01) << 6) | (ic[curr_ic].tx_cfgb.dcto & 0x3F));
    ic[curr_ic].configb.tx_data[4] = ((ic[curr_ic].tx_cfgb.dcc & 0xFF));
    ic[curr_ic].configb.tx_data[5] = (((ic[curr_ic].tx_cfgb.mute_st & 0x01) << 7) | (ic[curr_ic].tx_cfgb.dcc >>8 ));
  }

  status = BMS_OK;
  return status;
}

/* Function definition -------------------------------------------------------*/
/**
 * @brief adBms6815 Create Clr Flag Data
 * 
 * @param tIC total number of ICs
 * @param ic structure to the IC handle
 * @retval BMS status
 */
bmsStatus_t adBms6815CreateClrFlagData(uint8_t tIC, cell_asic *ic)
{
  bmsStatus_t status = BMS_ERROR;

  if((tIC <= 0)   ||\
    (NULL == ic))
	{
		return BMS_CMU_FUNCTION_PARAMETER_ERROR;
	}

  for(uint8_t curr_ic = 0; curr_ic < tIC; curr_ic++)
  {
    ic[curr_ic].flag_tx.data[0] = (((ic[curr_ic].clrflg_data.cl_vaov << 7) |(ic[curr_ic].clrflg_data.cl_vauv << 6) |(ic[curr_ic].clrflg_data.cl_vdov << 5) | (ic[curr_ic].clrflg_data.cl_vduv << 4)
                                    | (ic[curr_ic].clrflg_data.cl_aed << 3) | (ic[curr_ic].clrflg_data.cl_amed << 2) | (ic[curr_ic].clrflg_data.cl_ed << 1) | ic[curr_ic].clrflg_data.cl_med));
    ic[curr_ic].flag_tx.data[1] = (((ic[curr_ic].clrflg_data.cl_redf << 7) |(ic[curr_ic].clrflg_data.cl_comp << 6) |(ic[curr_ic].clrflg_data.cl_sleep << 5) | (ic[curr_ic].clrflg_data.cl_tmode << 4)
                                    |(ic[curr_ic].clrflg_data.cl_thsd << 2) | (ic[curr_ic].clrflg_data.cl_cpchk << 1) | ic[curr_ic].clrflg_data.cl_oscchk));
  }
  status = BMS_OK;
  return status;
}

/* Function definition -------------------------------------------------------*/
/**
 * @brief adBms6815 Parse Config for register a
 * 
 * @param tIC total number of ICs
 * @param ic structure to the IC handle
 * @param data pointer to data buffer
 * @retval BMS status
 */
bmsStatus_t adBms6815ParseConfiga(uint8_t tIC, cell_asic *ic, uint8_t *data)
{
  uint8_t address = 0;
  bmsStatus_t status = BMS_ERROR;

  if((tIC <= 0)   ||\
    (NULL == ic)  ||\
    (NULL == data))
	{
		return BMS_CMU_FUNCTION_PARAMETER_ERROR;
	}

  for(uint8_t curr_ic = 0; curr_ic < tIC; curr_ic++)
  {
    memcpy(&ic[curr_ic].configa.rx_data[0], &data[address], RX_DATA); /* dst , src , size */
    address = ((curr_ic+1) * (RX_DATA));

    ic[curr_ic].rx_cfga.comm_bk = (ic[curr_ic].configa.rx_data[0] & 0x01);
    ic[curr_ic].rx_cfga.mcal    = (ic[curr_ic].configa.rx_data[0] & 0x02) >> 1;
    ic[curr_ic].rx_cfga.cvmin   = (ic[curr_ic].configa.rx_data[0] & 0x0C) >> 2;
    ic[curr_ic].rx_cfga.ps      = (ic[curr_ic].configa.rx_data[0] & 0x30) >> 4;
    ic[curr_ic].rx_cfga.adcopt  = (ic[curr_ic].configa.rx_data[0] & 0x40) >> 6;
    ic[curr_ic].rx_cfga.refon   = (ic[curr_ic].configa.rx_data[0] & 0x80) >> 7;

    ic[curr_ic].rx_cfga.flag_d  = (ic[curr_ic].configa.rx_data[1] & 0xFF);

    ic[curr_ic].rx_cfga.soakon   = (ic[curr_ic].configa.rx_data[2] & 0x80) >> 7;
    ic[curr_ic].rx_cfga.owrng    = (((ic[curr_ic].configa.rx_data[2] & 0x40) >> 6));
    ic[curr_ic].rx_cfga.owa      = ((ic[curr_ic].configa.rx_data[2] & 0x38) >> 3);
    ic[curr_ic].rx_cfga.owc      = ((ic[curr_ic].configa.rx_data[2] & 0x07));

    ic[curr_ic].rx_cfga.gpo        = ((ic[curr_ic].configa.rx_data[3] & 0x7F));
    ic[curr_ic].rx_cfga.gpi        = ((ic[curr_ic].configa.rx_data[4] & 0x7F));

    ic[curr_ic].rx_cfga.rev   = ((ic[curr_ic].configa.rx_data[5] & 0xF0) >> 4);
  }
  
  status = BMS_OK;
  return status;
}

/* Function definition -------------------------------------------------------*/
/**
 * @brief adBms6815 Parse Config for register b
 * 
 * @param tIC total number of ICs
 * @param ic structure to the IC handle
 * @param data pointer to data buffer
 * @retval BMS status
 */
bmsStatus_t adBms6815ParseConfigb(uint8_t tIC, cell_asic *ic, uint8_t *data)
{
  uint8_t address = 0;
  bmsStatus_t status = BMS_ERROR;

  if((tIC <= 0)   ||\
    (NULL == ic)  ||\
    (NULL == data))
	{
		return BMS_CMU_FUNCTION_PARAMETER_ERROR;
	}

  for(uint8_t curr_ic = 0; curr_ic < tIC; curr_ic++)
  {
    memcpy(&ic[curr_ic].configb.rx_data[0], &data[address], RX_DATA); /* dst , src , size */
    address = ((curr_ic+1) * (RX_DATA));

    ic[curr_ic].rx_cfgb.vuv     = ((ic[curr_ic].configb.rx_data[0])  | ((ic[curr_ic].configb.rx_data[1] & 0x0F) << 8));
    ic[curr_ic].rx_cfgb.vov     = (ic[curr_ic].configb.rx_data[2]<<4) | ((ic[curr_ic].configb.rx_data[1] &0xF0)>>4)  ;
    ic[curr_ic].rx_cfgb.dtmen   = (((ic[curr_ic].configb.rx_data[3] & 0x80) >> 7));
    ic[curr_ic].rx_cfgb.dtrng   = ((ic[curr_ic].configb.rx_data[3] & 0x40) >> 6);
    ic[curr_ic].rx_cfgb.dcto    = ((ic[curr_ic].configb.rx_data[3] & 0x3F));
    ic[curr_ic].rx_cfgb.dcc     = ((ic[curr_ic].configb.rx_data[4]) | ((ic[curr_ic].configb.rx_data[5] & 0x0F) << 8));
    ic[curr_ic].rx_cfgb.mute_st = ((ic[curr_ic].configb.rx_data[5] & 0x80) >> 7);
  }

  status = BMS_OK;
  return status;
}

/* Function definition -------------------------------------------------------*/
/**
 * @brief adBms6815 Parse Config
 * 
 * @param grp Register Group
 * @param tIC total number of ICs
 * @param ic structure to the IC handle
 * @param data pointer to data buffer
 * @retval BMS status
 */
bmsStatus_t adBms6815ParseConfig(uint8_t tIC, cell_asic *ic, GRP grp, uint8_t *data)
{
  bmsStatus_t status = BMS_ERROR;

  if((tIC <= 0)         ||\
    (NULL == ic)        ||\
   !(IS_CMU_GRP(grp))   ||\
    (NULL == data))
	{
		return BMS_CMU_FUNCTION_PARAMETER_ERROR;
	}

  switch (grp)
  {
    case A:
      status = adBms6815ParseConfiga(tIC, &ic[0], &data[0]);
      break;

    case B:
      status = adBms6815ParseConfigb(tIC, &ic[0], &data[0]);
      break;

    default:
     status = BMS_ERROR;
      break;
  }

  return status;
}

/* Function definition -------------------------------------------------------*/
/**
 * @brief adBms6815 Parse Cell
 * 
 * @param tIC total number of ICs
 * @param ic structure to the IC handle
 * @param grp Register Group
 * @param cv_data pointer to data buffer
 * @retval BMS status
 */
bmsStatus_t adBms6815ParseCell(uint8_t tIC, cell_asic *ic, GRP grp, uint8_t *cv_data)
{
  uint8_t *data; 
  uint8_t data_size = 0; 
  uint8_t address = 0;
  bmsStatus_t status = BMS_ERROR;

  if((tIC <= 0)         ||\
    (NULL == ic)        ||\
   !(IS_CMU_GRP(grp))   ||\
    (NULL == cv_data))
	{
		return BMS_CMU_FUNCTION_PARAMETER_ERROR;
	}


  if(grp == ALL_GRP){data_size = RDCVALL_SIZE;}
  else {data_size = RX_DATA;}
  data = (uint8_t *)calloc(data_size, sizeof(uint8_t));
  if(data == NULL)
  {
    status = BMS_DYNAMIC_MEM_ALLOC_ERROR;
    return status;
    exit(0);
  }

  for(uint8_t curr_ic = 0; curr_ic < tIC; curr_ic++)
  {
    memcpy(&data[0], &cv_data[address], data_size); /* dst , src , size */
    address = ((curr_ic+1) * (data_size));
    switch (grp)
    {
    case A: /* Cell Register group A */
      ic[curr_ic].cell.c_codes[0] = (data[0] + (data[1] << 8));
      ic[curr_ic].cell.c_codes[1] = (data[2] + (data[3] << 8));
      ic[curr_ic].cell.c_codes[2] = (data[4] + (data[5] << 8));
      status = BMS_OK;
      break;

    case B: /* Cell Register group B */
      ic[curr_ic].cell.c_codes[3] = (data[0] + (data[1] << 8));
      ic[curr_ic].cell.c_codes[4] = (data[2] + (data[3] << 8));
      ic[curr_ic].cell.c_codes[5] = (data[4] + (data[5] << 8));
      status = BMS_OK;
      break;

    case C: /* Cell Register group C */
      ic[curr_ic].cell.c_codes[6] = (data[0] + (data[1] << 8));
      ic[curr_ic].cell.c_codes[7] = (data[2] + (data[3] << 8));
      ic[curr_ic].cell.c_codes[8] = (data[4] + (data[5] << 8));
      status = BMS_OK;
      break;

    case D: /* Cell Register group D */
      ic[curr_ic].cell.c_codes[9] =  (data[0] + (data[1] << 8));
      ic[curr_ic].cell.c_codes[10] = (data[2] + (data[3] << 8));
      ic[curr_ic].cell.c_codes[11] = (data[4] + (data[5] << 8));
      status = BMS_OK;
      break;

    case ALL_GRP: /* Cell Register group ALL */
      ic[curr_ic].cell.c_codes[0] = (data[0] + (data[1] << 8));
      ic[curr_ic].cell.c_codes[1] = (data[2] + (data[3] << 8));
      ic[curr_ic].cell.c_codes[2] = (data[4] + (data[5] << 8));
      ic[curr_ic].cell.c_codes[3] = (data[6] + (data[7] << 8));
      ic[curr_ic].cell.c_codes[4] = (data[8] + (data[9] << 8));
      ic[curr_ic].cell.c_codes[5] = (data[10] + (data[11] << 8));
      ic[curr_ic].cell.c_codes[6] = (data[12] + (data[13] << 8));
      ic[curr_ic].cell.c_codes[7] = (data[14] + (data[15] << 8));
      ic[curr_ic].cell.c_codes[8] = (data[16] + (data[17] << 8));
      ic[curr_ic].cell.c_codes[9] =  (data[18] + (data[19] << 8));
      ic[curr_ic].cell.c_codes[10] = (data[20] + (data[21] << 8));
      ic[curr_ic].cell.c_codes[11] = (data[22] + (data[23] << 8));
      status = BMS_OK;
      break;

    default:
      status = BMS_ERROR;
      break;
    }
  }
  free(data);
  return status;
}

/* Function definition -------------------------------------------------------*/
/**
 * @brief adBms6815 Parse DCell
 * 
 * @param tIC total number of ICs
 * @param ic structure to the IC handle
 * @param grp Register Group
 * @param dcv_data pointer to data buffer
 * @retval BMS status
 */
bmsStatus_t adBms6815ParseDCell(uint8_t tIC, cell_asic *ic, GRP grp, uint8_t *dcv_data)
{
  uint8_t *data; 
  uint8_t data_size = 0; 
  uint8_t address = 0;
  bmsStatus_t status = BMS_ERROR;

  if((tIC <= 0)         ||\
    (NULL == ic)        ||\
   !(IS_CMU_GRP(grp))   ||\
    (NULL == dcv_data))
	{
		return BMS_CMU_FUNCTION_PARAMETER_ERROR;
	}

  if(grp == ALL_GRP){data_size = RDCDALL_SIZE;}
  else {data_size = RX_DATA;}
  data = (uint8_t *)calloc(data_size, sizeof(uint8_t));
  if(data == NULL)
  {
    status = BMS_DYNAMIC_MEM_ALLOC_ERROR;
    return status;
    exit(0);
  }
  for(uint8_t curr_ic = 0; curr_ic < tIC; curr_ic++)
  {
    memcpy(&data[0], &dcv_data[address], data_size); /* dst , src , size */
    address = ((curr_ic+1) * (data_size));
    switch (grp)
    {
    case A: /* Cell Register group A */
      ic[curr_ic].dcell.dc_codes[0] = (data[0] + (data[1] << 8));
      ic[curr_ic].dcell.dc_codes[1] = (data[2] + (data[3] << 8));
      ic[curr_ic].dcell.dc_codes[2] = (data[4] + (data[5] << 8));
      status = BMS_OK;
      break;

    case B: /* Cell Register group B */
      ic[curr_ic].dcell.dc_codes[3] = (data[0] + (data[1] << 8));
      ic[curr_ic].dcell.dc_codes[4] = (data[2] + (data[3] << 8));
      ic[curr_ic].dcell.dc_codes[5] = (data[4] + (data[5] << 8));
      status = BMS_OK;
      break;

    case C: /* Cell Register group C */
      ic[curr_ic].dcell.dc_codes[6] = (data[0] + (data[1] << 8));
      ic[curr_ic].dcell.dc_codes[7] = (data[2] + (data[3] << 8));
      ic[curr_ic].dcell.dc_codes[8] = (data[4] + (data[5] << 8));
      status = BMS_OK;
      break;

    case D: /* Cell Register group D */
      ic[curr_ic].dcell.dc_codes[9] =  (data[0] + (data[1] << 8));
      ic[curr_ic].dcell.dc_codes[10] = (data[2] + (data[3] << 8));
      ic[curr_ic].dcell.dc_codes[11] = (data[4] + (data[5] << 8));
      status = BMS_OK;
      break;

    case ALL_GRP: /* Cell Register group ALL */
      ic[curr_ic].dcell.dc_codes[0] = (data[0] + (data[1] << 8));
      ic[curr_ic].dcell.dc_codes[1] = (data[2] + (data[3] << 8));
      ic[curr_ic].dcell.dc_codes[2] = (data[4] + (data[5] << 8));
      ic[curr_ic].dcell.dc_codes[3] = (data[6] + (data[7] << 8));
      ic[curr_ic].dcell.dc_codes[4] = (data[8] + (data[9] << 8));
      ic[curr_ic].dcell.dc_codes[5] = (data[10] + (data[11] << 8));
      ic[curr_ic].dcell.dc_codes[6] = (data[12] + (data[13] << 8));
      ic[curr_ic].dcell.dc_codes[7] = (data[14] + (data[15] << 8));
      ic[curr_ic].dcell.dc_codes[8] = (data[16] + (data[17] << 8));
      ic[curr_ic].dcell.dc_codes[9] =  (data[18] + (data[19] << 8));
      ic[curr_ic].dcell.dc_codes[10] = (data[20] + (data[21] << 8));
      ic[curr_ic].dcell.dc_codes[11] = (data[22] + (data[23] << 8));
      status = BMS_OK;
      break;

    default:
      status = BMS_ERROR;
      break;
    }
  }
  free(data);

  return status;
}

/* Function definition -------------------------------------------------------*/
/**
 * @brief adBms6815 Parse Aux
 * 
 * @param tIC total number of ICs
 * @param ic structure to the IC handle
 * @param grp Register Group
 * @param aux_data pointer to data buffer
 * @retval BMS status
 */
bmsStatus_t adBms6815ParseAux(uint8_t tIC, cell_asic *ic, GRP grp, uint8_t *aux_data)
{
  uint8_t *data; 
  uint8_t data_size = 0; 
  uint8_t address = 0;
  bmsStatus_t status = BMS_ERROR;

  if((tIC <= 0)         ||\
    (NULL == ic)        ||\
   !(IS_CMU_GRP(grp))   ||\
    (NULL == aux_data))
	{
		return BMS_CMU_FUNCTION_PARAMETER_ERROR;
	}

  if(grp == ALL_GRP){data_size = (RDASALL_SIZE-18);}
  else {data_size = RX_DATA;}
  data = (uint8_t *)calloc(data_size, sizeof(uint8_t));
  if(data == NULL)
  {
    status = BMS_DYNAMIC_MEM_ALLOC_ERROR;
    return status;
    exit(0);
  }
  for(uint8_t curr_ic = 0; curr_ic < tIC; curr_ic++)
  {
    memcpy(&data[0], &aux_data[address], data_size); /* dst , src , size */
    address = ((curr_ic+1) * (data_size));
    switch (grp)
    {
    case A: /* Aux Register group A */
      ic[curr_ic].aux.a_codes[0] = (data[0] + (data[1] << 8));
      ic[curr_ic].aux.a_codes[1] = (data[2] + (data[3] << 8));
      ic[curr_ic].aux.a_codes[2] = (data[4] + (data[5] << 8));
      status = BMS_OK;
      break;

    case B: /* Aux Register group B */
      ic[curr_ic].aux.a_codes[3] = (data[0] + (data[1] << 8));
      ic[curr_ic].aux.a_codes[4] = (data[2] + (data[3] << 8));
      ic[curr_ic].aux.a_codes[5] = (data[4] + (data[5] << 8));
      status = BMS_OK;
      break;

    case C: /* Aux Register group C */
      ic[curr_ic].aux.a_codes[6] = (data[0] + (data[1] << 8));
      ic[curr_ic].aux.a_codes[7] = (data[2] + (data[3] << 8));
      ic[curr_ic].aux.a_codes[8] = (data[4] + (data[5] << 8));
      status = BMS_OK;
      break;

    case ALL_GRP: /* Aux Register group ALL */
      ic[curr_ic].aux.a_codes[0] = (data[0] + (data[1] << 8));
      ic[curr_ic].aux.a_codes[1] = (data[2] + (data[3] << 8));
      ic[curr_ic].aux.a_codes[2] = (data[4] + (data[5] << 8));
      ic[curr_ic].aux.a_codes[3] = (data[6] + (data[7] << 8));
      ic[curr_ic].aux.a_codes[4] = (data[8] + (data[9] << 8));
      ic[curr_ic].aux.a_codes[5] = (data[10] + (data[11] << 8));
      ic[curr_ic].aux.a_codes[6] = (data[12] + (data[13] << 8));
      ic[curr_ic].aux.a_codes[7] = (data[14] + (data[15] << 8));
      ic[curr_ic].aux.a_codes[8] = (data[16] + (data[17] << 8));
      status = BMS_OK;
      break;

    default:
      status = BMS_ERROR;
      break;
    }
  }
  free(data);
  return status;
}

/* Function definition -------------------------------------------------------*/
/**
 * @brief adBms6815 Parse StatusA
 * 
 * @param tIC total number of ICs
 * @param ic structure to the IC handle
 * @param data pointer to data buffer
 * @retval BMS status
 */
bmsStatus_t adBms6815ParseStatusA(uint8_t tIC, cell_asic *ic, uint8_t *data)
{
  uint8_t address = 0;
  bmsStatus_t status = BMS_ERROR;

  if((tIC <= 0)         ||\
    (NULL == ic)        ||\
    (NULL == data))
	{
		return BMS_CMU_FUNCTION_PARAMETER_ERROR;
	}

  for(uint8_t curr_ic = 0; curr_ic < tIC; curr_ic++)
  {
    memcpy(&ic[curr_ic].stat.rx_data[0], &data[address], RX_DATA); /* dst , src , size */
    address = ((curr_ic+1) * (RX_DATA));
    ic[curr_ic].stata.sc   = (ic[curr_ic].stat.rx_data[0] | (ic[curr_ic].stat.rx_data[1] << 8));
    ic[curr_ic].stata.itmp = (ic[curr_ic].stat.rx_data[2] | (ic[curr_ic].stat.rx_data[3] << 8));
    ic[curr_ic].stata.va   = (ic[curr_ic].stat.rx_data[4] | (ic[curr_ic].stat.rx_data[5] << 8));
  }

  status = BMS_OK;
  return status;
}

/* Function definition -------------------------------------------------------*/
/**
 * @brief adBms6815 Parse StatusB
 * 
 * @param tIC total number of ICs
 * @param ic structure to the IC handle
 * @param data pointer to data buffer
 * @retval BMS status
 */
bmsStatus_t adBms6815ParseStatusB(uint8_t tIC, cell_asic *ic, uint8_t *data)
{
  uint8_t address = 0;
  bmsStatus_t status = BMS_ERROR;

  if((tIC <= 0)         ||\
    (NULL == ic)        ||\
    (NULL == data))
	{
		return BMS_CMU_FUNCTION_PARAMETER_ERROR;
	}

  for(uint8_t curr_ic = 0; curr_ic < tIC; curr_ic++)
  {
    memcpy(&ic[curr_ic].stat.rx_data[0], &data[address], RX_DATA); /* dst , src , size */
    address = ((curr_ic+1) * (RX_DATA));

    ic[curr_ic].statb.vd   = (ic[curr_ic].stat.rx_data[0] + (ic[curr_ic].stat.rx_data[1] << 8));
    ic[curr_ic].statb.c1uv = (ic[curr_ic].stat.rx_data[2] & 0x01);
    ic[curr_ic].statb.c1ov = ((ic[curr_ic].stat.rx_data[2] & 0x02) >> 1);
    ic[curr_ic].statb.c2uv = ((ic[curr_ic].stat.rx_data[2] & 0x04) >> 2);
    ic[curr_ic].statb.c2ov = ((ic[curr_ic].stat.rx_data[2] & 0x08) >> 3);
    ic[curr_ic].statb.c3uv = ((ic[curr_ic].stat.rx_data[2] & 0x10) >> 4);
    ic[curr_ic].statb.c3ov = ((ic[curr_ic].stat.rx_data[2] & 0x20) >> 5);
    ic[curr_ic].statb.c4uv = ((ic[curr_ic].stat.rx_data[2] & 0x40) >> 6);
    ic[curr_ic].statb.c4ov = ((ic[curr_ic].stat.rx_data[2] & 0x80) >> 7);

    ic[curr_ic].statb.c5uv = (ic[curr_ic].stat.rx_data[3] & 0x01);
    ic[curr_ic].statb.c5ov = ((ic[curr_ic].stat.rx_data[3] & 0x02) >> 1);
    ic[curr_ic].statb.c6uv = ((ic[curr_ic].stat.rx_data[3] & 0x04) >> 2);
    ic[curr_ic].statb.c6ov = ((ic[curr_ic].stat.rx_data[3] & 0x08) >> 3);
    ic[curr_ic].statb.c7uv = ((ic[curr_ic].stat.rx_data[3] & 0x10) >> 4);
    ic[curr_ic].statb.c7ov = ((ic[curr_ic].stat.rx_data[3] & 0x20) >> 5);
    ic[curr_ic].statb.c8uv = ((ic[curr_ic].stat.rx_data[3] & 0x40) >> 6);
    ic[curr_ic].statb.c8ov = ((ic[curr_ic].stat.rx_data[3] & 0x80) >> 7);

    ic[curr_ic].statb.c9uv =  (ic[curr_ic].stat.rx_data[4] & 0x01);
    ic[curr_ic].statb.c9ov =  ((ic[curr_ic].stat.rx_data[4] & 0x02) >> 1);
    ic[curr_ic].statb.c10uv = ((ic[curr_ic].stat.rx_data[4] & 0x04) >> 2);
    ic[curr_ic].statb.c10ov = ((ic[curr_ic].stat.rx_data[4] & 0x08) >> 3);
    ic[curr_ic].statb.c11uv = ((ic[curr_ic].stat.rx_data[4] & 0x10) >> 4);
    ic[curr_ic].statb.c11ov = ((ic[curr_ic].stat.rx_data[4] & 0x20) >> 5);
    ic[curr_ic].statb.c12uv = ((ic[curr_ic].stat.rx_data[4] & 0x40) >> 6);
    ic[curr_ic].statb.c12ov = ((ic[curr_ic].stat.rx_data[4] & 0x80) >> 7);
    ic[curr_ic].statb.oc_cntr   = ic[curr_ic].stat.rx_data[5];
  }

  status = BMS_OK;
  return status;
}

/* Function definition -------------------------------------------------------*/
/**
 * @brief adBms6815 Parse StatusC
 * 
 * @param tIC total number of ICs
 * @param ic structure to the IC handle
 * @param data pointer to data buffer
 * @retval BMS status
 */
bmsStatus_t adBms6815ParseStatusC(uint8_t tIC, cell_asic *ic, uint8_t *data)
{
  uint8_t address = 0;
  bmsStatus_t status = BMS_ERROR;

  if((tIC <= 0)         ||\
    (NULL == ic)        ||\
    (NULL == data))
	{
		return BMS_CMU_FUNCTION_PARAMETER_ERROR;
	}

  for(uint8_t curr_ic = 0; curr_ic < tIC; curr_ic++)
  {
    memcpy(&ic[curr_ic].stat.rx_data[0], &data[address], RX_DATA); /* dst , src , size */
    address = ((curr_ic+1) * (RX_DATA));
    ic[curr_ic].statc.otp_med    =  (ic[curr_ic].stat.rx_data[0] & 0x01);
    ic[curr_ic].statc.otp_ed     =  ((ic[curr_ic].stat.rx_data[0] & 0x02) >> 1);
    ic[curr_ic].statc.a_otp_med  =  ((ic[curr_ic].stat.rx_data[0] & 0x04) >> 2);
    ic[curr_ic].statc.a_otp_ed   =  ((ic[curr_ic].stat.rx_data[0] & 0x08) >> 3);
    ic[curr_ic].statc.vd_uvlo    =  ((ic[curr_ic].stat.rx_data[0] & 0x10) >> 4);
    ic[curr_ic].statc.va_ovhi    =  ((ic[curr_ic].stat.rx_data[0] & 0x20) >> 5);
    ic[curr_ic].statc.va_uvlo    =  ((ic[curr_ic].stat.rx_data[0] & 0x40) >> 6);
    ic[curr_ic].statc.va_ovhi    =  ((ic[curr_ic].stat.rx_data[0] & 0x80) >> 7);
    ic[curr_ic].statc.oscchk     =  (ic[curr_ic].stat.rx_data[1] & 0x01);
    ic[curr_ic].statc.cpchk      =  ((ic[curr_ic].stat.rx_data[1] & 0x02) >> 1);
    ic[curr_ic].statc.thsd       =  ((ic[curr_ic].stat.rx_data[1] & 0x04) >> 2);
    ic[curr_ic].statc.muxfail    =  ((ic[curr_ic].stat.rx_data[1] & 0x08) >> 3);
    ic[curr_ic].statc.tmodechk   =  ((ic[curr_ic].stat.rx_data[1] & 0x10) >> 4);
    ic[curr_ic].statc.sleep      =  ((ic[curr_ic].stat.rx_data[1] & 0x20) >> 5);
    ic[curr_ic].statc.compchk    =  ((ic[curr_ic].stat.rx_data[1] & 0x40) >> 6);
    ic[curr_ic].statc.redfail    =  ((ic[curr_ic].stat.rx_data[1] & 0x80) >> 7);

    ic[curr_ic].statc.adol1      =  ((ic[curr_ic].stat.rx_data[2]) | (ic[curr_ic].stat.rx_data[3] << 8));
    ic[curr_ic].statc.adol2      =  ((ic[curr_ic].stat.rx_data[4]) | (ic[curr_ic].stat.rx_data[5] << 8));
  }

  status = BMS_OK;
  return status;
}

/* Function definition -------------------------------------------------------*/
/**
 * @brief adBms6815 Parse Status
 * 
 * @param tIC total number of ICs
 * @param ic structure to the IC handle
 * @param grp Register Group
 * @param data pointer to data buffer
 * @retval BMS status
 */
bmsStatus_t adBms6815ParseStatus(uint8_t tIC, cell_asic *ic, GRP grp, uint8_t *data)
{
  bmsStatus_t status = BMS_ERROR;
  
  if((tIC <= 0)         ||\
    (NULL == ic)        ||\
   !(IS_CMU_GRP(grp))   ||\
    (NULL == data))
	{
		return BMS_CMU_FUNCTION_PARAMETER_ERROR;
	}
  
  switch (grp)
  {
    case A:
      adBms6815ParseStatusA(tIC, &ic[0], &data[0]);
      status = BMS_OK;
      break;

    case B:
      adBms6815ParseStatusB(tIC, &ic[0], &data[0]);
      status = BMS_OK;
      break;

    case C:
      adBms6815ParseStatusC(tIC, &ic[0], &data[0]);
      status = BMS_OK;
      break;

    case ALL_GRP:
      /* Status A base address data[0] index */
      adBms6815ParseStatusA(tIC, &ic[0], &data[0]);
      /* Status B base address data[6] index */
      adBms6815ParseStatusB(tIC, &ic[0], &data[6]);
      /* Status C base address data[12] index */
      adBms6815ParseStatusC(tIC, &ic[0], &data[12]);
      status = BMS_OK;
      break;

    default:
      status = BMS_ERROR;
      break;
  }

  return status;
}



/* Function definition -------------------------------------------------------*/
/**
 * @brief spi Send Command
 * 
 * @param tx_cmd transmit command
 * @retval BMS status
 */
bmsStatus_t spiSendCmd(uint8_t tx_cmd[2])
{
  bmsStatus_t status = BMS_ERROR;
  
  if((NULL == tx_cmd))
	{
		return BMS_CMU_FUNCTION_PARAMETER_ERROR;
	}
  
  uint8_t cmd[4];
  uint16_t cmd_pec;
  cmd[0] = tx_cmd[0];
  cmd[1] =  tx_cmd[1];
  cmd_pec = Pec15_Calc(2, cmd);
  cmd[2] = (uint8_t)(cmd_pec >> 8);
  cmd[3] = (uint8_t)(cmd_pec);
  adBmsCsLow(ccmSpi);
  status = spiWriteBytes(4, &cmd[0]);
  adBmsCsHigh(ccmSpi);    
  return status; 
}

/* Function definition -------------------------------------------------------*/
/**
 * @brief spi Read Data
 * 
 * @param tIC total number of ICs
 * @param tx_cmd transmit command
 * @param rx_data received data
 * @param pec_error pec error
 * @param cmd_cntr command counter
 * @param regData_size register data size
 * @retval BMS status
 */

uint16_t adbms6815CRCErrorCount = 0;
bmsStatus_t spiReadData(uint8_t tIC, uint8_t tx_cmd[2], uint8_t *rx_data, uint8_t *pec_error, uint8_t *cmd_cntr, uint8_t regData_size)
{
  bmsStatus_t status = BMS_ERROR;

  if((tIC <= 0)             ||\
    (NULL == tx_cmd)        ||\
    (NULL == rx_data)       ||\
    (NULL == pec_error)     ||\
    (NULL == cmd_cntr)      ||\
    (regData_size <= 0))
	{
		return BMS_CMU_FUNCTION_PARAMETER_ERROR;
	}

  uint8_t *data, *copyArray, src_address = 0;
  uint16_t cmd_pec, received_pec, calculated_pec;
  uint8_t BYTES_IN_REG = regData_size;
  uint8_t RX_BUFFER = (regData_size * tIC);

  data = (uint8_t *)calloc(RX_BUFFER, sizeof(uint8_t));
  copyArray = (uint8_t *)calloc(BYTES_IN_REG, sizeof(uint8_t));

  if((data == NULL) || (copyArray == NULL))
  {
    status = BMS_DYNAMIC_MEM_ALLOC_ERROR;
    return status;
    exit(0);
  }
  else
  {
    uint8_t cmd[4];
    cmd[0] = tx_cmd[0];
    cmd[1] = tx_cmd[1];
    cmd_pec = Pec15_Calc(2, cmd);
    cmd[2] = (uint8_t)(cmd_pec >> 8);
    cmd[3] = (uint8_t)(cmd_pec);
    
    adBmsCsLow(ccmSpi);
    status = spiWriteReadBytes(&cmd[0], &data[0], 4, RX_BUFFER);                 /* Read the configuration data of all ICs on the daisy chain into readdata array */
    if(status != BMS_OK)
    {
      free(data);
      free(copyArray);
      return status;
    }
    adBmsCsHigh(ccmSpi);

    for (uint8_t current_ic = 0; current_ic < tIC; current_ic++)     /* executes for each ic in the daisy chain and packs the data */
    {                                                                                                                                     /* Into the r_comm array as well as check the received data for any bit errors */
      for (uint8_t current_byte = 0; current_byte < (BYTES_IN_REG-2); current_byte++)
      {
        rx_data[(current_ic*BYTES_IN_REG)+current_byte] = data[current_byte + (current_ic*BYTES_IN_REG)];
      }
      /* Get command counter value */
      cmd_cntr[current_ic] = (data[(current_ic * BYTES_IN_REG) + (BYTES_IN_REG - 2)] >> 2);
      /* Get received pec value from ic*/
      received_pec = (uint16_t)(((data[(current_ic * BYTES_IN_REG) + (BYTES_IN_REG - 2)] & 0x03) << 8) | data[(current_ic * BYTES_IN_REG) + (BYTES_IN_REG - 1)]);
      /* Copy each ic correspond data + pec value for calculate data pec */
      memcpy(&copyArray[0], &data[src_address], BYTES_IN_REG);
      src_address = ((current_ic+1) * (regData_size));
      /* Calculate data pec */
      calculated_pec = (uint16_t)adBMS6815Pec10Calculate(&copyArray[0], true, (BYTES_IN_REG-2) );
      /* Match received pec with calculated pec */
      if (received_pec == calculated_pec)     { pec_error[current_ic] = 0; }/* If no error is there value set to 0 */
      else                                    
      { 
        /* If error is there value set to 1 */
        pec_error[current_ic] = 1; 
        status = BMS_CMU_PEC10_ERROR;
        adbms6815CRCErrorCount++;

#ifdef DEBUG
        //pec error sending on uart
        HAL_UART_Transmit(&hlpuart1, (uint8_t *)" PEC10 ERROR ", 13, 100);
#endif

        if(status != BMS_OK)
        {
          free(data);
          free(copyArray);
          return status;
        }
      }                               
    }
  }
  
  free(data);
  free(copyArray);

  return status;
}

/* Function definition -------------------------------------------------------*/
/**
 * @brief spi Write Data
 * 
 * @param tIC total number of ICs
 * @param tx_cmd transmit command
 * @param data data buffer
 * @param data_size data size
 * @retval BMS status
 */
bmsStatus_t spiWriteData(uint8_t tIC, uint8_t tx_cmd[2], uint8_t *data, uint8_t data_size)
{
  bmsStatus_t status = BMS_ERROR;

  if((tIC <= 0)             ||\
    (NULL == tx_cmd)        ||\
    (NULL == data)          ||\
    (data_size <= 0))
	{
		return BMS_CMU_FUNCTION_PARAMETER_ERROR;
	}

  uint8_t BYTES_IN_REG = 0;
  uint8_t CMD_LEN = 0;
  uint16_t data_pec, cmd_pec;
  uint8_t *cmd, *copyArray; 
  uint8_t src_address = 0;
  uint8_t cmd_index;

  BYTES_IN_REG = data_size;
  CMD_LEN = 4 + ((data_size + 2) * tIC);

  copyArray = (uint8_t *)calloc(data_size, sizeof(uint8_t));
  cmd = (uint8_t *)calloc(CMD_LEN, sizeof(uint8_t));

  if((cmd == NULL) || (copyArray == NULL))
  {
    status = BMS_DYNAMIC_MEM_ALLOC_ERROR;
    return status;
    exit(0);
  }
  else
  {
    cmd[0] = tx_cmd[0];
    cmd[1] = tx_cmd[1];
    cmd_pec = Pec15_Calc(2, cmd);
    cmd[2] = (uint8_t)(cmd_pec >> 8);
    cmd[3] = (uint8_t)(cmd_pec);
    cmd_index = 4;
    /* executes for each LTC68xx, this loops starts with the last IC on the stack */
    for (uint8_t current_ic = tIC; current_ic > 0; current_ic--)
    {
      src_address = ((current_ic-1) * data_size);
      /* The first configuration written is received by the last IC in the daisy chain */
      for (uint8_t current_byte = 0; current_byte < BYTES_IN_REG; current_byte++)
      {
        cmd[cmd_index] = data[((current_ic-1)*data_size)+current_byte];
        //tempBuff2[cmd_index] = data[((current_ic-1)*data_size)+current_byte];
        cmd_index = cmd_index + 1;
      }
      /* Copy each ic correspond data + pec value for calculate data pec */
      memcpy(&copyArray[0], &data[src_address], data_size); /* dst, src, size */
      /* calculating the PEC for each Ics configuration register data */
      data_pec = (uint16_t)adBMS6815Pec10Calculate(&copyArray[0], false, BYTES_IN_REG );

      cmd[cmd_index] = (uint8_t)(data_pec >> 8);
      cmd_index = cmd_index + 1;
      cmd[cmd_index] = (uint8_t)data_pec;
      cmd_index = cmd_index + 1;
    }

    adBmsCsLow(ccmSpi);
    status = spiWriteBytes(CMD_LEN, &cmd[0]);
    if(status != BMS_OK)
    {
      free(cmd);
      free(copyArray);
      return status;
    }
    adBmsCsHigh(ccmSpi);
  }

  free(cmd);
  free(copyArray);
  return status;
}
/* Function definition -------------------------------------------------------*/
/**
 * @brief adBms Read Data
 * 
 * @param tIC total number of ICs
 * @param ic structure to the IC handle
 * @param cmd_arg command argument
 * @param type type
 * @param group group
 * @retval BMS status
 */
bmsStatus_t adBmsReadData(uint8_t tIC, cell_asic *ic, uint8_t cmd_arg[2], TYPE type, GRP group)
{
  bmsStatus_t status = BMS_ERROR;
  uint16_t rBuff_size = 0;
  uint16_t regData_size = 0;

  if((tIC <= 0)             ||\
    (NULL == ic)            ||\
    (NULL == cmd_arg)       ||\
    !(IS_CMU_TYPE(type))    ||\
    !(IS_CMU_GRP(group)))
	{
		return BMS_CMU_FUNCTION_PARAMETER_ERROR;
	}

  if(group == ALL_GRP)
  {
    if(type == Rdcvall)         {rBuff_size = RDCVALL_SIZE; regData_size = RDCVALL_SIZE;}
    else if(type == Rdcdall)    {rBuff_size = RDCDALL_SIZE; regData_size = RDCDALL_SIZE;}
    else if(type == Rdasall)    {rBuff_size = RDASALL_SIZE; regData_size = RDASALL_SIZE;}
    else                        {}
  }
  else{rBuff_size = (tIC * RX_DATA); regData_size = RX_DATA;}

  uint8_t *read_buffer, *pec_error, *cmd_count;

  read_buffer = (uint8_t *)calloc(rBuff_size, sizeof(uint8_t));
  pec_error = (uint8_t *)calloc(tIC, sizeof(uint8_t));
  cmd_count = (uint8_t *)calloc(tIC, sizeof(uint8_t));

  if((pec_error == NULL) || (cmd_count == NULL) || (read_buffer == NULL))
  {
    status = BMS_DYNAMIC_MEM_ALLOC_ERROR;
    return status;
    exit(0);
  }
  else
  {
    status = spiReadData(tIC, &cmd_arg[0], &read_buffer[0], &pec_error[0], &cmd_count[0], regData_size);
    if(status != BMS_OK)
    {
      free(read_buffer);
      free(pec_error);
      free(cmd_count);
      return status;
    }    

    switch (type)
    {
      case Config:
        status = adBms6815ParseConfig(tIC, ic, group, &read_buffer[0]);
        for (uint8_t cic = 0; cic < tIC; cic++)
        {
          ic[cic].cccrc.cfgr_pec = pec_error[cic];
          ic[cic].cccrc.cmd_cntr = cmd_count[cic];
        }
        break;

      case Cell:
        status = adBms6815ParseCell(tIC, ic, group, &read_buffer[0]);
        for (uint8_t cic = 0; cic < tIC; cic++)
        {
          ic[cic].cccrc.cell_pec = pec_error[cic];
          ic[cic].cccrc.cmd_cntr = cmd_count[cic];
        }
        break;

      case D_volt:
        status = adBms6815ParseDCell(tIC, ic, group, &read_buffer[0]);
        for (uint8_t cic = 0; cic < tIC; cic++)
        {
          ic[cic].cccrc.dcell_pec = pec_error[cic];
          ic[cic].cccrc.cmd_cntr = cmd_count[cic];
        }
        break;

      case Aux:
        status = adBms6815ParseAux(tIC, ic, group, &read_buffer[0]);
        for (uint8_t cic = 0; cic < tIC; cic++)
        {
          ic[cic].cccrc.aux_pec = pec_error[cic];
          ic[cic].cccrc.cmd_cntr = cmd_count[cic];
        }
        break;

      case Status:
        status = adBms6815ParseStatus(tIC, ic, group, &read_buffer[0]);
        for (uint8_t cic = 0; cic < tIC; cic++)
        {
          ic[cic].cccrc.stat_pec = pec_error[cic];
          ic[cic].cccrc.cmd_cntr = cmd_count[cic];
        }
        break;

      case Pwm:
        adBms6815ParsePwm(tIC, ic, &read_buffer[0]);
        for (uint8_t cic = 0; cic < tIC; cic++)
        {
          ic[cic].cccrc.pwm_pec = pec_error[cic];
          ic[cic].cccrc.cmd_cntr = cmd_count[cic];
        }
        break;

      case Comm:
        adBms6815ParseComm(tIC, ic, &read_buffer[0]);
        for (uint8_t cic = 0; cic < tIC; cic++)
        {
          ic[cic].cccrc.comm_pec = pec_error[cic];
          ic[cic].cccrc.cmd_cntr = cmd_count[cic];
        }
        break;

      // case SID:
      //     adBms6815ParseSID(tIC, ic, &read_buffer[0]);
      //     for (uint8_t cic = 0; cic < tIC; cic++)
      //     {
      //         ic[cic].cccrc.sid_pec = pec_error[cic];
      //         ic[cic].cccrc.cmd_cntr = cmd_count[cic];
      //     }
      //     break;

      // case Rdcvall:
      //     adBms6815ParseCell(tIC, ic, group, &read_buffer[0]);
      //     for (uint8_t cic = 0; cic < tIC; cic++)
      //     {
      //         ic[cic].cccrc.cell_pec = pec_error[cic];
      //         ic[cic].cccrc.cmd_cntr = cmd_count[cic];
      //     }
      //     break;

      // case Rdcdall:
      //     adBms6815ParseDCell(tIC, ic, group, &read_buffer[0]);
      //     for (uint8_t cic = 0; cic < tIC; cic++)
      //     {
      //         ic[cic].cccrc.dcell_pec = pec_error[cic];
      //         ic[cic].cccrc.cmd_cntr = cmd_count[cic];
      //     }
      //     break;

      // case Rdasall:
      //     /* 36 byte + 2 byte pec = 18 byte aux data + 18 byte status A,B and C data */
      //     adBms6815ParseAux(tIC, ic, group, &read_buffer[0]);
      //     for (uint8_t cic = 0; cic < tIC; cic++)
      //     {
      //         ic[cic].cccrc.aux_pec = pec_error[cic];
      //         ic[cic].cccrc.cmd_cntr = cmd_count[cic];
      //     }
      //     /* Status A base address read_buffer [18] */
      //     adBms6815ParseStatus(tIC, ic, group, &read_buffer[18]);
      //     for (uint8_t cic = 0; cic < tIC; cic++)
      //     {
      //         ic[cic].cccrc.stat_pec = pec_error[cic];
      //         ic[cic].cccrc.cmd_cntr = cmd_count[cic];
      //     }
      //     break;

      default:
        status = BMS_ERROR;
        break;
      }
  }
  free(read_buffer);
  free(pec_error);
  free(cmd_count);
  return status;
}

/* Function definition -------------------------------------------------------*/
/**
 * @brief adBms Write Data
 * 
 * @param tIC total number of ICs
 * @param ic structure to the IC handle
 * @param cmd_arg command argument
 * @param type type
 * @param group group
 * @retval BMS status
 */
bmsStatus_t adBmsWriteData(uint8_t tIC, cell_asic *ic, uint8_t cmd_arg[2], TYPE type, GRP group)
{
  bmsStatus_t status = BMS_ERROR;  
  uint8_t data_len = 0;
  uint8_t write_size = 0;

  if((tIC <= 0)             ||\
    (NULL == ic)            ||\
    (NULL == cmd_arg)       ||\
    !(IS_CMU_TYPE(type))    ||\
    !(IS_CMU_GRP(group)))
	{
		return BMS_CMU_FUNCTION_PARAMETER_ERROR;
	}


  if(type == Clrflag)
  {
    data_len = CLR_FLAG, write_size = (CLR_FLAG * tIC);
  }
  else
  {
    data_len = TX_DATA, write_size = (TX_DATA * tIC);
  }

  uint8_t *write_buffer = (uint8_t *)calloc(write_size, sizeof(uint8_t));

  if(write_buffer == NULL)
  {
    status = BMS_DYNAMIC_MEM_ALLOC_ERROR;
    return status;
    exit(0);
  }
  else
  {
    switch (type)
    {
      case Config:
        switch (group)
        {
          case A:
            status = adBms6815CreateConfiga(tIC, &ic[0]);
            for (uint8_t cic = 0; cic < tIC; cic++)
            {
              for (uint8_t data = 0; data < data_len; data++)
              {
                write_buffer[(cic * data_len) + data] = ic[cic].configa.tx_data[data];
              }
            }
            break;
          case B:
            status = adBms6815CreateConfigb(tIC, &ic[0]);
            for (uint8_t cic = 0; cic < tIC; cic++)
            {
              for (uint8_t data = 0; data < data_len; data++)
              {
                write_buffer[(cic * data_len) + data] = ic[cic].configb.tx_data[data];
                //tempBuff[(cic * data_len) + data] = ic[cic].configb.tx_data[data];
              }
            }
            break;
          default:
            status = BMS_ERROR;
            break;
        }
        break;

      case Pwm:
        adBms6815CreatePwm(tIC, &ic[0]);
        for(uint8_t cic = 0; cic < tIC; cic++)
        {
          for(uint8_t data = 0; data < data_len; data++)
          {
            write_buffer[(cic * data_len) + data] = ic[cic].pwm.tx_data[data];
          }
        }
        break;

      case Clrflag:
        status = adBms6815CreateClrFlagData(tIC, &ic[0]);
        for (uint8_t cic = 0; cic < tIC; cic++)
        {
          for (uint8_t data = 0; data < data_len; data++)
          {
            write_buffer[(cic * data_len) + data] = ic[cic].flag_tx.data[data];
          }
        }
        break;

      case Comm:
        adBms6815CreateComm(tIC, &ic[0]);
        for(uint8_t cic = 0; cic < tIC; cic++)
        {
          for(uint8_t data = 0; data < data_len; data++)
          {
            write_buffer[(cic * data_len) + data] = ic[cic].com.tx_data[data];
          }
        }
        break;
      
      default:
        status = BMS_ERROR;
        break;
      }
  }
  status = spiWriteData(tIC, cmd_arg, &write_buffer[0], data_len);
  free(write_buffer);

  return status;
}

/* Function definition -------------------------------------------------------*/
/**
 * @brief Set the Over Voltage Threshold object
 * 
 * @param voltage 
 * @return uint16_t 
 */
uint16_t SetOverVoltageThreshold(float voltage)
{
  if((voltage <= 0))
	{
		return BMS_CMU_FUNCTION_PARAMETER_ERROR;
	}

  uint16_t vov_value;
  vov_value = (uint16_t )(voltage * 10000 / 16); /*(Comparison Voltage = VOV • 16 • 0.0001)*/
  return vov_value;
}

/* Function definition -------------------------------------------------------*/
/**
 * @brief Set the Under Voltage Threshold object
 * 
 * @param voltage 
 * @return uint16_t 
 */
uint16_t SetUnderVoltageThreshold(float voltage)
{
  if((voltage <= 0))
	{
		return BMS_CMU_FUNCTION_PARAMETER_ERROR;
	}

  uint16_t vuv_value;
  vuv_value = (uint16_t )(voltage * 10000 / 16);
  return vuv_value;
}

/* Function definition -------------------------------------------------------*/
/**
 * @brief adBms6815 init config A B
 * 
 * @param tIC total number of ICs
 * @param ic structure to the IC handle
 * @retval BMS status
 */
bmsStatus_t adBms6815_init_configA_B(uint8_t tIC, cell_asic *ic, float OV_THRESHOLD, float UV_THRESHOLD)
{
  bmsStatus_t status = BMS_ERROR;

  if((tIC <= 0)         ||\
    (NULL == ic)        ||\
    (OV_THRESHOLD <= 0) ||\
    (UV_THRESHOLD <= 0))
	{
    status = BMS_CMU_FUNCTION_PARAMETER_ERROR;
		return status;
	}

  status = adBmsCsHigh(cmuSpi);
  if(status != BMS_OK) return status;

  for(uint8_t cic = 0; cic < tIC; cic++)
  {
    /* Init config A */
    ic[cic].tx_cfga.refon = PWR_UP;
    ic[cic].tx_cfga.adcopt = ADC_OPT;
    ic[cic].tx_cfga.gpo = 0x7F;                /* All gpios pull down disable      */
    ic[cic].tx_cfga.soakon = 0;                /* Set default value = 0            */
    ic[cic].tx_cfga.owrng = 0;                 /* Set default value = 0            */
    ic[cic].tx_cfga.owa = 0;                   /* Set default value = 0            */

    /* Init config B */
    ic[cic].tx_cfgb.vov = SetOverVoltageThreshold(OV_THRESHOLD);   /*volt */
    ic[cic].tx_cfgb.vuv = SetUnderVoltageThreshold(UV_THRESHOLD);  /*volt */
  }

  status = adBmsWakeupIc(tIC);
  if(status != BMS_OK) return status;
  status = adBmsWriteData(tIC, &ic[0], WRCFGA, Config, A);
	if(status != BMS_OK) return status;
  status = adBmsWriteData(tIC, &ic[0], WRCFGB, Config, B);
  return status;
}

/* Function definition -------------------------------------------------------*/
/**
 * @brief adBms6815 write read config
 * 
 * @param tIC total number of ICs
 * @param ic structure to the IC handle
 * @retval BMS status
 */
bmsStatus_t adBms6815_write_read_config(uint8_t tIC, cell_asic *ic)
{
  bmsStatus_t status = BMS_ERROR;

  if((tIC <= 0)  ||\
    (NULL == ic))
	{
		return BMS_CMU_FUNCTION_PARAMETER_ERROR;
	}

  status = adBmsWakeupIc(tIC);
  if(status != BMS_OK) return status; 
  status = adBmsWriteData(tIC, &ic[0], WRCFGA, Config, A);
  if(status != BMS_OK) return status; 
  status = adBmsWriteData(tIC, &ic[0], WRCFGB, Config, B);
  if(status != BMS_OK) return status; 
  status = adBmsReadData(tIC, &ic[0], RDCFGA, Config, A);
  if(status != BMS_OK) return status;
  status = adBmsReadData(tIC, &ic[0], RDCFGB, Config, B);

  return status;
}

/* Function definition -------------------------------------------------------*/
/**
 * @brief adBms6815 read config
 * 
 * @param tIC total number of ICs
 * @param ic structure to the IC handle
 * @retval BMS status
 */
bmsStatus_t adBms6815_read_config(uint8_t tIC, cell_asic *ic)
{
  bmsStatus_t status = BMS_ERROR;

  if((tIC <= 0)  ||\
    (NULL == ic))
	{
		return BMS_CMU_FUNCTION_PARAMETER_ERROR;
	}

  status = adBmsWakeupIc(tIC);
  if(status != BMS_OK) return status;  
  status = adBmsReadData(tIC, &ic[0], RDCFGA, Config, A);
  if(status != BMS_OK) return status;
  status = adBmsReadData(tIC, &ic[0], RDCFGB, Config, B);

  return status;
}

/* Function definition -------------------------------------------------------*/
/**
 * @brief adBms6815 start adc cell voltage conversion
 * 
 * @param tIC total number of ICs
 * @retval BMS status
 */
bmsStatus_t adBms6815_start_adc_cell_voltage_conversion(uint8_t tIC)
{
  bmsStatus_t status = BMS_ERROR;

  if((tIC <= 0))
	{
		return BMS_CMU_FUNCTION_PARAMETER_ERROR;
	}

  status = adBmsWakeupIc(tIC);
  if(status != BMS_OK) return status;
  status = adBms6815_Adcv(ADC_CONVERSION_MODE, ADC_DCP, CELL_CH_TO_CONVERT);
  if(status != BMS_OK) return status;
  pladc_count = adBmsPollAdc(PLADC);

  return status;
}

/* Function definition -------------------------------------------------------*/
/**
 * @brief adBms6815 read cell voltages
 * 
 * @param tIC total number of ICs
 * @param ic structure to the IC handle
 * @retval BMS status
 */
bmsStatus_t adBms6815_read_cell_voltages(uint8_t tIC, cell_asic *ic)
{
  bmsStatus_t status = BMS_ERROR;

  if((tIC <= 0)  ||\
    (NULL == ic))
	{
		return BMS_CMU_FUNCTION_PARAMETER_ERROR;
	}

  status = adBmsWakeupIc(tIC);
  if(status != BMS_OK) return status;
  status = adBmsReadData(tIC, &ic[0], RDCVA, Cell, A);
  if(status != BMS_OK) return status;
  status = adBmsReadData(tIC, &ic[0], RDCVB, Cell, B);
  if(status != BMS_OK) return status;
  status = adBmsReadData(tIC, &ic[0], RDCVC, Cell, C);
  if(status != BMS_OK) return status;
  status = adBmsReadData(tIC, &ic[0], RDCVD, Cell, D);

  return status;
}

/* Function definition -------------------------------------------------------*/
/**
 * @brief adBms6815 start aux voltage conversion
 * 
 * @param tIC total number of ICs
 * @param ic structure to the IC handle
 * @retval BMS status
 */
bmsStatus_t adBms6815_start_aux_voltage_conversion(uint8_t tIC, cell_asic *ic)
{
  bmsStatus_t status = BMS_ERROR;

  if((tIC <= 0)  ||\
    (NULL == ic))
	{
		return BMS_CMU_FUNCTION_PARAMETER_ERROR;
	}

  for(uint8_t cic = 0; cic < tIC; cic++)
  {
    /* Init config A */
    ic[cic].tx_cfga.refon = PWR_UP;
    ic[cic].tx_cfga.gpo = 0x7F;                /* All gpios pull down disable      */
  }
  status = adBmsWakeupIc(tIC);
  if(status != BMS_OK) return status;
  status = adBmsWriteData(tIC, &ic[0], WRCFGA, Config, A);
  if(status != BMS_OK) return status;
  status = adBmss6815_Adax(ADC_CONVERSION_MODE, AUX_CH_TO_CONVERT);
  if(status != BMS_OK) return status;
  pladc_count = adBmsPollAdc(PLADC);
  return status;
}

/* Function definition -------------------------------------------------------*/
/**
 * @brief adBms6815 read aux voltages
 * 
 * @param tIC total number of ICs
 * @param ic structure to the IC handle
 * @retval BMS status
 */
bmsStatus_t adBms6815_read_aux_voltages(uint8_t tIC, cell_asic *ic)
{
  bmsStatus_t status = BMS_ERROR;

  if((tIC <= 0)  ||\
    (NULL == ic))
	{
		return BMS_CMU_FUNCTION_PARAMETER_ERROR;
	}

  status = adBmsWakeupIc(tIC);
  if(status != BMS_OK) return status;
  status = adBmsReadData(tIC, &ic[0], RDAUXA, Aux, A);
  if(status != BMS_OK) return status;
  status = adBmsReadData(tIC, &ic[0], RDAUXB, Aux, B);
  if(status != BMS_OK) return status;
  status = adBmsReadData(tIC, &ic[0], RDAUXC, Aux, C);
  return status;
}

/* Function definition -------------------------------------------------------*/
/**
 * @brief adBms6815 read status
 * 
 * @param tIC total number of ICs
 * @param ic structure to the IC handle
 * @retval BMS status
 */
bmsStatus_t adBms6815_read_status(uint8_t tIC, cell_asic *ic)
{
  bmsStatus_t status = BMS_ERROR;

  if((tIC <= 0)  ||\
    (NULL == ic))
	{
		return BMS_CMU_FUNCTION_PARAMETER_ERROR;
	}

  status = adBmsWakeupIc(tIC);
  if(status != BMS_OK) return status;
  status = adBms6815_Adstat(ADC_CONVERSION_MODE, STAT_CH_TO_CONVERT);
  if(status != BMS_OK) return status;
  pladc_count = adBmsPollAdc(PLADC);
  status = adBms6815_Adol(ADC_CONVERSION_MODE);
  if(status != BMS_OK) return status;
  pladc_count = pladc_count + adBmsPollAdc(PLADC);
  status = adBms6815_Adcv(ADC_CONVERSION_MODE, ADC_DCP, CELL_CH_TO_CONVERT);
  if(status != BMS_OK) return status;
  pladc_count = pladc_count + adBmsPollAdc(PLADC);

  status = adBmsReadData(tIC, &ic[0], RDSTATA, Status, A);
  if(status != BMS_OK) return status;
  status = adBmsReadData(tIC, &ic[0], RDSTATB, Status, B);
  if(status != BMS_OK) return status;
  status = adBmsReadData(tIC, &ic[0], RDSTATC, Status, C);
  return status;
}

/* Function definition -------------------------------------------------------*/
/**
 * @brief adBms6815 read cd voltages
 * 
 * @param tIC total number of ICs
 * @param ic structure to the IC handle
 * @retval BMS status
 */
bmsStatus_t adBms6815_read_cd_voltages(uint8_t tIC, cell_asic *ic)
{
  bmsStatus_t status = BMS_ERROR;

  if((tIC <= 0)  ||\
    (NULL == ic))
	{
		return BMS_CMU_FUNCTION_PARAMETER_ERROR;
	}

  status = adBmsWakeupIc(tIC);
  if(status != BMS_OK) return status;
  status = adBmsReadData(tIC, &ic[0], RDCDA, D_volt, A);
  if(status != BMS_OK) return status;
  status = adBmsReadData(tIC, &ic[0], RDCDB, D_volt, B);
  if(status != BMS_OK) return status;
  status = adBmsReadData(tIC, &ic[0], RDCDC, D_volt, C);
  if(status != BMS_OK) return status;
  status = adBmsReadData(tIC, &ic[0], RDCDD, D_volt, D);
  return status;
}

/* Function definition -------------------------------------------------------*/
/**
 * @brief adBms6815 Adcv
 * 
 * @param MD ADC modes
 * @param DCP Discharge cell set or clear 
 * @param CHCELL GPIO Channel selection
 * @retval BMS status
 */
bmsStatus_t adBms6815_Adcv(ADC_MODE MD, DCC_BIT DCP, CH CHCELL)
{
  bmsStatus_t status = BMS_ERROR;

  if(!(IS_CMU_ADC_MODE(MD))  ||\
    !(IS_CMU_DCC_BIT(DCP))   ||\
    !(IS_CMU_CH(CHCELL)))
	{
		return BMS_CMU_FUNCTION_PARAMETER_ERROR;
	}

  uint8_t cmd[2];
  uint8_t md_bits;
  md_bits = (MD & 0x02) >> 1;
  cmd[0] = md_bits + 0x02;
  md_bits = (MD & 0x01) << 7;
  cmd[1] =  md_bits + 0x60 + (DCP<<4) + CHCELL;
  status = spiSendCmd(cmd);
  return status;
}

/* Function definition -------------------------------------------------------*/
/**
 * @brief adBms6815 Adsc
 * 
 * @param MD ADC modes 
 * @param DCP Discharge cell set or clear  
 * @param CHCELL GPIO Channel selection 
 * @retval BMS status
 */
bmsStatus_t adBms6815_Adsc(ADC_MODE MD, DCC_BIT DCP, CH CHCELL)
{
  bmsStatus_t status = BMS_ERROR;

  if(!(IS_CMU_ADC_MODE(MD))  ||\
    !(IS_CMU_DCC_BIT(DCP))   ||\
    !(IS_CMU_CH(CHCELL)))
	{
		return BMS_CMU_FUNCTION_PARAMETER_ERROR;
	}

  uint8_t cmd[2];
  uint8_t md_bits;
  cmd[0] = (MD & 0x02) >> 1;
  md_bits = (MD & 0x01) << 7;
  cmd[1] =  md_bits + 0x68 + (DCP<<4) + CHCELL;
  status = spiSendCmd(cmd);
  return status;
}

/* Function definition -------------------------------------------------------*/
/**
 * @brief adBms6815 Adleak
 * 
 * @param MD ADC modes 
 * @param DCP Discharge cell set or clear  
 * @param CHCELL GPIO Channel selection 
 * @retval BMS status
 */
bmsStatus_t adBms6815_Adleak(ADC_MODE MD, DCC_BIT DCP, CH CHCELL)
{
  bmsStatus_t status = BMS_ERROR;

  if(!(IS_CMU_ADC_MODE(MD))  ||\
    !(IS_CMU_DCC_BIT(DCP))   ||\
    !(IS_CMU_CH(CHCELL)))
	{
		return BMS_CMU_FUNCTION_PARAMETER_ERROR;
	}

  uint8_t cmd[2];
  uint8_t md_bits;
  cmd[0] = (MD & 0x02) >> 1;
  md_bits = (MD & 0x01) << 7;
  cmd[1] =  md_bits + 0x60 + (DCP<<4) + CHCELL;
  status = spiSendCmd(cmd);
  return status;
}

/* Function definition -------------------------------------------------------*/
/**
 * @brief adBmss6815 Adax
 * 
 * @param MD ADC modes 
 * @param CHGPIO GPIO Channel selection
 * @retval BMS status
 */
bmsStatus_t adBmss6815_Adax(ADC_MODE MD, CHG CHGPIO)
{
  bmsStatus_t status = BMS_ERROR;
  
  if(!(IS_CMU_ADC_MODE(MD))  ||\
    !(IS_CMU_CHG(CHGPIO)))
	{
		return BMS_CMU_FUNCTION_PARAMETER_ERROR;
	}

  uint8_t cmd[2];
  uint8_t md_bits;
  md_bits = (MD & 0x02) >> 1;
  cmd[0] = md_bits + 0x04;
  md_bits = (MD & 0x01) << 7;
  cmd[1] = md_bits + CHGPIO ;
  status = spiSendCmd(cmd);
  return status;
}

/* Function definition -------------------------------------------------------*/
/**
 * @brief adBms6815 Axow
 * 
 * @param MD ADC modes 
 * @param PUP Pull Down/Up current during conversion
 * @param CHGPIO GPIO Channel selection 
 * @retval BMS status
 */
bmsStatus_t adBms6815_Axow(ADC_MODE MD, PUP_BIT PUP, CHG CHGPIO)
{
  bmsStatus_t status = BMS_ERROR;
  
  if(!(IS_CMU_ADC_MODE(MD))  ||\
    !(IS_CMU_PUP_BIT(PUP))   ||\
    !(IS_CMU_CHG(CHGPIO)))
	{
		return BMS_CMU_FUNCTION_PARAMETER_ERROR;
	}

  uint8_t cmd[2];
  uint8_t md_bits;
  md_bits = (MD & 0x02) >> 1;
  cmd[0] =  (md_bits + 0x04);
  md_bits = (MD & 0x01) << 7;
  cmd[1] =  md_bits + 0x10 + (PUP << 6) + CHGPIO;
  status = spiSendCmd(cmd);
  return status;
}

/* Function definition -------------------------------------------------------*/
/**
 * @brief adBms6815 Adstat
 * 
 * @param MD ADC modes 
 * @param CHSTAT Status Group Channel selection
 * @retval BMS status
 */
bmsStatus_t adBms6815_Adstat(ADC_MODE MD, CHST CHSTAT)
{
  bmsStatus_t status = BMS_ERROR;
  
  if(!(IS_CMU_ADC_MODE(MD))  ||\
    !(IS_CMU_CHST(CHSTAT)))
	{
		return BMS_CMU_FUNCTION_PARAMETER_ERROR;
	}

  uint8_t cmd[2];
  uint8_t md_bits;
  md_bits = (MD & 0x02) >> 1;
  cmd[0] =  (md_bits + 0x04);
  md_bits = (MD & 0x01) << 7;
  cmd[1] =   md_bits + 0x68 + CHSTAT;
  status = spiSendCmd(cmd);
  return status;
}

/* Function definition -------------------------------------------------------*/
/**
 * @brief adBms6815 Adcvax
 * 
 * @param MD ADC modes 
 * @retval BMS status
 */
bmsStatus_t adBms6815_Adcvax(ADC_MODE MD)
{
  bmsStatus_t status = BMS_ERROR;
  
  if(!(IS_CMU_ADC_MODE(MD)))
	{
		return BMS_CMU_FUNCTION_PARAMETER_ERROR;
	}

  uint8_t cmd[2];
  uint8_t md_bits;
  md_bits = (MD & 0x02) >> 1;
  cmd[0] = md_bits | 0x04;
  md_bits = (MD & 0x01) << 7;
  cmd[1] =  md_bits |0x6F;
  status = spiSendCmd(cmd);
  return status;
}

/* Function definition -------------------------------------------------------*/
/**
 * @brief adBms6815 Adcvsc
 * 
 * @param MD ADC modes 
 * @retval BMS status
 */
bmsStatus_t adBms6815_Adcvsc(ADC_MODE MD)
{
  bmsStatus_t status = BMS_ERROR;
  
  if(!(IS_CMU_ADC_MODE(MD)))
	{
		return BMS_CMU_FUNCTION_PARAMETER_ERROR;
	}

  uint8_t cmd[2];
  uint8_t md_bits;
  md_bits = (MD & 0x02) >> 1;
  cmd[0] = md_bits | 0x04;
  md_bits = (MD & 0x01) << 7;
  cmd[1] =  md_bits | 0x67;
  status = spiSendCmd(cmd);
  return status;
}

/* Function definition -------------------------------------------------------*/
/**
 * @brief adBms6815_Adol
 * 
 * @param MD ADC modes 
 * @retval BMS status
 */
bmsStatus_t adBms6815_Adol(ADC_MODE MD)
{
  bmsStatus_t status = BMS_ERROR;
  
  if(!(IS_CMU_ADC_MODE(MD)))
	{
		return BMS_CMU_FUNCTION_PARAMETER_ERROR;
	}

  uint8_t cmd[2];
  uint8_t md_bits;
  md_bits = (MD & 0x02) >> 1;
  cmd[0] =   md_bits + 0x02;
  md_bits = (MD & 0x01) << 7;
  cmd[1] =  md_bits + 0x01;
  status = spiSendCmd(cmd);
  return status;
}

/* Function definition -------------------------------------------------------*/
/**
 * @brief adBms6815 Cvow
 * 
 * @param MD ADC modes 
 * @retval BMS status
 */
bmsStatus_t adBms6815_Cvow(ADC_MODE MD)
{
  bmsStatus_t status = BMS_ERROR;
  
  if(!(IS_CMU_ADC_MODE(MD)))
	{
		return BMS_CMU_FUNCTION_PARAMETER_ERROR;
	}

  uint8_t cmd[2];
  uint8_t md_bits;
  md_bits = (MD & 0x02) >> 1;
  cmd[0] = md_bits + 0x02;
  md_bits =(MD & 0x01) << 7;
  cmd[1] = md_bits +0x08;
  status = spiSendCmd(cmd);
  return status;
}

/* Function definition -------------------------------------------------------*/
/**
 * @brief adBms6815 Cvpg
 * 
 * @param MD ADC modes 
 * @param PG Memory pattern
 * @retval BMS status
 */
bmsStatus_t adBms6815_Cvpg(ADC_MODE MD, MEM_PG PG)
{
  bmsStatus_t status = BMS_ERROR;
 
  if(!(IS_CMU_ADC_MODE(MD))  ||\
    !(IS_CMU_MEM_PG(PG)))
	{
		return BMS_CMU_FUNCTION_PARAMETER_ERROR;
	}

  uint8_t cmd[2];
  uint8_t md_bits;
  md_bits = (MD & 0x02) >> 1;
  cmd[0] = md_bits + 0x02;
  md_bits = (MD & 0x01) << 7;
  cmd[1] =  md_bits + ((PG & 0x03)<< 5) + 0x07;
  status = spiSendCmd(cmd);
  return status;
}

/* Function definition -------------------------------------------------------*/
/**
 * @brief adBms6815 Cdpg
 * 
 * @param MD ADC modes 
 * @param PG Memory pattern
 * @retval BMS status
 */
bmsStatus_t adBms6815_Cdpg(ADC_MODE MD, MEM_PG PG)
{
  bmsStatus_t status = BMS_ERROR;
 
  if(!(IS_CMU_ADC_MODE(MD))  ||\
    !(IS_CMU_MEM_PG(PG)))
	{
		return BMS_CMU_FUNCTION_PARAMETER_ERROR;
	}

  uint8_t cmd[2];
  uint8_t md_bits;
  md_bits = (MD & 0x02) >> 1;
  cmd[0] = md_bits + 0x02;
  md_bits = (MD & 0x01) << 7;
  cmd[1] =  md_bits + ((PG & 0x03)<< 5) + 0x1F;
  status = spiSendCmd(cmd);
  return status;
}

/* Function definition -------------------------------------------------------*/
/**
 * @brief adBms6815 Axpg
 * 
 * @param MD ADC modes 
 * @param PG Memory pattern
 * @retval BMS status
 */
bmsStatus_t adBms6815_Axpg(ADC_MODE MD, MEM_PG PG)
{
  bmsStatus_t status = BMS_ERROR;
 
  if(!(IS_CMU_ADC_MODE(MD))  ||\
    !(IS_CMU_MEM_PG(PG)))
	{
		return BMS_CMU_FUNCTION_PARAMETER_ERROR;
	}

  uint8_t cmd[2];
  uint8_t md_bits;
  md_bits = (MD & 0x02) >> 1;
  cmd[0] = md_bits + 0x04;
  md_bits = (MD & 0x01) << 7;
  cmd[1] =  md_bits + ((PG & 0x03)<< 5) + 0x07;
  status = spiSendCmd(cmd);
  return status;
}

/* Function definition -------------------------------------------------------*/
/**
 * @brief adBms6815 Statpg
 * 
 * @param MD ADC modes 
 * @param PG Memory pattern
 * @retval BMS status
 */
bmsStatus_t adBms6815_Statpg(ADC_MODE MD, MEM_PG PG)
{
  bmsStatus_t status = BMS_ERROR;
 
  if(!(IS_CMU_ADC_MODE(MD))  ||\
    !(IS_CMU_MEM_PG(PG)))
	{
		return BMS_CMU_FUNCTION_PARAMETER_ERROR;
	}

  uint8_t cmd[2];
  uint8_t md_bits;
  md_bits = (MD & 0x02) >> 1;
  cmd[0] = md_bits + 0x04;
  md_bits = (MD & 0x01) << 7;
  cmd[1] =  md_bits + ((PG & 0x03)<< 5) + 0x0F;
  status = spiSendCmd(cmd);
  return status;
}

/* Function definition -------------------------------------------------------*/
/**
 * @brief adBms6815 Adow
 * 
 * @param MD ADC modes 
 * @param PUP Pull Down/Up current during conversion 
 * @retval BMS status
 */
bmsStatus_t adBms6815_Adow(ADC_MODE MD, PUP_BIT PUP)
{
  bmsStatus_t status = BMS_ERROR;
 
  if(!(IS_CMU_ADC_MODE(MD))  ||\
    !(IS_CMU_PUP_BIT(PUP)))
	{
		return BMS_CMU_FUNCTION_PARAMETER_ERROR;
	}

  uint8_t cmd[2];
  uint8_t md_bits;
  md_bits = (MD & 0x02) >> 1;
  cmd[0] = md_bits + 0x02;
  md_bits = (MD & 0x01) << 7;
  cmd[1] =  md_bits + 0x28 + (PUP<<6);
  status = spiSendCmd(cmd);
  return status;
}

/* Function definition -------------------------------------------------------*/
/**
 * @brief adBms6815 enable mute
 * 
 * @param tIC total number of ICs
 * @param ic structure to the IC handle
 * @retval BMS status
 */
bmsStatus_t adBms6815_enable_mute(uint8_t tIC, cell_asic *ic)
{
  bmsStatus_t status = BMS_ERROR;

  if((tIC <= 0)         ||\
    (NULL == ic))
	{
		return BMS_CMU_FUNCTION_PARAMETER_ERROR;
	}

  adBmsWakeupIc(tIC);
  status = spiSendCmd(MUTE);
  if(status != BMS_OK) return status;
  status = adBmsReadData(tIC, &ic[0], RDCFGB, Config, B);

  return status;
}

/* Function definition -------------------------------------------------------*/
/**
 * @brief adBms6815 disable mute
 * 
 * @param tIC total number of ICs
 * @param ic structure to the IC handle
 * @retval BMS status
 */
bmsStatus_t adBms6815_disable_mute(uint8_t tIC, cell_asic *ic)
{
  bmsStatus_t status = BMS_ERROR;

  if((tIC <= 0)         ||\
    (NULL == ic))
	{
		return BMS_CMU_FUNCTION_PARAMETER_ERROR;
	}

  adBmsWakeupIc(tIC);
  status = spiSendCmd(UNMUTE);
  if(status != BMS_OK) return status;
  status = adBmsReadData(tIC, &ic[0], RDCFGB, Config, B);

  return status;
}

/* Function definition -------------------------------------------------------*/
/**
 * @brief adBms6815 set dcc discharge
 * 
 * @param tIC total number of ICs
 * @param ic structure to the IC handle
 * @retval BMS status
 */
bmsStatus_t adBms6815_set_dcc_discharge(uint16_t *cellNo, uint8_t tIC, cell_asic *ic)
{
  bmsStatus_t status = BMS_ERROR;

  if((cellNo < 0)      ||\
    (tIC <= 0)         ||\
    (NULL == ic))
	{
		return BMS_CMU_FUNCTION_PARAMETER_ERROR;
	}

  for(uint8_t cic = 0; cic < tIC; cic++)
  {
    /* Init config B */
    ic[cic].tx_cfgb.dcc = cellNo[cic];
  }

  adBmsWakeupIc(tIC);
  status = adBmsWriteData(tIC, &ic[0], WRCFGB, Config, B);
  if(status != BMS_OK) return status;
  status = adBmsReadData(tIC, &ic[0], RDCFGB, Config, B);

  return status;
}

/* Function definition -------------------------------------------------------*/
/**
 * @brief adBms6815 clear dcc discharge
 * 
 * @param tIC total number of ICs
 * @param ic structure to the IC handle
 * @retval BMS status
 */
bmsStatus_t adBms6815_clear_dcc_discharge(uint8_t tIC, cell_asic *ic)
{
  bmsStatus_t status = BMS_ERROR;

  if((tIC <= 0)         ||\
    (NULL == ic))
	{
		return BMS_CMU_FUNCTION_PARAMETER_ERROR;
	}

  for(uint8_t cic = 0; cic < tIC; cic++)
  {
    /* Init config B */
    ic[cic].tx_cfgb.dcc = 0;
  }

  adBmsWakeupIc(tIC);
  status = adBmsWriteData(tIC, &ic[0], WRCFGB, Config, B);
  if(status != BMS_OK) return status;
  status = adBmsReadData(tIC, &ic[0], RDCFGB, Config, B);

  return status;
}

/* Function definition -------------------------------------------------------*/
/**
 * @brief adBms6815 write read pwm duty cycle
 * 
 * @param tIC total number of ICs
 * @param ic structure to the IC handle
 * @param duty_cycle Duty cycle for pwm balancing
 * @retval BMS status
 */
bmsStatus_t adBms6815_write_read_pwm_duty_cycle(uint8_t tIC, cell_asic *ic, PWM_DUTY duty_cycle)
{
  bmsStatus_t status = BMS_ERROR;

  if((tIC <= 0)                     ||\
    (NULL == ic)                    ||\
    !(IS_CMU_PWM_DUTY(duty_cycle)))
	{
		return BMS_CMU_FUNCTION_PARAMETER_ERROR;
	}

  adBmsWakeupIc(tIC);
  SetPwmDutyCycle(tIC, &ic[0], duty_cycle);
  status = adBmsWriteData(tIC, &ic[0], WRPWM, Pwm, NONE);
  if(status != BMS_OK) return status;
  status = adBmsReadData(tIC, &ic[0], RDPWM, Pwm, NONE);

  return status;
}

/* Function definition -------------------------------------------------------*/
/**
 * @brief set pwm duty cycle
 * 
 * @param tIC total number of ICs
 * @param ic structure to the IC handle
 * @param duty_cycle Duty cycle for pwm balancing
 * @retval BMS status
 */
bmsStatus_t SetPwmDutyCycle(uint8_t tIC, cell_asic *ic, PWM_DUTY duty_cycle)
{
  bmsStatus_t status = BMS_ERROR;

  if((tIC <= 0)                     ||\
    (NULL == ic)                    ||\
    !(IS_CMU_PWM_DUTY(duty_cycle)))
	{
		return BMS_CMU_FUNCTION_PARAMETER_ERROR;
	}

  for(uint8_t curr_ic = 0; curr_ic < tIC; curr_ic++)
  {
    for(uint8_t pwmc = 0; pwmc < PWM; pwmc++)
    {
     ic[curr_ic].Pwm.pwm[pwmc] = duty_cycle;
    }
  }

  status = BMS_OK;
  return status;
}

/* Function definition -------------------------------------------------------*/
/**
 * @brief adBms6815 Parse Config for register pwm
 * 
 * @param tIC total number of ICs
 * @param ic structure to the IC handle
 * @param data pointer to data buffer
 * @retval BMS status
 */
bmsStatus_t adBms6815ParsePwm(uint8_t tIC, cell_asic *ic, uint8_t *data)
{
  uint8_t address = 0;
  bmsStatus_t status = BMS_ERROR;

  if((tIC <= 0)     ||\
    (NULL == ic)    ||\
    (NULL == data))
	{
		return BMS_CMU_FUNCTION_PARAMETER_ERROR;
	}

  for(uint8_t curr_ic = 0; curr_ic < tIC; curr_ic++)
  {
    memcpy(&ic[curr_ic].pwm.rx_data[0], &data[address], RX_DATA); /* dst , src , size */
    address = ((curr_ic+1) * (RX_DATA));
    ic[curr_ic].Pwm.pwm[0] = (ic[curr_ic].pwm.rx_data[0] & 0x0F);
    ic[curr_ic].Pwm.pwm[1] = ((ic[curr_ic].pwm.rx_data[0] & 0xF0) >> 4);
    ic[curr_ic].Pwm.pwm[2] = (ic[curr_ic].pwm.rx_data[1] & 0x0F);
    ic[curr_ic].Pwm.pwm[3] = ((ic[curr_ic].pwm.rx_data[1] & 0xF0) >> 4);
    ic[curr_ic].Pwm.pwm[4] = (ic[curr_ic].pwm.rx_data[2] & 0x0F);
    ic[curr_ic].Pwm.pwm[5] = ((ic[curr_ic].pwm.rx_data[2] & 0xF0) >> 4);
    ic[curr_ic].Pwm.pwm[6] = (ic[curr_ic].pwm.rx_data[3] & 0x0F);
    ic[curr_ic].Pwm.pwm[7] = ((ic[curr_ic].pwm.rx_data[3] & 0xF0) >> 4);
    ic[curr_ic].Pwm.pwm[8] = (ic[curr_ic].pwm.rx_data[4] & 0x0F);
    ic[curr_ic].Pwm.pwm[9] = ((ic[curr_ic].pwm.rx_data[4] & 0xF0) >> 4);
    ic[curr_ic].Pwm.pwm[10] = (ic[curr_ic].pwm.rx_data[5] & 0x0F);
    ic[curr_ic].Pwm.pwm[11] = ((ic[curr_ic].pwm.rx_data[5] & 0xF0) >> 4);
  }

  status = BMS_OK;
  return status;
}

/* Function definition -------------------------------------------------------*/
/**
 * @brief adBms6815 Create Config for register pwm
 * 
 * @param tIC total number of ICs
 * @param ic structure to the IC handle
 * @retval BMS status
 */
bmsStatus_t adBms6815CreatePwm(uint8_t tIC, cell_asic *ic)
{
  bmsStatus_t status = BMS_ERROR;

  if((tIC <= 0)     ||\
    (NULL == ic))
	{
		return BMS_CMU_FUNCTION_PARAMETER_ERROR;
	}

  for(uint8_t curr_ic = 0; curr_ic < tIC; curr_ic++)
  {
    ic[curr_ic].pwm.tx_data[0] = ((ic[curr_ic].Pwm.pwm[1] & 0x0F) << 4 | (ic[curr_ic].Pwm.pwm[0] & 0x0F));
    ic[curr_ic].pwm.tx_data[1] = ((ic[curr_ic].Pwm.pwm[3] & 0x0F) << 4 | (ic[curr_ic].Pwm.pwm[2] & 0x0F));
    ic[curr_ic].pwm.tx_data[2] = ((ic[curr_ic].Pwm.pwm[5] & 0x0F) << 4 | (ic[curr_ic].Pwm.pwm[4] & 0x0F));
    ic[curr_ic].pwm.tx_data[3] = ((ic[curr_ic].Pwm.pwm[7] & 0x0F) << 4 | (ic[curr_ic].Pwm.pwm[6] & 0x0F));
    ic[curr_ic].pwm.tx_data[4] = ((ic[curr_ic].Pwm.pwm[9] & 0x0F) << 4 | (ic[curr_ic].Pwm.pwm[8] & 0x0F));
    ic[curr_ic].pwm.tx_data[5] = ((ic[curr_ic].Pwm.pwm[11] & 0x0F) << 4 | (ic[curr_ic].Pwm.pwm[10] & 0x0F));
  }
  status = BMS_OK;
  return status;
}

/* Function definition -------------------------------------------------------*/
/**
 * @brief adBms6815 Parse Config for register comm
 * 
 * @param tIC total number of ICs
 * @param ic structure to the IC handle
 * @param data pointer to data buffer
 * @retval BMS status
 */
bmsStatus_t adBms6815ParseComm(uint8_t tIC, cell_asic *ic, uint8_t *data)
{
  bmsStatus_t status = BMS_ERROR;
  uint8_t address = 0;

  if((tIC <= 0)     ||\
    (NULL == ic)    ||\
    (NULL == data))
	{
		return BMS_CMU_FUNCTION_PARAMETER_ERROR;
	}

  for(uint8_t curr_ic = 0; curr_ic < tIC; curr_ic++)
  {
    memcpy(&ic[curr_ic].com.rx_data[0], &data[address], RX_DATA); /* dst , src , size */
    address = ((curr_ic+1) * (RX_DATA));
    ic[curr_ic].comm.icomm[0] = ((ic[curr_ic].com.rx_data[0] & 0xF0) >> 4);
    ic[curr_ic].comm.fcomm[0] = (ic[curr_ic].com.rx_data[0] & 0x0F);
    ic[curr_ic].comm.data[0] = (ic[curr_ic].com.rx_data[1]);
    ic[curr_ic].comm.icomm[1] = ((ic[curr_ic].com.rx_data[2] & 0xF0) >> 4);
    ic[curr_ic].comm.data[1] = (ic[curr_ic].com.rx_data[3]);
    ic[curr_ic].comm.fcomm[1] = (ic[curr_ic].com.rx_data[2] & 0x0F);
    ic[curr_ic].comm.icomm[2] = ((ic[curr_ic].com.rx_data[4] & 0xF0) >> 4);
    ic[curr_ic].comm.data[2] = (ic[curr_ic].com.rx_data[5]);
    ic[curr_ic].comm.fcomm[2] = (ic[curr_ic].com.rx_data[4] & 0x0F);
  }

  status = BMS_OK;
  return status;
}


/* Function definition -------------------------------------------------------*/
/**
 * @brief adBms6815 Create Config for register comm
 * 
 * @param tIC total number of ICs
 * @param ic structure to the IC handle
 * @retval BMS status
 */

bmsStatus_t adBms6815CreateComm(uint8_t tIC, cell_asic *ic)
{
  bmsStatus_t status = BMS_ERROR;

  if((tIC <= 0)         ||\
    (NULL == ic))
	{
		return BMS_CMU_FUNCTION_PARAMETER_ERROR;
	}

  for(uint8_t curr_ic = 0; curr_ic < tIC; curr_ic++)
  {
    ic[curr_ic].com.tx_data[0] = ((ic[curr_ic].comm.icomm[0] & 0x0F)  << 4  | (ic[curr_ic].comm.fcomm[0]   & 0x0F));
    ic[curr_ic].com.tx_data[1] = ((ic[curr_ic].comm.data[0] ));
    ic[curr_ic].com.tx_data[2] = ((ic[curr_ic].comm.icomm[1] & 0x0F)  << 4 ) | (ic[curr_ic].comm.fcomm[1]   & 0x0F);
    ic[curr_ic].com.tx_data[3] = ((ic[curr_ic].comm.data[1]));
    ic[curr_ic].com.tx_data[4] = ((ic[curr_ic].comm.icomm[2] & 0x0F)  << 4  | (ic[curr_ic].comm.fcomm[2]   & 0x0F));
    ic[curr_ic].com.tx_data[5] = ((ic[curr_ic].comm.data[2]));
  }

  status = BMS_OK;
  return status;
}
/* End of File ---------------------------------------------------------------*/
