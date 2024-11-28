/*
 * mpu6050.h
 *
 *  Created on: Nov 28, 2024
 *      Author: huse2
 */

#ifndef INC_MPU6050_H_
#define INC_MPU6050_H_

#include "main.h"

#define TIMEOUT                   1000
#define MPU6050_DEVICE_ADDRESS    0xD0
#define MPU6050_REG_WHO_AM_I      0x75
#define MPU6050_REG_PWR_MGMT_1    0x6B
#define MPU6050_REG_PWR_MGMT_2    0x6C
#define MPU6050_REG_ACCEL_CONFIG  0x1C
#define MPU6050_REG_ACCEL_XOUT_H  0x3B
#define MPU6050_REG_GYRO_CONFIG   0x1B
#define MPU6050_REG_GYRO_XOUT_H   0x43
#define MPU6050_REG_TEMP_OUT_H    0x41

#define MPU6050_ACCEL_RANGE_2G     0x00
#define MPU6050_ACCEL_RANGE_4G     0x01
#define MPU6050_ACCEL_RANGE_8G     0x02
#define MPU6050_ACCEL_RANGE_16G    0x03

#define MPU6050_GYRO_RANGE_250     0x00
#define MPU6050_GYRO_RANGE_500     0x01
#define MPU6050_GYRO_RANGE_1000    0x02
#define MPU6050_GYRO_RANGE_2000    0x03

typedef enum{
	READ_FAIL=0,
	READ_SUCCESS=1,
}MPU6050ReadStatus;
typedef enum{
	WRITE_FAIL=0,
	WRITE_SUCCESS=1,
}MPU6050WriteStatus;
typedef enum{
	INIT_FAIL=0,
	INIT_SUCCESS=1,
}MPU6050InitStatus;

typedef struct{
	uint8_t ClkSel: 3;
	uint8_t Temp_Dis: 1;
	uint8_t Reserved: 1;
	uint8_t Cycle: 1;
	uint8_t Sleep: 1;
	uint8_t Device_Reset: 1;

}PowerManagementRegister_t;

typedef struct{
	uint8_t Reserved: 3;
	uint8_t AFS_Sel: 2;
	uint8_t ZA_ST: 1;
	uint8_t YA_ST: 1;
	uint8_t XA_ST: 1;

}AccelConfigRegister_t;

typedef struct{
	uint8_t Reserved: 3;
	uint8_t FS_Sel: 2;
	uint8_t ZG_ST: 1;
	uint8_t YG_ST: 1;
	uint8_t XG_ST: 1;

}GyroConfigRegister_t;


int MPU6050_ScanDeviceID(I2C_HandleTypeDef *hi2cx);
MPU6050InitStatus MPU6050_Init(I2C_HandleTypeDef *hi2cx, uint8_t AFS_SEL, uint8_t FS_SEL);
void MPU6050_getAccelValue(I2C_HandleTypeDef *hi2cx, int16_t *accelData);
void MPU6050_getGyroValue(I2C_HandleTypeDef *hi2cx, int16_t *gyroData);
float MPU6050_getTempValue(I2C_HandleTypeDef *hi2cx, int16_t *tempData);
void MPU6050_getAccelIng(int16_t *accelData, uint8_t AFS_SEL, float *accelDataIng);
void MPU6050_getGyroIns(int16_t *gyroData, uint8_t FS_SEL, float *gyroDataIns);

#endif /* INC_MPU6050_H_ */
