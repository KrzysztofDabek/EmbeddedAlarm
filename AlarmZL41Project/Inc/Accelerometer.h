/*
 * Accelerometer.h
 *
 *  Created on: 11.04.2017
 *      Author: Krzysztof
 */

#include "stm32f4xx_hal.h"
#include "main.h"

#ifndef ACCELEROMETER_H_
#define ACCELEROMETER_H_

#define ACC_I2C_HANDLE hi2c1
#define ACC_I2C_TIMEOUT 100

#define MPU6050_DEVICE_ADDRESS	0x68
#define MPU6050_PWR_MGMT1 		0x6B
#define MPU6050_PWR_MGMT2 		0x6C
#define MPU6050_CONFIG        	0x1A
#define MPU6050_ACC_CONFIG 		0x1C
#define MPU6050_MOT_THR        	0x1F
#define MPU6050_MOT_DUR        	0x20
#define MPU6050_INT_PIN_CFG		0x37
#define MPU6050_INT_ENABLE		0x38
#define MPU6050_DMP_INT_STATUS	0x39
#define MPU6050_INT_STATUS		0x3A
#define MPU6050_ACC_XOUT_H		0x3B
#define MPU6050_ACC_XOUT_L		0x3C
#define MPU6050_ACC_YOUT_H		0x3D
#define MPU6050_ACC_YOUT_L		0x3E
#define MPU6050_ACC_ZOUT_H		0x3F
#define MPU6050_ACC_ZOUT_L		0x40

HAL_StatusTypeDef AccelerometerManagePower();
HAL_StatusTypeDef AccelerometerConfigure();
HAL_StatusTypeDef AccelerometerInit();

#endif /* ACCELEROMETER_H_ */
