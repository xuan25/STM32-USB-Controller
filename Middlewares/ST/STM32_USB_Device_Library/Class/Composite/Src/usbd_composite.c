/* Includes ------------------------------------------------------------------*/
#include "usbd_composite.h"
#include "usbd_desc.h"
#include "usbd_ctlreq.h"

#include "usbd_customhid.h"
#include "usbd_midi.h"

/** @addtogroup STM32_USB_DEVICE_LIBRARY
  * @{
  */


/** @defgroup USBD_COMPOSITE
  * @brief usbd core module
  * @{
  */

/** @defgroup USBD_COMPOSITE_Private_TypesDefinitions
  * @{
  */
/**
  * @}
  */


/** @defgroup USBD_COMPOSITE_Private_Defines
  * @{
  */

/**
  * @}
  */


/** @defgroup USBD_COMPOSITE_Private_Macros
  * @{
  */

/**
  * @}
  */




/** @defgroup USBD_COMPOSITE_Private_FunctionPrototypes
  * @{
  */


static uint8_t  USBD_COMPOSITE_Init (USBD_HandleTypeDef *pdev, 
								   uint8_t cfgidx);

static uint8_t  USBD_COMPOSITE_DeInit (USBD_HandleTypeDef *pdev, 
									 uint8_t cfgidx);

static uint8_t  USBD_COMPOSITE_Setup (USBD_HandleTypeDef *pdev, 
									USBD_SetupReqTypedef *req);

static const uint8_t  *USBD_COMPOSITE_GetCfgDesc (uint16_t *length);

static const uint8_t  *USBD_COMPOSITE_GetDeviceQualifierDesc (uint16_t *length);

static uint8_t  USBD_COMPOSITE_DataIn (USBD_HandleTypeDef *pdev, uint8_t epnum);

static uint8_t  USBD_COMPOSITE_DataOut (USBD_HandleTypeDef *pdev, uint8_t epnum);

static uint8_t  USBD_COMPOSITE_EP0_RxReady (USBD_HandleTypeDef *pdev);

/**
  * @}
  */

/** @defgroup USBD_COMPOSITE_Private_Variables
  * @{
  */

USBD_ClassTypeDef  USBD_COMPOSITE_ClassDriver = 
{
	USBD_COMPOSITE_Init,
	USBD_COMPOSITE_DeInit,
	USBD_COMPOSITE_Setup,
	NULL, //USBD_COMPOSITE_EP0_TxReady,
	USBD_COMPOSITE_EP0_RxReady,
	USBD_COMPOSITE_DataIn,
	USBD_COMPOSITE_DataOut,
	NULL, //USBD_COMPOSITE_SOF,
	NULL, //USBD_COMPOSITE_IsoINIncomplete,
	NULL, //USBD_COMPOSITE_IsoOutIncomplete,
	USBD_COMPOSITE_GetCfgDesc,
	USBD_COMPOSITE_GetCfgDesc,
	USBD_COMPOSITE_GetCfgDesc,
	USBD_COMPOSITE_GetDeviceQualifierDesc,
};

#if defined ( __ICCARM__ ) /*!< IAR Compiler */
#pragma data_alignment=4
#endif

#define USB_COMPOSITE_CONFIG_DESC_SIZ       (9 + (USB_CUSTOM_HID_CONFIG_DESC_SIZ - 9) + (USB_MIDI_CONFIG_DESC_SIZE - 9))

/* USB MSC+CDC device Configuration Descriptor */
static const uint8_t USBD_COMPOSITE_CfgDesc[USB_COMPOSITE_CONFIG_DESC_SIZ] =
{
	0x09,         /* bLength: Configuation Descriptor size */
	USB_DESC_TYPE_CONFIGURATION, /* bDescriptorType: Configuration */
	USB_COMPOSITE_CONFIG_DESC_SIZ, /* wTotalLength: Bytes returned */
	0x00,
	0x02,         /*bNumInterfaces: 2 interface*/
	0x01,         /*bConfigurationValue: Configuration value*/
	0x00,         /*iConfiguration: Index of string descriptor describing the configuration*/
	0xC0,         /*bmAttributes: bus powered and Supports Remote Wakeup */
	0x32,         /*MaxPower 100 mA: this current is used for detecting Vbus*/
	/* 09 bytes */

	/************** Descriptor of CUSTOM HID interface ****************/
  /* 09 */
  0x09,                                               /* bLength: Interface Descriptor size*/
  USB_DESC_TYPE_INTERFACE,                            /* bDescriptorType: Interface descriptor type */
  COMPOSITE_CUSTOM_HID_INTERFACE_IDX,                 /* bInterfaceNumber: Number of Interface */
  0x00,                                               /* bAlternateSetting: Alternate setting */
  0x02,                                               /* bNumEndpoints*/
  0x03,                                               /* bInterfaceClass: CUSTOM_HID */
  0x00,                                               /* bInterfaceSubClass : 1=BOOT, 0=no boot */
  0x00,                                               /* nInterfaceProtocol : 0=none, 1=keyboard, 2=mouse */
  0x00,                                               /* iInterface: Index of string descriptor */
  /******************** Descriptor of CUSTOM_HID *************************/
  /* 18 */
  0x09,                                               /* bLength: CUSTOM_HID Descriptor size */
  CUSTOM_HID_DESCRIPTOR_TYPE,                         /* bDescriptorType: CUSTOM_HID */
  0x11,                                               /* bCUSTOM_HIDUSTOM_HID: CUSTOM_HID Class Spec release number */
  0x01,
  0x00,                                               /* bCountryCode: Hardware target country */
  0x01,                                               /* bNumDescriptors: Number of CUSTOM_HID class descriptors
                                                         to follow */
  0x22,                                               /* bDescriptorType */
  USBD_CUSTOM_HID_REPORT_DESC_SIZE,                   /* wItemLength: Total length of Report descriptor */
  0x00,
  /******************** Descriptor of Custom HID endpoints ********************/
  /* 27 */
  0x07,                                               /* bLength: Endpoint Descriptor size */
  USB_DESC_TYPE_ENDPOINT,                             /* bDescriptorType: */

  COMPOSITE_CUSTOM_HID_EPIN_ADDR,                     /* bEndpointAddress: Endpoint Address (IN) */
  0x03,                                               /* bmAttributes: Interrupt endpoint */
  CUSTOM_HID_EPIN_SIZE,                               /* wMaxPacketSize: 2 Bytes max */
  0x00,
  CUSTOM_HID_FS_BINTERVAL,                            /* bInterval: Polling Interval */
  /* 34 */

  0x07,                                               /* bLength: Endpoint Descriptor size */
  USB_DESC_TYPE_ENDPOINT,                             /* bDescriptorType: */
  COMPOSITE_CUSTOM_HID_EPOUT_ADDR,                    /* bEndpointAddress: Endpoint Address (OUT) */
  0x03,                                               /* bmAttributes: Interrupt endpoint */
  CUSTOM_HID_EPOUT_SIZE,                              /* wMaxPacketSize: 2 Bytes max  */
  0x00,
  CUSTOM_HID_FS_BINTERVAL,                            /* bInterval: Polling Interval */
  /* 41 */

  /************** MIDI Adapter Standard MS Interface Descriptor ****************/
  0x09,                   /*bLength: Interface Descriptor size*/
  USB_DESC_TYPE_INTERFACE,/*bDescriptorType: Interface descriptor type*/
  COMPOSITE_MIDI_INTERFACE_IDX,                   /*bInterfaceNumber: Index of this interface.*/
  0x00,                   /*bAlternateSetting: Alternate setting*/
  0x02,                   /*bNumEndpoints*/
  0x01,                   /*bInterfaceClass: AUDIO*/
  0x03,                   /*bInterfaceSubClass : MIDISTREAMING*/
  0x00,                   /*nInterfaceProtocol : Unused*/
  0x00,                   /*iInterface: Unused*/

  /******************** MIDI Adapter Class-specific MS Interface Descriptor ********************/
  /* USB_MIDI_CLASS_DESC_SHIFT */
  0x07,                 /*bLength: Descriptor size*/
  0x24,                 /*bDescriptorType: CS_INTERFACE descriptor*/
  0x01,                 /*bDescriptorSubtype: MS_HEADER subtype*/
  0x00,
  0x01,                 /*BcdADC: Revision of this class specification*/
  USB_MIDI_REPORT_DESC_SIZE,
  0x00,                  /*wTotalLength: Total size of class-specific descriptors*/

  /******************** MIDI Adapter MIDI IN Jack Descriptor (External) ********************/
  0x06,                   /*bLength: Size of this descriptor, in bytes*/
  0x24,                   /*bDescriptorType: CS_INTERFACE descriptor.*/
  0x02,                   /*bDescriptorSubtype: MIDI_IN_JACK subtype*/
  0x02,                   /*bJackType: EXTERNAL.*/
  MIDI_JACK_1,            /*bJackID: ID of this Jack.*/
  0x00,                   /*iJack: Unused.*/

  /******************** MIDI Adapter MIDI OUT Jack Descriptor (Embedded) ********************/
  0x09,                   /*bLength: Size of this descriptor, in bytes*/
  0x24,                   /*bDescriptorType: CS_INTERFACE descriptor.*/
  0x03,                   /*bDescriptorSubtype: MIDI_OUT_JACK subtype*/
  0x01,                   /*bJackType: EMBEDDED*/
  MIDI_JACK_2,            /*bJackID: ID of this Jack.*/
  0x01,                   /*bNrInputPins: Number of Input Pins of this Jack.*/
  MIDI_JACK_1,            /*BaSourceID(1): ID of the Entity to which this Pin is connected.*/
  0x01,                   /*BaSourcePin(1): Output Pin number of the Entity to which this Input Pin is connected.*/
  0x00,                   /*iJack: Unused.*/

#if MIDI_OUT_PORTS_NUM >= 1
  /******************** MIDI Adapter MIDI IN Jack Descriptor (Embedded) ********************/
  0x06,                   /*bLength: Size of this descriptor, in bytes*/
  0x24,                   /*bDescriptorType: CS_INTERFACE descriptor.*/
  0x02,                   /*bDescriptorSubtype: MIDI_IN_JACK subtype*/
  0x01,                   /*bJackType: EMBEDDED*/
  MIDI_JACK_17,           /*bJackID: ID of this Jack.*/
  0x00,                   /*iJack: Unused.*/

  /******************** MIDI Adapter MIDI OUT Jack Descriptor (External) ********************/
  0x09,                   /*bLength: Size of this descriptor, in bytes*/
  0x24,                   /*bDescriptorType: CS_INTERFACE descriptor.*/
  0x03,                   /*bDescriptorSubtype: MIDI_OUT_JACK subtype*/
  0x02,                   /*bJackType: EXTERNAL.*/
  MIDI_JACK_18,           /*bJackID: ID of this Jack.*/
  0x01,                   /*bNrInputPins: Number of Input Pins of this Jack.*/
  MIDI_JACK_17,           /*BaSourceID(1): ID of the Entity to which this Pin is connected.*/
  0x01,                   /*BaSourcePin(1): Output Pin number of the Entity to which this Input Pin is connected.*/
  0x00,                   /*iJack: Unused.*/
#endif

#if MIDI_OUT_PORTS_NUM >= 2
  /******************** MIDI Adapter MIDI IN Jack Descriptor (Embedded) ********************/
  0x06,                   /*bLength: Size of this descriptor, in bytes*/
  0x24,                   /*bDescriptorType: CS_INTERFACE descriptor.*/
  0x02,                   /*bDescriptorSubtype: MIDI_IN_JACK subtype*/
  0x01,                   /*bJackType: EMBEDDED*/
  MIDI_JACK_19,           /*bJackID: ID of this Jack.*/
  0x00,                   /*iJack: Unused.*/

  /******************** MIDI Adapter MIDI OUT Jack Descriptor (External) ********************/
  0x09,                   /*bLength: Size of this descriptor, in bytes*/
  0x24,                   /*bDescriptorType: CS_INTERFACE descriptor.*/
  0x03,                   /*bDescriptorSubtype: MIDI_OUT_JACK subtype*/
  0x02,                   /*bJackType: EXTERNAL.*/
  MIDI_JACK_20,           /*bJackID: ID of this Jack.*/
  0x01,                   /*bNrInputPins: Number of Input Pins of this Jack.*/
  MIDI_JACK_19,           /*BaSourceID(1): ID of the Entity to which this Pin is connected.*/
  0x01,                   /*BaSourcePin(1): Output Pin number of the Entity to which this Input Pin is connected.*/
  0x00,                   /*iJack: Unused.*/
#endif

#if MIDI_OUT_PORTS_NUM >= 3
  /******************** MIDI Adapter MIDI IN Jack Descriptor (Embedded) ********************/
  0x06,                   /*bLength: Size of this descriptor, in bytes*/
  0x24,                   /*bDescriptorType: CS_INTERFACE descriptor.*/
  0x02,                   /*bDescriptorSubtype: MIDI_IN_JACK subtype*/
  0x01,                   /*bJackType: EMBEDDED*/
  MIDI_JACK_21,           /*bJackID: ID of this Jack.*/
  0x00,                   /*iJack: Unused.*/

  /******************** MIDI Adapter MIDI OUT Jack Descriptor (External) ********************/
  0x09,                   /*bLength: Size of this descriptor, in bytes*/
  0x24,                   /*bDescriptorType: CS_INTERFACE descriptor.*/
  0x03,                   /*bDescriptorSubtype: MIDI_OUT_JACK subtype*/
  0x02,                   /*bJackType: EXTERNAL.*/
  MIDI_JACK_22,           /*bJackID: ID of this Jack.*/
  0x01,                   /*bNrInputPins: Number of Input Pins of this Jack.*/
  MIDI_JACK_21,           /*BaSourceID(1): ID of the Entity to which this Pin is connected.*/
  0x01,                   /*BaSourcePin(1): Output Pin number of the Entity to which this Input Pin is connected.*/
  0x00,                   /*iJack: Unused.*/
#endif

#if MIDI_OUT_PORTS_NUM >= 4
  /******************** MIDI Adapter MIDI IN Jack Descriptor (Embedded) ********************/
  0x06,                   /*bLength: Size of this descriptor, in bytes*/
  0x24,                   /*bDescriptorType: CS_INTERFACE descriptor.*/
  0x02,                   /*bDescriptorSubtype: MIDI_IN_JACK subtype*/
  0x01,                   /*bJackType: EMBEDDED*/
  MIDI_JACK_23,           /*bJackID: ID of this Jack.*/
  0x00,                   /*iJack: Unused.*/

  /******************** MIDI Adapter MIDI OUT Jack Descriptor (External) ********************/
  0x09,                   /*bLength: Size of this descriptor, in bytes*/
  0x24,                   /*bDescriptorType: CS_INTERFACE descriptor.*/
  0x03,                   /*bDescriptorSubtype: MIDI_OUT_JACK subtype*/
  0x02,                   /*bJackType: EXTERNAL.*/
  MIDI_JACK_24,           /*bJackID: ID of this Jack.*/
  0x01,                   /*bNrInputPins: Number of Input Pins of this Jack.*/
  MIDI_JACK_23,           /*BaSourceID(1): ID of the Entity to which this Pin is connected.*/
  0x01,                   /*BaSourcePin(1): Output Pin number of the Entity to which this Input Pin is connected.*/
  0x00,                   /*iJack: Unused.*/
#endif

#if MIDI_OUT_PORTS_NUM >= 5
  /******************** MIDI Adapter MIDI IN Jack Descriptor (Embedded) ********************/
  0x06,                   /*bLength: Size of this descriptor, in bytes*/
  0x24,                   /*bDescriptorType: CS_INTERFACE descriptor.*/
  0x02,                   /*bDescriptorSubtype: MIDI_IN_JACK subtype*/
  0x01,                   /*bJackType: EMBEDDED*/
  MIDI_JACK_25,           /*bJackID: ID of this Jack.*/
  0x00,                   /*iJack: Unused.*/

  /******************** MIDI Adapter MIDI OUT Jack Descriptor (External) ********************/
  0x09,                   /*bLength: Size of this descriptor, in bytes*/
  0x24,                   /*bDescriptorType: CS_INTERFACE descriptor.*/
  0x03,                   /*bDescriptorSubtype: MIDI_OUT_JACK subtype*/
  0x02,                   /*bJackType: EXTERNAL.*/
  MIDI_JACK_26,           /*bJackID: ID of this Jack.*/
  0x01,                   /*bNrInputPins: Number of Input Pins of this Jack.*/
  MIDI_JACK_25,           /*BaSourceID(1): ID of the Entity to which this Pin is connected.*/
  0x01,                   /*BaSourcePin(1): Output Pin number of the Entity to which this Input Pin is connected.*/
  0x00,                   /*iJack: Unused.*/
#endif

#if MIDI_OUT_PORTS_NUM >= 6
  /******************** MIDI Adapter MIDI IN Jack Descriptor (Embedded) ********************/
  0x06,                   /*bLength: Size of this descriptor, in bytes*/
  0x24,                   /*bDescriptorType: CS_INTERFACE descriptor.*/
  0x02,                   /*bDescriptorSubtype: MIDI_IN_JACK subtype*/
  0x01,                   /*bJackType: EMBEDDED*/
  MIDI_JACK_27,           /*bJackID: ID of this Jack.*/
  0x00,                   /*iJack: Unused.*/

  /******************** MIDI Adapter MIDI OUT Jack Descriptor (External) ********************/
  0x09,                   /*bLength: Size of this descriptor, in bytes*/
  0x24,                   /*bDescriptorType: CS_INTERFACE descriptor.*/
  0x03,                   /*bDescriptorSubtype: MIDI_OUT_JACK subtype*/
  0x02,                   /*bJackType: EXTERNAL.*/
  MIDI_JACK_28,           /*bJackID: ID of this Jack.*/
  0x01,                   /*bNrInputPins: Number of Input Pins of this Jack.*/
  MIDI_JACK_27,           /*BaSourceID(1): ID of the Entity to which this Pin is connected.*/
  0x01,                   /*BaSourcePin(1): Output Pin number of the Entity to which this Input Pin is connected.*/
  0x00,                   /*iJack: Unused.*/
#endif

#if MIDI_OUT_PORTS_NUM >= 7
  /******************** MIDI Adapter MIDI IN Jack Descriptor (Embedded) ********************/
  0x06,                   /*bLength: Size of this descriptor, in bytes*/
  0x24,                   /*bDescriptorType: CS_INTERFACE descriptor.*/
  0x02,                   /*bDescriptorSubtype: MIDI_IN_JACK subtype*/
  0x01,                   /*bJackType: EMBEDDED*/
  MIDI_JACK_29,           /*bJackID: ID of this Jack.*/
  0x00,                   /*iJack: Unused.*/

  /******************** MIDI Adapter MIDI OUT Jack Descriptor (External) ********************/
  0x09,                   /*bLength: Size of this descriptor, in bytes*/
  0x24,                   /*bDescriptorType: CS_INTERFACE descriptor.*/
  0x03,                   /*bDescriptorSubtype: MIDI_OUT_JACK subtype*/
  0x02,                   /*bJackType: EXTERNAL.*/
  MIDI_JACK_30,           /*bJackID: ID of this Jack.*/
  0x01,                   /*bNrInputPins: Number of Input Pins of this Jack.*/
  MIDI_JACK_29,           /*BaSourceID(1): ID of the Entity to which this Pin is connected.*/
  0x01,                   /*BaSourcePin(1): Output Pin number of the Entity to which this Input Pin is connected.*/
  0x00,                   /*iJack: Unused.*/
#endif

#if MIDI_OUT_PORTS_NUM >= 8
  /******************** MIDI Adapter MIDI IN Jack Descriptor (Embedded) ********************/
  0x06,                   /*bLength: Size of this descriptor, in bytes*/
  0x24,                   /*bDescriptorType: CS_INTERFACE descriptor.*/
  0x02,                   /*bDescriptorSubtype: MIDI_IN_JACK subtype*/
  0x01,                   /*bJackType: EMBEDDED*/
  MIDI_JACK_31,           /*bJackID: ID of this Jack.*/
  0x00,                   /*iJack: Unused.*/

  /******************** MIDI Adapter MIDI OUT Jack Descriptor (External) ********************/
  0x09,                   /*bLength: Size of this descriptor, in bytes*/
  0x24,                   /*bDescriptorType: CS_INTERFACE descriptor.*/
  0x03,                   /*bDescriptorSubtype: MIDI_OUT_JACK subtype*/
  0x02,                   /*bJackType: EXTERNAL.*/
  MIDI_JACK_32,           /*bJackID: ID of this Jack.*/
  0x01,                   /*bNrInputPins: Number of Input Pins of this Jack.*/
  MIDI_JACK_31,           /*BaSourceID(1): ID of the Entity to which this Pin is connected.*/
  0x01,                   /*BaSourcePin(1): Output Pin number of the Entity to which this Input Pin is connected.*/
  0x00,                   /*iJack: Unused.*/
#endif

  /******************** MIDI Adapter Standard Bulk OUT Endpoint Descriptor ********************/
  0x09,                   /*bLength: Size of this descriptor, in bytes*/
  USB_DESC_TYPE_ENDPOINT, /*bDescriptorType: ENDPOINT descriptor.*/
  COMPOSITE_MIDI_EPOUT_ADDR,        /*bEndpointAddress: OUT Endpoint 1.*/
  0x02,                   /*bmAttributes: Bulk, not shared.*/
  MIDI_EPOUT_SIZE, 
  0x00,                   /*wMaxPacketSize*/
  0x00,                   /*bInterval: Ignored for Bulk. Set to zero.*/
  0x00,                   /*bRefresh: Unused.*/
  0x00,                   /*bSynchAddress: Unused.*/

  /******************** MIDI Adapter Class-specific Bulk OUT Endpoint Descriptor ********************/
  (4 + MIDI_OUT_PORTS_NUM), /*bLength: Size of this descriptor, in bytes*/
  0x25,                     /*bDescriptorType: CS_ENDPOINT descriptor*/
  0x01,                     /*bDescriptorSubtype: MS_GENERAL subtype.*/
  MIDI_OUT_PORTS_NUM,       /*bNumEmbMIDIJack: Number of embedded MIDI IN Jacks.*/
#if MIDI_OUT_PORTS_NUM >= 1
  MIDI_JACK_17,             /*BaAssocJackID(1): ID of the Embedded MIDI IN Jack.*/
#endif
#if MIDI_OUT_PORTS_NUM >= 2
  MIDI_JACK_19,             /*BaAssocJackID(2): ID of the Embedded MIDI IN Jack.*/
#endif
#if MIDI_OUT_PORTS_NUM >= 3
  MIDI_JACK_21,             /*BaAssocJackID(3): ID of the Embedded MIDI IN Jack.*/
#endif
#if MIDI_OUT_PORTS_NUM >= 4
  MIDI_JACK_23,             /*BaAssocJackID(4): ID of the Embedded MIDI IN Jack.*/
#endif
#if MIDI_OUT_PORTS_NUM >= 5
  MIDI_JACK_25,             /*BaAssocJackID(5): ID of the Embedded MIDI IN Jack.*/
#endif
#if MIDI_OUT_PORTS_NUM >= 6
  MIDI_JACK_27,             /*BaAssocJackID(6): ID of the Embedded MIDI IN Jack.*/
#endif
#if MIDI_OUT_PORTS_NUM >= 7
  MIDI_JACK_29,             /*BaAssocJackID(7): ID of the Embedded MIDI IN Jack.*/
#endif
#if MIDI_OUT_PORTS_NUM >= 8
  MIDI_JACK_31,             /*BaAssocJackID(8): ID of the Embedded MIDI IN Jack.*/
#endif

  /******************** MIDI Adapter Standard Bulk IN Endpoint Descriptor ********************/
  0x09,                    /*bLength: Size of this descriptor, in bytes*/
  USB_DESC_TYPE_ENDPOINT,  /*bDescriptorType: ENDPOINT descriptor.*/
  COMPOSITE_MIDI_EPIN_ADDR,          /*bEndpointAddress: IN Endpoint 1.*/
  0x02,                    /*bmAttributes: Bulk, not shared.*/
  MIDI_EPIN_SIZE, 
  0x00,                    /*wMaxPacketSize*/
  0x00,                    /*bInterval: Ignored for Bulk. Set to zero.*/
  0x00,                    /*bRefresh: Unused.*/
  0x00,                    /*bSynchAddress: Unused.*/

  /******************** MIDI Adapter Class-specific Bulk IN Endpoint Descriptor ********************/
  (4 + MIDI_IN_PORTS_NUM), /*bLength: Size of this descriptor, in bytes*/
  0x25,                    /*bDescriptorType: CS_ENDPOINT descriptor*/
  0x01,                    /*bDescriptorSubtype: MS_GENERAL subtype.*/
  MIDI_IN_PORTS_NUM,       /*bNumEmbMIDIJack: Number of embedded MIDI OUT Jacks.*/
#if MIDI_IN_PORTS_NUM >= 1
  MIDI_JACK_2,             /*BaAssocJackID(1): ID of the Embedded MIDI OUT Jack.*/
#endif
#if MIDI_IN_PORTS_NUM >= 2
  MIDI_JACK_4,             /*BaAssocJackID(2): ID of the Embedded MIDI OUT Jack.*/
#endif
#if MIDI_IN_PORTS_NUM >= 3
  MIDI_JACK_6,             /*BaAssocJackID(3): ID of the Embedded MIDI OUT Jack.*/
#endif
#if MIDI_IN_PORTS_NUM >= 4
  MIDI_JACK_8,             /*BaAssocJackID(4): ID of the Embedded MIDI OUT Jack.*/
#endif
#if MIDI_IN_PORTS_NUM >= 5
  MIDI_JACK_10,            /*BaAssocJackID(5): ID of the Embedded MIDI OUT Jack.*/
#endif
#if MIDI_IN_PORTS_NUM >= 6
  MIDI_JACK_12,            /*BaAssocJackID(6): ID of the Embedded MIDI OUT Jack.*/
#endif
#if MIDI_IN_PORTS_NUM >= 7
  MIDI_JACK_14,            /*BaAssocJackID(7): ID of the Embedded MIDI OUT Jack.*/
#endif
#if MIDI_IN_PORTS_NUM >= 8
  MIDI_JACK_16,            /*BaAssocJackID(8): ID of the Embedded MIDI OUT Jack.*/
#endif
};

#define USB_MAX_PACKET_SIZE 0x40

#if defined ( __ICCARM__ ) /*!< IAR Compiler */
#pragma data_alignment=4
#endif
/* USB Standard Device Descriptor */
static uint8_t USBD_COMPOSITE_DeviceQualifierDesc[USB_LEN_DEV_QUALIFIER_DESC] =
{
	USB_LEN_DEV_QUALIFIER_DESC,
	USB_DESC_TYPE_DEVICE_QUALIFIER,
	0x00,
	0x02,
	0x00,
	0x00,
	0x00,
	USB_MAX_PACKET_SIZE,
	0x01,
	0x00,
};

/**
  * @}
  */

/** @defgroup USBD_COMPOSITE_Private_Functions
  * @{
  */

/**
  * @brief  USBD_COMPOSITE_Init
  *         Initialize the HID+MIDI interface
  * @param  pdev: device instance
  * @param  cfgidx: Configuration index
  * @retval status
  */
static uint8_t  USBD_COMPOSITE_Init (USBD_HandleTypeDef *pdev, 
								   uint8_t cfgidx)
{
	/* HID initialization */
	uint8_t ret = USBD_CUSTOM_HID_Init (pdev, cfgidx);
	if(ret != 0)
		return ret;

	/* MIDI initialization */
	ret = USBD_MIDI_Init (pdev, cfgidx);
	if(ret != 0)
		return ret;

	return USBD_OK;
}

/**
  * @brief  USBD_COMPOSITE_Init
  *         DeInitialize the HID+MIDI layer
  * @param  pdev: device instance
  * @param  cfgidx: Configuration index
  * @retval status
  */
static uint8_t  USBD_COMPOSITE_DeInit (USBD_HandleTypeDef *pdev, uint8_t cfgidx)
{
	/* HID De-initialization */
	USBD_CUSTOM_HID_DeInit(pdev, cfgidx);

	/* MIDI De-initialization */
	USBD_MIDI_DeInit(pdev, cfgidx);

	return USBD_OK;
}

/**
  * @brief  USBD_COMPOSITE_Setup
  *         Handle the HID+MIDI specific requests
  * @param  pdev: instance
  * @param  req: usb requests
  * @retval status
  */
static uint8_t  USBD_COMPOSITE_Setup (USBD_HandleTypeDef *pdev, USBD_SetupReqTypedef *req)
{
	// Route requests to HID interface or its endpoints to HID class implementaion
	if(((req->bmRequest & USB_REQ_RECIPIENT_MASK) == USB_REQ_RECIPIENT_INTERFACE && req->wIndex == COMPOSITE_CUSTOM_HID_INTERFACE_IDX) ||
		((req->bmRequest & USB_REQ_RECIPIENT_MASK) == USB_REQ_RECIPIENT_ENDPOINT && ((req->wIndex & 0x7F) == COMPOSITE_CUSTOM_HID_EP_IDX)))
	{
		return USBD_CUSTOM_HID_Setup(pdev, req);
	}

	return USBD_MIDI_Setup(pdev, req);
}

/**
  * @brief  USBD_COMPOSITE_DataIn
  *         handle data IN Stage
  * @param  pdev: device instance
  * @param  epnum: endpoint index
  * @retval status
  */
static uint8_t  USBD_COMPOSITE_DataIn (USBD_HandleTypeDef *pdev,
									 uint8_t epnum)
{
	if(epnum == COMPOSITE_CUSTOM_HID_EP_IDX)
		return USBD_CUSTOM_HID_DataIn(pdev, epnum);

	return USBD_MIDI_DataIn(pdev, epnum);
}

/**
  * @brief  USBD_COMPOSITE_DataOut
  *         handle data OUT Stage
  * @param  pdev: device instance
  * @param  epnum: endpoint index
  * @retval status
  */
static uint8_t  USBD_COMPOSITE_DataOut (USBD_HandleTypeDef *pdev,
									  uint8_t epnum)
{
	if(epnum == COMPOSITE_CUSTOM_HID_EP_IDX)
		return USBD_CUSTOM_HID_DataOut(pdev, epnum);

	return USBD_MIDI_DataOut(pdev, epnum);
}


/**
  * @brief  USBD_COMPOSITE_GetCfgDesc
  *         return configuration descriptor
  * @param  length : pointer data length
  * @retval pointer to descriptor buffer
  */
static const uint8_t  *USBD_COMPOSITE_GetCfgDesc (uint16_t *length)
{
	*length = sizeof (USBD_COMPOSITE_CfgDesc);
	return USBD_COMPOSITE_CfgDesc;
}

/**
* @brief  DeviceQualifierDescriptor 
*         return Device Qualifier descriptor
* @param  length : pointer data length
* @retval pointer to descriptor buffer
*/
const uint8_t *USBD_COMPOSITE_DeviceQualifierDescriptor (uint16_t *length)
{
	*length = sizeof (USBD_COMPOSITE_DeviceQualifierDesc);
	return USBD_COMPOSITE_DeviceQualifierDesc;
}


/**
  * @brief  USBD_COMPOSITE_EP0_RxReady
  *         handle EP0 Rx Ready event
  * @param  pdev: device instance
  * @retval status
  */
static uint8_t  USBD_COMPOSITE_EP0_RxReady (USBD_HandleTypeDef *pdev)
{
	return USBD_CUSTOM_HID_EP0_RxReady(pdev);
}

/**
* @brief  DeviceQualifierDescriptor 
*         return Device Qualifier descriptor
* @param  length : pointer data length
* @retval pointer to descriptor buffer
*/
const uint8_t  *USBD_COMPOSITE_GetDeviceQualifierDesc (uint16_t *length)
{
	*length = sizeof (USBD_COMPOSITE_DeviceQualifierDesc);
	return USBD_COMPOSITE_DeviceQualifierDesc;
}

/**
  * @}
  */


/**
  * @}
  */


/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
