#ifndef BLE_SENDER_H
#define BLE_SENDER_H

#include <stddef.h>
#include <stdint.h>

// Configuration Settings
static const char BLE_DEVICE_NAME[]             = "XIAO_Man";
static const char BLE_IMU_SERVICE_ID[]          = "550e8400-e29b-41d4-a716-446655440000";
static const char BLE_IMU_CHARACTERISTIC_ID[]   = "550e8400-e29b-41d4-a716-446655440001";

static const uint8_t IMU_I2C_ADDRESS = 0x6A; // XIAO nRF52840 Sense onboard LSM6DS3 IMU I2C address

static const bool ENABLE_ACCEL_DATA = true;
static const bool ENABLE_GYRO_DATA  = true;
static const int16_t SENSOR_SCALE   = 1000;

static const bool ENABLE_SERIAL_DEBUG       = true;
static const unsigned long SERIAL_BAUD_RATE = 115200;

static const unsigned long ADVERTISEMENT_UPDATE_INTERVAL_MS = 150;

struct ImuSample {
    int16_t ax;
    int16_t ay;
    int16_t az;
    int16_t gx;
    int16_t gy;
    int16_t gz;
};

ImuSample readIMU();
void sendIMU(const ImuSample &sample);
void logIMU(const ImuSample &sample);

#endif
