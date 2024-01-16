/**

/* Standard C includes -------------------------------------------------------*/
#include "main.h"
/* Private includes ----------------------------------------------------------*/
#include "batteryCooling.h"

/* Self include --------------------------------------------------------------*/

/* Type definiton ------------------------------------------------------------*/

/* External variables definiton ----------------------------------------------*/
struct bcsParameters_t bcsParameters;


/* Private constants ---------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

/* Private custom code -------------------------------------------------------*/

/* Function definition -------------------------------------------------------*/
/**
/**
 * @brief Set state of thermal system to heating, cooling, reverse or OFF
 *
 */
void thermal_system_set()
{
	if(bcsParameters.coolingType!=OFF)
	{
		if (bcsParameters.ar_enable == 0) {
			disable_thermal_system();
		}
		else if (bcsParameters.cellTempMax > bcsParameters.ar_cooling_temp || (bcsParameters.ar_Status == COOLING_STATE && bcsParameters.cellTempMax > (bcsParameters.ar_cooling_temp - bcsParameters.ar_cooling_temp / 10))) {
			//cooling
			if(bcsParameters.displaySoC > 20.0)
				enable_thermal_system_cool();
			else
				disable_thermal_system();
		}
		else if (bcsParameters.cellTempMin < bcsParameters.ar_heating_temp || (bcsParameters.ar_Status == REVERSE_STATE && bcsParameters.cellTempMin < (bcsParameters.ar_heating_temp + bcsParameters.ar_heating_temp / 10))) {
			// Reverse polarity
			if(bcsParameters.displaySoC > 20.0)
				enable_thermal_system_heat();
			else
				disable_thermal_system();
		}
		else{
			// disable ar
			disable_thermal_system();
		}
	}
	else
	{
		disable_thermal_system();
	}
}

/**
 * @brief Set state of thermal system to heating, cooling, reverse or OFF during charging state
 *
 */
void thermal_system_set_charging()
{
	if(bcsParameters.coolingType!=OFF)
	{
		if (bcsParameters.ar_enable == 0)
		{
			disable_thermal_system();
		}
		else if (bcsParameters.cellTempMax > bcsParameters.ar_cooling_temp_charging ||
				(bcsParameters.ar_Status == COOLING_STATE &&
				bcsParameters.cellTempMax > (bcsParameters.ar_cooling_temp_charging -
										bcsParameters.ar_cooling_temp_charging / 10)))
		{
			// Cooling
			enable_pump();
			disable_reverse_polarity();
			enable_pelteir();
			enable_ar_fan();
			disable_heating_element();
			// disable_reverse_polarity
			bcsParameters.ar_Status = COOLING_STATE;
		}
		// Add for heating element
		else if (bcsParameters.cellTempMin < bcsParameters.ar_heating_temp_charging ||
				(bcsParameters.ar_Status == REVERSE_STATE &&
				bcsParameters.cellTempMin < (bcsParameters.ar_heating_temp_charging +
									bcsParameters.ar_heating_temp_charging / 10)))
		{
			// Reverse polarity
			enable_pump();
			enable_reverse_polarity();
			enable_pelteir();
			enable_ar_fan();
			enable_heating_element();
			// Reverse
			bcsParameters.ar_Status = REVERSE_STATE;
		}
		else
		{
			// disable ar
			disable_thermal_system();
		}
  }
}

/**
 * @brief Disable thermal system
 *
 */
void disable_thermal_system()
{
	// Disable pump
	disable_pump();
	disable_pelteir();
	disable_reverse_polarity();
	disable_ar_fan();
	bcsParameters.ar_Status = OFF_STATE;
}

/**
 * @brief enable thermal system in cooling mode
 *
 */
void enable_thermal_system_cool()
{
	if(bcsParameters.ar_Status != COOLING_STATE){
		enable_pump();
		disable_pelteir();
		tx_thread_sleep(50);
		disable_reverse_polarity();
		tx_thread_sleep(50);
		enable_pelteir();
		enable_ar_fan();
		disable_heating_element();
		bcsParameters.ar_Status = COOLING_STATE;
	}
}

/**
 * @brief enable thermal system in heating mode
 *
 */
void enable_thermal_system_heat()
{
	if(bcsParameters.ar_Status != REVERSE_STATE){
		enable_pump();
		disable_pelteir();
		tx_thread_sleep(50);
		enable_reverse_polarity();
		tx_thread_sleep(50);
		enable_pelteir();
		enable_ar_fan();
		enable_heating_element();
		bcsParameters.ar_Status = REVERSE_STATE;
	}
}


/* Function definition -------------------------------------------------------*/
void enable_pump()
{
	HAL_GPIO_WritePin(AR_PUMP_PORT, AR_PUMP_PIN, GPIO_PIN_SET); 
}

void disable_pump()
{
	HAL_GPIO_WritePin(HEATING_ELEM_OUT_PORT, AR_PUMP_PIN, GPIO_PIN_RESET); 
}

void enable_pelteir()
{
	HAL_GPIO_WritePin(AR_MICRO_PORT, AR_MICRO_PIN, GPIO_PIN_SET);  
}

void disable_pelteir()
{
	HAL_GPIO_WritePin(AR_MICRO_PORT, AR_MICRO_PIN, GPIO_PIN_RESET);
}

void enable_ar_fan()
{
	HAL_GPIO_WritePin(AR_FAN_PORT, AR_FAN_PIN, GPIO_PIN_SET);
}

void disable_ar_fan()
{
	HAL_GPIO_WritePin(AR_FAN_PORT, AR_FAN_PIN, GPIO_PIN_RESET);
}

void enable_heating_element()
{
	HAL_GPIO_WritePin(HEATING_ELEM_PORT, HEATING_ELEM_PIN, GPIO_PIN_SET);
}

void disable_heating_element()
{
	HAL_GPIO_WritePin(HEATING_ELEM_PORT, HEATING_ELEM_PIN, GPIO_PIN_RESET);
}

void enable_reverse_polarity()
{
	HAL_GPIO_WritePin(HEATING_ELEM_OUT_PORT, HEATING_ELEM_OUT_PIN, GPIO_PIN_SET);
}

void disable_reverse_polarity()
{
	HAL_GPIO_WritePin(HEATING_ELEM_OUT_PORT, HEATING_ELEM_OUT_PIN, GPIO_PIN_RESET); 
}


/**
/* Function definition -------------------------------------------------------*/
/**

/* End of File ---------------------------------------------------------------*/

