# ESP32-S3-WROOM-2 Ultrasonic Sensor

## ESP32 with HC-SR04 Ultrasonic Sensor

Wire the HC-SR04 ultrasonic sensor to the ESP32 as shown in the following schematic diagram. We’re connecting the Trig 
pin to GPIO 2 and the Echo pin to GPIO 1, but you can use any other suitable pins.

```
+-----------------------+     +-----------------------+
|     HC-SR04 Sensor    |     |    ESP32-S3-WROOM-2   |
|                       |     |                       |
|       +------+        |     |       +-------+       |
|       |      |        |     |       |       |       |
|       | Trig |----------------------| GPIO4 |       |
|       | Echo |----------------------| GPIO5 |       |
|       | VCC  |----------------------| 5V    |       |
|       | GND  |----------------------| GND   |       |
|       |      |----------------------|       |       |
|       +------+        |     |       +-------+       |
|                       |     |                       |
+-----------------------+     +-----------------------+
```

| Ultrasonic Sensor | ESP32-S3 |
|-------------------|----------|
| `Trig`            | `GPIO4`  |
| `Echo`            | `GPIO5`  |
| `VCC`             | `5V`     |
| `GND`             | `GND`    |
