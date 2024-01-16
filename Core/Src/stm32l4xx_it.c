/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    stm32l4xx_it.c
  * @brief   Interrupt Service Routines.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32l4xx_it.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "flashControls.h"
#include "onBoardFlash.h"
#include "adc.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN TD */

/* USER CODE END TD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/* External variables --------------------------------------------------------*/
extern TIM_HandleTypeDef htim7;
extern TIM_HandleTypeDef htim6;
extern DMA_HandleTypeDef hdma_adc2;
extern CAN_HandleTypeDef hcan1;
/* USER CODE BEGIN EV */

/* USER CODE END EV */

/******************************************************************************/
/*           Cortex-M4 Processor Interruption and Exception Handlers          */
/******************************************************************************/
/**
  * @brief This function handles Non maskable interrupt.
  */
void NMI_Handler(void)
{
  /* USER CODE BEGIN NonMaskableInt_IRQn 0 */

  /* USER CODE END NonMaskableInt_IRQn 0 */
  /* USER CODE BEGIN NonMaskableInt_IRQn 1 */
  while (1)
  {
    systemReset();
  }
  /* USER CODE END NonMaskableInt_IRQn 1 */
}

// /**
//   * @brief This function handles Hard fault interrupt.
//   */
// void HardFault_Handler(void)
// {
//   /* USER CODE BEGIN HardFault_IRQn 0 */

//   /* USER CODE END HardFault_IRQn 0 */
//   while (1)
//   {
//     /* USER CODE BEGIN W1_HardFault_IRQn 0 */
//     /* USER CODE END W1_HardFault_IRQn 0 */
//   }
// }

/**
  * @brief This function handles Memory management fault.
  */
void MemManage_Handler(void)
{
  /* USER CODE BEGIN MemoryManagement_IRQn 0 */

  /* USER CODE END MemoryManagement_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_MemoryManagement_IRQn 0 */
    systemReset();
    /* USER CODE END W1_MemoryManagement_IRQn 0 */
  }
}

/**
  * @brief This function handles Prefetch fault, memory access fault.
  */
void BusFault_Handler(void)
{
  /* USER CODE BEGIN BusFault_IRQn 0 */

  /* USER CODE END BusFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_BusFault_IRQn 0 */
    systemReset();
    /* USER CODE END W1_BusFault_IRQn 0 */
  }
}

/**
  * @brief This function handles Undefined instruction or illegal state.
  */
void UsageFault_Handler(void)
{
  /* USER CODE BEGIN UsageFault_IRQn 0 */

  /* USER CODE END UsageFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_UsageFault_IRQn 0 */
    systemReset();
    /* USER CODE END W1_UsageFault_IRQn 0 */
  }
}

/**
  * @brief This function handles System service call via SWI instruction.
  */
void SVC_Handler(void)
{
  /* USER CODE BEGIN SVCall_IRQn 0 */

  /* USER CODE END SVCall_IRQn 0 */
  /* USER CODE BEGIN SVCall_IRQn 1 */

  /* USER CODE END SVCall_IRQn 1 */
}

/**
  * @brief This function handles Debug monitor.
  */
void DebugMon_Handler(void)
{
  /* USER CODE BEGIN DebugMonitor_IRQn 0 */

  /* USER CODE END DebugMonitor_IRQn 0 */
  /* USER CODE BEGIN DebugMonitor_IRQn 1 */

  /* USER CODE END DebugMonitor_IRQn 1 */
}

// /**
//   * @brief This function handles Pendable request for system service.
//   */
// void PendSV_Handler(void)
// {
//   /* USER CODE BEGIN PendSV_IRQn 0 */

//   /* USER CODE END PendSV_IRQn 0 */
//   /* USER CODE BEGIN PendSV_IRQn 1 */

//   /* USER CODE END PendSV_IRQn 1 */
// }

// /**
//   * @brief This function handles System tick timer.
//   */
// void SysTick_Handler(void)
// {
//   /* USER CODE BEGIN SysTick_IRQn 0 */

//   /* USER CODE END SysTick_IRQn 0 */

//   /* USER CODE BEGIN SysTick_IRQn 1 */

//   /* USER CODE END SysTick_IRQn 1 */
// }

/******************************************************************************/
/* STM32L4xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32l4xx.s).                    */
/******************************************************************************/

/**
  * @brief This function handles TIM7 global interrupt.
  */
void TIM7_IRQHandler(void)
{
  /* USER CODE BEGIN TIM7_IRQn 0 */

  /* USER CODE END TIM7_IRQn 0 */
  HAL_TIM_IRQHandler(&htim7);
  /* USER CODE BEGIN TIM7_IRQn 1 */

  /* USER CODE END TIM7_IRQn 1 */
}


//double tempPackFastCurrent = 0.0;
/**
  * @brief This function handles TIM6 global interrupt, DAC channel1 and channel2 underrun error interrupts.
  */
void TIM6_DAC_IRQHandler(void)
{
  /* USER CODE BEGIN TIM6_DAC_IRQn 0 */

  /* USER CODE END TIM6_DAC_IRQn 0 */
  HAL_TIM_IRQHandler(&htim6);
  /* USER CODE BEGIN TIM6_DAC_IRQn 1 */

  /* USER CODE END TIM6_DAC_IRQn 1 */
}

/**
  * @brief This function handles DMA1 channel2 global interrupt.
  */
void DMA1_Channel2_IRQHandler(void)
{
  /* USER CODE BEGIN DMA1_Channel2_IRQn 0 */

  /* USER CODE END DMA1_Channel2_IRQn 0 */
  HAL_DMA_IRQHandler(&hdma_adc2);
  /* USER CODE BEGIN DMA1_Channel2_IRQn 1 */

  /* USER CODE END DMA1_Channel2_IRQn 1 */
}

/**
  * @brief This function handles CAN1 RX0 interrupts.
  */
void CAN1_RX0_IRQHandler(void)
{
	HAL_CAN_IRQHandler(&hcan1);
}

/**
  * @brief This function handles CAN1 RX1 interrupt.
  */
void CAN1_RX1_IRQHandler(void)
{
	HAL_CAN_IRQHandler(&hcan1);
}

/**
  * @brief This function handles PVD interrupt.
  */
void PVD_PVM_IRQHandler(void)
{
  /* USER CODE BEGIN PVD_PVM_IRQn 0 */

  /* USER CODE END PVD_PVM_IRQn 0 */
  HAL_PWREx_PVD_PVM_IRQHandler();
  /* USER CODE BEGIN PVD_PVM_IRQn 1 */

  /* USER CODE END PVD_PVM_IRQn 1 */
}

/**
  * @brief This function handles EXTI line2 interrupt.
  */
void EXTI2_IRQHandler(void)
{
  /* USER CODE BEGIN EXTI2_IRQn 0 */

  /* USER CODE END EXTI2_IRQn 0 */
  HAL_GPIO_EXTI_IRQHandler(SC_InputDetection_2949_Pin);
  /* USER CODE BEGIN EXTI2_IRQn 1 */

  /* USER CODE END EXTI2_IRQn 1 */
}

/**
  * @brief This function handles EXTI line[9:5] interrupts.
  */
void EXTI9_5_IRQHandler(void)
{
  /* USER CODE BEGIN EXTI9_5_IRQn 0 */

  /* USER CODE END EXTI9_5_IRQn 0 */
  HAL_GPIO_EXTI_IRQHandler(SC_Detection_Pin);
  /* USER CODE BEGIN EXTI9_5_IRQn 1 */

  /* USER CODE END EXTI9_5_IRQn 1 */
}

uint8_t externalShortCircuitDetectionFlag = 0;

/**
  * @brief This function handles ADC1 and ADC2 interrupts.
  */
void ADC1_2_IRQHandler(void)
{
  /* USER CODE BEGIN ADC1_2_IRQn 0 */

  TIM3->CCR1 = 0;
  TIM3->CCR2 = 0;
  TIM3->CCR3 = 0;
  TIM3->CCR4 = 0;
  externalShortCircuitDetectionFlag = 1;

  /* USER CODE END ADC1_2_IRQn 0 */
  HAL_ADC_IRQHandler(&hadc1);
  HAL_ADC_IRQHandler(&hadc2);
  /* USER CODE BEGIN ADC1_2_IRQn 1 */

  /* USER CODE END ADC1_2_IRQn 1 */
}

/* USER CODE BEGIN 1 */
#define HARD_FAULT_RESET_COUNTER_MAX_LIMIT  20

#define HARD_FAULT_NUMBER_OF_REGISTERS    14
#define HARD_FAULT_RESET_COUNTER_LENGTH   1
#define HARD_FAULT_NUMBER_OF_PARAMETERS   ((HARD_FAULT_NUMBER_OF_REGISTERS) + (HARD_FAULT_RESET_COUNTER_LENGTH))
#define HARD_FAULT_DATA_SIZE              ((HARD_FAULT_NUMBER_OF_PARAMETERS) + (CRC_LENGTH))
#define NA                                1 //not applicable

__IO uint64_t hardFaultData[HARD_FAULT_DATA_SIZE] = {0};

__attribute__((optimize("O0"))) void hard_fault_handler_c(unsigned long *hardfault_args)
{
	volatile uint32_t stacked_r0 = 0;
	volatile uint32_t stacked_r1 = 0;
	volatile uint32_t stacked_r2 = 0;
	volatile uint32_t stacked_r3 = 0;
	volatile uint32_t stacked_r12 = 0;
	volatile uint32_t stacked_lr = 0;
	volatile uint32_t stacked_pc = 0;
	volatile uint32_t stacked_psr = 0;
	volatile uint32_t _CFSR = 0;
	volatile uint32_t _HFSR = 0;
	volatile uint32_t _DFSR = 0;
	volatile uint32_t _AFSR = 0;
	volatile uint32_t _BFAR = 0;
	volatile uint32_t _MMAR = 0;
  volatile uint32_t counter = 0;
  bmsStatus_t status = BMS_ERROR;

	stacked_r0  = ((uint32_t)hardfault_args[0]);
	stacked_r1  = ((uint32_t)hardfault_args[1]);
	stacked_r2  = ((uint32_t)hardfault_args[2]);
	stacked_r3  = ((uint32_t)hardfault_args[3]);
	stacked_r12 = ((uint32_t)hardfault_args[4]);
	stacked_lr  = ((uint32_t)hardfault_args[5]);
	stacked_pc  = ((uint32_t)hardfault_args[6]);
	stacked_psr = ((uint32_t)hardfault_args[7]);

	// Configurable Fault Status Register
	// Consists of MMSR, BFSR and UFSR
	_CFSR = (*((volatile uint32_t *)(0xE000ED28)));

	// Hard Fault Status Register
	_HFSR = (*((volatile uint32_t *)(0xE000ED2C)));

	// Debug Fault Status Register
	_DFSR = (*((volatile uint32_t *)(0xE000ED30)));

	// Auxiliary Fault Status Register
	_AFSR = (*((volatile uint32_t *)(0xE000ED3C)));

	// Read the Fault Address Registers. These may not contain valid values.
	// Check BFARVALID/MMARVALID to see if they are valid values
	// MemManage Fault Address Register
	_MMAR = (*((volatile uint32_t *)(0xE000ED34)));
	// Bus Fault Address Register
	_BFAR = (*((volatile uint32_t *)(0xE000ED38)));

  //read counter stored in the flash
  status = readDataFromFlash(FLASH_EXCEPTION_SECTION, FLASH_EXCEPTION_ADDRESS, hardFaultData, HARD_FAULT_DATA_SIZE);

  if((status != BMS_OK) && (status != BMS_FLASH_EXCEPTION_DATA_NOT_FOUND))
  {
    systemReset();
  }

  counter = hardFaultData[14];
  counter++;
  
  if(counter > HARD_FAULT_RESET_COUNTER_MAX_LIMIT)
  {
    //send error on the CAN, BMS is going to hardfault more than it should
  }

  hardFaultData[0] = stacked_r0;
  hardFaultData[1] = stacked_r1;
  hardFaultData[2] = stacked_r2;
  hardFaultData[3] = stacked_r3;
  hardFaultData[4] = stacked_r12;
  hardFaultData[5] = stacked_lr;
  hardFaultData[6] = stacked_pc;
  hardFaultData[7] = stacked_psr;

  hardFaultData[8]  = _CFSR;
  hardFaultData[9]  = _HFSR;
  hardFaultData[10] = _DFSR;
  hardFaultData[11] = _AFSR;
  hardFaultData[12] = _BFAR;
  hardFaultData[13] = _MMAR;
  hardFaultData[14] = counter;
  hardFaultData[15] = calculateFlashCRC(hardFaultData, HARD_FAULT_NUMBER_OF_PARAMETERS);

  writeDataToFlash(FLASH_EXCEPTION_SECTION, FLASH_EXCEPTION_ADDRESS, hardFaultData, HARD_FAULT_DATA_SIZE, (uint8_t*)NA);

  systemReset();
}


/**
  * @brief  Initiate a system reset request to reset the MCU.
  * @retval None
  */
void systemReset(void)
{
  /* System Reset */
  NVIC_SystemReset();
}

/* USER CODE END 1 */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/