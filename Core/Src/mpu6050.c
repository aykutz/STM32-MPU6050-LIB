/*
 * mpu6050.c
 *
 *  Created on: Nov 28, 2024
 *      Author: huse2
 */

#include "mpu6050.h"



int MPU6050_ScanDeviceID(I2C_HandleTypeDef *hi2cx){

	for(uint8_t address=0; address<255; address++){
		if(HAL_I2C_IsDeviceReady(hi2cx, address, 1, TIMEOUT)==HAL_OK){
			return address;
		}
	}
	return -1;
}


static MPU6050ReadStatus MPU6050_ReadRegisterData(I2C_HandleTypeDef *hi2cx, uint16_t registerAddress, uint16_t sizeofData, uint8_t *dataBuffer){
	if(HAL_I2C_Mem_Read(hi2cx, MPU6050_DEVICE_ADDRESS, registerAddress, 1, dataBuffer, sizeofData, TIMEOUT)==HAL_OK){
		return READ_SUCCESS;
	}
	return READ_FAIL;
}

static MPU6050WriteStatus MPU6050_WriteRegisterData(I2C_HandleTypeDef *hi2cx, uint16_t registerAddress, uint16_t value){
	uint8_t data[2]={0};
	data[0]=registerAddress;
	data[1]=value;

	if(HAL_I2C_Master_Transmit(hi2cx, MPU6050_DEVICE_ADDRESS, data, sizeof(data), TIMEOUT)==HAL_OK){
		return WRITE_SUCCESS;
	}
	return WRITE_FAIL;
}

MPU6050InitStatus MPU6050_Init(I2C_HandleTypeDef *hi2cx, uint8_t AFS_SEL, uint8_t FS_SEL){
	uint8_t dataBuffer=0;
	MPU6050_ReadRegisterData(hi2cx, MPU6050_REG_WHO_AM_I, 1, &dataBuffer);
	if(dataBuffer != 0x68)
	{
		return INIT_FAIL;
	}


	uint8_t tempReg = 0;

	PowerManagementRegister_t powerManagement={0};
	powerManagement.ClkSel = 0;
	powerManagement.Temp_Dis = 0;
	powerManagement.Reserved = 0;
	powerManagement.Cycle = 0;
	powerManagement.Sleep = 0;
	powerManagement.Device_Reset = 0;
	tempReg=*((uint8_t*)&powerManagement);
	MPU6050_WriteRegisterData(hi2cx, MPU6050_REG_PWR_MGMT_1, tempReg);
	///////////////////////////////////////////////////////////////////////////////////////
	AccelConfigRegister_t accelConfig = {0};
	accelConfig.Reserved = 0;
	accelConfig.AFS_Sel =AFS_SEL;
	accelConfig.ZA_ST = 0;
	accelConfig.YA_ST = 0;
	accelConfig.XA_ST = 0;
	tempReg=*((uint8_t*)&accelConfig);
	MPU6050_WriteRegisterData(hi2cx, MPU6050_REG_ACCEL_CONFIG, tempReg);
	////////////////////////////////////////////////////////////////////////////////////////
	GyroConfigRegister_t gyroConfig = {0};
	gyroConfig.Reserved = 0;
	gyroConfig.FS_Sel = FS_SEL;
	gyroConfig.ZG_ST = 0;
	gyroConfig.YG_ST = 0;
	gyroConfig.XG_ST = 0;
	tempReg=*((uint8_t*)&gyroConfig);
	MPU6050_WriteRegisterData(hi2cx, MPU6050_REG_GYRO_CONFIG, tempReg);
	////////////////////////////////////////////////////////////////////////////////////////

	return INIT_SUCCESS;
}


void MPU6050_getAccelValue(I2C_HandleTypeDef *hi2cx, int16_t *accelData){
	uint8_t data[6]={0};
	MPU6050_ReadRegisterData(hi2cx, MPU6050_REG_ACCEL_XOUT_H, 6, data);
	accelData[0]=(int16_t)(data[0]<<8 | data[1]);
	accelData[1]=(int16_t)(data[2]<<8 | data[3]);
	accelData[2]=(int16_t)(data[4]<<8 | data[5]);
}



void MPU6050_getGyroValue(I2C_HandleTypeDef *hi2cx, int16_t *gyroData){
	uint8_t data[6]={0};
	MPU6050_ReadRegisterData(hi2cx, MPU6050_REG_GYRO_XOUT_H, 6, data);
	gyroData[0]=(int16_t)(data[0]<<8 | data[1]);
	gyroData[1]=(int16_t)(data[2]<<8 | data[3]);
	gyroData[2]=(int16_t)(data[4]<<8 | data[5]);
}

float MPU6050_getTempValue(I2C_HandleTypeDef *hi2cx, int16_t *tempData){
	uint8_t data[2]={0};
	float temperature=0;
	MPU6050_ReadRegisterData(hi2cx, MPU6050_REG_TEMP_OUT_H, 2, data);
	*tempData=(int16_t)(data[0]<<8 | data[1]);
	temperature=(float)*tempData/340.0 + 36.53;
	return temperature;
}


void MPU6050_getAccelIng(int16_t *accelData, uint8_t AFS_SEL, float *accelDataIng)
{
	if(AFS_SEL == 0x00){
		accelDataIng[0] = (float) accelData[0] / 16384.0;
		accelDataIng[1] = (float) accelData[1] / 16384.0;
		accelDataIng[2] = (float) accelData[2] / 16384.0;
	}
	else if(AFS_SEL == 0x01){
		accelDataIng[0] = (float) accelData[0] / 8192.0;
		accelDataIng[1] = (float) accelData[1] / 8192.0;
		accelDataIng[2] = (float) accelData[2] / 8192.0;
	}
	else if(AFS_SEL == 0x02){
		accelDataIng[0] = (float) accelData[0] / 4096.0;
		accelDataIng[1] = (float) accelData[1] / 4096.0;
		accelDataIng[2] = (float) accelData[2] / 4096.0;
	}
	else if(AFS_SEL == 0x03){
		accelDataIng[0] = (float) accelData[0] / 2048.0;
		accelDataIng[1] = (float) accelData[1] / 2048.0;
		accelDataIng[2] = (float) accelData[2] / 2048.0;
	}
}


void MPU6050_getGyroIns(int16_t *gyroData, uint8_t FS_SEL, float *gyroDataIns)
{
	if(FS_SEL == 0x00){
		gyroDataIns[0] = (float) gyroData[0] / 131.0;
		gyroDataIns[1] = (float) gyroData[1] / 131.0;
		gyroDataIns[2] = (float) gyroData[2] / 131.0;
	}
	else if(FS_SEL == 0x01){
		gyroDataIns[0] = (float) gyroData[0] / 65.5;
		gyroDataIns[1] = (float) gyroData[1] / 65.5;
		gyroDataIns[2] = (float) gyroData[2] / 65.5;
	}
	else if(FS_SEL == 0x02){
		gyroDataIns[0] = (float) gyroData[0] / 32.8;
		gyroDataIns[1] = (float) gyroData[1] / 32.8;
		gyroDataIns[2] = (float) gyroData[2] / 32.8;
	}
	else if(FS_SEL == 0x03){
		gyroDataIns[0] = (float) gyroData[0] / 16.4;
		gyroDataIns[1] = (float) gyroData[1] / 16.4;
		gyroDataIns[2] = (float) gyroData[2] / 16.4;
	}
}



