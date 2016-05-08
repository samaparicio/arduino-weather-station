/* ADAFRUIT FEATHER 32u4 WEATHER STATION
  pinout https://learn.adafruit.com/system/assets/assets/000/030/918/original/microcomputers_2771_pinout_v1_0.png?1457305552
  Logs sensor data to logger
  sends sensor data over to an XBEE via Serial1
  stack adafruit adalogger featherwing on 32u4 feather - https://www.adafruit.com/products/2922
  connect BME280 via I2C - https://www.adafruit.com/products/2652
  connect INA3221 via I2C - http://www.switchdoc.com/ina3221-breakout-board/
*/


#include <SD.h> //needed for the SD card part of the adalogger
#include <Wire.h> //needed for I2C communication
#include <RTClib.h> //needed for the RTC chip in the adalogger
#include <SDL_Arduino_INA3221.h> //needed for the energy monitoring breakout board
#include <Adafruit_BME280.h> // needed for the BME280 sensor
#include "LowPower.h"


// BME 280
Adafruit_BME280 bme;
#define SEALEVELPRESSURE_HPA (1017.0) //this is a setting to calibrate elevation. Get from NWS for your location


// SOLAR MEASURING
SDL_Arduino_INA3221 ina3221;
#define LIPO_BATTERY_CHANNEL 2
#define SOLAR_CELL_CHANNEL 1
#define OUTPUT_CHANNEL 3

// SD CARD LOGGING
File logfile;

// Real Time Clock
RTC_PCF8523 rtc;

// ON/OFF SWITCHES FOR WHERE TO LOG
#define ECHO_TO_SERIAL   1 // echo data to serial port
#define ECHO_TO_XBEE     1 // enable or disable xbee sending

//pin that is connected to DTR / pin 9 of XBEE
const int XBee_wake = 5;

// VARIABLES FOR LOGGING DATA
float temp = 0;
float pressure = 0;
float altitude = 0;
float humidity = 0;
float busvoltage1 = 0;
float current_mA1 = 0;
float busvoltage2 = 0;
float current_mA2 = 0;
float busvoltage3 = 0;
float current_mA3 = 0;

void error(char *str)
{
  Serial.print("error: ");
  Serial.println(str);
  while (1);
}

void setup(void)
{

  turnXBeeOn();

  Serial1.begin(9600); // XBee
  Serial.begin(57600); // UART / Serial monitor

  initializeBME280();
  initializeRTC();
  initializeSDCard();
  //initialize energy measurement
  ina3221.begin();

  delay(1000);
}

void loop(void)
{

  turnXBeeOn();
  readTheSensors();
  readPowerConsumption();
  logThings();
  //give the xbee a chance to finish transmitting
  delay(1500);
  turnXBeeOff();

  // power down the arduino for ~1 min
  for (int i = 0; i < 7; i++) {
    // Power down the microcontroller for 8 s with ADC and BOD module disabled
    LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF);
  }
}
