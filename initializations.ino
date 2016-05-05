void initializeBME280(void) {

  //initialize the BME280 sensor
  if (!bme.begin()) {
    Serial.println("Could not find a valid BME280 sensor, check wiring!");
    while (1);
  }

}

void initializeRTC(void) {
  // connect to RTC
  Wire.begin();
  if (!rtc.begin()) {
    logfile.println("RTC failed");
    Serial.println("RTC failed");
  }
}

void initializeSDCard(void) {
  // initialize the SD card
  Serial.print("Initializing SD card...");

  // make sure that the default chip select pin is set to
  // output, even if you don't use it:
  pinMode(10, OUTPUT);

  // see if the card is present and can be initialized:
  if (!SD.begin(10)) //10 is the chip select pin
  {
    error("Card failed, or not present");
  }
  Serial.println("card initialized.");

  // TODO: this code will fail to create a Csv file after there are 100 files in the sd card
  // create a new file
  char filename[] = "LOGGER00.CSV";
  for (uint8_t i = 0; i < 100; i++) {
    filename[6] = i / 10 + '0';
    filename[7] = i % 10 + '0';
    if (! SD.exists(filename)) {
      // only open a new file if it doesn't exist
      logfile = SD.open(filename, FILE_WRITE);
      break;  // leave the loop!
    }
  }

  if (! logfile) {
    error("couldnt create file");
  }

  Serial.print("Logging to: ");
  Serial.println(filename);

  logfile.println("------TIMESTAMP-----, TEMP , PRESSUR, ALTIT,HUMID, mA1 , V1 ,  mA2, V2 ,  mA3 ,  V3 ,");
  Serial.println("------TIMESTAMP-----, TEMP , PRESSUR, ALTIT,HUMID, mA1 , V1 ,  mA2, V2 ,  mA3 ,  V3 ,");
  Serial1.println("------TIMESTAMP-----, TEMP , PRESSUR, ALTIT,HUMID, mA1 , V1 ,  mA2, V2 ,  mA3 ,  V3 ,");

}

