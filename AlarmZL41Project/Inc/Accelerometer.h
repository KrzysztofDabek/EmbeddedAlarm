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

#define GRAVITATIONAL_ACCELERATION 9.80665

#define ACC_I2C_HANDLE hi2c1
#define ACC_I2C_TIMEOUT 100
#define ACC_AXIS_COUNT 3
#define ACC_RESOLUTION_G 2
#define ACC_MOTION_THRESHOLD_mg 100
#define ACC_MOTION_THRESHOLD_RESOLUTION_mg 2
#define ACC_MOTION_DURATION_ms 100
#define ACC_MOTION_DURATION_RESOLUTION_ms 1

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

uint16_t g_AxisMeasurements[ACC_AXIS_COUNT];
float g_AggregatedAxisMeasurements[ACC_AXIS_COUNT];
uint8_t g_MotionFlag;

HAL_StatusTypeDef AccelerometerManagePower();
HAL_StatusTypeDef AccelerometerConfigure();
HAL_StatusTypeDef AccelerometerMotionIntConfigure();
HAL_StatusTypeDef AccelerometerInit();
HAL_StatusTypeDef GetAccelerometerData();
void AggregateAccelerometerMeasurementsTo_g();
#endif /* ACCELEROMETER_H_ */
