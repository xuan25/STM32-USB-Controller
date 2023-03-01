/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : usbd_composite_if.h
  * @version        : v1.0_Cube
  * @brief          : Header for usbd_COMPOSITE_if.c file.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USBD_COMPOSITE_IF_H__
#define __USBD_COMPOSITE_IF_H__

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "usbd_composite.h"

/* USER CODE BEGIN INCLUDE */

/* USER CODE END INCLUDE */

/** @addtogroup STM32_USB_OTG_DEVICE_LIBRARY
  * @brief For Usb device.
  * @{
  */

/** @defgroup USBD_COMPOSITE USBD_COMPOSITE
  * @brief Usb custom human interface device module.
  * @{
  */

/** @defgroup USBD_COMPOSITE_Exported_Defines USBD_COMPOSITE_Exported_Defines
  * @brief Defines.
  * @{
  */

/* USER CODE BEGIN EXPORTED_DEFINES */

#define CTRL_REPORT_ID      4u
#define CTRL_REPORT_LENGTH  3u

#define CTRL_VOLUME_INCREMENT   0x01u
#define CTRL_VOLUME_DECREMENT   0x02u
#define CTRL_MUTE               0x04u
#define CTRL_PLAY_PAUSE         0x08u
#define CTRL_NEXT               0x10u
#define CTRL_PREVIOUS           0x20u

#define KEYBOARD_REPORT_ID      2u
#define KEYBOARD_REPORT_LENGTH  9u

#define MOUSE_REPORT_ID     1u
#define MOUSE_REPORT_LENGTH 5u

/* USER CODE END EXPORTED_DEFINES */

/**
  * @}
  */

/** @defgroup USBD_COMPOSITE_Exported_Types USBD_COMPOSITE_Exported_Types
  * @brief Types.
  * @{
  */

/* USER CODE BEGIN EXPORTED_TYPES */

/* USER CODE END EXPORTED_TYPES */

/**
  * @}
  */

/** @defgroup USBD_COMPOSITE_Exported_Macros USBD_COMPOSITE_Exported_Macros
  * @brief Aliases.
  * @{
  */

/* USER CODE BEGIN EXPORTED_MACRO */

/* USER CODE END EXPORTED_MACRO */

/**
  * @}
  */

/** @defgroup USBD_COMPOSITE_Exported_Variables USBD_COMPOSITE_Exported_Variables
  * @brief Public variables.
  * @{
  */

/** CUSTOMHID Interface callback. */
// extern USBD_COMPOSITE_ItfTypeDef USBD_CustomHID_fops_FS;

/* USER CODE BEGIN EXPORTED_VARIABLES */

/* USER CODE END EXPORTED_VARIABLES */

/**
  * @}
  */

/** @defgroup USBD_COMPOSITE_Exported_FunctionsPrototype USBD_COMPOSITE_Exported_FunctionsPrototype
  * @brief Public functions declaration.
  * @{
  */

/* USER CODE BEGIN EXPORTED_FUNCTIONS */

uint8_t USBD_COMPOSITE_CUSTOM_HID_SendCtrlReport_FS(uint16_t ctrl);
uint8_t USBD_COMPOSITE_CUSTOM_HID_SendKeyboardReport_FS(uint8_t modifier, uint8_t oem, uint8_t key1, uint8_t key2, uint8_t key3, uint8_t key4, uint8_t key5, uint8_t key6);
uint8_t USBD_COMPOSITE_CUSTOM_HID_SendMouseReport_FS(uint8_t buttons, uint8_t x, uint8_t y, uint8_t wheel);

/* USER CODE END EXPORTED_FUNCTIONS */

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /* __USBD_COMPOSITE_IF_H__ */

