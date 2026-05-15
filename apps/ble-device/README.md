# BLE IMU Sender

## Configure

Edit `ble_sender.h` before uploading the sketch:

- `BLE_DEVICE_NAME`: BLE advertising/local name.
- `BLE_IMU_SERVICE_ID`: custom BLE service UUID advertised by the device.
- `BLE_IMU_CHARACTERISTIC_ID`: custom BLE characteristic UUID used for IMU reads/notifications.
- `IMU_I2C_ADDRESS`: onboard IMU address. The XIAO nRF52840 Sense default is `0x6A`.
- `ENABLE_ACCEL_DATA`: includes or omits accelerometer `ax/ay/az` values in the BLE payload.
- `ENABLE_GYRO_DATA`: includes or omits gyroscope `gx/gy/gz` values in the BLE payload.
- `SENSOR_SCALE`: multiplier used before packing float IMU readings into `int16_t` values.
- `ENABLE_SERIAL_DEBUG`: enables or disables serial IMU logging.
- `SERIAL_BAUD_RATE`: serial monitor baud rate.
- `ADVERTISEMENT_UPDATE_INTERVAL_MS`: delay between advertisement payload updates.

When both sensor groups are enabled, the IMU characteristic payload is 12 bytes:
`ax, ay, az, gx, gy, gz` as little-endian `int16_t` values. If one group is
disabled, its three values are omitted from the payload.

## Arduino Setup

1. Install Arduino IDE.
2. Add this Additional Boards Manager URL:

   ```text
   https://files.seeedstudio.com/arduino/package_seeeduino_boards_index.json
   ```

3. Install `Seeed nRF52 mbed-enabled Boards`.
4. Select `Seeed XIAO nRF52840 Sense` and the correct COM port.
5. Install these libraries:
   - `ArduinoBLE`
   - `Seeed Arduino LSM6DS3` by Seeed Studio, version `2.0.4`

The board is ready for programming after these steps.
