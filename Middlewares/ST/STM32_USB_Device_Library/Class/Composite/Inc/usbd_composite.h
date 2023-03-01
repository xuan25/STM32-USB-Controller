
/* Define to prevent recursive inclusion -------------------------------------*/ 
#ifndef __USB_COMPOSITE_CORE_H
#define __USB_COMPOSITE_CORE_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include  "usbd_ioreq.h"

#define INEP_DIR_MASK   0x80

#define COMPOSITE_CUSTOM_HID_INTERFACE_IDX    0x0
#define COMPOSITE_MIDI_INTERFACE_IDX          0x1

#define COMPOSITE_CUSTOM_HID_EP_IDX   0x01
#define COMPOSITE_MIDI_EP_IDX         0x02


#define COMPOSITE_CUSTOM_HID_EPOUT_ADDR   COMPOSITE_CUSTOM_HID_EP_IDX
#define COMPOSITE_CUSTOM_HID_EPIN_ADDR    COMPOSITE_CUSTOM_HID_EP_IDX | INEP_DIR_MASK
#define COMPOSITE_MIDI_EPOUT_ADDR         COMPOSITE_MIDI_EP_IDX
#define COMPOSITE_MIDI_EPIN_ADDR          COMPOSITE_MIDI_EP_IDX | INEP_DIR_MASK

// override endpoint address
#define CUSTOM_HID_EPIN_ADDR    COMPOSITE_CUSTOM_HID_EPIN_ADDR
#define CUSTOM_HID_EPOUT_ADDR   COMPOSITE_CUSTOM_HID_EPOUT_ADDR
#define MIDI_EPIN_ADDR          COMPOSITE_MIDI_EPIN_ADDR
#define MIDI_EPOUT_ADDR         COMPOSITE_MIDI_EPOUT_ADDR

uint8_t USBD_COMPOSITE_CUSTOM_HID_SendReport(USBD_HandleTypeDef *pdev, uint8_t *report, uint16_t len);

/** @addtogroup STM32_USB_DEVICE_LIBRARY
  * @{
  */
  

/** @defgroup USBD_COMPOSITE_Exported_Defines
  * @{
  */ 
//#define TEMPLATE_EPIN_ADDR                 0x81
//#define TEMPLATE_EPIN_SIZE                 0x10

/**
  * @}
  */ 


/** @defgroup USBD_CORE_Exported_TypesDefinitions
  * @{
  */

/**
  * @}
  */ 



/** @defgroup USBD_CORE_Exported_Macros
  * @{
  */ 

/**
  * @}
  */ 

/** @defgroup USBD_CORE_Exported_Variables
  * @{
  */ 

extern USBD_ClassTypeDef  USBD_COMPOSITE_ClassDriver;
/**
  * @}
  */ 

/** @defgroup USB_CORE_Exported_Functions
  * @{
  */ 
/**
  * @}
  */ 

#ifdef __cplusplus
}
#endif

#endif  /* __USB_COMPOSITE_CORE_H */
/**
  * @}
  */ 

/**
  * @}
  */ 