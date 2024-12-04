idf_component_register(
  SRCS "ultrasonic_sensor.c" "key_loader.c"
  INCLUDE_DIRS "include"
  REQUIRES ultrasonic
  PRIV_REQUIRES mbedtls json esp_http_client spiffs
)