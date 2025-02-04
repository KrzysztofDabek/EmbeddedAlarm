/*
 * USBTransmission.h
 *
 *  Created on: 12.05.2017
 *      Author: Krzysztof
 */

#ifndef USBTRANSMISSION_H_
#define USBTRANSMISSION_H_

#include "stm32f4xx_hal.h"
#include "main.h"

#define MAX_INPUT_DATA_SIZE 50
#define MAX_OUTPUT_DATA_SIZE 50
#define USB_LOG_REQUEST_A "LOG"
#define USB_LOG_REQUEST_a "log"
#define USB_LOG_REQUEST_SIZE 3

volatile uint8_t g_ReceivedDataBuffer[MAX_INPUT_DATA_SIZE];
volatile uint8_t g_USBRcvCpltFlag;

USBD_StatusTypeDef USB_TransmitPositionLog();
void USB_CheckReceivedData();

#endif /* USBTRANSMISSION_H_ */
