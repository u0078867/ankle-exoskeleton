
#pragma once

#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <FlashStorage.h>

#include "../exo_glob.h"



typedef struct {
  boolean valid;
  adafruit_bno055_offsets_t data;
} imu_cal_t;


void displaySensorOffsets(const adafruit_bno055_offsets_t &calibData);
void getImuCalibFromFlashZERO(Adafruit_BNO055 &bno, FlashStorageClass<imu_cal_t> &flashStore);
void setImuCalibToFlashZERO(Adafruit_BNO055 &bno, FlashStorageClass<imu_cal_t> &flashStore);
void displaySensorDetails(Adafruit_BNO055 &bno);
void getImuStatus(Adafruit_BNO055 &bno, uint8_t *systemStatus, uint8_t *selfTestResults, uint8_t *systemError);
void getImuCalStatus(Adafruit_BNO055 &bno, uint8_t *calSystem, uint8_t *calGyro, uint8_t *calAccel, uint8_t *calMag);
