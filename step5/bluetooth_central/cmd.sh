ac compile -q -b esp32:esp32:nodemcu-32s && \
  ac upload -p /dev/ttyUSB0 -b esp32:esp32:nodemcu-32s && \
  ac monitor -q -p /dev/ttyUSB0 -c baudrate=115200
