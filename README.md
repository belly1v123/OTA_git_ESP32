This project demonstrates Over-The-Air (OTA) firmware updates for ESP32 via HTTP. The device automatically checks for the latest firmware version from a server and updates itself if a newer version is available.

✨ Features
HTTP-based OTA update (no USB cable after first flash)

Version checking to prevent unnecessary updates

LED status indication for firmware versions

v1.0.1 →  Blinking RED LED

v1.0.2 →  Blinking BLUE LED

Automatic reboot after update

Firmware hosted on GitHub Releases or any HTTP server

⚙️ How It Works
ESP32 connects to Wi-Fi.

Sends HTTP GET request to version.txt file on server.

Compares with locally stored firmware version.

If a newer version is found:

Downloads firmware.bin from server

Verifies firmware header

Updates flash memory

Reboots into new firmware

Hosting Firmware on GitHub
Create a GitHub repository for firmware.

Build your .bin file from Arduino IDE or PlatformIO.

Upload firmware.bin and version.txt to the repo.

Enable GitHub Pages and copy the raw file URLs.

Upload Initial Firmware
Upload via USB once using Arduino IDE / PlatformIO.
Future updates will happen automatically via OTA.
