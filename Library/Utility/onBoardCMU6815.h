/**
 * @file onBoardCMU6815.h
 * @author Arpit Gupta (arpit@euler)
 * @brief 
 * @version 0.1
 * @date 2022-04-09
 * 
 * @copyright Copyright (c) 2022
 * 
 */

/* Header Guard --------------------------------------------------------------*/
#ifndef ONBOARDCMU6815_H_
#define ONBOARDCMU6815_H_

/* Standard C includes -------------------------------------------------------*/
#include "stdint.h"
/* Private includes ----------------------------------------------------------*/
#include "bmsConfiguration.h"
/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/
#define CELL                12      /* Bms ic number of cell               */
#define CD                  12      /* Bms ic number of diagnostic cell    */
#define AUX                 9       /* Bms ic number of Aux                */
#define STAT                6       /* Bms ic number of STAT               */
#define PWM                 12      /* Bms ic number of Pwm channel        */
#define COMM                3       /* GPIO communication comm reg         */
#define RSID                6       /* Read sid byte                       */
#define TX_DATA             6       /* Bms tx data byte                    */
#define RX_DATA             8       /* Bms rx data byte                    */
#define CLR_FLAG            2       /* Clr Flag data byte                  */
#define RDCVALL_SIZE        26      /* RDCVALL data byte size              */
#define RDCDALL_SIZE        26      /* RDCDALL data byte size              */
#define RDASALL_SIZE        38      /* RDASALL data byte size              */

#define multiplierVolt      0.0001  /*ADBMS6815 Multiplier - 100uV         */
/* Private typedef -----------------------------------------------------------*/
/* ADBMS6815 config A register structure */
typedef struct
{
    uint8_t       refon   :1;
    uint8_t       adcopt  :1;
    uint8_t       ps      :2;
    uint8_t       cvmin   :2;
    uint8_t       mcal    :1;
    uint8_t       comm_bk :1;
    uint8_t       flag_d  :8;
    uint8_t       soakon  :1;
    uint8_t       owrng   :1;
    uint8_t       owa     :3;
    uint8_t       owc     :3;
    uint8_t       gpo     :7;
    uint8_t       gpi     :7;
    uint8_t       rev     :4;
}cfa_;

/* ADBMS6815 config B register structure */
typedef struct
{
    uint16_t  vuv     :16;
    uint16_t  vov     :16;
    uint8_t   dtmen   :1;
    uint8_t   dtrng   :1;
    uint8_t   dcto    :6;
    uint16_t  dcc     :16;
    uint8_t   mute_st :1;
}cfb_;


/* ADBMS6815 claer flag reg. data structure*/
typedef struct
{
    uint8_t       cl_med    :1;
    uint8_t       cl_ed     :1;
    uint8_t       cl_amed   :1;
    uint8_t       cl_aed    :1;
    uint8_t       cl_vduv   :1;
    uint8_t       cl_vdov   :1;
    uint8_t       cl_vauv   :1;
    uint8_t       cl_vaov   :1;
    uint8_t       cl_oscchk :1;
    uint8_t       cl_cpchk  :1;
    uint8_t       cl_thsd   :1;
    uint8_t       cl_tmode  :1;
    uint8_t       cl_sleep  :1;
    uint8_t       cl_comp   :1;
    uint8_t       cl_redf   :1;
}clrflg_;

/* Cell Voltage Data structure */
typedef struct
{
    uint16_t c_codes[CELL]; /* Cell Voltage Codes */
} cv_;

/* Cell Diagnostic Register Data structure */
typedef struct
{
    uint16_t dc_codes[CELL]; /* Cell Diagnostic Register Codes */
} dcv_;

/* Aux Voltage Data Structure*/
typedef struct
{
    uint16_t a_codes[AUX]; /* Aux Voltage Codes */
} ax_;

/* Clr Flag Data Structure*/
typedef struct
{
    uint8_t data[CLR_FLAG]; /* Clear flag array*/
} flag_;

/* Aux Open wire structure*/
typedef struct
{
    uint16_t adax_gpio[AUX];      /* Adax gpio voltage            */
    uint16_t axow_pd[AUX];        /* Axow pull down voltage       */
    uint16_t axow_pu[AUX];        /* Aux pull up voltage          */
    uint16_t delta_pu[AUX];       /* Aux delta pull up voltage    */
    uint16_t delta_pd[AUX];       /* Aux delta pull down voltage  */
    uint16_t delta_volt[AUX];     /* Delta voltage                */
} axopen_wire_;

/* Memory pattern structure*/
typedef struct
{
    uint16_t cvpg_pg01[CELL];
    uint16_t cdpg_pg01[CD];
    uint16_t axpg_pg01[AUX];
    uint16_t statpg_pg01[STAT];
    uint16_t cvpg_inv_pg01[CELL];
    uint16_t cdpg_inv_pg01[CD];
    uint16_t axpg_inv_pg01[AUX];
    uint16_t statpg_inv_pg01[STAT];
} mem_pattn_;

/* Status A register Data structure*/
typedef struct
{
    uint16_t  sc;
    uint16_t  itmp;
    uint16_t  va;
} sta_;

/* Status B register Data structure*/
typedef struct
{
    uint16_t vd;
    uint8_t c1uv          :1;
    uint8_t c2uv          :1;
    uint8_t c3uv          :1;
    uint8_t c4uv          :1;
    uint8_t c5uv          :1;
    uint8_t c6uv          :1;
    uint8_t c7uv          :1;
    uint8_t c8uv          :1;
    uint8_t c9uv          :1;
    uint8_t c10uv         :1;
    uint8_t c11uv         :1;
    uint8_t c12uv         :1;

    uint8_t c1ov          :1;
    uint8_t c2ov          :1;
    uint8_t c3ov          :1;
    uint8_t c4ov          :1;
    uint8_t c5ov          :1;
    uint8_t c6ov          :1;
    uint8_t c7ov          :1;
    uint8_t c8ov          :1;
    uint8_t c9ov          :1;
    uint8_t c10ov         :1;
    uint8_t c11ov         :1;
    uint8_t c12ov         :1;
    uint8_t oc_cntr;
} stb_;

/* Status C register Data structure*/
typedef struct
{
    uint16_t      otp_med     :1;
    uint8_t       otp_ed      :1;
    uint8_t       a_otp_med   :1;
    uint8_t       a_otp_ed    :1;
    uint8_t       vd_uvlo     :1;
    uint8_t       vd_ovhl     :1;
    uint8_t       va_uvlo     :1;
    uint8_t       va_ovhi     :1;
    uint8_t       oscchk      :1;
    uint8_t       cpchk       :1;
    uint8_t       thsd        :1;
    uint8_t       muxfail     :1;
    uint8_t       tmodechk    :1;
    uint8_t       sleep       :1;
    uint8_t       compchk     :1;
    uint8_t       redfail     :1;
    uint16_t      adol1;
    uint16_t      adol2;
} stc_;

/* Pwm register Data structure*/
typedef struct
{
    uint8_t pwm[PWM];
} pwm_;

/* COMM register Data structure*/
typedef struct
{
    uint8_t fcomm[COMM];
    uint8_t icomm[COMM];
    uint8_t data[COMM];
} com_;

/*SID Register Structure */
typedef struct
{
    uint8_t sid[RSID];
} sid_;

/* Transmit byte and recived byte data structure */
typedef struct
{
    uint8_t tx_data[TX_DATA];
    uint8_t rx_data[RX_DATA];
} ic_register_;

/* Command counter and pec error data structure */
typedef struct
{
    uint8_t cmd_cntr;
    uint8_t cfgr_pec;
    uint8_t cell_pec;
    uint8_t dcell_pec;
    uint8_t aux_pec;
    uint8_t stat_pec;
    uint8_t comm_pec;
    uint8_t pwm_pec;
    uint8_t sid_pec;
} cmdcnt_pec_;

/* Diagnostic test result data structure */
typedef struct
{
    uint8_t adol;
    uint8_t osc_mismatch;
    uint8_t mux_decoder;
    uint8_t digital_redundancy;
    uint8_t mux_chargepump;
    uint8_t thsd;
    uint8_t fusebits;
    uint8_t cellopen_wire[CELL];
    uint8_t auxopen_wire[AUX];
} diag_test_;

/* BMS ic main structure */
typedef struct
{
    cfa_          tx_cfga;
    cfa_          rx_cfga;
    cfb_          tx_cfgb;
    cfb_          rx_cfgb;
    clrflg_       clrflg_data;
    cv_           cell;
    dcv_          dcell;
    ax_           aux;
    sta_          stata;
    stb_          statb;
    stc_          statc;
    com_          comm;
    pwm_          Pwm;
    sid_          sid;
    ic_register_  configa;
    ic_register_  configb;
    ic_register_  stat;
    ic_register_  com;
    flag_         flag_tx;
    ic_register_  pwm;
    ic_register_  rsid;
    cmdcnt_pec_   cccrc;
    diag_test_    diag_result;
    axopen_wire_  ax_opwire;
    mem_pattn_    mem_pattn;
} cell_asic;

/* ADC modes */
typedef enum
{
    MODE_422Hz_TO_1KHz = 0,
    MODE_27KHz_TO_14KHz,
    MODE_7KHz_TO_3KHz,
    MODE_26Hz_TO_2KHz
}ADC_MODE;

/* GPIO Channel selection */
typedef enum
{
    ALL_CELLS = 0,
    CELL1_7,
    CELL2_8,
    CELL3_9,
    CELL4_10,
    CELL5_11,
    CELL6_12,
}CH;

/* GPIO Channel selection */
typedef enum
{
    GPIOS_VREF2_VREF3 = 0,
    GPIO1,
    GPIO2,
    GPIO3,
    GPIO4,
    GPIO5,
    GPIO6,
    GPIO7,
    VREF2_VREF3
}CHG;

/* Status Group Channel selection */
typedef enum
{
    SC_ITMP_VA_VD = 0,
    SC,
    ITMP,
    VA,
    VD
}CHST;

/* Pull Down/Up current during conversion */
typedef enum 
{ 
    PUP_DOWN = 0x0, 
    PUP_UP = 0x1 
} PUP_BIT;

/* Discharge permitted */
typedef enum 
{ 
    DCP_OFF = 0x0, 
    DCP_ON = 0x1
} DCP;

/* Memory pattern */
typedef enum 
{ 
    PG01 = 0x1, 
    PG10 = 0x2
} MEM_PG;

/* ############# AdBms6815 Mem bits ##################### */

/* Configuration Register A */

/* Refernece remains power up/down */
typedef enum  
{ 
    PWR_DOWN = 0x0, 
    PWR_UP = 0x1 
} REFON;

typedef enum  
{ 
    ADCOPT_OFF = 0x0, 
    ADCOPT_ON = 0x1 
} ADCOPT;

/* PS bits */
typedef enum
{
    PS0_ADC1_2 = 0,              /* Redundancy is applied sequentially to ADC1 and ADC2 digital paths during cell
                                    conversions and applied to ADC1 During AUX and STATUS Conversions*/
    PS1_ADC1,                  /* Redundancy is applied only to the ADC1 digital path*/
    PS2_ADC2,                  /* Redundancy is applied only to the ADC2 digital path*/
    PS3_ADC2,                  /* Redundancy is applied only to the ADC2 digital path*/
} PS;

/* CVMIN bits */
typedef enum
{
    CVMIN_DEFAULT = 0,         /* No threshold is enabled. The ?V results for CVOW are reported as measured,
                                    regardless of initial cell voltage (default).*/
    CVMIN_01,                  /* Initial cell voltage threshold = VREF1 × 1/4 (0.8 V typical).*/
    CVMIN_10,                  /* Initial cell voltage threshold = VREF1 × 3/8 (1.2 V typical).*/
    CVMIN_11,                  /* Initial cell voltage threshold = VREF1 × 1/2 (1.6 V typical)*/
} CVMIN;

/* Mcal bit */
typedef enum  
{ 
    MCAL_OFF = 0x0, 
    MCAL_ON = 0x1 
} MCAL;

/* Communication Break */
typedef enum  
{ 
    COMM_BK_OFF = 0x0, 
    COMM_BK_ON = 0x1 
} COMM_BK;

/* Fault flags */
typedef enum
{
    FLAG_D0 = 0,          /* FLAG_D[0]: forces oscillator counter fast (sets OSCCHK on ADC operation)*/
    FLAG_D1,              /* FLAG_D[1]: forces oscillator counter slow (sets OSCCHK on ADC operation)*/
    FLAG_D2,              /* FLAG_D[3:2]: no overvoltage, undervoltage, or CP detection diagnostic is forced*/
    FLAG_D3,              /* FLAG_D[3:2]: forces undervoltage and CP detection (sets VA_UVLO and VD_UVLO on ADC operation, sets CPCHK on ADAX)*/
                            /* FLAG_D[3:2]: forces overvoltage detection (sets VA_OVHI and VD_OVHI on ADC operation)*/
    FLAG_D4,              /* FLAG_D[4]: forces mux diagnostic to fail (sets MUXFAIL on DIAGN, sets COMPCHK on ADC operation and DIAGN)*/
    FLAG_D5,              /* FLAG_D[5]: forces digital redundancy check to fail (ADC result values receive digital redundancy error codes) and sets the REDFAIL bit on ADC operation*/
    FLAG_D6,              /* FLAG_D[6]: forces fuse ED/MED (sets A_OTP_ED and A_OTP_MED and OTP_ED and OTP_MED on ADC operation)*/
    FLAG_D7,              /* FLAG_D[7]: forces TMODE diagnostic flag (sets TMODECHK) and sets the THSD flag bit */
} FLAG_D;

typedef enum  
{ 
    FLAG_CLR = 0x0, 
    FLAG_SET = 0x1 
} CFGA_FLAG;

/* Enables or disable soak time for all commands */
typedef enum  
{ 
    SOAKON_CLR = 0x0, 
    SOAKON_SET = 0x1 
} SOAKON;

/* Set soak time range Long/Short */
// typedef enum  
// { 
//     SHORT = 0x0, 
//     LONG = 0x1 
// } OWRNG;

/* Open wire sokon time owc */
typedef enum  
{
    OWC0 = 0x0, 
    OWC1, 
    OWC2, 
    OWC3, 
    OWC4, 
    OWC5, 
    OWC6, 
    OWC7
} OWC;

/* Open wire sokon time owa */
typedef enum  
{
    OWA0 = 0x0, 
    OWA1, 
    OWA2, 
    OWA3, 
    OWA4, 
    OWA5, 
    OWA6, 
    OWA7
    } OWA;

/* GPO Pins */
typedef enum
{
    GPO1 = 0,
    GPO2,
    GPO3,
    GPO4,
    GPO5,
    GPO6,
    GPO7,
} GPO;

/*!
*  \enum GPIO
* GPIO: GPIO Pin Control.
*/
/* GPO Pin Control */
typedef enum  
{ 
    GPO_CLR = 0x0, 
    GPO_SET = 0x1 
} CFGA_GPO;

/* GPIO Pin Control */
typedef enum  
{ 
    PULL_DOWN_ON = 0x0, 
    PULL_DOWN_OFF = 0x1 
} GPIO;

/* Configuration Register B */

/* Enable Dis-charge Timer Monitor */
typedef enum  
{ 
    DTMEN_OFF = 0x0, 
    DTMEN_ON = 0x1 
} DTMEN;

/* Discharge Timer Range Setting */
typedef enum  
{ 
    RANG_0_TO_63_MIN = 0x0, 
    RANG_0_TO_16_8_HR = 0x1 
} DTRNG;

/* DCTO timeout values*/
typedef enum
{
    DCTO_TIMEOUT = 0,
    TIME_1MIN_OR_0_26HR,
    TIME_2MIN_OR_0_53HR,
    TIME_3MIN_OR_0_8HR,
    TIME_4MIN_OR_1_06HR,
    TIME_5MIN_OR_1_33HR,
    TIME_6MIN_OR_1_6HR,
  /* If required then add more time out value follow data sheet for more details*/
} DCTO;

/* DCC bits */
typedef enum
{
    DCC1 = 0,
    DCC2,
    DCC3,
    DCC4,
    DCC5,
    DCC6,
    DCC7,
    DCC8,
    DCC9,
    DCC10,
    DCC11,
    DCC12,
} DCC;

/* PWM Duty cycle */
typedef enum
{
    PWM_0_0_PCT = 0,      /* 0.0%  (default) */
    PWM_3_3_PCT,          /* 3.3%            */
    PWM_6_7_PCT,          /* 6.7%            */
    PWM_10_0_PCT,         /* 10.0%           */
    PWM_13_0_PCT,         /* 13.3%           */
    PWM_16_7_PCT,         /* 16.7%           */
    PWM_20_0_PCT,         /* 20.0%           */
    PWM_23_3_PCT,         /* 23.3%           */
    PWM_26_7_PCT,         /* 26.7%           */
    PWM_30_0_PCT,         /* 30.0%           */
    PWM_33_3_PCT,         /* 33.3%           */
    PWM_36_7_PCT,         /* 36.7%           */
    PWM_40_0_PCT,         /* 40.0%           */
    PWM_43_3_PCT,         /* 43.3%           */
    PWM_46_7_PCT,         /* 46.7%           */
    PWM_50_0_PCT,         /* 50.0%           */
} PWM_DUTY;

/* Discharge cell set and clear  */
typedef enum 
{ 
    DCC_BIT_CLR = 0x0, 
    DCC_BIT_SET = 0x1 
} DCC_BIT;

/* Clear flags status bit */
typedef enum  
{ 
    CL_FLAG_CLR = 0x0, 
    CL_FLAG_SET = 0x1 
} FLAG;

/*!
*  \enum General Enums
*/
typedef enum 
{ 
    ALL_GRP = 0x0, 
    A, 
    B, 
    C, 
    D, 
    NONE
} GRP;

typedef enum 
{ 
    Cell = 0x0, 
    Aux, 
    Status, 
    Pwm, 
    D_volt, 
    Config, 
    Comm, 
    SID, 
    Clrflag, 
    Rdcvall, 
    Rdcdall, 
    Rdasall
} TYPE;

typedef enum 
{ 
    DISABLED = 0x0, 
    ENABLED = 0x1
} LOOP_MEASURMENT;

typedef enum 
{ 
    PASS = 0x0, 
    FAIL 
} RESULT;

typedef enum 
{ 
    ADOL = 0x0, 
    OSC_MISMATCH, 
    MUX_DECODER, 
    DIGITAL_REDUNDANCY, 
    MUX_CHARGEPUMP, 
    THSD, 
    FUSEBITS
} DIAGNOSTIC_TYPE;

#define IS_CMU_COMMAND(VALUE)          (((VALUE) == true)        || ((VALUE) == false))
#define IS_CMU_DCC_BIT(VALUE)          (((VALUE) == DCC_BIT_CLR) || ((VALUE) == DCC_BIT_SET))

#define IS_CMU_GRP(VALUE)              (((VALUE) == ALL_GRP)    || ((VALUE) == A)      ||\
                                        ((VALUE) == B)          || ((VALUE) == C)      ||\
                                        ((VALUE) == D)          || ((VALUE) == NONE))

#define IS_CMU_TYPE(VALUE)             (((VALUE) == Cell)    || ((VALUE) == Aux)      ||\
                                        ((VALUE) == Status)  || ((VALUE) == Pwm)      ||\
                                        ((VALUE) == D_volt)  || ((VALUE) == Config)   ||\
                                        ((VALUE) == Comm)    || ((VALUE) == SID)      ||\
                                        ((VALUE) == Clrflag) || ((VALUE) == Rdcvall)  ||\
                                        ((VALUE) == Rdcdall) || ((VALUE) == Rdasall))

#define IS_CMU_CHG(VALUE)              (((VALUE) == GPIOS_VREF2_VREF3) || ((VALUE) == GPIO1)      ||\
                                        ((VALUE) == GPIO2) || ((VALUE) == GPIO3)                  ||\
                                        ((VALUE) == GPIO5) || ((VALUE) == GPIO6)                  ||\
                                        ((VALUE) == GPIO7) || ((VALUE) == VREF2_VREF3))

#define IS_CMU_PWM_DUTY(VALUE)         (((VALUE) == PWM_0_0_PCT)  || ((VALUE) == PWM_3_3_PCT)       ||\
                                        ((VALUE) == PWM_6_7_PCT)  || ((VALUE) == PWM_10_0_PCT)      ||\
                                        ((VALUE) == PWM_13_0_PCT) || ((VALUE) == PWM_16_7_PCT)      ||\
                                        ((VALUE) == PWM_20_0_PCT) || ((VALUE) == PWM_23_3_PCT)      ||\
                                        ((VALUE) == PWM_26_7_PCT) || ((VALUE) == PWM_30_0_PCT)      ||\
                                        ((VALUE) == PWM_33_3_PCT) || ((VALUE) == PWM_36_7_PCT)      ||\
                                        ((VALUE) == PWM_40_0_PCT) || ((VALUE) == PWM_43_3_PCT)      ||\
                                        ((VALUE) == PWM_46_7_PCT) || ((VALUE) == PWM_50_0_PCT))

#define IS_CMU_ADC_MODE(VALUE)         (((VALUE) == MODE_422Hz_TO_1KHz) || ((VALUE) == MODE_27KHz_TO_14KHz) ||\
                                        ((VALUE) == MODE_7KHz_TO_3KHz)  || ((VALUE) == MODE_26Hz_TO_2KHz))

#define IS_CMU_MEM_PG(VALUE)           (((VALUE) == PG01) || ((VALUE) == PG10))

#define IS_CMU_CHST(VALUE)             (((VALUE) == SC_ITMP_VA_VD) || ((VALUE) == SC) ||\
                                        ((VALUE) == ITMP)          || ((VALUE) == VA) ||\
                                        ((VALUE) == VD))

#define IS_CMU_PUP_BIT(VALUE)          (((VALUE) == PUP_DOWN) || ((VALUE) == PUP_UP))

#define IS_CMU_CH(VALUE)               (((VALUE) == ALL_CELLS) || ((VALUE) == CELL1_7)  ||\
                                        ((VALUE) == CELL2_8)   || ((VALUE) == CELL3_9)  ||\
                                        ((VALUE) == CELL4_10)  || ((VALUE) == CELL5_11) ||\
                                        ((VALUE) == CELL6_12))
/* External variables --------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Inline functions ----------------------------------------------------------*/

/* Function prototypes -------------------------------------------------------*/
bmsStatus_t adBms6815CreateConfiga(uint8_t tIC, cell_asic *ic);
bmsStatus_t adBms6815CreateConfigb(uint8_t tIC, cell_asic *ic);
bmsStatus_t adBms6815CreateClrFlagData(uint8_t tIC, cell_asic *ic);

bmsStatus_t adBms6815_init_configA_B(uint8_t tIC, cell_asic *ic, float OV_THRESHOLD, float UV_THRESHOLD);
bmsStatus_t adBms6815_read_config(uint8_t tIC, cell_asic *ic);
bmsStatus_t adBms6815_write_read_config(uint8_t tIC, cell_asic *ic);
bmsStatus_t adBms6815_start_adc_cell_voltage_conversion(uint8_t tIC);
bmsStatus_t adBms6815_read_cell_voltages(uint8_t tIC, cell_asic *ic);
bmsStatus_t adBms6815_start_aux_voltage_conversion(uint8_t tIC, cell_asic *ic);
bmsStatus_t adBms6815_read_aux_voltages(uint8_t tIC, cell_asic *ic);
bmsStatus_t adBms6815_read_status(uint8_t tIC, cell_asic *ic);
bmsStatus_t adBms6815_read_cd_voltages(uint8_t tIC, cell_asic *ic);
      
bmsStatus_t adBms6815_set_dcc_discharge(uint16_t *cellNo, uint8_t tIC, cell_asic *ic);
bmsStatus_t adBms6815_clear_dcc_discharge(uint8_t tIC, cell_asic *ic);

bmsStatus_t adBms6815_write_read_pwm_duty_cycle(uint8_t tIC, cell_asic *ic, PWM_DUTY duty_cycle);
bmsStatus_t adBms6815_enable_mute(uint8_t tIC, cell_asic *ic);
bmsStatus_t adBms6815_disable_mute(uint8_t tIC, cell_asic *ic);

bmsStatus_t adBms6815ParseComm(uint8_t tIC, cell_asic *ic, uint8_t *data);
bmsStatus_t adBms6815CreateComm(uint8_t tIC, cell_asic *ic);

#endif
/* End of File ---------------------------------------------------------------*/

