/**
 * @file analogControls.h
 * @brief 
 * @version 0.1
 * @date 2022-04-09
 * 
 * @copyright Copyright (c) 2022
 * 
 */

/* Header Guard --------------------------------------------------------------*/
#ifndef BATTERYCOOLING_H_
#define BATTERYCOOLING_H_

/* Standard C includes -------------------------------------------------------*/

/* Private includes ----------------------------------------------------------*/
#include "threadxMain.h"

/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/
#define HEATING_ELEM_OUT_PIN   GPIO_PIN_0
#define HEATING_ELEM_OUT_PORT  GPIOA
#define HEATING_ELEM_PIN       GPIO_PIN_1
#define HEATING_ELEM_PORT      GPIOA
#define AR_FAN_PIN             GPIO_PIN_2
#define AR_FAN_PORT            GPIOA
#define AR_PUMP_PIN            GPIO_PIN_2
#define AR_PUMP_PORT           GPIOA
#define AR_MICRO_PIN           GPIO_PIN_3
#define AR_MICRO_PORT          GPIOA

/* Private typedef -----------------------------------------------------------*/
typedef enum{
    OFF =0,
    AR100 = 1,
    AR200 = 2,
} coolingSystemType;

typedef enum ArcReactor_en
{
	OFF_STATE     = 0,
	COOLING_STATE = 1,
	REVERSE_STATE = 2,
	HEATING_STATE = 3,
} ArcReactor_en;

typedef struct bcsParameters_t
{
    coolingSystemType coolingType;
	// ARC Reactor parameters
	ArcReactor_en ar_Status;
	uint8_t ar_enable;
	uint8_t ar_cooling_temp;
	uint8_t ar_heating_temp;
	uint8_t ar_cooling_temp_charging;
	uint8_t ar_heating_temp_charging;
	uint8_t ar_cartrage_temp;
	uint8_t displaySoC;
	uint8_t cellTempMax;
	int8_t  cellTempMin;
	
}bcsParameters_t;

// bcsParameters_t bcsParameters;

/* External variables --------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Inline functions ----------------------------------------------------------*/

/* Function prototypes -------------------------------------------------------*/

void thermal_system_set();
void thermal_system_set_charging();
void disable_thermal_system();

void enable_pump();
void disable_pump();
void enable_pelteir();
void disable_pelteir();
void enable_ar_fan();
void disable_ar_fan();
void enable_heating_element();
void disable_heating_element();
void enable_reverse_polarity();
void disable_reverse_polarity();
void enable_thermal_system_cool();
void enable_thermal_system_heat();



#endif
/* End of File ---------------------------------------------------------------*/

