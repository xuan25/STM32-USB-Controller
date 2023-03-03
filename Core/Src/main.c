/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "usb_device.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#include "led.h"
#include "key.h"
#include "usbd_custom_hid_if.h"
#include "usbd_midi_if.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

typedef struct KeyData {
  GPIO_TypeDef* GPIOx;
  uint16_t GPIO_Pin;
  uint16_t ID;
} KeyData;

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

#define KEY_PRESSED  GPIO_PIN_RESET
#define KEY_RELEASED GPIO_PIN_SET

#define LED_ON  GPIO_PIN_RESET
#define LED_OFF GPIO_PIN_SET

#define NUM_KEYS 2u
#define NUM_LEDS 2u

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

uint16_t ctrlState;
uint16_t midiState;

static Key keys[NUM_KEYS];
static LED leds[NUM_LEDS];

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
/* USER CODE BEGIN PFP */

void UpdateStateLED();
void Key_Scan(Key* key);
void Key_ScanAll();
void OnKeyStateChanged(Key* sender, uint8_t oldState, uint8_t newState);

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

uint16_t ctrlState = 0;

static Key keys[NUM_KEYS] = {
  {
    .UserData = &((KeyData){ 
      .GPIOx = KEY_0_GPIO_Port,
      .GPIO_Pin = KEY_0_Pin,
      .ID = 0,
    }),
    .State = KEY_RELEASED,
    .OnStateChanged = OnKeyStateChanged,
  },
  {
    .UserData = &((KeyData){ 
      .GPIOx = KEY_1_GPIO_Port,
      .GPIO_Pin = KEY_1_Pin,
      .ID = 1,
    }),
    .State = KEY_RELEASED,
    .OnStateChanged = OnKeyStateChanged,
  },
};

static LED leds[NUM_LEDS] = {
  {
    .GPIOx = LED_1_GPIO_Port,
    .GPIO_Pin = LED_1_Pin
  },
  {
    .GPIOx = LED_2_GPIO_Port,
    .GPIO_Pin = LED_2_Pin
  }
};

void UpdateStateLED() {
  if (ctrlState != 0) {
    LED_SetState(&leds[0], LED_ON);
  }
  else {
    LED_SetState(&leds[0], LED_OFF);
  }
  if (midiState != 0) {
    LED_SetState(&leds[1], LED_ON);
  }
  else {
    LED_SetState(&leds[1], LED_OFF);
  }
}

void Key_Scan(Key* key) {
  KeyData* keyData = (KeyData*)key->UserData;
  uint8_t keyLevel = HAL_GPIO_ReadPin(keyData->GPIOx, keyData->GPIO_Pin);
  Key_Update(key, keyLevel);
}

void Key_ScanAll() {
  for (int i=0; i<NUM_KEYS; i++) {
    Key* key = &keys[i];
    Key_Scan(key);
  }
}

void OnKeyStateChanged(Key* sender, uint8_t oldState, uint8_t newState) {
  KeyData* keyData = (KeyData*)sender->UserData;
  switch (keyData->ID)
  {
  case 0:
    if (newState == KEY_PRESSED) {
      ctrlState = ctrlState | CTRL_PLAY_PAUSE;
      USBD_CUSTOM_HID_SendCtrlReport_FS(ctrlState);
      UpdateStateLED();
    } else {
      ctrlState = ctrlState & ~CTRL_PLAY_PAUSE;
      USBD_CUSTOM_HID_SendCtrlReport_FS(ctrlState);
      UpdateStateLED();
    }
    break;
  case 1:
    if (newState == KEY_PRESSED) {
      // ctrlState = ctrlState | CTRL_NEXT;
      // USBD_CUSTOM_HID_SendCtrlReport_FS(ctrlState);
      midiState = ~midiState;
      USBD_MIDI_SendCCMessage_FS(0x0, 0x0, 80, midiState);
      UpdateStateLED();
    } else {
      // ctrlState = ctrlState & ~CTRL_NEXT;
      // USBD_CUSTOM_HID_SendCtrlReport_FS(ctrlState);
      UpdateStateLED();
    }
    break;
  default:
    break;
  }
}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USB_DEVICE_Init();
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

    Key_ScanAll();

  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOF, LED_1_Pin|LED_2_Pin, GPIO_PIN_SET);

  /*Configure GPIO pins : KEY_1_Pin KEY_0_Pin */
  GPIO_InitStruct.Pin = KEY_1_Pin|KEY_0_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pins : LED_1_Pin LED_2_Pin */
  GPIO_InitStruct.Pin = LED_1_Pin|LED_2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
