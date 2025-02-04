/*
 * USBTransmission.c
 *
 *  Created on: 12.05.2017
 *      Author: Krzysztof
 */
#include<string.h>

#include"usb_device.h"
#include"usbd_cdc_if.h"
#include"tim.h"

#include"EEPROM.h"
#include"PositionLog.h"
#include"USBTransmission.h"

USBD_StatusTypeDef TransmitPositionLog()
{
	uint8_t OutputData[MAX_OUTPUT_DATA_SIZE];
	uint16_t MessageSize;
	USBD_StatusTypeDef TransmisionStatus;
	USBD_CDC_HandleTypeDef *hcdc = (USBD_CDC_HandleTypeDef*)hUsbDeviceFS.pClassData;

	if(hcdc->TxState != USBD_OK)
		return hcdc->TxState;
	for(uint8_t i=0; i<POSITION_LOG_CAPACITY;++i)
	{
		while(hcdc->TxState != USBD_OK)
			;
		MessageSize=sprintf(OutputData,"%d ",g_PositionLog.Positions[i]);
		TransmisionStatus=CDC_Transmit_FS(OutputData,MessageSize);
		if(TransmisionStatus==USBD_FAIL)
			return TransmisionStatus;
	}
	return TransmisionStatus;
}

void USB_CheckReceivedData()
{
	if(g_USBRcvCpltFlag)
	{
		g_USBRcvCpltFlag=0;

		if( !strncmp(g_ReceivedDataBuffer,USB_LOG_REQUEST_A,USB_LOG_REQUEST_SIZE) ||
				!strncmp(g_ReceivedDataBuffer,USB_LOG_REQUEST_A,USB_LOG_REQUEST_SIZE))
			TransmitPositionLog();
		else
		{
			uint8_t OutputData[MAX_OUTPUT_DATA_SIZE];
			uint16_t MessageSize;
			MessageSize = sprintf(OutputData, "Wrong Command!");
			CDC_Transmit_FS(OutputData, MessageSize);
		}
	}
}
