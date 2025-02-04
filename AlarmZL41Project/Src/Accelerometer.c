/*
 * Accelerometer.c
 *
 *  Created on: 11.04.2017
 *      Author: Krzysztof
 */

#include"i2c.h"
#include"gpio.h"
#include"GSM.h"
#include"GPS.h"
#include"Accelerometer.h"

/*
 * No Reset, Sleep Mode off, CycleMode on, x,
 * Temperature Sensor disabled, Clock Source internal
 */
uint8_t g_MPU6050_PowerManagement1Settings[1] = { 0b00101000 };
/*
 * Wake up 20 Hz,
 * Accelerometer Axis Stand-by Mode: X off, Y off, Z off
 * Gyroscope Axis Stan-by Mode: X off, Y off, Z off
 */
uint8_t g_MPU6050_PowerManagement2Settings[1] = { 0b10000000 };

/*
 * Self Axis Test: X disabled, Y disabled, Z disabled
 * Full scale range for Accelerometer: +/- 16g
 */
uint8_t g_MPU6050_AccelerometerSettings[1] = { 0b00011000 };

/*
 * Active high, push-pull, pulse mode off,
 * Clear on any read,
 */
uint8_t g_MPU6050_InterruptSettings[1] = { 0b00110000 };

/*
 * Motion Interrupt enabled, other disabled
 */
uint8_t g_MPU6050_InterruptEnableSettings[1] = { 0b01000000 };

uint8_t g_MPU6050_MotionInterruptThreshold[1] = { ACC_MOTION_THRESHOLD_mg / ACC_MOTION_THRESHOLD_RESOLUTION_mg };

uint8_t g_MPU6050_MotionInterruptDuration[1] = { ACC_MOTION_DURATION_ms / ACC_MOTION_DURATION_RESOLUTION_ms };

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if(GPIO_Pin == Accelerometer_Interrupt_Pin)
	{
		g_MotionFlag = !g_MotionFlag;//popraw
		//GPS_StartGettingPosition();
		GSM_StartSendingPosition();
	}
}

HAL_StatusTypeDef AccelerometerManagePower()
{
	HAL_StatusTypeDef TransmissionStatus;
	TransmissionStatus = HAL_I2C_Mem_Write(&ACC_I2C_HANDLE,	MPU6050_DEVICE_ADDRESS,
			MPU6050_PWR_MGMT1, 1, g_MPU6050_PowerManagement1Settings, 1, ACC_I2C_TIMEOUT);
	if (TransmissionStatus != HAL_OK)
		return TransmissionStatus;
	TransmissionStatus = HAL_I2C_Mem_Write(&ACC_I2C_HANDLE,	MPU6050_DEVICE_ADDRESS,
			MPU6050_PWR_MGMT2, 1, g_MPU6050_PowerManagement2Settings, 1, ACC_I2C_TIMEOUT);
	return TransmissionStatus;
}

HAL_StatusTypeDef AccelerometerConfigure()
{
	HAL_StatusTypeDef TransmissionStatus;
	TransmissionStatus = HAL_I2C_Mem_Write(&ACC_I2C_HANDLE,	MPU6050_DEVICE_ADDRESS,
			MPU6050_ACC_CONFIG, 1, g_MPU6050_AccelerometerSettings, 1, ACC_I2C_TIMEOUT);
	return TransmissionStatus;
}

HAL_StatusTypeDef AccelerometerMotionIntConfigure()
{
	HAL_StatusTypeDef TransmissionStatus;
	TransmissionStatus = HAL_I2C_Mem_Write(&ACC_I2C_HANDLE,	MPU6050_DEVICE_ADDRESS,
			MPU6050_INT_PIN_CFG,1,g_MPU6050_InterruptSettings,1, ACC_I2C_TIMEOUT);
	if (TransmissionStatus != HAL_OK)
		return TransmissionStatus;
	TransmissionStatus = HAL_I2C_Mem_Write(&ACC_I2C_HANDLE,	MPU6050_DEVICE_ADDRESS,
				MPU6050_INT_ENABLE,1,g_MPU6050_InterruptEnableSettings,1, ACC_I2C_TIMEOUT);
	if (TransmissionStatus != HAL_OK)
		return TransmissionStatus;
	TransmissionStatus = HAL_I2C_Mem_Write(&ACC_I2C_HANDLE,	MPU6050_DEVICE_ADDRESS,
				MPU6050_MOT_THR,1,g_MPU6050_MotionInterruptThreshold,1,ACC_I2C_TIMEOUT);
	if (TransmissionStatus != HAL_OK)
		return TransmissionStatus;
	TransmissionStatus = HAL_I2C_Mem_Write(&ACC_I2C_HANDLE,	MPU6050_DEVICE_ADDRESS,
				MPU6050_MOT_DUR,1,g_MPU6050_MotionInterruptDuration,1,ACC_I2C_TIMEOUT);
	return TransmissionStatus;
}

HAL_StatusTypeDef GetAccelerometerData()
{
	uint8_t RawAccelerometerData[2*ACC_AXIS_COUNT];
		HAL_StatusTypeDef TransmissionStatus;

		TransmissionStatus = HAL_I2C_Mem_Read(&ACC_I2C_HANDLE,	MPU6050_DEVICE_ADDRESS,
				MPU6050_ACC_XOUT_H,
				1, RawAccelerometerData, 2*ACC_AXIS_COUNT,
				ACC_I2C_TIMEOUT);
		if(TransmissionStatus!= HAL_OK)
			return TransmissionStatus;

		g_AxisMeasurements[0] = ((RawAccelerometerData[0] << 8) | RawAccelerometerData[1]);
		g_AxisMeasurements[1] = ((RawAccelerometerData[2] << 8) | RawAccelerometerData[3]);
		g_AxisMeasurements[2] = ((RawAccelerometerData[4] << 8) | RawAccelerometerData[5]);
		return TransmissionStatus;
}

void AggregateAccelerometerMeasurementsTo_g()
{
	for (uint8_t i = 0; i < ACC_AXIS_COUNT; ++i)
		g_AggregatedAxisMeasurements[i] =
				(float) g_AxisMeasurements[i]
						* ACC_RESOLUTION_G / (float) INT16_MAX;
}

HAL_StatusTypeDef AccelerometerInit()
{
	HAL_StatusTypeDef InitStatus;
	g_MotionFlag=0;
	InitStatus=AccelerometerManagePower();
	if(InitStatus!= HAL_OK)
		return InitStatus;
	InitStatus=AccelerometerConfigure();
	if(InitStatus!= HAL_OK)
			return InitStatus;
	InitStatus=AccelerometerMotionIntConfigure();
	return InitStatus;
}
