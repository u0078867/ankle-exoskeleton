
#include "exo_imu.h"

//#ifdef DEBUG_IMU
/**************************************************************************/
/*
    Display the raw calibration offset and radius data
*/
/**************************************************************************/
void displaySensorOffsets(const adafruit_bno055_offsets_t &calibData)
{
  Serial.print("Accelerometer: ");
  Serial.print(calibData.accel_offset_x); Serial.print(" ");
  Serial.print(calibData.accel_offset_y); Serial.print(" ");
  Serial.print(calibData.accel_offset_z); Serial.print(" ");

  Serial.print("\nGyro: ");
  Serial.print(calibData.gyro_offset_x); Serial.print(" ");
  Serial.print(calibData.gyro_offset_y); Serial.print(" ");
  Serial.print(calibData.gyro_offset_z); Serial.print(" ");

  Serial.print("\nMag: ");
  Serial.print(calibData.mag_offset_x); Serial.print(" ");
  Serial.print(calibData.mag_offset_y); Serial.print(" ");
  Serial.print(calibData.mag_offset_z); Serial.print(" ");

  Serial.print("\nAccel Radius: ");
  Serial.print(calibData.accel_radius);

  Serial.print("\nMag Radius: ");
  Serial.print(calibData.mag_radius);
}
//#endif


/**************************************************************************/
/*
    Get calibration data from emulated EEPROM in Arduino ZERO
    (see https://github.com/adafruit/Adafruit_BNO055/blob/master/examples/restore_offsets/restore_offsets.ino)
*/
/**************************************************************************/
void getImuCalibFromFlashZERO(Adafruit_BNO055 &bno, FlashStorageClass<imu_cal_t> &flashStore)
{

  adafruit_bno055_offsets_t calibrationData;
  imu_cal_t imuCal;

  imuCal = flashStore.read();

  if (imuCal.valid == false) {
#ifdef DEBUG_IMU
    Serial.println("\nNo calibration data for this sensor exists in Flash\n");
#endif
    delay(500);
  } else {
#ifdef DEBUG_IMU
    Serial.println("\nFound calibration for this sensor in Flash.\n");
#endif
    calibrationData = imuCal.data;

#ifdef DEBUG_IMU
    displaySensorOffsets(calibrationData);
#endif

#ifdef DEBUG_IMU
    Serial.println("\n\nRestoring calibration data to the BNO055...");
#endif
    bno.setSensorOffsets(calibrationData);

#ifdef DEBUG_IMU
    Serial.println("\n\nCalibration data loaded into BNO055\n");
#endif
  }
}


/**************************************************************************/
/*
    Set calibration data to emulated EEPROM in Arduino ZERO
    (see https://github.com/adafruit/Adafruit_BNO055/blob/master/examples/restore_offsets/restore_offsets.ino)
*/
/**************************************************************************/
void setImuCalibToFlashZERO(Adafruit_BNO055 &bno, FlashStorageClass<imu_cal_t> &flashStore)
{

  adafruit_bno055_offsets_t calibrationData;
  imu_cal_t imuCal;

#ifdef DEBUG_IMU
    displaySensorOffsets(calibrationData);
#endif

  imuCal.valid = true;
  imuCal.data = calibrationData;

  flashStore.write(imuCal);

#ifdef DEBUG_IMU
  Serial.println("\nNew calibration data for this sensor set to Flash\n");
#endif
}


/**************************************************************************/
/*
    Displays some basic information on this sensor from the unified
    sensor API sensor_t type (see Adafruit_Sensor for more information)
*/
/**************************************************************************/
void displaySensorDetails(Adafruit_BNO055 &bno)
{
  sensor_t sensor;
  bno.getSensor(&sensor);
  Serial.println("------------------------------------");
  Serial.print  ("Sensor:       "); Serial.println(sensor.name);
  Serial.print  ("Driver Ver:   "); Serial.println(sensor.version);
  Serial.print  ("Unique ID:    "); Serial.println(sensor.sensor_id);
  Serial.print  ("Max Value:    "); Serial.print(sensor.max_value); Serial.println(" xxx");
  Serial.print  ("Min Value:    "); Serial.print(sensor.min_value); Serial.println(" xxx");
  Serial.print  ("Resolution:   "); Serial.print(sensor.resolution); Serial.println(" xxx");
  Serial.println("------------------------------------");
  Serial.println("");
  delay(500);
}

/**************************************************************************/
/*
    Display some basic info about the sensor status
*/
/**************************************************************************/
void getImuStatus(Adafruit_BNO055 &bno, uint8_t *systemStatus, uint8_t *selfTestResults, uint8_t *systemError)
{
  /* Get the system status values (mostly for DEBUG_IMUging purposes) */

  bno.getSystemStatus(systemStatus, selfTestResults, systemError);

#ifdef DEBUG_IMU
  /* Display the results in the Serial Monitor */
  Serial.println("Orientation Sensor Test");
  Serial.println("");
  Serial.print("System Status: 0x");
  Serial.println(*systemStatus, HEX);
  Serial.print("Self Test:     0x");
  Serial.println(*selfTestResults, HEX);
  Serial.print("System Error:  0x");
  Serial.println(*systemError, HEX);
  Serial.println("");
#endif

  delay(500);
}

/**************************************************************************/
/*
    Display sensor calibration status
*/
/**************************************************************************/
void getImuCalStatus(Adafruit_BNO055 &bno, uint8_t *calSystem, uint8_t *calGyro, uint8_t *calAccel, uint8_t *calMag)
{
  /* Get the four calibration values (0..3) */
  /* Any sensor data reporting 0 should be ignored, */
  /* 3 means 'fully calibrated" */

  bno.getCalibration(calSystem, calGyro, calAccel, calMag);

#ifdef DEBUG_IMU
  /* The data should be ignored until the system calibration is > 0 */
  //Serial.print("\t");
  if (!*calSystem)
  {
    Serial.print("! ");
  }

  /* Display the individual values */
  Serial.print("Sys:");
  Serial.print(*calSystem, DEC);
  Serial.print(" G:");
  Serial.print(*calGyro, DEC);
  Serial.print(" A:");
  Serial.print(*calAccel, DEC);
  Serial.print(" M:");
  Serial.print(*calMag, DEC);
  Serial.println();
#endif
}
