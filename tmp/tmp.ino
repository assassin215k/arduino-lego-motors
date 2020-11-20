    //  Make a Toggle Switch Button
    // Define the pins being used
    int pinLed = 13;
    int pinSwitch = 5;
    // declaring variables to hold the new and old switch states
    boolean oldSwitchState = LOW;
    boolean newSwitchState = LOW;
    boolean LEDstatus = LOW;
    void setup()
    {
      Serial.begin(9600);
      pinMode(pinLed, OUTPUT);
      digitalWrite(pinLed, LOW);
      pinMode(pinSwitch, INPUT);
    }
    void loop()
    {
      newSwitchState = digitalRead(pinSwitch);
      if ( newSwitchState != oldSwitchState )
      {
        // has the button switch been closed?
        if ( newSwitchState == HIGH )
        {
          if ( LEDstatus == LOW ) {
            digitalWrite(pinLed, HIGH);
            LEDstatus = HIGH;
          }
          else                    {
            digitalWrite(pinLed, LOW);
            LEDstatus = LOW;
          }
        }
        oldSwitchState = newSwitchState;
      }
    }
