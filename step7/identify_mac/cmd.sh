ac compile -q -b esp32:esp32:esp32c3 --board-options "CDCOnBoot=cdc" && \
  ac upload -p /dev/ttyACM0 -b esp32:esp32:esp32c3 && \
  ac monitor -q -p /dev/ttyACM0 -c baudrate=115200
