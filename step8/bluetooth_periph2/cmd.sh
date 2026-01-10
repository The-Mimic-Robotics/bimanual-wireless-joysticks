ac compile -q -b esp32:esp32:esp32c3 --board-options "CDCOnBoot=cdc" && \
  ac upload -p /dev/ttyACM1 -b esp32:esp32:esp32c3
