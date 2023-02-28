/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : usbd_custom_hid_if.c
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
#include "usbd_custom_hid_if.h"

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

/** @addtogroup USBD_CUSTOM_HID
  * @{
  */

/** @defgroup USBD_CUSTOM_HID_Private_TypesDefinitions USBD_CUSTOM_HID_Private_TypesDefinitions
  * @brief Private types.
  * @{
  */

/* USER CODE BEGIN PRIVATE_TYPES */

/* USER CODE END PRIVATE_TYPES */

/**
  * @}
  */

/** @defgroup USBD_CUSTOM_HID_Private_Defines USBD_CUSTOM_HID_Private_Defines
  * @brief Private defines.
  * @{
  */

/* USER CODE BEGIN PRIVATE_DEFINES */

/* USER CODE END PRIVATE_DEFINES */

/**
  * @}
  */

/** @defgroup USBD_CUSTOM_HID_Private_Macros USBD_CUSTOM_HID_Private_Macros
  * @brief Private macros.
  * @{
  */

/* USER CODE BEGIN PRIVATE_MACRO */

/* USER CODE END PRIVATE_MACRO */

/**
  * @}
  */

/** @defgroup USBD_CUSTOM_HID_Private_Variables USBD_CUSTOM_HID_Private_Variables
  * @brief Private variables.
  * @{
  */

/** Usb HID report descriptor. */
__ALIGN_BEGIN static uint8_t CUSTOM_HID_ReportDesc_FS[USBD_CUSTOM_HID_REPORT_DESC_SIZE] __ALIGN_END =
{
  /* USER CODE BEGIN 0 */

  // ==== Consumer Control ====
  /*                                     == Report Structure ==
    +========+==========+==========+==========+==========+==========+==========+==========+==========+
    |        |    0     |    1     |    2     |    3     |    4     |    5     |    6     |    7     |
    +========+==========+==========+==========+==========+==========+==========+==========+==========+
    |  0x00  |                                     Report ID (4)                                     |
    +--------+----------+----------+----------+----------+----------+----------+----------+----------+
    |  0x08  | Vol Inc  | Vol Dec  |   Mute   |Play/Pause|   Next   | Previous | Reserved | Reserved |
    +--------+----------+----------+----------+----------+----------+----------+----------+----------+
    |  0x10  | Reserved | Reserved | Reserved | Reserved | Reserved | Reserved | Reserved | Reserved |
    +--------+----------+----------+----------+----------+----------+----------+----------+----------+
  */
  0x05, 0x0c,                    // Usage Page (Consumer Devices)
  0x09, 0x01,                    // Usage (Consumer Control)
  0xa1, 0x01,                    // Collection (Application)
  0x85, 0x04,                    //   REPORT_ID (4)
  
  0x09, 0xe9,                    //   Usage (Volume Increment)
  0x09, 0xea,                    //   Usage (Volume Decrement)
  0x09, 0xe2,                    //   Usage (Mute)
  0x09, 0xcd,                    //   Usage (Play/Pause)
  0x09, 0xb5,                    //   Usage (Scan Next Track)
  0x09, 0xb6,                    //   Usage (Scan Previous Track)
  0x15, 0x00,                    //   Logical Minimum (0)
  0x25, 0x01,                    //   Logical Maximum (1) (OOC: single throw momentary switch)
  0x75, 0x01,                    //   Report Size (1)
  0x95, 0x10,                    //   Report Count (16) (16 reports in 2 bytes; each bit controlling one usage)
  0x81, 0x06,                    //   Input (Data, Variable, Relative)

  0xc0,                          // End Collection

  // ==== Keyboard ====
  /*                                     == Report Structure ==
    +========+==========+==========+==========+==========+==========+==========+==========+==========+
    |  bit   |    0     |    1     |    2     |    3     |    4     |    5     |    6     |    7     |
    +========+==========+==========+==========+==========+==========+==========+==========+==========+
    |  0x00  |                                     Report ID (2)                                     |
    +--------+----------+----------+----------+----------+----------+----------+----------+----------+
    |  0x08  |                                       Modifiers                                       |
    +--------+----------+----------+----------+----------+----------+----------+----------+----------+
    |  0x10  |                                          OEM                                          |
    +--------+----------+----------+----------+----------+----------+----------+----------+----------+
    |  0x18  |                                         Key 1                                         |
    +--------+----------+----------+----------+----------+----------+----------+----------+----------+
    |  0x20  |                                         Key 2                                         |
    +--------+----------+----------+----------+----------+----------+----------+----------+----------+
    |  0x28  |                                         Key 3                                         |
    +--------+----------+----------+----------+----------+----------+----------+----------+----------+
    |  0x30  |                                         Key 4                                         |
    +--------+----------+----------+----------+----------+----------+----------+----------+----------+
    |  0x38  |                                         Key 5                                         |
    +--------+----------+----------+----------+----------+----------+----------+----------+----------+
    |  0x40  |                                         Key 6                                         |
    +--------+----------+----------+----------+----------+----------+----------+----------+----------+
  */
  0x05, 0x01,                    // USAGE_PAGE (Generic Desktop)  // 47
  0x09, 0x06,                    // USAGE (Keyboard)
  0xa1, 0x01,                    // COLLECTION (Application)
  0x85, 0x02,                    //   REPORT_ID (2)

  0x05, 0x07,                    //   USAGE_PAGE (Keyboard)
  0x19, 0xe0,                    //   USAGE_MINIMUM (Keyboard LeftControl)
  0x29, 0xe7,                    //   USAGE_MAXIMUM (Keyboard Right GUI)
  0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
  0x25, 0x01,                    //   LOGICAL_MAXIMUM (1) (DV)
  0x75, 0x01,                    //   REPORT_SIZE (1)
  0x95, 0x08,                    //   REPORT_COUNT (8) (8 report in 1 bytes; each bit controlling one modifier)
  0x81, 0x02,                    //   INPUT (Data,Var,Abs)

  0x95, 0x01,                    //   REPORT_COUNT (1)
  0x75, 0x08,                    //   REPORT_SIZE (8) (1 report in 1 bytes; OEM reserved)
  0x81, 0x03,                    //   INPUT (Cnst,Var,Abs)

  0x95, 0x06,                    //   REPORT_COUNT (6)
  0x75, 0x08,                    //   REPORT_SIZE (8) (6 report in 6 bytes; each byte controlling one key)
  0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
  0x25, 0x65,                    //   LOGICAL_MAXIMUM (101) (Sel)
  0x05, 0x07,                    //   USAGE_PAGE (Keyboard)
  0x19, 0x00,                    //   USAGE_MINIMUM (Reserved (no event indicated))
  0x29, 0x65,                    //   USAGE_MAXIMUM (Keyboard Application)
  0x81, 0x00,                    //   INPUT (Data,Ary,Abs)

  0xc0,                          // END_COLLECTION

  // ==== Mouse ====
  /*                                     == Report Structure ==
    +========+==========+==========+==========+==========+==========+==========+==========+==========+
    |        |    0     |    1     |    2     |    3     |    4     |    5     |    6     |    7     |
    +========+==========+==========+==========+==========+==========+==========+==========+==========+
    |  0x00  |                                     Report ID (1)                                     |
    +--------+----------+----------+----------+----------+----------+----------+----------+----------+
    |  0x08  | Button 1 | Button 2 | Button 3 | Reserved | Reserved | Reserved | Reserved | Reserved |
    +--------+----------+----------+----------+----------+----------+----------+----------+----------+
    |  0x10  |                                           X                                           |
    +--------+----------+----------+----------+----------+----------+----------+----------+----------+
    |  0x18  |                                           Y                                           |
    +--------+----------+----------+----------+----------+----------+----------+----------+----------+
    |  0x20  |                                         Wheel                                         |
    +--------+----------+----------+----------+----------+----------+----------+----------+----------+
  */
  0x05, 0x01,                    // USAGE_PAGE (Generic Desktop)  // 54
  0x09, 0x02,                    // USAGE (Mouse)
  0xa1, 0x01,                    // COLLECTION (Application)
  0x09, 0x01,                    //   USAGE (Pointer)
  0xa1, 0x00,                    //   COLLECTION (Physical)
  0x85, 0x01,                    //     REPORT_ID (1)

  0x05, 0x09,                    //     USAGE_PAGE (Button)
  0x19, 0x01,                    //     USAGE_MINIMUM (Button 1)
  0x29, 0x03,                    //     USAGE_MAXIMUM (Button 3)
  0x15, 0x00,                    //     LOGICAL_MINIMUM (0)
  0x25, 0x01,                    //     LOGICAL_MAXIMUM (1) (OOC: single throw momentary switch)
  0x95, 0x03,                    //     REPORT_COUNT (3)
  0x75, 0x01,                    //     REPORT_SIZE (1) (3 report in 3 bytes; each byte controlling one button)
  0x81, 0x02,                    //     INPUT (Data,Var,Abs)

  0x95, 0x01,                    //     REPORT_COUNT (1)
  0x75, 0x05,                    //     REPORT_SIZE (5) (1 report in 5 bytes; filler for byte alignment)
  0x81, 0x03,                    //     INPUT (Cnst,Var,Abs)

  0x05, 0x01,                    //     USAGE_PAGE (Generic Desktop)
  0x09, 0x30,                    //     USAGE (X)
  0x09, 0x31,                    //     USAGE (Y)
  0x09, 0x38,                    //     USAGE (Wheel)
  0x15, 0x81,                    //     LOGICAL_MINIMUM (-127)
  0x25, 0x7f,                    //     LOGICAL_MAXIMUM (127) (DV)
  0x75, 0x08,                    //     REPORT_SIZE (8)
  0x95, 0x03,                    //     REPORT_COUNT (3) (3 report in 24 bytes; bytes are controlling X, Y and Wheel)
  0x81, 0x06,                    //     INPUT (Data,Var,Rel)

  0xc0,                          //   END_COLLECTION

  /* USER CODE END 0 */
  0xC0    /*     END_COLLECTION	             */
};

/* USER CODE BEGIN PRIVATE_VARIABLES */

/* USER CODE END PRIVATE_VARIABLES */

/**
  * @}
  */

/** @defgroup USBD_CUSTOM_HID_Exported_Variables USBD_CUSTOM_HID_Exported_Variables
  * @brief Public variables.
  * @{
  */
extern USBD_HandleTypeDef hUsbDeviceFS;

/* USER CODE BEGIN EXPORTED_VARIABLES */

/* USER CODE END EXPORTED_VARIABLES */
/**
  * @}
  */

/** @defgroup USBD_CUSTOM_HID_Private_FunctionPrototypes USBD_CUSTOM_HID_Private_FunctionPrototypes
  * @brief Private functions declaration.
  * @{
  */

static int8_t CUSTOM_HID_Init_FS(void);
static int8_t CUSTOM_HID_DeInit_FS(void);
static int8_t CUSTOM_HID_OutEvent_FS(uint8_t event_idx, uint8_t state);

/**
  * @}
  */

USBD_CUSTOM_HID_ItfTypeDef USBD_CustomHID_fops_FS =
{
  CUSTOM_HID_ReportDesc_FS,
  CUSTOM_HID_Init_FS,
  CUSTOM_HID_DeInit_FS,
  CUSTOM_HID_OutEvent_FS
};

/** @defgroup USBD_CUSTOM_HID_Private_Functions USBD_CUSTOM_HID_Private_Functions
  * @brief Private functions.
  * @{
  */

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Initializes the CUSTOM HID media low layer
  * @retval USBD_OK if all operations are OK else USBD_FAIL
  */
static int8_t CUSTOM_HID_Init_FS(void)
{
  /* USER CODE BEGIN 4 */
  return (USBD_OK);
  /* USER CODE END 4 */
}

/**
  * @brief  DeInitializes the CUSTOM HID media low layer
  * @retval USBD_OK if all operations are OK else USBD_FAIL
  */
static int8_t CUSTOM_HID_DeInit_FS(void)
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
static int8_t CUSTOM_HID_OutEvent_FS(uint8_t event_idx, uint8_t state)
{
  /* USER CODE BEGIN 6 */
  UNUSED(event_idx);
  UNUSED(state);

  /* Start next USB packet transfer once data processing is completed */
  if (USBD_CUSTOM_HID_ReceivePacket(&hUsbDeviceFS) != (uint8_t)USBD_OK)
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
static int8_t USBD_CUSTOM_HID_SendReport_FS(uint8_t *report, uint16_t len)
{
  return USBD_CUSTOM_HID_SendReport(&hUsbDeviceFS, report, len);
}
*/

uint8_t USBD_CUSTOM_HID_SendCtrlReport_FS(uint16_t ctrl){
  uint8_t ctrlReportBuffer[CTRL_REPORT_LENGTH];
  ctrlReportBuffer[0] = CTRL_REPORT_ID;
  ctrlReportBuffer[1] = ctrl & 0xFF;
  ctrlReportBuffer[2] = ctrl >> 8 & 0xFF;
  return USBD_CUSTOM_HID_SendReport(&hUsbDeviceFS, ctrlReportBuffer, CTRL_REPORT_LENGTH);
}

uint8_t USBD_CUSTOM_HID_SendKeyboardReport_FS(uint8_t modifier, uint8_t oem, uint8_t key1, uint8_t key2, uint8_t key3, uint8_t key4, uint8_t key5, uint8_t key6) {
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
  return USBD_CUSTOM_HID_SendReport(&hUsbDeviceFS, keyboardReportBuffer, CTRL_REPORT_LENGTH);
}

uint8_t USBD_CUSTOM_HID_SendMouseReport_FS(uint8_t buttons, uint8_t x, uint8_t y, uint8_t wheel) {
  uint8_t mouseReportBuffer[MOUSE_REPORT_LENGTH];
  mouseReportBuffer[0] = MOUSE_REPORT_ID;
  mouseReportBuffer[1] = buttons;
  mouseReportBuffer[2] = x;
  mouseReportBuffer[3] = y;
  mouseReportBuffer[4] = wheel;
  return USBD_CUSTOM_HID_SendReport(&hUsbDeviceFS, mouseReportBuffer, CTRL_REPORT_LENGTH);
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

