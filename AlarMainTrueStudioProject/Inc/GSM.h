/*
 * GSM.h
 *
 *  Created on: 10.04.2017
 *      Author: Krzysztof
 */

#ifndef GSM_H_
#define GSM_H_

#define GSM_AUTOBAUDRATE_ONSET 3 //seconds
#define GSM_AUTOBAUDRATE_TIMEOUT 1000 //milliseconds
#define GSM_TRANSMISION_TIMEOUT 100 //milliseconds
#define GSM_AUTOBAUDRATE_MESSAGE_SIZE 2 //bytes
#define GSM_AUTOBAUDRATE_ATTEMPTS 3

HAL_StatusTypeDef GSM_AutoBaudrate_Synchronize(RTC_TimeTypeDef p_RTCtime);

#endif /* GSM_H_ */