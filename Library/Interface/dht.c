/**

/* Standard C includes -------------------------------------------------------*/

#include "bmsConfiguration.h"
#include "dht.h"
#include "main.h"
#include "tim.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/

/* Self include --------------------------------------------------------------*/

/* Type definiton ------------------------------------------------------------*/
// #define DHT11_PORT GPIOA
// #define DHT11_PIN GPIO_PIN_1
#define DHT11_PORT GPIOC
#define DHT11_PIN GPIO_PIN_4
/* External variables definiton ----------------------------------------------*/

/* Private constants ---------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
// CAN_HandleTypeDef hcan1;
// TIM_HandleTypeDef htim2;
// CAN_TxHeaderTypeDef TxHeader;

// uint8_t TxData[8];
// uint32_t TxMailbox;

uint8_t Rh_byte1, Rh_byte2, Temp_byte1, Temp_byte2;
uint16_t SUM, RH, TEMP;

float Temperature = 0;
float Humidity = 0;
uint8_t Presence = 0;
// uint8_t count=0;
// uint64_t count1=0;
/* Private function prototypes -----------------------------------------------*/
// void SystemClock_Config(void);
// static void MX_GPIO_Init(void);
// static void MX_TIM2_Init(void);
// static void MX_CAN1_Init(void);
/* Private custom code -------------------------------------------------------*/
// int main(void)
// {
//   /* USER CODE BEGIN 1 */

//   /* USER CODE END 1 */

//   /* MCU Configuration--------------------------------------------------------*/

//   /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
//   HAL_Init();

//   /* USER CODE BEGIN Init */

//   /* USER CODE END Init */

//   /* Configure the system clock */
//   SystemClock_Config();

//   /* USER CODE BEGIN SysInit */

//   /* USER CODE END SysInit */

//   /* Initialize all configured peripherals */
//   MX_GPIO_Init();
//   //MX_TIM2_Init();
//   MX_CAN1_Init();
//   /* USER CODE BEGIN 2 */
//   HAL_GPIO_WritePin(GPIOA,GPIO_PIN_8,GPIO_PIN_RESET);
//   //HAL_TIM_Base_Start(&htim2);
//   HAL_CAN_Start(&hcan1);


//   /* USER CODE END 2 */

//   /* Infinite loop */
//   /* USER CODE BEGIN WHILE */
//   while (1)
//   {
//     /* USER CODE END WHILE */

//     /* USER CODE BEGIN 3 */




//   	  // TxHeader.DLC = 8;
//   	  // TxHeader.IDE = CAN_ID_STD;
//   	  // TxHeader.RTR = CAN_RTR_DATA;
//   	  // TxHeader.StdId = 0x100;
//   	  // TxData[0]=Temp_byte1;
//   	  // TxData[1]=Rh_byte1;

//   	  // HAL_CAN_AddTxMessage(&hcan1, &TxHeader, TxData, &TxMailbox);
//   	  // HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_14);
//   	  //count++;
// 	    //HAL_Delay(1000);
//   }
//   /* USER CODE END 3 */
// }
/* Function definition -------------------------------------------------------*/
void delay (uint16_t time)
{
	/* change your code here for the delay in microseconds */
	__HAL_TIM_SET_COUNTER(&htim2, 0);
	while ((__HAL_TIM_GET_COUNTER(&htim2))<time);
}

void DHT11_Start (void)
{
	Set_Pin_Output (DHT11_PORT, DHT11_PIN);  // set the pin as output
	HAL_GPIO_WritePin (DHT11_PORT, DHT11_PIN, 0);   // pull the pin low
	delay (18000);   // wait for 18ms
    HAL_GPIO_WritePin (DHT11_PORT, DHT11_PIN, 1);   // pull the pin high
	Set_Pin_Input(DHT11_PORT, DHT11_PIN);    // set as input
	delay (20);   // wait for 20us
}

void Set_Pin_Output (GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.Pin = GPIO_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOx, &GPIO_InitStruct);
}

void Set_Pin_Input (GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.Pin = GPIO_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(GPIOx, &GPIO_InitStruct);
}


uint8_t DHT11_Check_Response (void)
{
	uint8_t Response = 0;
	delay (40);
	if (!(HAL_GPIO_ReadPin (DHT11_PORT, DHT11_PIN)))
	{
		delay (80);
		if ((HAL_GPIO_ReadPin (DHT11_PORT, DHT11_PIN))) Response = 1;
		else Response = -1; // 255
	}
	unsigned long startTime = HAL_GetTick();
	while ((HAL_GPIO_ReadPin (DHT11_PORT, DHT11_PIN)))
	{
		//count++;
		if(HAL_GetTick() - startTime > 100){
			break;
		}
	}// wait for the pin to go low

	return Response;
}




// static uint32_t lasttick=0;
// uint32_t currenttick=0;
// uint32_t dur=0;
// void YourFunction(void) {
// 	currenttick=HAL_GetTick();
// 	if(lasttick==0) dur=0;
// 	else dur=currenttick-lasttick;
// 	lasttick=currenttick;
// }

uint8_t DHT11_Read (void)
{
	uint8_t i=0,j;
	for (j=0;j<8;j++)
	{
		while (!(HAL_GPIO_ReadPin (DHT11_PORT, DHT11_PIN)));   // wait for the pin to go high
		delay(40);
		if (!(HAL_GPIO_ReadPin (DHT11_PORT, DHT11_PIN)))   // if the pin is low
		{
			i&= ~(1<<(7-j));   // write 0
		}
		else i|= (1<<(7-j));  // if the pin is high, write 1
		unsigned long startTime = HAL_GetTick();
		while ((HAL_GPIO_ReadPin (DHT11_PORT, DHT11_PIN)))
		{
			if(HAL_GetTick() - startTime > 100){
			break;
			}  // wait for the pin to go low
		}
	}
	return i;
}
void dht11(uint8_t *humid,uint8_t *temp){
	DHT11_Start();
	Presence = DHT11_Check_Response();
	Rh_byte1 = DHT11_Read ();
	Rh_byte2 = DHT11_Read ();
	Temp_byte1 = DHT11_Read ();
	Temp_byte2 = DHT11_Read ();
	SUM = DHT11_Read();

	*temp = (float) Temp_byte1;
	*humid = (float) Rh_byte1;
}
/* USE_FULL_ASSERT */

/**

/* Function definition -------------------------------------------------------*/
/**

/* End of File ---------------------------------------------------------------*/

