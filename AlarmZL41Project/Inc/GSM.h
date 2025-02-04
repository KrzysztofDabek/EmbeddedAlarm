/*
 * GSM.h
 *
 *  Created on: 10.04.2017
 *      Author: Krzysztof
 */

#ifndef GSM_H_
#define GSM_H_

#define TIM7_POSTSCALER 60 //seconds

#define GSM_huart huart6

#define GSM_ATREADY_MESSAGE_SIZE 19 //bytes
#define GSM_ATREADY_TIMEOUT 3000 //milliseconds
#define GSM_RETURN_MESSAGE_SIZE 4 //bytes
#define GSM_AUTOBAUDRATE_TIMEOUT 1000 //milliseconds
#define GSM_TRANSMISSION_TIMEOUT 100 //milliseconds
#define GSM_AUTOBAUDRATE_MESSAGE_SIZE 4 //bytes
#define GSM_AUTOBAUDRATE_ATTEMPTS 8
#define GSM_SIMCARD_PIN "0897"
#define GSM_RECEIVER_PHONE_NUMBER "664872393"
#define GSM_MAX_MESSAGE_SIZE 50

HAL_StatusTypeDef g_GSM_LastCommandStatus;

void GSM_AutoBaudrate_Synchronize();
void GSM_VerifyAutoBaudrate();

HAL_StatusTypeDef GSM_VerifyCommand();
void GSM_UnlockSIMCard();
void GSM_ConfigureSMS();
void GSM_StartSendingPosition();
void GSM_SendPosition();

void GSM_Init();

uint8_t isBaudrateSet();
#endif /* GSM_H_ */
