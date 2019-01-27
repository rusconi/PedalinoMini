/*  __________           .___      .__  .__                   ___ ________________    ___
 *  \______   \ ____   __| _/____  |  | |__| ____   ____     /  / \__    ___/     \   \  \   
 *   |     ___// __ \ / __ |\__  \ |  | |  |/    \ /  _ \   /  /    |    | /  \ /  \   \  \  
 *   |    |   \  ___// /_/ | / __ \|  |_|  |   |  (  <_> ) (  (     |    |/    Y    \   )  )
 *   |____|    \___  >____ |(____  /____/__|___|  /\____/   \  \    |____|\____|__  /  /  /
 *                 \/     \/     \/             \/           \__\                 \/  /__/
 *                                                                (c) 2018 alf45star
 *                                                        https://github.com/alf45tar/Pedalino
 */

#include <SSD1306Wire.h>
#include <OLEDDisplayUi.h>

#ifdef ARDUINO_ARCH_ESP8266
#include <ESP8266WiFi.h>
#endif

#ifdef ARDUINO_ARCH_ESP32
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#endif

#define OLED_I2C_ADDRESS  0x3c
#define OLED_I2C_SDA      SDA
#define OLED_I2C_SCL      SCL


#define WIFI_LOGO_WIDTH   78
#define WIFI_LOGO_HEIGHT  64

const uint8_t WiFiLogo[] PROGMEM = {
  0x00, 0x00, 0x00, 0xFC, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0xFF, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xE0, 0xFF, 
  0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF0, 0xFF, 0x03, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF8, 0x07, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0xFC, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x7E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x3F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x1F, 0xC0, 
  0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x0F, 0xF0, 0x03, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x07, 0xFC, 0x03, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0xC0, 0x07, 0xFE, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0xC0, 0x03, 0x7F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xE0, 
  0x83, 0x0F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xE0, 0x81, 0x07, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xE0, 0xC1, 0x03, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0xE0, 0xC1, 0x03, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0xE0, 0xE1, 0xC3, 0x03, 0xF0, 0x0F, 0x00, 0x00, 0x00, 
  0x00, 0xE0, 0xE1, 0xC1, 0x03, 0xF8, 0x0F, 0x00, 0x00, 0x00, 0x00, 0xE0, 
  0xE1, 0xC1, 0x03, 0xF8, 0x0F, 0x00, 0x00, 0x00, 0x00, 0xE0, 0xC0, 0x81, 
  0x01, 0xF8, 0x0F, 0x00, 0x00, 0x00, 0x0F, 0x00, 0x00, 0x00, 0x00, 0x78, 
  0x00, 0x00, 0x00, 0x00, 0x1F, 0x00, 0x00, 0x00, 0x00, 0x78, 0x00, 0x00, 
  0x00, 0x00, 0x1F, 0x00, 0x00, 0x00, 0x00, 0x78, 0x00, 0x00, 0x00, 0x00, 
  0x1C, 0x78, 0xE0, 0xF1, 0x03, 0xF8, 0xC7, 0x0F, 0x00, 0x00, 0x3C, 0x7C, 
  0xF0, 0xF1, 0x03, 0xF8, 0xC7, 0x0F, 0x00, 0x00, 0x3C, 0x7C, 0xF0, 0xF1, 
  0x03, 0xF8, 0xC7, 0x0F, 0x00, 0x00, 0x3C, 0x7C, 0x70, 0xC0, 0x03, 0x78, 
  0x00, 0x0F, 0x00, 0x00, 0x38, 0xFE, 0x78, 0xC0, 0x03, 0x78, 0x00, 0x0F, 
  0x00, 0x00, 0x38, 0xFE, 0x78, 0xC0, 0xE3, 0x78, 0x00, 0x0F, 0x00, 0x00, 
  0x78, 0xFE, 0x39, 0xC0, 0xE3, 0x78, 0x00, 0x0F, 0x00, 0x00, 0x78, 0xEF, 
  0x39, 0xC0, 0xE3, 0x78, 0x00, 0x0F, 0x00, 0x00, 0x70, 0xCF, 0x3D, 0xC0, 
  0xE3, 0x78, 0x00, 0x0F, 0x00, 0x00, 0x70, 0xC7, 0x1D, 0xC0, 0x03, 0x78, 
  0x00, 0x0F, 0x00, 0x00, 0xF0, 0xC7, 0x1F, 0xC0, 0x03, 0x78, 0x00, 0x0F, 
  0x00, 0x00, 0xF0, 0x83, 0x1F, 0xC0, 0x03, 0x78, 0x00, 0x0F, 0x00, 0x00, 
  0xE0, 0x83, 0x1F, 0xC0, 0x0F, 0x78, 0x00, 0x1F, 0x00, 0x00, 0xE0, 0x83, 
  0x0F, 0xC0, 0x0F, 0x78, 0x00, 0x3F, 0x00, 0x00, 0xE0, 0x01, 0x0F, 0xC0, 
  0x0F, 0x78, 0x00, 0x3F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x0E, 0x1C, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x0F, 0x1E, 0x3E, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x0F, 0x1E, 0x3E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x0F, 0x0E, 0x1E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x0F, 0x1E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0F, 0x1E, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC0, 0x07, 0x1E, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0xC0, 0x07, 0x1F, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0xFC, 0x03, 0x0F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0xFF, 0x81, 0x0F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 
  0x80, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3F, 0xC0, 0x07, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0F, 0xE0, 0x03, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF0, 0x03, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0xF8, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0xFE, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 
  0x7F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x3F, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x0F, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x07, 0x00, 0x00
};

#define BLUETOOTH_LOGO_WIDTH   35
#define BLUETOOTH_LOGO_HEIGHT  54

const uint8_t BluetoothLogo[] PROGMEM = {
  0x00, 0xFC, 0xFF, 0x01, 0x00, 0x00, 0xFF, 0xFF, 0x07, 0x00, 0xC0, 0xFF, 
  0xFF, 0x1F, 0x00, 0xE0, 0xFF, 0xFF, 0x3F, 0x00, 0xF0, 0xFF, 0xFF, 0x7F, 
  0x00, 0xF8, 0xFF, 0xFF, 0xFF, 0x00, 0xFC, 0xFF, 0xFE, 0xFF, 0x01, 0xFC, 
  0xFF, 0xFE, 0xFF, 0x01, 0xFE, 0xFF, 0xFC, 0xFF, 0x03, 0xFE, 0xFF, 0xF8, 
  0xFF, 0x03, 0xFF, 0xFF, 0xF0, 0xFF, 0x07, 0xFF, 0xFF, 0xE0, 0xFF, 0x07, 
  0xFF, 0xFF, 0xC0, 0xFF, 0x07, 0xFF, 0xFF, 0x80, 0xFF, 0x07, 0xFF, 0xFF, 
  0x80, 0xFF, 0x07, 0xFF, 0xFF, 0x08, 0xFF, 0x07, 0xFF, 0xFF, 0x18, 0xFE, 
  0x07, 0xFF, 0xFD, 0x38, 0xFC, 0x07, 0xFF, 0xF8, 0x78, 0xF8, 0x07, 0xFF, 
  0xF0, 0x78, 0xF8, 0x07, 0xFF, 0xE1, 0x38, 0xFC, 0x07, 0xFF, 0xC3, 0x18, 
  0xFE, 0x07, 0xFF, 0x87, 0x08, 0xFF, 0x07, 0xFF, 0x0F, 0x80, 0xFF, 0x07, 
  0xFF, 0x1F, 0xC0, 0xFF, 0x07, 0xFF, 0x3F, 0xE0, 0xFF, 0x07, 0xFF, 0x7F, 
  0xF0, 0xFF, 0x07, 0xFF, 0x7F, 0xF0, 0xFF, 0x07, 0xFF, 0x3F, 0xE0, 0xFF, 
  0x07, 0xFF, 0x1F, 0xC0, 0xFF, 0x07, 0xFF, 0x0F, 0x80, 0xFF, 0x07, 0xFF, 
  0x87, 0x08, 0xFF, 0x07, 0xFF, 0xC3, 0x18, 0xFE, 0x07, 0xFF, 0xE1, 0x38, 
  0xFC, 0x07, 0xFF, 0xF0, 0x78, 0xF8, 0x07, 0xFF, 0xF8, 0x78, 0xF8, 0x07, 
  0xFF, 0xFD, 0x78, 0xF8, 0x07, 0xFF, 0xFF, 0x38, 0xFC, 0x07, 0xFF, 0xFF, 
  0x18, 0xFE, 0x07, 0xFF, 0xFF, 0x08, 0xFF, 0x07, 0xFF, 0xFF, 0x80, 0xFF, 
  0x07, 0xFF, 0xFF, 0xC0, 0xFF, 0x07, 0xFF, 0xFF, 0xC0, 0xFF, 0x07, 0xFF, 
  0xFF, 0xE0, 0xFF, 0x07, 0xFE, 0xFF, 0xF0, 0xFF, 0x03, 0xFE, 0xFF, 0xF8, 
  0xFF, 0x03, 0xFC, 0xFF, 0xFC, 0xFF, 0x01, 0xFC, 0xFF, 0xFE, 0xFF, 0x01, 
  0xF8, 0xFF, 0xFE, 0xFF, 0x00, 0xF0, 0xFF, 0xFF, 0x7F, 0x00, 0xE0, 0xFF, 
  0xFF, 0x3F, 0x00, 0xC0, 0xFF, 0xFF, 0x1F, 0x00, 0x00, 0xFF, 0xFF, 0x07, 
  0x00, 0x00, 0xFC, 0xFF, 0x01, 0x00
};


const uint8_t activeSymbol[] PROGMEM = {
  B00000000,
  B00000000,
  B00011000,
  B00100100,
  B01000010,
  B01000010,
  B00100100,
  B00011000
};

const uint8_t inactiveSymbol[] PROGMEM = {
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00011000,
  B00011000,
  B00000000,
  B00000000
};

// Font generated or edited with the glyphEditor
const uint8_t wifiSignal[] PROGMEM = {
0x14, // Width: 20
0x0A, // Height: 10
0x30, // First char: 48
0x08, // Number of chars: 8
// Jump Table:
0x00, 0x00, 0x11, 0x14, // 48
0x00, 0x11, 0x11, 0x14, // 49
0x00, 0x22, 0x12, 0x14, // 50
0x00, 0x34, 0x16, 0x14, // 51
0x00, 0x4A, 0x1A, 0x14, // 52
0x00, 0x64, 0x1E, 0x14, // 53
0x00, 0x82, 0x22, 0x14, // 54
0x00, 0xA4, 0x26, 0x14, // 55
// Font Data:
0x03, 0x00, 0x05, 0x00, 0x09, 0x00, 0x11, 0x00, 0xFF, 0x03, 0x11, 0x00, 0x09, 0x00, 0x05, 0x00, 0x03, // 48
0x03, 0x00, 0x05, 0x00, 0x09, 0x00, 0x11, 0x00, 0xFF, 0x03, 0x11, 0x00, 0x09, 0x03, 0x05, 0x00, 0x03, // 49
0x03, 0x00, 0x05, 0x00, 0x09, 0x00, 0x11, 0x00, 0xFF, 0x03, 0x11, 0x00, 0x09, 0x03, 0x05, 0x00, 0x83, 0x03, // 50
0x03, 0x00, 0x05, 0x00, 0x09, 0x00, 0x11, 0x00, 0xFF, 0x03, 0x11, 0x00, 0x09, 0x03, 0x05, 0x00, 0x83, 0x03, 0x00, 0x00, 0xC0, 0x03, // 51
0x03, 0x00, 0x05, 0x00, 0x09, 0x00, 0x11, 0x00, 0xFF, 0x03, 0x11, 0x00, 0x09, 0x03, 0x05, 0x00, 0x83, 0x03, 0x00, 0x00, 0xC0, 0x03, 0x00, 0x00, 0xE0, 0x03, // 52
0x03, 0x00, 0x05, 0x00, 0x09, 0x00, 0x11, 0x00, 0xFF, 0x03, 0x11, 0x00, 0x09, 0x03, 0x05, 0x00, 0x83, 0x03, 0x00, 0x00, 0xC0, 0x03, 0x00, 0x00, 0xE0, 0x03, 0x00, 0x00, 0xF0, 0x03, // 53
0x03, 0x00, 0x05, 0x00, 0x09, 0x00, 0x11, 0x00, 0xFF, 0x03, 0x11, 0x00, 0x09, 0x03, 0x05, 0x00, 0x83, 0x03, 0x00, 0x00, 0xC0, 0x03, 0x00, 0x00, 0xE0, 0x03, 0x00, 0x00, 0xF0, 0x03, 0x00, 0x00, 0xF8, 0x03, // 54
0x03, 0x00, 0x05, 0x00, 0x09, 0x00, 0x11, 0x00, 0xFF, 0x03, 0x11, 0x00, 0x09, 0x03, 0x05, 0x00, 0x83, 0x03, 0x00, 0x00, 0xC0, 0x03, 0x00, 0x00, 0xE0, 0x03, 0x00, 0x00, 0xF0, 0x03, 0x00, 0x00, 0xF8, 0x03, 0x00, 0x00, 0xFC, 0x03, // 55
};

// Font generated or edited with the glyphEditor
const uint8_t bluetoothSign[] PROGMEM = {
0x08, // Width: 8
0x0A, // Height: 10
0x30, // First char: 48
0x02, // Number of chars: 2
// Jump Table:
0xFF, 0xFF, 0x00, 0x08, // 48
0x00, 0x00, 0x0D, 0x08, // 49
// Font Data:
0x82, 0x00, 0x44, 0x00, 0x28, 0x00, 0xFF, 0x01, 0x11, 0x01, 0xAA, 0x00, 0x44, // 49
};      

// Font generated or edited with the glyphEditor
const uint8_t blynkSign[] PROGMEM = {
0x0A, // Width: 10
0x0A, // Height: 10
0x30, // First char: 48
0x02, // Number of chars: 2
// Jump Table:
0xFF, 0xFF, 0x00, 0x0A, // 48
0x00, 0x00, 0x14, 0x0A, // 49
// Font Data:
0x11, 0x02, 0x12, 0x01, 0x00, 0x00, 0xFC, 0x01, 0x25, 0x01, 0x24, 0x01, 0xD8, 0x00, 0x00, 0x00, 0x12, 0x01, 0x11, 0x02, // 49
};

// Font generated or edited with the glyphEditor
const uint8_t profileSign10[] PROGMEM = {
0x0A, // Width: 10
0x0A, // Height: 10
0x30, // First char: 48
0x03, // Number of chars: 3
// Jump Table:
0x00, 0x00, 0x14, 0x0A, // 48
0x00, 0x14, 0x14, 0x0A, // 49
0x00, 0x28, 0x14, 0x0A, // 50
// Font Data:
0xFE, 0x01, 0x01, 0x02, 0x01, 0x02, 0xF9, 0x02, 0x25, 0x02, 0x25, 0x02, 0xF9, 0x02, 0x01, 0x02, 0x01, 0x02, 0xFE, 0x01, // 48
0xFE, 0x01, 0x01, 0x02, 0x01, 0x02, 0xFD, 0x02, 0x95, 0x02, 0x95, 0x02, 0x79, 0x02, 0x01, 0x02, 0x01, 0x02, 0xFE, 0x01, // 49
0xFE, 0x01, 0x01, 0x02, 0x01, 0x02, 0x79, 0x02, 0x85, 0x02, 0x85, 0x02, 0x49, 0x02, 0x01, 0x02, 0x01, 0x02, 0xFE, 0x01, // 50
};

// Font generated or edited with the glyphEditor
const uint8_t profileSign[] PROGMEM = {
0x0A, // Width: 10
0x0B, // Height: 11
0x30, // First char: 48
0x03, // Number of chars: 3
// Jump Table:
0x00, 0x00, 0x14, 0x0A, // 48
0x00, 0x14, 0x14, 0x0A, // 49
0x00, 0x28, 0x14, 0x0A, // 50
// Font Data:
0xFE, 0x03, 0x01, 0x04, 0x01, 0x04, 0xF9, 0x05, 0x25, 0x04, 0x25, 0x04, 0xF9, 0x05, 0x01, 0x04, 0x01, 0x04, 0xFE, 0x03, // 48
0xFE, 0x03, 0x01, 0x04, 0x01, 0x04, 0xFD, 0x05, 0x25, 0x05, 0x25, 0x05, 0xF9, 0x04, 0x01, 0x04, 0x01, 0x04, 0xFE, 0x03, // 49
0xFE, 0x03, 0x01, 0x04, 0x01, 0x04, 0xF9, 0x04, 0x05, 0x05, 0x05, 0x05, 0x89, 0x04, 0x01, 0x04, 0x01, 0x04, 0xFE, 0x03, // 50
};

// Font generated or edited with the glyphEditor
const uint8_t _midiIcons[] PROGMEM = {
0x14, // Width: 20
0x0A, // Height: 10
0x30, // First char: 48
0x04, // Number of chars: 4
// Jump Table:
0xFF, 0xFF, 0x00, 0x14, // 48
0x00, 0x00, 0x22, 0x14, // 49
0x00, 0x22, 0x28, 0x14, // 50
0x00, 0x4A, 0x21, 0x14, // 51
// Font Data:
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7E, 0x01, 0x02, 0x01, 0x7E, 0x01, 0x02, 0x01, 0x7C, 0x01, 0x00, 0x01, 0x7E, 0x01, 0x00, 0x01, 0x7A, 0x01, 0x42, 0x01, 0x42, 0x01, 0x3C, 0x01, 0x00, 0x01, 0x7E, 0x01, // 49
0x7A, 0x00, 0x00, 0x00, 0xF8, 0x01, 0x48, 0x00, 0x30, 0x00, 0x00, 0x00, 0x7E, 0x01, 0x02, 0x01, 0x7E, 0x01, 0x02, 0x01, 0x7C, 0x01, 0x00, 0x01, 0x7E, 0x01, 0x00, 0x01, 0x7A, 0x01, 0x42, 0x01, 0x42, 0x01, 0x3C, 0x01, 0x00, 0x01, 0x7E, 0x01, // 50
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7C, 0x00, 0x82, 0x00, 0x82, 0x00, 0x7C, 0x00, 0x00, 0x00, 0x4C, 0x00, 0x92, 0x00, 0x92, 0x00, 0x64, 0x00, 0x00, 0x00, 0x7C, 0x00, 0x82, 0x00, 0x82, 0x00, 0x44, // 51
};

// Font generated or edited with the glyphEditor
const uint8_t midiIcons[] PROGMEM = {
0x14, // Width: 20
0x0A, // Height: 10
0x30, // First char: 48
0x04, // Number of chars: 4
// Jump Table:
0xFF, 0xFF, 0x00, 0x14, // 48
0x00, 0x00, 0x26, 0x14, // 49
0x00, 0x26, 0x28, 0x14, // 50
0x00, 0x4E, 0x26, 0x14, // 51
// Font Data:
0x00, 0x00, 0xFF, 0x03, 0xFF, 0x03, 0x81, 0x02, 0xFD, 0x02, 0x81, 0x02, 0xFD, 0x02, 0x81, 0x02, 0xFF, 0x02, 0x81, 0x02, 0xFF, 0x02, 0x85, 0x02, 0xBD, 0x02, 0xBD, 0x02, 0x83, 0x02, 0xFF, 0x02, 0x81, 0x02, 0xFF, 0x03, 0xFF, 0x03, // 49
0x85, 0x03, 0xFF, 0x03, 0x07, 0x02, 0xB7, 0x03, 0xCF, 0x03, 0xFF, 0x03, 0x81, 0x02, 0xFD, 0x02, 0x81, 0x02, 0xFD, 0x02, 0x83, 0x02, 0xFF, 0x02, 0x81, 0x02, 0xFF, 0x02, 0x85, 0x02, 0xBD, 0x02, 0xBD, 0x02, 0xC3, 0x02, 0xFF, 0x02, 0x81, 0x02, // 50
0x00, 0x00, 0xFF, 0x03, 0xFF, 0x03, 0x83, 0x03, 0x7D, 0x03, 0x7D, 0x03, 0x83, 0x03, 0xFF, 0x03, 0xB3, 0x03, 0x6D, 0x03, 0x6D, 0x03, 0x9B, 0x03, 0xFF, 0x03, 0x83, 0x03, 0x7D, 0x03, 0x7D, 0x03, 0xBB, 0x03, 0xFF, 0x03, 0xFF, 0x03, // 51
};
// Font generated or edited with the glyphEditor
const uint8_t batteryIndicator[] PROGMEM = {
0x14, // Width: 20
0x0A, // Height: 10
0x30, // First char: 48
0x04, // Number of chars: 4
// Jump Table:
0x00, 0x00, 0x28, 0x14, // 48
0x00, 0x28, 0x28, 0x14, // 49
0x00, 0x50, 0x28, 0x14, // 50
0x00, 0x78, 0x28, 0x14, // 51
// Font Data:
0x78, 0x00, 0x48, 0x00, 0xFF, 0x03, 0x01, 0x02, 0x01, 0x02, 0x01, 0x02, 0x01, 0x02, 0x01, 0x02, 0x01, 0x02, 0x01, 0x02, 0x01, 0x02, 0x01, 0x02, 0x01, 0x02, 0x01, 0x02, 0x01, 0x02, 0x01, 0x02, 0x01, 0x02, 0x01, 0x02, 0x01, 0x02, 0xFF, 0x03, // 48
0x78, 0x00, 0x48, 0x00, 0xFF, 0x03, 0x01, 0x02, 0x01, 0x02, 0x01, 0x02, 0x01, 0x02, 0x01, 0x02, 0x01, 0x02, 0x01, 0x02, 0x01, 0x02, 0x01, 0x02, 0x01, 0x02, 0x01, 0x02, 0xFD, 0x02, 0xFD, 0x02, 0xFD, 0x02, 0xFD, 0x02, 0x01, 0x02, 0xFF, 0x03, // 49
0x78, 0x00, 0x48, 0x00, 0xFF, 0x03, 0x01, 0x02, 0x01, 0x02, 0x01, 0x02, 0x01, 0x02, 0x01, 0x02, 0x01, 0x02, 0xFD, 0x02, 0xFD, 0x02, 0xFD, 0x02, 0xFD, 0x02, 0x01, 0x02, 0xFD, 0x02, 0xFD, 0x02, 0xFD, 0x02, 0xFD, 0x02, 0x01, 0x02, 0xFF, 0x03, // 50
0x78, 0x00, 0x48, 0x00, 0xFF, 0x03, 0x01, 0x02, 0xFD, 0x02, 0xFD, 0x02, 0xFD, 0x02, 0xFD, 0x02, 0x01, 0x02, 0xFD, 0x02, 0xFD, 0x02, 0xFD, 0x02, 0xFD, 0x02, 0x01, 0x02, 0xFD, 0x02, 0xFD, 0x02, 0xFD, 0x02, 0xFD, 0x02, 0x01, 0x02, 0xFF, 0x03, // 51
};

// Font generated or edited with the glyphEditor
const uint8_t beats4[] PROGMEM = {
0x20, // Width: 32
0x20, // Height: 32
0x30, // First char: 48
0x04, // Number of chars: 4
// Jump Table:
0x00, 0x00, 0x7E, 0x20, // 48
0x00, 0x7E, 0x7F, 0x20, // 49
0x00, 0xFD, 0x7F, 0x20, // 50
0x01, 0x7C, 0x7F, 0x20, // 51
// Font Data:
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x7F, 0x00, 0x00, 0xFF, 0x7F, 0x00, 0x00, 0xFF, 0x7F, 0x00, 0x00, 0xFF, 0x7F, 0x00, 0x00, 0xFF, 0x7F, 0x00, 0x00, 0xFE, 0x7F, 0x00, 0x00, 0xFE, 0x7F, 0x00, 0x00, 0xFC, 0x7F, 0x00, 0x00, 0xFC, 0x7F, 0x00, 0x00, 0xF8, 0x7F, 0x00, 0x00, 0xF0, 0x7F, 0x00, 0x00, 0xE0, 0x7F, 0x00, 0x00, 0xC0, 0x7F, 0x00, 0x00, 0x00, 0x7F, 0x00, 0x00, 0x00, 0x7C, // 48
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x7F, 0xFE, 0xFF, 0xFF, 0x7F, 0xFE, 0xFF, 0xFF, 0x7F, 0xFE, 0xFF, 0xFF, 0x7F, 0xFE, 0xFF, 0xFF, 0x7F, 0xFE, 0xFF, 0xFE, 0x7F, 0xFE, 0x7F, 0xFE, 0x7F, 0xFE, 0x7F, 0xFC, 0x7F, 0xFE, 0x3F, 0xFC, 0x7F, 0xFE, 0x3F, 0xF8, 0x7F, 0xFE, 0x1F, 0xF0, 0x7F, 0xFE, 0x0F, 0xE0, 0x7F, 0xFE, 0x07, 0xC0, 0x7F, 0xFE, 0x03, 0x00, 0x7F, 0xFE, 0x00, 0x00, 0x7C, 0x3E, // 49
0x00, 0x00, 0x3E, 0x00, 0x00, 0x00, 0xFE, 0x00, 0x00, 0x00, 0xFE, 0x03, 0x00, 0x00, 0xFE, 0x07, 0x00, 0x00, 0xFE, 0x0F, 0x00, 0x00, 0xFE, 0x1F, 0x00, 0x00, 0xFE, 0x3F, 0x00, 0x00, 0xFE, 0x3F, 0x00, 0x00, 0xFE, 0x7F, 0x00, 0x00, 0xFE, 0x7F, 0x00, 0x00, 0xFE, 0xFF, 0x00, 0x00, 0xFE, 0xFF, 0x00, 0x00, 0xFE, 0xFF, 0x00, 0x00, 0xFE, 0xFF, 0x00, 0x00, 0xFE, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x7F, 0xFE, 0xFF, 0xFF, 0x7F, 0xFE, 0xFF, 0xFF, 0x7F, 0xFE, 0xFF, 0xFF, 0x7F, 0xFE, 0xFF, 0xFF, 0x7F, 0xFE, 0xFF, 0xFE, 0x7F, 0xFE, 0x7F, 0xFE, 0x7F, 0xFE, 0x7F, 0xFC, 0x7F, 0xFE, 0x3F, 0xFC, 0x7F, 0xFE, 0x3F, 0xF8, 0x7F, 0xFE, 0x1F, 0xF0, 0x7F, 0xFE, 0x0F, 0xE0, 0x7F, 0xFE, 0x07, 0xC0, 0x7F, 0xFE, 0x03, 0x00, 0x7F, 0xFE, 0x00, 0x00, 0x7C, 0x3E, // 50
0x00, 0x7C, 0x3E, 0x00, 0x00, 0x7F, 0xFE, 0x00, 0xC0, 0x7F, 0xFE, 0x03, 0xE0, 0x7F, 0xFE, 0x07, 0xF0, 0x7F, 0xFE, 0x0F, 0xF8, 0x7F, 0xFE, 0x1F, 0xFC, 0x7F, 0xFE, 0x3F, 0xFC, 0x7F, 0xFE, 0x3F, 0xFE, 0x7F, 0xFE, 0x7F, 0xFE, 0x7F, 0xFE, 0x7F, 0xFF, 0x7F, 0xFE, 0xFF, 0xFF, 0x7F, 0xFE, 0xFF, 0xFF, 0x7F, 0xFE, 0xFF, 0xFF, 0x7F, 0xFE, 0xFF, 0xFF, 0x7F, 0xFE, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x7F, 0xFE, 0xFF, 0xFF, 0x7F, 0xFE, 0xFF, 0xFF, 0x7F, 0xFE, 0xFF, 0xFF, 0x7F, 0xFE, 0xFF, 0xFF, 0x7F, 0xFE, 0xFF, 0xFE, 0x7F, 0xFE, 0x7F, 0xFE, 0x7F, 0xFE, 0x7F, 0xFC, 0x7F, 0xFE, 0x3F, 0xFC, 0x7F, 0xFE, 0x3F, 0xF8, 0x7F, 0xFE, 0x1F, 0xF0, 0x7F, 0xFE, 0x0F, 0xE0, 0x7F, 0xFE, 0x07, 0xC0, 0x7F, 0xFE, 0x03, 0x00, 0x7F, 0xFE, 0x00, 0x00, 0x7C, 0x3E, // 51
};

// Font generated or edited with the glyphEditor
const uint8_t beats4off[] PROGMEM = {
0x20, // Width: 32
0x20, // Height: 32
0x30, // First char: 48
0x04, // Number of chars: 4
// Jump Table:
0x00, 0x00, 0x7F, 0x20, // 48
0x00, 0x7F, 0x7F, 0x20, // 49
0x00, 0xFE, 0x7F, 0x20, // 50
0x01, 0x7D, 0x7F, 0x20, // 51
// Font Data:
0x00, 0x7C, 0x3E, 0x00, 0x00, 0x43, 0xC2, 0x00, 0xC0, 0x40, 0x02, 0x03, 0x20, 0x40, 0x02, 0x04, 0x10, 0x40, 0x02, 0x08, 0x08, 0x40, 0x02, 0x10, 0x04, 0x40, 0x02, 0x20, 0x04, 0x40, 0x02, 0x20, 0x02, 0x40, 0x02, 0x40, 0x02, 0x40, 0x02, 0x40, 0x01, 0x40, 0x02, 0x80, 0x01, 0x40, 0x02, 0x80, 0x01, 0x40, 0x02, 0x80, 0x01, 0x40, 0x02, 0x80, 0xFF, 0x7F, 0xFE, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x7F, 0xFE, 0xFF, 0x01, 0x40, 0x02, 0x80, 0xFD, 0x5F, 0x02, 0x80, 0xFD, 0x5F, 0x02, 0x80, 0xF9, 0x5F, 0x02, 0x80, 0xFA, 0x5F, 0x02, 0x40, 0xF2, 0x5F, 0x02, 0x40, 0xF4, 0x5F, 0x02, 0x20, 0xE4, 0x5F, 0x02, 0x20, 0xC8, 0x5F, 0x02, 0x10, 0x90, 0x5F, 0x02, 0x08, 0x20, 0x5E, 0x02, 0x04, 0xC0, 0x58, 0x02, 0x03, 0x00, 0x43, 0xC2, 0x00, 0x00, 0x7C, 0x3E, // 48
0x00, 0x7C, 0x3E, 0x00, 0x00, 0x43, 0xC2, 0x00, 0xC0, 0x40, 0x02, 0x03, 0x20, 0x40, 0x02, 0x04, 0x10, 0x40, 0x02, 0x08, 0x08, 0x40, 0x02, 0x10, 0x04, 0x40, 0x02, 0x20, 0x04, 0x40, 0x02, 0x20, 0x02, 0x40, 0x02, 0x40, 0x02, 0x40, 0x02, 0x40, 0x01, 0x40, 0x02, 0x80, 0x01, 0x40, 0x02, 0x80, 0x01, 0x40, 0x02, 0x80, 0x01, 0x40, 0x02, 0x80, 0xFF, 0x7F, 0xFE, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x7F, 0xFE, 0xFF, 0x01, 0x40, 0x02, 0x80, 0x01, 0x40, 0xFA, 0xBF, 0x01, 0x40, 0xFA, 0xBF, 0x01, 0x40, 0xFA, 0x9F, 0x02, 0x40, 0xFA, 0x5F, 0x02, 0x40, 0xFA, 0x4F, 0x04, 0x40, 0xFA, 0x2F, 0x04, 0x40, 0xFA, 0x27, 0x08, 0x40, 0xFA, 0x13, 0x10, 0x40, 0xFA, 0x09, 0x20, 0x40, 0x7A, 0x04, 0xC0, 0x40, 0x1A, 0x03, 0x00, 0x43, 0xC2, 0x00, 0x00, 0x7C, 0x3E, // 49
0x00, 0x7C, 0x3E, 0x00, 0x00, 0x43, 0xC2, 0x00, 0xC0, 0x40, 0x1A, 0x03, 0x20, 0x40, 0x7A, 0x04, 0x10, 0x40, 0xFA, 0x09, 0x08, 0x40, 0xFA, 0x13, 0x04, 0x40, 0xFA, 0x27, 0x04, 0x40, 0xFA, 0x2F, 0x02, 0x40, 0xFA, 0x4F, 0x02, 0x40, 0xFA, 0x5F, 0x01, 0x40, 0xFA, 0x9F, 0x01, 0x40, 0xFA, 0xBF, 0x01, 0x40, 0xFA, 0xBF, 0x01, 0x40, 0x02, 0x80, 0xFF, 0x7F, 0xFE, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x7F, 0xFE, 0xFF, 0x01, 0x40, 0x02, 0x80, 0x01, 0x40, 0x02, 0x80, 0x01, 0x40, 0x02, 0x80, 0x01, 0x40, 0x02, 0x80, 0x02, 0x40, 0x02, 0x40, 0x02, 0x40, 0x02, 0x40, 0x04, 0x40, 0x02, 0x20, 0x04, 0x40, 0x02, 0x20, 0x08, 0x40, 0x02, 0x10, 0x10, 0x40, 0x02, 0x08, 0x20, 0x40, 0x02, 0x04, 0xC0, 0x40, 0x02, 0x03, 0x00, 0x43, 0xC2, 0x00, 0x00, 0x7C, 0x3E, // 50
0x00, 0x7C, 0x3E, 0x00, 0x00, 0x43, 0xC2, 0x00, 0xC0, 0x58, 0x02, 0x03, 0x20, 0x5E, 0x02, 0x04, 0x90, 0x5F, 0x02, 0x08, 0xC8, 0x5F, 0x02, 0x10, 0xE4, 0x5F, 0x02, 0x20, 0xF4, 0x5F, 0x02, 0x20, 0xF2, 0x5F, 0x02, 0x40, 0xFA, 0x5F, 0x02, 0x40, 0xF9, 0x5F, 0x02, 0x80, 0xFD, 0x5F, 0x02, 0x80, 0xFD, 0x5F, 0x02, 0x80, 0x01, 0x40, 0x02, 0x80, 0xFF, 0x7F, 0xFE, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x7F, 0xFE, 0xFF, 0x01, 0x40, 0x02, 0x80, 0x01, 0x40, 0x02, 0x80, 0x01, 0x40, 0x02, 0x80, 0x01, 0x40, 0x02, 0x80, 0x02, 0x40, 0x02, 0x40, 0x02, 0x40, 0x02, 0x40, 0x04, 0x40, 0x02, 0x20, 0x04, 0x40, 0x02, 0x20, 0x08, 0x40, 0x02, 0x10, 0x10, 0x40, 0x02, 0x08, 0x20, 0x40, 0x02, 0x04, 0xC0, 0x40, 0x02, 0x03, 0x00, 0x43, 0xC2, 0x00, 0x00, 0x7C, 0x3E, // 51
};

// Font generated or edited with the glyphEditor
const uint8_t block[] PROGMEM = {
0x12, // Width: 18
0x12, // Height: 18
0x30, // First char: 48
0x03, // Number of chars: 3
// Jump Table:
0x00, 0x00, 0x36, 0x12, // 48
0x00, 0x36, 0x36, 0x12, // 49
0x00, 0x6C, 0x36, 0x12, // 50
// Font Data:
0xFE, 0xFF, 0x01, 0x01, 0x00, 0x02, 0x01, 0x00, 0x02, 0x01, 0x00, 0x02, 0x01, 0x00, 0x02, 0x01, 0x00, 0x02, 0x01, 0x00, 0x02, 0x01, 0x00, 0x02, 0x01, 0x00, 0x02, 0x01, 0x00, 0x02, 0x01, 0x00, 0x02, 0x01, 0x00, 0x02, 0x01, 0x00, 0x02, 0x01, 0x00, 0x02, 0x01, 0x00, 0x02, 0x01, 0x00, 0x02, 0x01, 0x00, 0x02, 0xFE, 0xFF, 0x01, // 48
0xFE, 0xFF, 0x01, 0x01, 0x00, 0x02, 0x01, 0x00, 0x02, 0x01, 0x00, 0x02, 0x01, 0x00, 0x02, 0x01, 0x00, 0x02, 0x01, 0x00, 0x02, 0x01, 0x03, 0x02, 0x81, 0x07, 0x02, 0x81, 0x07, 0x02, 0x01, 0x03, 0x02, 0x01, 0x00, 0x02, 0x01, 0x00, 0x02, 0x01, 0x00, 0x02, 0x01, 0x00, 0x02, 0x01, 0x00, 0x02, 0x01, 0x00, 0x02, 0xFE, 0xFF, 0x01, // 49
0xFE, 0xFF, 0x01, 0x01, 0x00, 0x02, 0xF9, 0x7F, 0x02, 0xFD, 0xFF, 0x02, 0xFD, 0xFF, 0x02, 0xFD, 0xFF, 0x02, 0xFD, 0xFF, 0x02, 0xFD, 0xFF, 0x02, 0xFD, 0xFF, 0x02, 0xFD, 0xFF, 0x02, 0xFD, 0xFF, 0x02, 0xFD, 0xFF, 0x02, 0xFD, 0xFF, 0x02, 0xFD, 0xFF, 0x02, 0xFD, 0xFF, 0x02, 0xF9, 0x7F, 0x02, 0x01, 0x00, 0x02, 0xFE, 0xFF, 0x01, // 50
};

// Initialize the OLED display using Wire library
SSD1306Wire   display(OLED_I2C_ADDRESS, OLED_I2C_SDA, OLED_I2C_SCL);
OLEDDisplayUi ui(&display);
bool          uiUpdate = true;

bool blynk_cloud_connected();
extern bool             appleMidiConnected;
extern AsyncWebSocket   webSocket;
extern AsyncEventSource events;

void display_clear()
{
  display.clear();
  display.display();
}

void display_progress_bar_title(String title)
{
  display.clear();
  display.setFont(ArialMT_Plain_10);
  display.setTextAlignment(TEXT_ALIGN_CENTER_BOTH);
  display.drawString(display.getWidth() / 2, display.getHeight() / 2 - 10, title.c_str());
  display.display();
}

void display_progress_bar_title2(String title1, String title2)
{
  display.clear();
  display.setTextAlignment(TEXT_ALIGN_CENTER);
  display.setFont(ArialMT_Plain_10);
  display.drawString(display.getWidth() / 2, 0, title1.c_str());
  display.setFont(ArialMT_Plain_16);
  display.drawString(display.getWidth() / 2, 10, title2.c_str());
  display.display();
}

void display_progress_bar_update(unsigned int progress, unsigned int total)
{
  display.drawProgressBar(4, 32, 120, 8, 100*progress/total);
  display.display();
}

void topOverlay(OLEDDisplay *display, OLEDDisplayUiState* state)
{
#ifdef WIFI
  static int signal;

  display->setTextAlignment(TEXT_ALIGN_LEFT);
  display->setFont(wifiSignal);
  signal = (4*signal + WiFi.RSSI()) / 5;
  if      (signal < -90) display->drawString(0, 0, String(0));
  else if (signal < -85) display->drawString(0, 0, String(1));
  else if (signal < -80) display->drawString(0, 0, String(2));
  else if (signal < -75) display->drawString(0, 0, String(3));
  else if (signal < -70) display->drawString(0, 0, String(4));
  else if (signal < -65) display->drawString(0, 0, String(5));
  else if (signal < -60) display->drawString(0, 0, String(6));
  else                   display->drawString(0, 0, String(7));
#endif

  display->setFont(bluetoothSign);
  if (bleMidiConnected) display->drawString(24, 0, String(1));
  else display->drawString(24, 0, String(0));

  display->setFont(blynkSign);
  if (blynk_cloud_connected()) display->drawString(36, 0, String(1));
  else display->drawString(36, 0, String(0));

  display->setTextAlignment(TEXT_ALIGN_RIGHT);
  display->setFont(batteryIndicator);
  display->drawString(128, 0, String((millis()>>10)%4));

  display->setTextAlignment(TEXT_ALIGN_CENTER);
  display->setFont(profileSign);
  display->drawString(64 + 10*currentProfile, 0, String(currentProfile));
}

void bottomOverlay(OLEDDisplay *display, OLEDDisplayUiState* state)
{
  if (lastUsed == lastUsedPedal && lastUsed != 0xFF && millis() < endMillis2) {
    byte p = map(pedals[lastUsedPedal].pedalValue[0], 0, MIDI_RESOLUTION - 1, 0, 100);
    display->drawProgressBar(4, 54, 120, 8, p);
  }
  else { 
    display->drawLine(0, 51, 127, 51);

    display->setTextAlignment(TEXT_ALIGN_LEFT);
    display->setFont(ArialMT_Plain_10);
    display->drawString(0, 54, String("Bank " + String(currentBank+1)));

#ifdef WIFI
    display->setTextAlignment(TEXT_ALIGN_RIGHT);
    display->setFont(midiIcons);
    if(appleMidiConnected) display->drawString(84, 54, String(1));
    else display->drawString(84, 54, String(0));

    if (interfaces[PED_IPMIDI].midiIn) display->drawString(106, 54, String(2));
    else display->drawString(106, 54, String(0));

    if (interfaces[PED_OSC].midiIn) display->drawString(128, 54, String(3));
    else display->drawString(128, 54, String(0));
#endif
  }
}

void drawRect (OLEDDisplay *display, int16_t x0, int16_t y0, int16_t x1, int16_t y1)
{
  display->drawLine(x0+1, y0,   x1-1, y0);
  display->drawLine(x1,   y0+1, x1,   y1-1);
  display->drawLine(x1-1, y1,   x0+1, y1);
  display->drawLine(x0,   y1-1, x0,   y0+1);
}

void drawFrame1(OLEDDisplay *display, OLEDDisplayUiState* state, int16_t x, int16_t y)
{
  if (millis() < endMillis2) {
    ui.disableAutoTransition();
    ui.switchToFrame(0);
    if (banks[currentBank][lastUsed].pedalName[0] == 0) {
      display->setTextAlignment(TEXT_ALIGN_CENTER);
      switch (m1) {
        case midi::NoteOn:
        case midi::NoteOff:
          drawRect(display, 64-22, 15, 64+24, 15+23);
          display->setFont(ArialMT_Plain_10);
          display->drawString( 64 + x, 39 + y, String("Note"));
          display->setFont(ArialMT_Plain_24);
          display->drawString( 64 + x, 14 + y, String(m2));
          display->setFont(ArialMT_Plain_10);
          display->drawString(110 + x, 39 + y, String("Velocity"));
          display->setFont(ArialMT_Plain_16);
          display->drawString(110 + x, 22 + y, String(m3));
          break;
        case midi::ControlChange:
          drawRect(display, 64-22, 15, 64+24, 15+23);
          display->setFont(ArialMT_Plain_10);
          display->drawString( 64 + x, 39 + y, String("CC"));
          display->setFont(ArialMT_Plain_24);
          display->drawString( 64 + x, 14 + y, String(m2));
          display->setFont(ArialMT_Plain_10);
          display->drawString(110 + x, 39 + y, String("Value"));
          display->setFont(ArialMT_Plain_16);
          display->drawString(110 + x, 22 + y, String(m3));
          break;
        case midi::ProgramChange:
          drawRect(display, 84-22, 15, 84+24, 15+23);
          display->setFont(ArialMT_Plain_10);
          display->drawString(84 + x, 39 + y, String("PC"));
          display->setFont(ArialMT_Plain_24);
          display->drawString(84 + x, 14 + y, String(m2));
          break;
        case midi::PitchBend:
          drawRect(display, 84-38, 15, 84+36, 15+23);
          display->setFont(ArialMT_Plain_10);
          display->drawString(84 + x, 39 + y, String("Pitch")); 
          display->setFont(ArialMT_Plain_24);
          display->drawString(84 + x, 14 + y, String(m2));  
          break;
      }
      display->setFont(ArialMT_Plain_10);
      display->drawString(18 + x, 39 + y, String("Channel"));
      display->setFont(ArialMT_Plain_16);
      display->drawString(18 + x, 22 + y, String(m4));
    }
    else {
      display->setTextAlignment(TEXT_ALIGN_CENTER_BOTH);
      display->setFont(ArialMT_Plain_24);
      display->drawString(64, 32, String(banks[currentBank][lastUsed].pedalName)); 
    }    
  }  
  else if (MTC.getMode() == MidiTimeCode::SynchroClockMaster ||
           MTC.getMode() == MidiTimeCode::SynchroClockSlave) {
    display->setFont(ArialMT_Plain_24);
    display->setTextAlignment(TEXT_ALIGN_CENTER);
    display->drawString(22 + x, 12 + y, String(bpm));
    display->setFont(ArialMT_Plain_10);
    display->drawString(22 + x, 36 + y, "BPM");
    display->setTextAlignment(TEXT_ALIGN_RIGHT);
    switch (timeSignature) {
      case PED_TIMESIGNATURE_2_4:
        display->drawString(128 + x, 36 + y, "2/4");
        break;
      case PED_TIMESIGNATURE_4_4:
        display->drawString(128 + x, 36 + y, "4/4");
        break;
      case PED_TIMESIGNATURE_3_4:
        display->drawString(128 + x, 36 + y, "3/4");
        break;
      case PED_TIMESIGNATURE_3_8:
        display->drawString(128 + x, 36 + y, "3/8");
        break;
      case PED_TIMESIGNATURE_6_8:
        display->drawString(128 + x, 36 + y, "6/8");
        break;
      case PED_TIMESIGNATURE_9_8:
        display->drawString(128 + x, 36 + y, "9/8");
        break;
      case PED_TIMESIGNATURE_12_8:
        display->drawString(128 + x, 36 + y, "12/8");
        break;
    }
    display->setTextAlignment(TEXT_ALIGN_CENTER);
    if (MTC.getMode() == MidiTimeCode::SynchroClockMaster)
      display->drawString(68 + x, 36 + y, "Master");
    else if (MTC.getMode() == MidiTimeCode::SynchroClockSlave)
      display->drawString(68 + x, 36 + y, "Slave");
    /*
    display->setTextAlignment(TEXT_ALIGN_RIGHT);
    display->setFont(MTC.isPlaying() ? beats4 : beats4off);
    display->drawString(112 + x, 14 + x, String(MTC.getBeat()));
    */
    display->setTextAlignment(TEXT_ALIGN_RIGHT);
    display->setFont(block);
    switch (timeSignature) {
      case PED_TIMESIGNATURE_2_4:
        display->drawString( 68 + x, 16 + x, String(0));
        display->drawString( 88 + x, 16 + x, String(0));
        break;
      case PED_TIMESIGNATURE_4_4:
        display->drawString( 68 + x, 16 + x, String(0));
        display->drawString( 88 + x, 16 + x, String(0));
        display->drawString(108 + x, 16 + x, String(0));
        display->drawString(128 + x, 16 + x, String(0));
        break;
      case PED_TIMESIGNATURE_3_4:
      case PED_TIMESIGNATURE_3_8:
      case PED_TIMESIGNATURE_6_8:
      case PED_TIMESIGNATURE_9_8:
      case PED_TIMESIGNATURE_12_8:
        display->drawString( 68 + x, 16 + x, String(0));
        display->drawString( 88 + x, 16 + x, String(0));
        display->drawString(108 + x, 16 + x, String(0));
        break;
    }
    switch (MTC.getBeat()) {
      case 0:
        if (MTC.isPlaying())
          display->drawString( 68 + x, 16 + x, String(2));
        else
          display->drawString( 68 + x, 16 + x, String(1));
        break;
      case 1:
        if (MTC.isPlaying())
          display->drawString( 88 + x, 16 + x, String(2));
        else
          display->drawString( 88 + x, 16 + x, String(1));
        break;
      case 2:
        if (MTC.isPlaying())
          display->drawString(108 + x, 16 + x, String(2));
        else
          display->drawString(108 + x, 16 + x, String(1));
        break;
      case 3:
        if (MTC.isPlaying())
          display->drawString(128 + x, 16 + x, String(2));
        else
          display->drawString(128 + x, 16 + x, String(1));
        break;
    }
    ui.disableAutoTransition();
  }
  else if (MTC.getMode() == MidiTimeCode::SynchroMTCMaster ||
           MTC.getMode() == MidiTimeCode::SynchroMTCSlave) {
    char buf[12];
    sprintf(buf, "%02d:%02d:%02d:%02d", MTC.getHours(), MTC.getMinutes(), MTC.getSeconds(), MTC.getFrames());
    display->setFont(ArialMT_Plain_24);
    display->setTextAlignment(TEXT_ALIGN_CENTER_BOTH);
    display->drawString(64 + x, 32 + y, buf);
    ui.disableAutoTransition();
  }
  else {
    display->setFont(ArialMT_Plain_16);
    display->setTextAlignment(TEXT_ALIGN_CENTER_BOTH);
    display->drawString(64 + x, 32 + y, MODEL);
    display->setFont(ArialMT_Plain_10);
    display->setTextAlignment(TEXT_ALIGN_LEFT);
    display->drawString(110 + x, 16 + y, String("TM"));
    ui.enableAutoTransition();
  }
  
  events.send(MTC.isPlaying() ? "1" : "0", "play");
  
  if (MTC.getMode() == MidiTimeCode::SynchroClockMaster ||
      MTC.getMode() == MidiTimeCode::SynchroClockSlave)  {
    char buf[4];
    events.send("", "mtc");
    sprintf(buf, "%3d", bpm);
    events.send(buf, "bpm");
    sprintf(buf, "%d", MTC.getBeat() + 1);
    events.send(buf, "beat");
    switch (timeSignature) {
      case PED_TIMESIGNATURE_2_4:
        events.send("2/4", "timesignature");
        break;
      case PED_TIMESIGNATURE_4_4:
        events.send("4/4", "timesignature");
        break;
      case PED_TIMESIGNATURE_3_4:
        events.send("3/4", "timesignature");
        break;
      case PED_TIMESIGNATURE_3_8:
        events.send("3/8", "timesignature");
        break;
      case PED_TIMESIGNATURE_6_8:
        events.send("6/8", "timesignature");
        break;
      case PED_TIMESIGNATURE_9_8:
        events.send("9/8", "timesignature");
        break;
      case PED_TIMESIGNATURE_12_8:
        events.send("12/8", "timesignature");
        break;
    }
  }

  if (MTC.getMode() == MidiTimeCode::SynchroMTCMaster ||
      MTC.getMode() == MidiTimeCode::SynchroMTCSlave) {
    char buf[12];
    sprintf(buf, "%02d:%02d:%02d:%02d", MTC.getHours(), MTC.getMinutes(), MTC.getSeconds(), MTC.getFrames());
    events.send(buf, "mtc");
    events.send("", "bpm");
    events.send("", "beat");
    events.send("", "timesignature");
  }
}

void drawFrame2(OLEDDisplay *display, OLEDDisplayUiState* state, int16_t x, int16_t y)
{
  if (MTC.isPlaying() || MTC.getMode() != PED_MTC_NONE || millis() < endMillis2)
    ui.switchToFrame(0);

  display->setFont(ArialMT_Plain_10);
  display->setTextAlignment(TEXT_ALIGN_LEFT);
  display->drawString(0 + x, 16 + y, "Device:");
  display->setTextAlignment(TEXT_ALIGN_RIGHT);
  display->drawString(128 + x, 16 + y, host);
#ifdef WIFI
  switch (WiFi.getMode()) {
    case WIFI_AP:
    case WIFI_AP_STA:
      display->setTextAlignment(TEXT_ALIGN_LEFT);
      display->drawString(0 + x, 26 + y, "AP:");
      display->setTextAlignment(TEXT_ALIGN_RIGHT);
      display->drawString(128 + x, 26 + y, wifiSoftAP);
      display->setTextAlignment(TEXT_ALIGN_LEFT);
      display->drawString(0 + x, 36 + y, "AP IP:");
      display->setTextAlignment(TEXT_ALIGN_RIGHT);
      display->drawString(128 + x, 36 + y, WiFi.softAPIP().toString());
      break;
    case WIFI_STA:
      display->setTextAlignment(TEXT_ALIGN_LEFT);
      display->drawString(0 + x, 26 + y, "SSID:");
      display->setTextAlignment(TEXT_ALIGN_RIGHT);
      display->drawString(128 + x, 26 + y, wifiSSID);
      display->setTextAlignment(TEXT_ALIGN_LEFT);
      display->drawString(0 + x, 36 + y, "IP:");
      display->setTextAlignment(TEXT_ALIGN_RIGHT);
      display->drawString(128 + x, 36 + y, WiFi.localIP().toString());
      break;
  }
#endif
}

void drawFrame3(OLEDDisplay *display, OLEDDisplayUiState* state, int16_t x, int16_t y)
{
  display->setTextAlignment(TEXT_ALIGN_CENTER);
  display->setFont(ArialMT_Plain_16);
  display->drawString(64 + x, 16 + y, host);
  display->drawString(64 + x, 36 + y, WiFi.localIP().toString());
}

// This array keeps function pointers to all frames
// frames are the single views that slide in
FrameCallback frames[] = { drawFrame1, drawFrame2 };
int frameCount = sizeof(frames) / sizeof(FrameCallback);

// Overlays are statically drawn on top of a frame
OverlayCallback overlays[] = { topOverlay, bottomOverlay };
int overlaysCount = sizeof(overlays) / sizeof(OverlayCallback);

void display_init()
{
  display.init();
  display.flipScreenVertically();
  display.setContrast(255);

#ifdef WIFI
  display.clear();
  display.drawXbm((display.getWidth() - WIFI_LOGO_WIDTH) / 2, (display.getHeight() - WIFI_LOGO_HEIGHT) / 2, WIFI_LOGO_WIDTH, WIFI_LOGO_HEIGHT, WiFiLogo);
  display.display();
  delay(500);
#endif

#ifdef BLE
  display.clear();
  display.drawXbm((display.getWidth() - BLUETOOTH_LOGO_WIDTH) / 2, (display.getHeight() - BLUETOOTH_LOGO_HEIGHT) / 2, BLUETOOTH_LOGO_WIDTH, BLUETOOTH_LOGO_HEIGHT, BluetoothLogo);
  display.display();
  delay(500);
#endif

 // The ESP is capable of rendering 60fps in 80Mhz mode
	// but that won't give you much time for anything else
	// run it in 160Mhz mode or just set it to 30 fps
  ui.setTargetFPS(60);

	// Customize the active and inactive symbol
  ui.setActiveSymbol(activeSymbol);
  ui.setInactiveSymbol(inactiveSymbol);

  // You can change this to
  // TOP, LEFT, BOTTOM, RIGHT
  ui.setIndicatorPosition(BOTTOM);

  // Defines where the first frame is located in the bar.
  ui.setIndicatorDirection(LEFT_RIGHT);

  // Disable drawing of all indicators.
  ui.disableAllIndicators();

  // You can change the transition that is used
  // SLIDE_LEFT, SLIDE_RIGHT, SLIDE_UP, SLIDE_DOWN
  ui.setFrameAnimation(SLIDE_LEFT);

  // Add frames
  ui.setFrames(frames, frameCount);

  // Add overlays
  ui.setOverlays(overlays, overlaysCount);
  
  // Initialising the UI will init the display too.
  ui.init();

  display.flipScreenVertically();
}

void display_ui_update_disable()
{
  uiUpdate = false;
}

void display_ui_update_enable()
{
  uiUpdate = true;
}

void display_update(bool force = false)
{
  if (uiUpdate) ui.update();
}