ac compile -q -b esp32:esp32:esp32 --board-options "PartitionScheme=huge_app" && \
  ac upload -p /dev/ttyUSB0 -b esp32:esp32:esp32
