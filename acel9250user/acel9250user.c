/**
 * @file   acel9250_testuser.c
 * @author Ericson Joseph
 * @date   16 Jun 2019
 * @version 0.1
 * @brief  A Linux user space program that communicates with the acel9250.c LKM. It passes a
 * string to the LKM and reads the response from the LKM. For this example to work the device
 * must be called /dev/acel9250.
 */

#include "acel9250user.h"
#include "stdio.h"
#include "stdlib.h"
#include <fcntl.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>

#define BUFFER_LENGTH 256 ///< The buffer length (crude but fine)

static uint8_t receive[BUFFER_LENGTH]; ///< The receive buffer from the LKM

static MPU9250_control_t control;

static void MPU9250_Init(uint8_t *data, size_t len) {

	memcpy(control._buffer, data, len);
	control._tempScale   = 333.87f;
	control._tempOffset  = 21.0f;
	control._numSamples  = 100;
	control._axs		 = 1.0f;
	control._ays		 = 1.0f;
	control._azs		 = 1.0f;
	control._maxCounts   = 1000;
	control._deltaThresh = 0.3f;
	control._coeff		 = 8;
	control._hxs		 = 1.0f;
	control._hys		 = 1.0f;
	control._hzs		 = 1.0f;
	control.tX[0]		 = 0;
	control.tX[1]		 = 1;
	control.tX[2]		 = 0;
	control.tY[0]		 = 1;
	control.tY[1]		 = 0;
	control.tY[2]		 = 0;
	control.tZ[0]		 = 0;
	control.tZ[1]		 = 0;
	control.tZ[2]		 = -1;

	control._accelScale = MPU9250_G * 16.0f / 32767.5f; // setting the accel scale to 16G
	control._accelRange = MPU9250_ACCEL_RANGE_16G;

	control._gyroScale = 2000.0f / 32767.5f * MPU9250_D2R;
	control._gyroRange = MPU9250_GYRO_RANGE_2000DPS;

	control._bandwidth = MPU9250_DLPF_BANDWIDTH_184HZ;
	control._srd	   = 0;

	control._tcounts  = (((int16_t)control._buffer[0]) << 8) | control._buffer[1];
	control._axcounts = (((int16_t)control._buffer[2]) << 8) | control._buffer[3];
	control._aycounts = (((int16_t)control._buffer[4]) << 8) | control._buffer[5];
	control._azcounts = (((int16_t)control._buffer[6]) << 8) | control._buffer[7];
	control._gxcounts = (((int16_t)control._buffer[8]) << 8) | control._buffer[9];
	control._gycounts = (((int16_t)control._buffer[10]) << 8) | control._buffer[11];
	control._gzcounts = (((int16_t)control._buffer[12]) << 8) | control._buffer[13];

	control._ax = (((float)(control.tX[0] * control._axcounts + control.tX[1] * control._aycounts + control.tX[2] * control._azcounts) * control._accelScale) - control._axb) * control._axs;
	control._ay = (((float)(control.tY[0] * control._axcounts + control.tY[1] * control._aycounts + control.tY[2] * control._azcounts) * control._accelScale) - control._ayb) * control._ays;
	control._az = (((float)(control.tZ[0] * control._axcounts + control.tZ[1] * control._aycounts + control.tZ[2] * control._azcounts) * control._accelScale) - control._azb) * control._azs;

	control._gx = ((float)(control.tX[0] * control._gxcounts + control.tX[1] * control._gycounts + control.tX[2] * control._gzcounts) * control._gyroScale) - control._gxb;
	control._gy = ((float)(control.tY[0] * control._gxcounts + control.tY[1] * control._gycounts + control.tY[2] * control._gzcounts) * control._gyroScale) - control._gyb;
	control._gz = ((float)(control.tZ[0] * control._gxcounts + control.tZ[1] * control._gycounts + control.tZ[2] * control._gzcounts) * control._gyroScale) - control._gzb;

	control._t = ((((float)control._tcounts) - control._tempOffset) / control._tempScale) + control._tempOffset;
}

static float getTemp(uint8_t *buf) {
	float temp  = 0.0f;
	int16_t aux = (((int16_t)buf[0]) << 8) | buf[1];
	temp		= ((((float)aux) - 21.0f) / 333.87f) + 21.0f;
	return temp;
}

int main() {

	int err = -1;
	int ret, fd;
	char stringToSend[BUFFER_LENGTH];
	printf("Starting device test code example...\n");
	fd = open("/dev/acel9250", O_RDWR);
	if (fd < 0) {
		perror("Failed to open the device...");
		return err;
	}

	ret = read(fd, receive, BUFFER_LENGTH);
	if (ret < 0) {
		perror("Failed to read the message from the device.");
		return err;
	}

	MPU9250_Init(receive, ret);

	printf("Temp: %.1f\n", control._t);
	printf("Accl: %.3f x %.3f y %.3f z\n", control._ax, control._ay, control._az);
	printf("Gyro: %.3f x %.3f y %.3f z\n", control._gx, control._gy, control._gz);

	return 0;
}
