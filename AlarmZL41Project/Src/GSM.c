/*
 * GSM.c
 *
 *  Created on: 10.04.2017
 *      Author: Krzysztof
 */
#include "stm32f4xx_hal.h"
#include "rtc.h"
#include "tim.h"
#include "usart.h"
#include <stdlib.h>
#include "string.h"
#include "EEPROM.h"
#include "PositionLog.h"
#include "GSM.h"

unsigned char g_GSM_ReceivedData[50];
unsigned char g_GSM_LastCommand[50];
uint8_t g_GSM_LastCommandSize;

volatile uint8_t g_GSM_BaudrateSetFlag=0;
volatile uint8_t g_Tim7Seconds=0;

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef* htim)
{
	if(htim==&htim6)
	{
		HAL_UART_Transmit_IT(&GSM_huart,"AT\r\n",GSM_AUTOBAUDRATE_MESSAGE_SIZE);
		g_GSM_LastCommandSize = sizeof("AT\r\n")-1;
	}
	else if(htim==&htim7)
	{
		++g_Tim7Seconds;
		g_Tim7Seconds%=TIM7_POSTSCALER;
		if(g_Tim7Seconds==0)
			GSM_SendPosition();
	}
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef* huart)
{
	if(huart==&GSM_huart)
	{
		if(!g_GSM_BaudrateSetFlag)
			GSM_VerifyAutoBaudrate();
		else
			g_GSM_LastCommandStatus=GSM_VerifyCommand();
	}
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef* huart)
{
	if(huart==&GSM_huart)
		if(!g_GSM_BaudrateSetFlag)
			HAL_UART_Receive_IT(&GSM_huart,g_GSM_ReceivedData,GSM_RETURN_MESSAGE_SIZE+5);
		else
			HAL_UART_Receive_IT(&GSM_huart,g_GSM_ReceivedData,g_GSM_LastCommandSize+7);
}

void GSM_AutoBaudrate_Synchronize()
{
		if(!g_GSM_BaudrateSetFlag)
		{
			HAL_TIM_Base_Start_IT(&htim6);
			HAL_UART_Transmit_IT(&GSM_huart,"AT\r\n",GSM_AUTOBAUDRATE_MESSAGE_SIZE);
			g_GSM_LastCommandSize = sizeof("AT\r\n")-1;
		}
}

void GSM_VerifyAutoBaudrate()
{
	if(strncmp(g_GSM_ReceivedData,"AT\r\r\nOK\r\n",g_GSM_LastCommandSize+GSM_RETURN_MESSAGE_SIZE)==0)
	{
		g_GSM_BaudrateSetFlag=1;
		HAL_TIM_Base_Stop_IT(&htim6);
	}
}

void GSM_UnlockSIMCard()
{
	if(g_GSM_BaudrateSetFlag)
	{
		uint8_t data[GSM_MAX_MESSAGE_SIZE];
		uint16_t MessageSize;
		MessageSize=sprintf(data,"AT+CLCK=\"SC\",1\"%s\"\r\n",GSM_SIMCARD_PIN);
		HAL_UART_Transmit_IT(&GSM_huart,data,MessageSize);
		strncpy(g_GSM_LastCommand ,data,MessageSize-2);
		g_GSM_LastCommandSize = MessageSize-2;

		MessageSize = sprintf(data, "AT+CPIN=\"%s\"\r\n",GSM_SIMCARD_PIN);
		HAL_UART_Transmit_IT(&GSM_huart, data, MessageSize);
		strncpy(g_GSM_LastCommand, data, MessageSize - 2);
		g_GSM_LastCommandSize = MessageSize - 2;
	}
}

void GSM_ConfigureSMS()
{
	if(g_GSM_BaudrateSetFlag)
	{
		uint8_t data[GSM_MAX_MESSAGE_SIZE];
		uint16_t MessageSize;
		MessageSize=sprintf(data,"AT+CMGF=1\r\n",GSM_SIMCARD_PIN);
		HAL_UART_Transmit_IT(&GSM_huart,data,MessageSize);
		strncpy(g_GSM_LastCommand ,data,MessageSize-2);
		g_GSM_LastCommandSize = MessageSize-2;
	}
}

void GSM_StartSendingPosition()
{
	if(g_GSM_BaudrateSetFlag)
	{
		HAL_TIM_Base_Start_IT(&htim7);
	}
}

void GSM_SendPosition()
{
	if(g_GSM_BaudrateSetFlag)
	{
		uint8_t data[GSM_MAX_MESSAGE_SIZE];
		uint16_t MessageSize;
		MessageSize=sprintf(data,"AT+CMGS=\"%s\"\r\n",GSM_RECEIVER_PHONE_NUMBER);
		HAL_UART_Transmit_IT(&GSM_huart,data,MessageSize);
		strncpy(g_GSM_LastCommand ,data,MessageSize-2);
		g_GSM_LastCommandSize = MessageSize-2;

		//Zmienic na odpowiednie dane
		for(uint8_t i=0; i<POSITION_LOG_CAPACITY;++i)
		{
			MessageSize=sprintf(data,"%d, ",g_PositionLog.Positions[i]);
			HAL_UART_Transmit_IT(&GSM_huart,data,MessageSize);
		}
		MessageSize=sprintf(data,"%d\r\n",0x1A); //Send as hex ?
		HAL_UART_Transmit_IT(&GSM_huart,data,MessageSize);
	}
}

void GSM_Init()
{
	GSM_AutoBaudrate_Synchronize();
	GSM_UnlockSIMCard();
	GSM_ConfigureSMS();
}

HAL_StatusTypeDef GSM_VerifyCommand()
{
	if(strncmp(g_GSM_ReceivedData,strcat(g_GSM_LastCommand,"\r\r\nOK\r\n"),g_GSM_LastCommandSize+7))
		return HAL_OK;
	else
		return HAL_ERROR;
}
uint8_t isBaudrateSet()
{
	return g_GSM_BaudrateSetFlag;
}
