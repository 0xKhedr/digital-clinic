/*
 * XIAO nRF52840 Sense – BLE IMU Broadcaster
 * Sends accelerometer + gyroscope data via BLE notifications
 */

#include "ble_sender.h"

#include <ArduinoBLE.h> // BLE library
#include "LSM6DS3.h"    // IMU driver
#include "Wire.h"       // I2C communication

LSM6DS3 imu(I2C_MODE, IMU_I2C_ADDRESS);
BLEService imuService(BLE_IMU_SERVICE_ID);
BLECharacteristic imuChar(BLE_IMU_CHARACTERISTIC_ID, BLERead | BLENotify, 12);

void setup() {
    Serial.begin(SERIAL_BAUD_RATE);

    // IMU Sensor Initialization
    if (imu.begin() != 0) {
        Serial.println("[FAIL] IMU initialization failed");
        while (1); // Halt if the IMU chip doesn't respond
    }

    // BLE Radio Initialization
    if (!BLE.begin()) {
        Serial.println("[FAIL] BLE failed to start");
        while (1); // Halt in an infinite loop if the Bluetooth radio is broken
    }

    BLE.setDeviceName(BLE_DEVICE_NAME);
    BLE.setLocalName(BLE_DEVICE_NAME);

    BLE.setAdvertisedService(imuService);
    imuService.addCharacteristic(imuChar);
    BLE.addService(imuService);

    BLE.advertise(); // Start advertising
    Serial.println("[INFO] BLE beacon started");
}

void loop() {
    BLEDevice central = BLE.central(); // Look for a gateway trying to connect
    
    if (central) {
        Serial.print("Connected to central: ");
        Serial.println(central.address());

        while (central.connected()) {
            ImuSample sample = readIMU();
            sendIMU(sample);

            if (ENABLE_SERIAL_DEBUG) {
                logIMU(sample);
            }
        
            delay(ADVERTISEMENT_UPDATE_INTERVAL_MS);
        }

        Serial.print("[INFO] Disconnected from central: ");
        Serial.println(central.address());
    }
}

ImuSample readIMU() {
    ImuSample sample = {0, 0, 0, 0, 0, 0};

    if (ENABLE_ACCEL_DATA) {
        sample.ax = int16_t(imu.readFloatAccelX() * SENSOR_SCALE);  // Accelerometer X [g-force]
        sample.ay = int16_t(imu.readFloatAccelY() * SENSOR_SCALE);  // Accelerometer Y [g-force]
        sample.az = int16_t(imu.readFloatAccelZ() * SENSOR_SCALE);  // Accelerometer Z [g-force]
    }
    
    if (ENABLE_GYRO_DATA) {
        sample.gx = int16_t(imu.readFloatGyroX() * SENSOR_SCALE);   // Gyroscope X [degrees/second]
        sample.gy = int16_t(imu.readFloatGyroY() * SENSOR_SCALE);   // Gyroscope Y [degrees/second]
        sample.gz = int16_t(imu.readFloatGyroZ() * SENSOR_SCALE);   // Gyroscope Z [degrees/second]
    }

    return sample;
}

void sendIMU(const ImuSample &sample) {
    uint8_t data[12];
    size_t dataLength = 0;

    if (ENABLE_ACCEL_DATA) {
        int16_t accelData[] = {sample.ax, sample.ay, sample.az};
        memcpy(data + dataLength, accelData, sizeof(accelData));
        dataLength += sizeof(accelData);
    }

    if (ENABLE_GYRO_DATA) {
        int16_t gyroData[] = {sample.gx, sample.gy, sample.gz};
        memcpy(data + dataLength, gyroData, sizeof(gyroData));
        dataLength += sizeof(gyroData);
    }

    if (dataLength > 0) {
        imuChar.writeValue(data, dataLength);
    }
}

void logIMU(const ImuSample &sample) {
    Serial.print("[DATA] ");

    if (ENABLE_ACCEL_DATA) {
        Serial.print("acc=");
        Serial.print(sample.ax); Serial.print(", ");
        Serial.print(sample.ay); Serial.print(", ");
        Serial.print(sample.az);
    }

    if (ENABLE_ACCEL_DATA && ENABLE_GYRO_DATA) {
        Serial.print(" | ");
    }

    if (ENABLE_GYRO_DATA) {
        Serial.print("gyro=");
        Serial.print(sample.gx); Serial.print(", ");
        Serial.print(sample.gy); Serial.print(", ");
        Serial.print(sample.gz);
    }

    if (!ENABLE_ACCEL_DATA && !ENABLE_GYRO_DATA) {
        Serial.print("No sensor data enabled");
    }

    Serial.println();
}
