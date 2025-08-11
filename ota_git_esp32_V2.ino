#include <WiFi.h>
#include <HTTPClient.h>
#include <HTTPUpdate.h>
#include <WiFiClientSecure.h>

#define RED_PIN    14
#define GREEN_PIN  12
#define BLUE_PIN   13

const char* ssid = "Pranjal_2.4";
const char* password = "PK@98400";

String FirmwareVer = "1.0.2";

#define URL_fw_Version "https://raw.githubusercontent.com/belly1v123/OTA_git_ESP32/refs/heads/main/version.txt"
#define URL_fw_Bin "https://raw.githubusercontent.com/belly1v123/OTA_git_ESP32/refs/heads/main/fw.bin"

int incomingByte;

void setup() {
  Serial.begin(115200);
  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);

  Serial.println("Firmware Version: " + FirmwareVer);

  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected to WiFi");
}

void loop() {
  
  digitalWrite(RED_PIN, LOW);
  digitalWrite(GREEN_PIN, LOW);
  digitalWrite(BLUE_PIN, HIGH);
  delay(500);
  digitalWrite(BLUE_PIN, LOW);
  delay(500);

  if (Serial.available() > 0) {
    incomingByte = Serial.read();
    if (incomingByte == 'U') {
      Serial.println("Firmware Update Requested...");
      if (FirmwareVersionCheck()) {
        firmwareUpdate();
      }
    }
  }
}

int FirmwareVersionCheck() {
  String payload;
  int httpCode;

  WiFiClientSecure* client = new WiFiClientSecure;
  client->setInsecure();  // Ignore SSL cert (dev only)

  HTTPClient https;
  String fullURL = String(URL_fw_Version) + "?" + String(rand());  // Cache busting

  if (https.begin(*client, fullURL)) {
    httpCode = https.GET();
    if (httpCode == HTTP_CODE_OK) {
      payload = https.getString();
    } else {
      Serial.printf("Version check failed: %d\n", httpCode);
    }
    https.end();
  }
  delete client;

  if (httpCode == HTTP_CODE_OK) {
    payload.trim();
    if (payload.equals(FirmwareVer)) {
      Serial.println("Already on latest firmware.");
      return 0;
    } else {
      Serial.println("New version found: " + payload);
      return 1;
    }
  }

  return 0;
}

void firmwareUpdate() {
  WiFiClientSecure client;
  client.setInsecure();  // Skip cert verification (OK for testing)

  t_httpUpdate_return ret = httpUpdate.update(client, URL_fw_Bin);

  switch (ret) {
    case HTTP_UPDATE_FAILED:
      Serial.printf("Update failed: (%d): %s\n", httpUpdate.getLastError(), httpUpdate.getLastErrorString().c_str());
      break;

    case HTTP_UPDATE_NO_UPDATES:
      Serial.println("No updates available.");
      break;

    case HTTP_UPDATE_OK:
      Serial.println("Update successful.");
      break;
  }
}
