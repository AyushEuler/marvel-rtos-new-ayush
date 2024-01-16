/**
 * @file bmsConfiguration.h
 * @author Arpit Gupta (arpit@euler)
 * @brief 
 * @version 0.1
 * @date 2022-04-15
 * 
 * @copyright Copyright (c) 2022
 * 
 */

/* Header Guard --------------------------------------------------------------*/
#ifndef BMSCONFIGURATION_H_
#define BMSCONFIGURATION_H_

/* Standard C includes -------------------------------------------------------*/

/* Private includes ----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

/**************** Configuration Selection ------------------------------------------------------------*/
#define THREADX
#define LTC2949
#define DEBUG_CAN
// #define ENABLE_LTC2949_SHORT_CIRCUIT
// #define ADS1120     /* External 16 Bit ADC for current sensing*/

#define CAN_MATRIX_V_0_3            // 3WL5 CAN v0.3
// #define CAN_CHARGER_EXTENDED_ID
// #define SOC_IN_FLASH


// #define EEPROM_TEST
// #define FUNCTIONALTESTS

/*************************** HARDWARE VERSION SELECTION **************************/

#define HARDWARE_VERSION_3_1_1
// #define HARDWARE_VERSION_3_0

/*************************** Debug ***************************/

// #define WWDOG
// #define EKF
#define LTC2949_DEBUG
// #define DEBUG

/*************************** Tests ***************************/

// #define CBMS_TEST

// #define OTD_TEST_EXTERNAL
// #define OCC_TEST
// #define OCD_TEST

/**************** Configuration Selection End ------------------------------------------------------------*/


/**************** Battery Parameters Selection ------------------------------------------------------------*/
// #define HILOAD_ONE_72V
//#define HILOAD_ONE_64V
//#define HILOAD_ONE_48V
//#define STORM_ONE

#define HILOAD_ONE_LFP
// #define STORM_ONE_LFP
// #define CELL_VOLTAGES_LIMITS_OLD
//#define CELL_VOLTAGES_LIMITS_NEW
#define CELL_VOLTAGES_LFP

/**************** Battery Parameters End ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/
#define MARVEL_HW_VER                   32
#define MARVEL_FW_VER                   04                                               //Aug22Release1
#define MARVEL_CONFIG                   1
// #define MARVEL_PART_NO                  AH900016                                          //4W-/0 - 1-Byte

#define SW_MAJOR_VERSION                0
#define SW_MINOR_VERSION                4
#define SW_PATCH_VERSION                1
#define SW_CONFIG_VERSION               1
#define HW_MAJOR_VERSION                3
#define HW_MINOR_VERSION                2


// #define DHT11_SENSOR
#define FUNCTIONALTESTS

/* configure cell battery parameters based on cell chemistry and vehicle type*/
#ifdef HILOAD_ONE_72V
    #define NUMBER_OF_CMU                   2                                                   //48S Config
    #define MAX_CELL_PER_SLAVE              12                                                  //CMU-ADBMS6815
    #define CELL_IN_SERIES                  20                                                  //20S

    #define PACK_MAX_CAPACITY_Ah            180                                                 //Ah
    #define CURRENT_LIMIT_CHARGE            100                                                 //Amp
    #define CURRENT_LIMIT_REGEN             120                       9                          //Amp
    #define CURRENT_LIMIT_DISCHARGE         200                                                 //Amp

    #define PRECHARGE_TIMEOUT               1                                                   //seconds
#endif

#ifdef HILOAD_ONE_64V
    #define NUMBER_OF_CMU                   2                                                   //48S Config
    #define MAX_CELL_PER_SLAVE              12                                                  //CMU-ADBMS6815
    #define CELL_IN_SERIES                  18                                                  //20S

    #define PACK_MAX_CAPACITY_Ah            200                                                 //Ah
    #define CURRENT_LIMIT_CHARGE            100                                                 //Amp
    #define CURRENT_LIMIT_REGEN             120                                                 //Amp
    #define CURRENT_LIMIT_DISCHARGE         200                                                 //Amp

    #define PRECHARGE_TIMEOUT               1                                                   //seconds
#endif

#ifdef HILOAD_ONE_48V
    #define NUMBER_OF_CMU                   2                                                   //48S Config
    #define MAX_CELL_PER_SLAVE              12                                                  //CMU-ADBMS6815
    #define CELL_IN_SERIES                  14                                                  //20S

    #define PACK_MAX_CAPACITY_Ah            188                                                 //Ah
    #define CURRENT_LIMIT_CHARGE            100                                                 //Amp
    #define CURRENT_LIMIT_REGEN             120                                                 //Amp
    #define CURRENT_LIMIT_DISCHARGE         200                                                 //Amp

    #define PRECHARGE_TIMEOUT               1                                                   //seconds
#endif

#ifdef STORM_ONE
    #define NUMBER_OF_CMU                   4                                                   //48S Config
    #define MAX_CELL_PER_SLAVE              12                                                  //CMU-ADBMS6815
    #define CELL_IN_SERIES                  42                                                  //42S

    #define PACK_MAX_CAPACITY_Ah            120                                                 //Ah
    #define CURRENT_LIMIT_CHARGE            100                                                 //Amp
    #define CURRENT_LIMIT_REGEN             130                                                 //Amp
    #define CURRENT_LIMIT_DISCHARGE         200                                                 //Amp

    #define PRECHARGE_TIMEOUT               5                                                   //seconds
#endif

#ifdef STORM_ONE_LFP
    #define NUMBER_OF_CMU                   4                                                   //48S Config
    #define MAX_CELL_PER_SLAVE              12                                                  //CMU-ADBMS6815
    #define CELL_IN_SERIES                  45                                                  //20S

    #define PACK_MAX_CAPACITY_Ah            180                                                 //Ah
    
    //
    #define CURRENT_LIMIT_CHARGE            200                                                 //Amp
    #define CURRENT_LIMIT_REGEN             120                                                 //Amp
    #define CURRENT_LIMIT_DISCHARGE         205                                                 //Amp
    #define PRECHARGE_TIMEOUT               5                                                   //seconds
#endif

/* configure cell voltages and temperature parameters based on cell chemistry*/
#ifdef CELL_VOLTAGES_LIMITS_OLD
    #define CELL_MAX_VOLTAGE_THRESHOLD_mV           4150                                               //Volts
    #define CELL_MIN_VOLTAGE_THRESHOLD_mV           3100                                                //Volts
    #define CELL_BALANCING_START_VOLTAGE_mV         4050                                               //Volts 
    #define CELL_MAX_TEMP_THRESHOLD_C               50                                                  //degC
    #define CELL_MIN_TEMP_THRESHOLD_C               10                                                  //degC    
#endif

#ifdef CELL_VOLTAGES_LIMITS_NEW
    #define CELL_MAX_VOLTAGE_THRESHOLD_mV           4150                                                 //Volts
    #define CELL_MIN_VOLTAGE_THRESHOLD_mV           3150                                                 //Volts
    #define CELL_BALANCING_START_VOLTAGE_mV         4050                                                 //Volts   
    #define CELL_MAX_TEMP_THRESHOLD_C               50                                                  //degC
    #define CELL_MIN_TEMP_THRESHOLD_C               10                                                  //degC                                     
#endif                                             

#ifdef CELL_VOLTAGES_LFP
    //cell voltage
    #define CELL_MAX_VOLTAGE_THRESHOLD_mV            3430                                                 //Volts
    #define CELL_MIN_VOLTAGE_THRESHOLD_mV            2900                                                 //Volts
    #define CELL_BALANCING_START_VOLTAGE_mV          3200                                                 //Volts   
    #define CELL_MAX_TEMP_THRESHOLD_C                45                                                   //degC
    #define CELL_MIN_TEMP_THRESHOLD_C                00                                                   //degC    
#endif

#ifdef HILOAD_ONE_LFP
    #define NUMBER_OF_CMU                       2                                                   //48S Config
    #define CELL_IN_SERIES                      20                                                  //20S
    
    //cell per slave
    #define MAX_CELL_PER_SLAVE                  12                                                  //CMU-ADBMS6815
    #define CELL_IN_SLAVE_1                     10 
    #define CELL_IN_SLAVE_2                     10 
    #define CELL_IN_SLAVE_3                     0 
    #define CELL_IN_SLAVE_4                     0 
    #define CELL_IN_SLAVE_5                     0 
    #define CELL_IN_SLAVE_6                     0 
    #define CELL_IN_SLAVE_7                     0 
    #define CELL_IN_SLAVE_8                     0 
    #define CELL_IN_SLAVE_9                     0 
    #define CELL_IN_SLAVE_10                    0 
    #define CELL_IN_SLAVE_11                    0 
    #define CELL_IN_SLAVE_12                    0 

    #define CELL_IMBALANCE_THRESHOLD_mV         10                                                  //milliVolts

    //current
    #define MAX_CHARGE_CURRENT_THRESHOLD_A      180
    #define MAX_DISCHARGE_CURRENT_THRESHOLD_A   200

    //capacity
    #define PACK_MAX_CAPACITY_Ah                200  
    #define PACK_MIN_CAPACITY_Ah                1                                                 //Ah
    #define CURRENT_LIMIT_DISCHARGE             205                                                 //Amp


#endif

/* configure CAN matix V2 parameters*/
#ifdef CAN_MATRIX_V_0_3
    #define MAX_ALLOWED_CELL_VOLTAGE    ((CELL_MAX_VOLTAGE_THRESHOLD_mV))
    #define MAX_ALLOWED_PACK_VOLTAGE    (((CELL_IN_SERIES) * (CELL_MAX_VOLTAGE_THRESHOLD_mV)) * 0.001)  //Volts
    #define MAX_ALLOWED_CELL_TEMP       CELL_MAX_TEMP_THRESHOLD_C
    #define MAX_ALLOWED_CURRENT         CURRENT_LIMIT_DISCHARGE
#endif

#define MAX_PACK_VOLTAGE_THRESHOLD      ((CELL_MAX_VOLTAGE_THRESHOLD_mV * CELL_IN_SERIES) * 0.001)         //Volts
#define MIN_PACK_VOLTAGE_THRESHOLD      ((CELL_MIN_VOLTAGE_THRESHOLD_mV * CELL_IN_SERIES) * 0.001)         //Volts

#define MAX_NUM_CMU                     12                                                  //144S

//ntc
#define EXTERNAL_NTC_PER_SLAVE          4                                                   //CMU-ADBMS6815
#define INTERNAL_NTC_PER_SLAVE          3                                                   //CMU-ADBMS6815
#define NTC_BETA_VALUE_AT_25C           3988                                                //TDK Sensor

//shunt
#define SHUNT_RESISTOR_uOhm             167                                                 //micro-Ohm   //50mV/300A

//timeouts
#define TIME_100ms                      100
#define TIME_200ms                      200
#define ERROR_TIMEOUT_ms                500
#define WARNING_TIMEOUT_ms              500
#define RECOVERY_TIMEOUT_ms             500

//board temp
#define BOARD_MAX_TEMP_THRESHOLD_C      90                                                   //degC
#define BOARD_MIN_TEMP_THRESHOLD_C      00 

//protection threshold
#define MAX_ALLOWED_TEMP_CHANGE_C       50
#define MAX_ALLOWED_TEMP_GRADIENT_C     50
#define HIGH_IMBALANCE_ERROR_mV         500

//balancing
#define BALANCING_OFF_TIME_ms           500
#define BALANCING_ON_TIME_ms            100

//contactor
#define CONTACTOR_CUT_OFF_TIME_ms       100
#define PRE_CHARGE_CURRENT_mA           200
    
//cc
#define VEHICLE_REST_TIME_m             30                         
#define VEHICLE_REST_CURRENT_A          5

//data pipeline
#define CHARGE_CURRENT_DETECTION_THRESHOLD_A    5   


/*************** FLASH CONFIG DATA RANGE MAX MIN VALUE START  ***************************************************************/

#define NUMBER_OF_CMU_MIN_VALUE								1
#define NUMBER_OF_CMU_MAX_VALUE								12

#define CELL_IN_SERIES_MIN_VALUE							1
#define CELL_IN_SERIES_MAX_VALUE							144

//cell per slave
#define MAX_CELL_PER_SLAVE_MIN_VALUE						1
#define MAX_CELL_PER_SLAVE_MAX_VALUE						24

#define CELL_IN_SLAVE_MIN_VALUE								1
#define CELL_IN_SLAVE_MAX_VALUE								24

//ntc
#define EXTERNAL_NTC_PER_SLAVE_MIN_VALUE					1
#define EXTERNAL_NTC_PER_SLAVE_MAX_VALUE					12

#define INTERNAL_NTC_PER_SLAVE_MIN_VALUE					1
#define INTERNAL_NTC_PER_SLAVE_MAX_VALUE					12

#define NTC_BETA_VALUE_AT_25C_MIN_VALUE						1
#define NTC_BETA_VALUE_AT_25C_MAX_VALUE						10000

//shunt
#define SHUNT_RESISTOR_uOhm_MIN_VALUE						1
#define SHUNT_RESISTOR_uOhm_MAX_VALUE						10000

//cell voltage
#define CELL_MAX_VOLTAGE_THRESHOLD_mV_MIN_VALUE				1
#define CELL_MAX_VOLTAGE_THRESHOLD_mV_MAX_VALUE				7000
#define CELL_MIN_VOLTAGE_THRESHOLD_mV_MIN_VALUE				1
#define CELL_MIN_VOLTAGE_THRESHOLD_mV_MAX_VALUE				7000
#define CELL_BALANCING_START_VOLTAGE_mV_MIN_VALUE			1
#define CELL_BALANCING_START_VOLTAGE_mV_MAX_VALUE			7000
#define CELL_IMBALANCE_THRESHOLD_mV_MIN_VALUE				1
#define CELL_IMBALANCE_THRESHOLD_mV_MAX_VALUE				5000

//current
#define MAX_CHARGE_CURRENT_THRESHOLD_A_MIN_VALUE			1
#define MAX_CHARGE_CURRENT_THRESHOLD_A_MAX_VALUE			1000
#define MAX_DISCHARGE_CURRENT_THRESHOLD_A_MIN_VALUE			1
#define MAX_DISCHARGE_CURRENT_THRESHOLD_A_MAX_VALUE			1000

//capacity
#define PACK_MAX_CAPACITY_Ah_MIN_VALUE						1
#define PACK_MAX_CAPACITY_Ah_MAX_VALUE						1000
#define PACK_MIN_CAPACITY_Ah_MIN_VALUE						1
#define PACK_MIN_CAPACITY_Ah_MAX_VALUE						1000

//cell temp
#define CELL_MAX_TEMP_THRESHOLD_C_MIN_VALUE					1
#define CELL_MAX_TEMP_THRESHOLD_C_MAX_VALUE					100
#define CELL_MIN_TEMP_THRESHOLD_C_MIN_VALUE					1
#define CELL_MIN_TEMP_THRESHOLD_C_MAX_VALUE					100

//board temp
#define BOARD_MAX_TEMP_THRESHOLD_C_MIN_VALUE				1
#define BOARD_MAX_TEMP_THRESHOLD_C_MAX_VALUE				100
#define BOARD_MIN_TEMP_THRESHOLD_C_MIN_VALUE				1
#define BOARD_MIN_TEMP_THRESHOLD_C_MAX_VALUE				100

//timeout
#define ERROR_TIMEOUT_ms_MIN_VALUE							1
#define ERROR_TIMEOUT_ms_MAX_VALUE							10000
#define WARNING_TIMEOUT_ms_MIN_VALUE						1
#define WARNING_TIMEOUT_ms_MAX_VALUE						10000
#define RECOVERY_TIMEOUT_ms_MIN_VALUE						1
#define RECOVERY_TIMEOUT_ms_MAX_VALUE						10000

//protection threshold
#define MAX_ALLOWED_TEMP_CHANGE_C_MIN_VALUE					1
#define MAX_ALLOWED_TEMP_CHANGE_C_MAX_VALUE					100
#define MAX_ALLOWED_TEMP_GRADIENT_C_MIN_VALUE				1
#define MAX_ALLOWED_TEMP_GRADIENT_C_MAX_VALUE				100
#define HIGH_IMBALANCE_ERROR_mV_MIN_VALUE					1
#define HIGH_IMBALANCE_ERROR_mV_MAX_VALUE					1000

//balancing
#define BALANCING_OFF_TIME_ms_MIN_VALUE						1
#define BALANCING_OFF_TIME_ms_MAX_VALUE						10000
#define BALANCING_ON_TIME_ms_MIN_VALUE						1
#define BALANCING_ON_TIME_ms_MAX_VALUE						10000

//contactor
#define CONTACTOR_CUT_OFF_TIME_ms_MIN_VALUE					1
#define CONTACTOR_CUT_OFF_TIME_ms_MAX_VALUE					10000
#define PRE_CHARGE_CURRENT_mA_MIN_VALUE						1
#define PRE_CHARGE_CURRENT_mA_MAX_VALUE						10000   //10 A
	
//cc
#define VEHICLE_REST_TIME_m_MIN_VALUE						1
#define VEHICLE_REST_TIME_m_MAX_VALUE						300     //5 hours
#define VEHICLE_REST_CURRENT_A_MIN_VALUE					1
#define VEHICLE_REST_CURRENT_A_MAX_VALUE					20

//data pipeline
#define CHARGE_CURRENT_DETECTION_THRESHOLD_A_MIN_VALUE		1
#define CHARGE_CURRENT_DETECTION_THRESHOLD_A_MAX_VALUE		20

//ARC REACTOR
#define AR_CARTRAGE_TEMP                                    0
#define AR_COOLING_TEMP                                     0
#define AR_COOLING_TEMP_CHARGING                            0
#define AR_ENABLE                                           0
#define AR_HEATING_TEMP                                     0
#define AR_HEATING_TEMP_CHARGING                            0
#define AR_COOLING_TYPE                                     0

/*************** FLASH CONFIG DATA RANGE MAX MIN VALUE END  ***************************************************************/

//-- 
#define BLEEDING_CURRENT_mA                                 200
#define CC_TIMEOUT_mS                                       500
#define DEBOUNCING_TIMEOUT_mS                               1000
#define SENSOR_ACCURACY                                     100       //percentage
#define SENSOR_OFFSET_mA                                    0
#define CAPACITY_DELTA_CHECK_mAh                            10   
#define EQUIVALENT_CYCLE_CAPACITY                           360000  

/*************** RETRY VALUE START  ***************************************************************************************/

#define RETRY(RETRY_MAX_COUNT, RETRY_STATUS, CODE_BLOCK) \
    do { \
        uint8_t retryCount; \
        for (retryCount = 0; retryCount < RETRY_MAX_COUNT; retryCount++) \
        { \
            RETRY_STATUS = CODE_BLOCK; \
        } \
    } while (0) 

            // if (RETRY_STATUS == BMS_OK) \
            //     break; \

#define RETRY_PRIORITY_ZERO         5
#define RETRY_PRIORITY_ONE          4
#define RETRY_PRIORITY_TWO          3
#define RETRY_PRIORITY_THREE        2
#define RETRY_PRIORITY_FOUR         1
#define RETRY_PRIORITY_FIVE         0

#define RETRY_CRC_ERROR             RETRY_PRIORITY_ZERO
#define RETRY_CMU_READ              RETRY_PRIORITY_ZERO
#define RETRY_CCM_READ              RETRY_PRIORITY_ZERO
#define RETRY_FLASH_READ            RETRY_PRIORITY_ZERO
#define RETRY_CAN_COMMUNICATION     RETRY_PRIORITY_ZERO
#define RETRY_OTA                   RETRY_PRIORITY_ZERO

#define RETRY_CAN_INIT              RETRY_PRIORITY_ZERO
#define RETRY_CMU_INIT              RETRY_PRIORITY_ZERO
#define RETRY_CCM_INIT              RETRY_PRIORITY_ZERO

#define RETRY_SHORT_CIRCUIT         RETRY_PRIORITY_FOUR

/*************** RETRY VALUE END  ***************************************************************/

/*************** BALANCING DERATING START  ***************************************************************/

#define BALANCING_DERATING_START_TEMP_C         80
#define BALANCING_DERATING_END_TEMP_C           90

#define BALANCING_MAX_ON_TIME_ms                400
#define BALANCING_MIN_ON_TIME_ms                100

#define BALANCING_MAX_OFF_TIME_ms               200
#define BALANCING_MIN_OFF_TIME_ms               100

/*************** BALANCING DERATING START  ***************************************************************/

#define CHECK_STATUS(x)  if(x!=0)return(x)

/* Private typedef -----------------------------------------------------------*/
/**
  * @brief  BMS Status structure definition
  */
typedef enum bmsStatus
{
  BMS_OK = 0,
  BMS_ERROR,
  BMS_BUSY,
  BMS_TIMEOUT,
  BMS_DYNAMIC_MEM_ALLOC_ERROR,
  BMS_CMU_PEC10_ERROR,
  BMS_CMU_NOT_READY,
  BMS_CCM_PEC15_ERROR,
  BMS_CCM_TIMEOUT,
  BMS_EEPROM_WRITE_FAILED,
  BMS_EEPROM_READ_FAILED,
  BMS_FLASH_CRC_NOT_EQUAL,
  BMS_FLASH_EXCEPTION_DATA_NOT_FOUND,
  BMS_FLASH_CONFIG_DATA_OUT_OF_RANGE,
  BMS_FLASH_CONFIG_DATA_OUT_OF_INDEX,
  BMS_FLASH_CONFIG_RECEIVED_DATA_IS_SAME,
  BMS_FUNCTION_PARAMETER_ERROR,
  BMS_CAN_FUNCTION_PARAMETER_ERROR,
  BMS_SPI_FUNCTION_PARAMETER_ERROR,
  BMS_I2C_FUNCTION_PARAMETER_ERROR,
  BMS_UART_FUNCTION_PARAMETER_ERROR,
  BMS_GPIO_FUNCTION_PARAMETER_ERROR,
  BMS_TIMER_FUNCTION_PARAMETER_ERROR,
  BMS_WATCH_DOG_FUNCTION_PARAMETER_ERROR,
  BMS_EEPROM_FUNCTION_PARAMETER_ERROR,
  BMS_FLASH_FUNCTION_PARAMETER_ERROR,
  BMS_MEMORY_FUNCTION_PARAMETER_ERROR,
  BMS_CRC_FUNCTION_PARAMETER_ERROR,
  BMS_CONVERSION_FUNCTION_PARAMETER_ERROR,
  BMS_CMU_FUNCTION_PARAMETER_ERROR,
  BMS_CCM_FUNCTION_PARAMETER_ERROR,
  BMS_FILTER_FUNCTION_PARAMETER_ERROR,
  BMS_DATA_PROCESSING_FUNCTION_PARAMETER_ERROR,
  BMS_COULOMB_COUNTING_FUNCTION_PARAMETER_ERROR,
  BMS_CC_EKF_FUNCTION_PARAMETER_ERROR,
  BMS_THRESHOLD_FUNCTION_PARAMETER_ERROR,
  BMS_CELL_BALANCING_FUNCTION_PARAMETER_ERROR,
  BMS_STATE_MACHINE_FUNCTION_PARAMETER_ERROR,
  BMS_CHARGING_FUNCTION_PARAMETER_ERROR,
  BMS_CONTACTOR_FUNCTION_PARAMETER_ERROR,
  BMS_PROTECTION_FUNCTION_PARAMETER_ERROR,
  BMS_DATA_PIPELINE_FUNCTION_PARAMETER_ERROR,
  BMS_COTA_UPDATE_ERROR
} bmsStatus_t;

typedef enum
{
    OTC_ERROR_TEMPERATURE_INDEX,
    OTC_WARNING_TEMPERATURE_INDEX,
    OTC_RECOVERY_TEMPERATURE_INDEX,

    OTD_ERROR_TEMPERATURE_INDEX,
    OTD_WARNING_TEMPERATURE_INDEX,
    OTD_RECOVERY_TEMPERATURE_INDEX,

    UTC_ERROR_TEMPERATURE_INDEX,
    UTC_WARNING_TEMPERATURE_INDEX,
    UTC_RECOVERY_TEMPERATURE_INDEX,

    UTD_ERROR_TEMPERATURE_INDEX,
    UTD_WARNING_TEMPERATURE_INDEX,
    UTD_RECOVERY_TEMPERATURE_INDEX

}temperatureLimitsIndex_t;


/* EXTERNAL - Error condition will be generated externally. INTERNAL - System is already in error condition. */

#define OCC_ERROR_CURRENT_A       185
#define OCC_WARNING_CURRENT_A     180

#define OCD_ERROR_CURRENT_A       185
#define OCD_WARNING_CURRENT_A     180

/*-------------------------  Group 1  -----------------------------------------------------------*/

#define OTC_ERROR_TEMPERATURE_GROUP_1           55
#define OTC_WARNING_TEMPERATURE_GROUP_1         52
#define OTC_RECOVERY_TEMPERATURE_GROUP_1        50

#define OTD_ERROR_TEMPERATURE_GROUP_1           55
#define OTD_WARNING_TEMPERATURE_GROUP_1         52
#define OTD_RECOVERY_TEMPERATURE_GROUP_1        50

#define UTC_ERROR_TEMPERATURE_GROUP_1           0
#define UTC_WARNING_TEMPERATURE_GROUP_1         2
#define UTC_RECOVERY_TEMPERATURE_GROUP_1        5

#define UTD_ERROR_TEMPERATURE_GROUP_1           0
#define UTD_WARNING_TEMPERATURE_GROUP_1         2
#define UTD_RECOVERY_TEMPERATURE_GROUP_1        5


/*-------------------------  Group 2  -----------------------------------------------------------*/

#define OTC_ERROR_TEMPERATURE_GROUP_2           95
#define OTC_WARNING_TEMPERATURE_GROUP_2         92
#define OTC_RECOVERY_TEMPERATURE_GROUP_2        90

#define OTD_ERROR_TEMPERATURE_GROUP_2           95
#define OTD_WARNING_TEMPERATURE_GROUP_2         92
#define OTD_RECOVERY_TEMPERATURE_GROUP_2        90

#define UTC_ERROR_TEMPERATURE_GROUP_2           0
#define UTC_WARNING_TEMPERATURE_GROUP_2         2
#define UTC_RECOVERY_TEMPERATURE_GROUP_2        5

#define UTD_ERROR_TEMPERATURE_GROUP_2           0
#define UTD_WARNING_TEMPERATURE_GROUP_2         2
#define UTD_RECOVERY_TEMPERATURE_GROUP_2        5


/* External variables --------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Inline functions ----------------------------------------------------------*/

/* Function prototypes -------------------------------------------------------*/

#endif
/* End of File ---------------------------------------------------------------*/
