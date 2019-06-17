#ifndef __ACEL9260_H__
#define __ACEL9260_H__

#include <stdint.h>

#define MPU9250_G                     9.807f
#define MPU9250_D2R                   3.14159265359f/180.0f

typedef enum
{
   MPU9250_ACCEL_RANGE_2G,
   MPU9250_ACCEL_RANGE_4G,
   MPU9250_ACCEL_RANGE_8G,
   MPU9250_ACCEL_RANGE_16G
} MPU9250_AccelRange_t;

typedef enum
{
   MPU9250_GYRO_RANGE_250DPS,
   MPU9250_GYRO_RANGE_500DPS,
   MPU9250_GYRO_RANGE_1000DPS,
   MPU9250_GYRO_RANGE_2000DPS
} MPU9250_GyroRange_t;

typedef enum
{
   MPU9250_DLPF_BANDWIDTH_184HZ,
   MPU9250_DLPF_BANDWIDTH_92HZ,
   MPU9250_DLPF_BANDWIDTH_41HZ,
   MPU9250_DLPF_BANDWIDTH_20HZ,
   MPU9250_DLPF_BANDWIDTH_10HZ,
   MPU9250_DLPF_BANDWIDTH_5HZ
} MPU9250_DlpfBandwidth_t;

typedef enum
{
   MPU9250_LP_ACCEL_ODR_0_24HZ  = 0,
   MPU9250_LP_ACCEL_ODR_0_49HZ  = 1,
   MPU9250_LP_ACCEL_ODR_0_98HZ  = 2,
   MPU9250_LP_ACCEL_ODR_1_95HZ  = 3,
   MPU9250_LP_ACCEL_ODR_3_91HZ  = 4,
   MPU9250_LP_ACCEL_ODR_7_81HZ  = 5,
   MPU9250_LP_ACCEL_ODR_15_63HZ = 6,
   MPU9250_LP_ACCEL_ODR_31_25HZ = 7,
   MPU9250_LP_ACCEL_ODR_62_50HZ = 8,
   MPU9250_LP_ACCEL_ODR_125HZ   = 9,
   MPU9250_LP_ACCEL_ODR_250HZ   = 10,
   MPU9250_LP_ACCEL_ODR_500HZ   = 11
} MPU9250_LpAccelOdr_t;

//Control structure for MPU9250 operation (only one IMU per project)
typedef struct {
   
   // scale factors
   float _accelScale;
   float _gyroScale;
   float _magScaleX;
   float _magScaleY;
   float _magScaleZ;
   float _tempScale;
   float _tempOffset;
   
   // configuration
   MPU9250_AccelRange_t    _accelRange;
   MPU9250_GyroRange_t     _gyroRange;
   MPU9250_DlpfBandwidth_t _bandwidth;
   uint8_t _srd;

   // buffer for reading from sensor
   uint8_t _buffer[21];

   // data buffer
   float _ax, _ay, _az;
   float _gx, _gy, _gz;
   float _hx, _hy, _hz;
   float _t;

   // gyro bias estimation
   uint8_t _numSamples;
   double _gxbD, _gybD, _gzbD;
   float _gxb, _gyb, _gzb;

   // accel bias and scale factor estimation
   double _axbD, _aybD, _azbD;
   float _axmax, _aymax, _azmax;
   float _axmin, _aymin, _azmin;
   float _axb, _ayb, _azb;
   float _axs;
   float _ays;
   float _azs;

   // magnetometer bias and scale factor estimation
   uint16_t _maxCounts;
   float _deltaThresh;
   uint8_t _coeff;
   uint16_t _counter;
   float _framedelta, _delta;
   float _hxfilt, _hyfilt, _hzfilt;
   float _hxmax, _hymax, _hzmax;
   float _hxmin, _hymin, _hzmin;
   float _hxb, _hyb, _hzb;
   float _hxs;
   float _hys;
   float _hzs;
   float _avgs;

   // data counts
   int16_t _axcounts, _aycounts, _azcounts;
   int16_t _gxcounts, _gycounts, _gzcounts;
   int16_t _hxcounts, _hycounts, _hzcounts;
   int16_t _tcounts;

   // transformation matrix
   /* transform the accel and gyro axes to match the magnetometer axes */
   int16_t tX[3];
   int16_t tY[3];
   int16_t tZ[3];

   // track success of interacting with sensor
   int8_t _status;

} MPU9250_control_t;

#endif