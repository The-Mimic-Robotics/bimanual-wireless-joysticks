ac compile --fqbn esp32:esp32:esp32 && \
  ac upload -p /dev/ttyUSB0 --fqbn esp32:esp32:esp32 && \
  ac monitor -p /dev/ttyUSB0 -c baudrate=115200
