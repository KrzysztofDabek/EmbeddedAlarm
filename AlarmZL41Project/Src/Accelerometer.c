/*
 * Accelerometer.c
 *
 *  Created on: 11.04.2017
 *      Author: Krzysztof
 */

#include"i2c.h"
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

HAL_StatusTypeDef AccelerometerInit()
{
	HAL_StatusTypeDef InitStatus;
	InitStatus=AccelerometerManagePower();
	if(InitStatus!= HAL_OK)
		return InitStatus;
	InitStatus=AccelerometerConfigure();
	return InitStatus;
}
