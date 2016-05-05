void logThings(void) {
  // fetch the time from the RTC clock
  DateTime now = rtc.now();
  printDate(now);
  printFloat(temp);
  printFloat(pressure);
  printFloat(altitude);
  printFloat(humidity);
  printFloat(current_mA1);
  printFloat(busvoltage1);
  printFloat(current_mA2);
  printFloat(busvoltage2);
  printFloat(current_mA3);
  printFloat(busvoltage3);

  logfile.println();
  logfile.flush();

  Serial.println();
  Serial1.println();
  
}


void printFloat(float f)
{

#if ECHO_TO_SERIAL
  Serial.print(f);
  Serial.print(",");
#endif

#if ECHO_TO_XBEE
  Serial1.print(f);
  Serial1.print(",");
#endif

  logfile.print(f);
  logfile.print(",");

}


void printDate(DateTime d)
{
  logfile.print('"');
  logfile.print(d.year(), DEC);
  logfile.print("/");
  logfile.print(d.month(), DEC);
  logfile.print("/");
  logfile.print(d.day(), DEC);
  logfile.print(" ");
  logfile.print(d.hour(), DEC);
  logfile.print(":");
  logfile.print(d.minute(), DEC);
  logfile.print(":");
  logfile.print(d.second(), DEC);
  logfile.print('"');
  logfile.print(", ");

#if ECHO_TO_SERIAL

  Serial.print('"');
  Serial.print(d.year(), DEC);
  Serial.print("/");
  Serial.print(d.month(), DEC);
  Serial.print("/");
  Serial.print(d.day(), DEC);
  Serial.print(" ");
  Serial.print(d.hour(), DEC);
  Serial.print(":");
  Serial.print(d.minute(), DEC);
  Serial.print(":");
  Serial.print(d.second(), DEC);
  Serial.print('"');
  Serial.print(", ");
#endif

#if ECHO_TO_XBEE
  Serial1.print('"');
  Serial1.print(d.year(), DEC);
  Serial1.print("/");
  Serial1.print(d.month(), DEC);
  Serial1.print("/");
  Serial1.print(d.day(), DEC);
  Serial1.print(" ");
  Serial1.print(d.hour(), DEC);
  Serial1.print(":");
  Serial1.print(d.minute(), DEC);
  Serial1.print(":");
  Serial1.print(d.second(), DEC);
  Serial1.print('"');
  Serial1.print(", ");
#endif

}

