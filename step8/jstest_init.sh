sudo pacman -S jstest joyutils

sudo vim /etc/bluetooth/main.conf
// add ControllerMode = dual

bluetoothctl
power on
agent on
default-agent
scan on
// find your device
scan off
pair <mac_address>
trust <mac_address>
info <mac_address>
quit

blueman-manager
// Connect To:
// select "Serial Port"
// DO NOT CLOSE THE WINDOW!

jstest /dev/rfcomm0
