/*
 * PositionLog.h
 *
 *  Created on: 12.05.2017
 *      Author: Krzysztof
 */

#ifndef POSITIONLOG_H_
#define POSITIONLOG_H_

#include "stm32f4xx_hal.h"
#include "main.h"

#define POSITION_LOG_CAPACITY 3

typedef struct PositionLogStruct_t
{
	uint16_t Positions[POSITION_LOG_CAPACITY];
} PositionLogStruct_t;

PositionLogStruct_t g_PositionLog;

void PositionLog_PushBack();

#endif /* POSITIONLOG_H_ */
