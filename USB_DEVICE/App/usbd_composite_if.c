/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : usbd_composite_if.c
  * @version        : v1.0_Cube
  * @brief          : USB Device Custom HID interface file.
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
#include "usbd_composite_if.h"

/* USER CODE BEGIN INCLUDE */

/* USER CODE END INCLUDE */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/

/* USER CODE END PV */

/** @addtogroup STM32_USB_OTG_DEVICE_LIBRARY
  * @brief Usb device.
  * @{
  */

/** @addtogroup USBD_COMPOSITE
  * @{
  */

/** @defgroup USBD_COMPOSITE_Private_TypesDefinitions USBD_COMPOSITE_Private_TypesDefinitions
  * @brief Private types.
  * @{
  */

/* USER CODE BEGIN PRIVATE_TYPES */

/* USER CODE END PRIVATE_TYPES */

/**
  * @}
  */

/** @defgroup USBD_COMPOSITE_Private_Defines USBD_COMPOSITE_Private_Defines
  * @brief Private defines.
  * @{
  */

/* USER CODE BEGIN PRIVATE_DEFINES */

/* USER CODE END PRIVATE_DEFINES */

/**
  * @}
  */

/** @defgroup USBD_COMPOSITE_Private_Macros USBD_COMPOSITE_Private_Macros
  * @brief Private macros.
  * @{
  */

/* USER CODE BEGIN PRIVATE_MACRO */

/* USER CODE END PRIVATE_MACRO */

/**
  * @}
  */

/** @defgroup USBD_COMPOSITE_Private_Variables USBD_COMPOSITE_Private_Variables
  * @brief Private variables.
  * @{
  */

/* USER CODE BEGIN PRIVATE_VARIABLES */

/* USER CODE END PRIVATE_VARIABLES */

/**
  * @}
  */

/** @defgroup USBD_COMPOSITE_Exported_Variables USBD_COMPOSITE_Exported_Variables
  * @brief Public variables.
  * @{
  */
extern USBD_HandleTypeDef hUsbDeviceFS;

/* USER CODE BEGIN EXPORTED_VARIABLES */

/* USER CODE END EXPORTED_VARIABLES */
/**
  * @}
  */

/** @defgroup USBD_COMPOSITE_Private_FunctionPrototypes USBD_COMPOSITE_Private_FunctionPrototypes
  * @brief Private functions declaration.
  * @{
  */

static int8_t COMPOSITE_Init_FS(void);
static int8_t COMPOSITE_DeInit_FS(void);
static int8_t COMPOSITE_OutEvent_FS(uint8_t event_idx, uint8_t state);

/**
  * @}
  */

// USBD_COMPOSITE_ItfTypeDef USBD_CustomHID_fops_FS =
// {
//   COMPOSITE_ReportDesc_FS,
//   COMPOSITE_Init_FS,
//   COMPOSITE_DeInit_FS,
//   COMPOSITE_OutEvent_FS
// };

/** @defgroup USBD_COMPOSITE_Private_Functions USBD_COMPOSITE_Private_Functions
  * @brief Private functions.
  * @{
  */

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Initializes the CUSTOM HID media low layer
  * @retval USBD_OK if all operations are OK else USBD_FAIL
  */
static int8_t COMPOSITE_Init_FS(void)
{
  /* USER CODE BEGIN 4 */
  return (USBD_OK);
  /* USER CODE END 4 */
}

/**
  * @brief  DeInitializes the CUSTOM HID media low layer
  * @retval USBD_OK if all operations are OK else USBD_FAIL
  */
static int8_t COMPOSITE_DeInit_FS(void)
{
  /* USER CODE BEGIN 5 */
  return (USBD_OK);
  /* USER CODE END 5 */
}

/**
  * @brief  Manage the CUSTOM HID class events
  * @param  event_idx: Event index
  * @param  state: Event state
  * @retval USBD_OK if all operations are OK else USBD_FAIL
  */
static int8_t COMPOSITE_OutEvent_FS(uint8_t event_idx, uint8_t state)
{
  /* USER CODE BEGIN 6 */
  UNUSED(event_idx);
  UNUSED(state);

  /* Start next USB packet transfer once data processing is completed */
  if (USBD_COMPOSITE_ReceivePacket(&hUsbDeviceFS) != (uint8_t)USBD_OK)
  {
    return -1;
  }

  return (USBD_OK);
  /* USER CODE END 6 */
}

/* USER CODE BEGIN 7 */
/**
  * @brief  Send the report to the Host
  * @param  report: The report to be sent
  * @param  len: The report length
  * @retval USBD_OK if all operations are OK else USBD_FAIL
  */
/*
static int8_t USBD_COMPOSITE_SendReport_FS(uint8_t *report, uint16_t len)
{
  return USBD_COMPOSITE_SendReport(&hUsbDeviceFS, report, len);
}
*/

uint8_t USBD_COMPOSITE_CUSTOM_HID_SendCtrlReport_FS(uint16_t ctrl){
  uint8_t ctrlReportBuffer[CTRL_REPORT_LENGTH];
  ctrlReportBuffer[0] = CTRL_REPORT_ID;
  ctrlReportBuffer[1] = ctrl & 0xFF;
  ctrlReportBuffer[2] = ctrl >> 8 & 0xFF;
  return USBD_COMPOSITE_CUSTOM_HID_SendReport(&hUsbDeviceFS, ctrlReportBuffer, CTRL_REPORT_LENGTH);
}

uint8_t USBD_COMPOSITE_CUSTOM_HID_SendKeyboardReport_FS(uint8_t modifier, uint8_t oem, uint8_t key1, uint8_t key2, uint8_t key3, uint8_t key4, uint8_t key5, uint8_t key6) {
  uint8_t keyboardReportBuffer[KEYBOARD_REPORT_LENGTH];
  keyboardReportBuffer[0] = KEYBOARD_REPORT_ID;
  keyboardReportBuffer[1] = modifier;
  keyboardReportBuffer[2] = oem;
  keyboardReportBuffer[3] = key1;
  keyboardReportBuffer[4] = key2;
  keyboardReportBuffer[5] = key3;
  keyboardReportBuffer[6] = key4;
  keyboardReportBuffer[7] = key5;
  keyboardReportBuffer[8] = key6;
  return USBD_COMPOSITE_CUSTOM_HID_SendReport(&hUsbDeviceFS, keyboardReportBuffer, CTRL_REPORT_LENGTH);
}

uint8_t USBD_COMPOSITE_CUSTOM_HID_SendMouseReport_FS(uint8_t buttons, uint8_t x, uint8_t y, uint8_t wheel) {
  uint8_t mouseReportBuffer[MOUSE_REPORT_LENGTH];
  mouseReportBuffer[0] = MOUSE_REPORT_ID;
  mouseReportBuffer[1] = buttons;
  mouseReportBuffer[2] = x;
  mouseReportBuffer[3] = y;
  mouseReportBuffer[4] = wheel;
  return USBD_COMPOSITE_CUSTOM_HID_SendReport(&hUsbDeviceFS, mouseReportBuffer, CTRL_REPORT_LENGTH);
}

/* USER CODE END 7 */

/* USER CODE BEGIN PRIVATE_FUNCTIONS_IMPLEMENTATION */

/* USER CODE END PRIVATE_FUNCTIONS_IMPLEMENTATION */
/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

