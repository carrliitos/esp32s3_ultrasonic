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

## Google Cloud: ESP32 Ultrasonic Sensor

### APIs Used

- **Cloud Pub/Sub API**: Provides reliable, many-to-many, asynchronous messaging between applications. 

### Project Info

- **Project ID**: esp32-ultrasonic-sensor-443705
- **Service Account Information**:
  - Service Account Name: esp32-ultrasonic-sensor-sa
  - Service Account ID: esp32-ultrasonic-sensor-sa
  - Service Account Email Address: esp32-ultrasonic-sensor-sa@esp32-ultrasonic-sensor-443705.iam.gserviceaccount.com
  - Service Account JSON Key File: `*.json`
- **Pub/Sub Topic**:
  - **Pub/Sub Topic ID**: esp32-ultrasonic-topic
  - **Pub/Sub Topic Name**: projects/esp32-ultrasonic-sensor-443705/topics/esp32-ultrasonic-topic 

