/* ADAFRUIT FEATHER 32u4 WEATHER STATION
 pinout https://learn.adafruit.com/system/assets/assets/000/030/918/original/microcomputers_2771_pinout_v1_0.png?1457305552
 Logs sensor data to logger
 sends some sensor data over to an XBEE via Serial1
 stack data adafruit adalogger featherwing on 32u4 feather
 connect BME280 via I2C
*/


#include <SD.h> //needed for the SD card part of the adalogger
#include <Wire.h> //needed for I2C communication
#include <RTClib.h> //needed for the RTC chip in the adalogger
#include <SDL_Arduino_INA3221.h> //needed for the energy monitoring breakout board
#include <Adafruit_BME280.h> // needed for the BME280 sensor


// BME 280
Adafruit_BME280 bme;
#define SEALEVELPRESSURE_HPA (1017.0)


// SOLAR MEASURING
SDL_Arduino_INA3221 ina3221;
#define LIPO_BATTERY_CHANNEL 2
#define SOLAR_CELL_CHANNEL 1
#define OUTPUT_CHANNEL 3

// SD CARD LOGGING
File logfile;
uint32_t syncTime = 0; // time of last sync()

// how many milliseconds between grabbing data and logging it. 1000 ms is once a second
#define LOG_INTERVAL  1000 // mills between entries (reduce to take more/faster data)

// how many milliseconds before writing the logged data permanently to disk
// set it to the LOG_INTERVAL to write each time (safest)
// set it to 10*LOG_INTERVAL to write all data every 10 datareads, you could lose up to
// the last 10 reads if power is lost but it uses less power and is much faster!
#define SYNC_INTERVAL 1000 // mills between calls to flush() - to write data to the card


// Real Time Clock
RTC_PCF8523 rtc; 

// ON/OFF SWITCHES FOR WHERE TO LOG
#define ECHO_TO_SERIAL   1 // echo data to serial port
#define WAIT_TO_START    0 // Wait for serial input in setup()
#define ECHO_TO_XBEE     1 // enable or disable xbee sending


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

  // delay for the amount of time we want between readings
  delay((LOG_INTERVAL - 1) - (millis() % LOG_INTERVAL));

  // READ THE SENSORS
  readTheSensors();
  
  // READ THE POWER CONSUMPTION
  readPowerConsumption();

  //log things
  logThings();
}
