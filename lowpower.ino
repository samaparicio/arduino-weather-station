void turnXBeeOn(void) {
  pinMode(XBee_wake, OUTPUT);
  digitalWrite(XBee_wake, LOW);
  delay(1000); // let the xbee stretch and come to its senses
}

void turnXBeeOff(void) {
  pinMode(XBee_wake, INPUT); // put pin in a high impedeance state
  digitalWrite(XBee_wake, HIGH);
}


