#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

const int buttonPowerPin = 5;
const int pinX      = A14; // X
const int pinY      = A15; // Y
const int ledPin    = 13;
boolean powerStatePrev = true; // предыдущее состояние кнопки
boolean powerState = false;

const int speedForwardPin = 22;
const int speedBackwardPin = 23;
const int speedPWMPin = 44;
const int anglePWNPin = 46;

boolean powerSwitchStateOld = LOW;
boolean powerSwitchStateNew = LOW;

int velocityValuePrev = 0;

int angleValuePrev = 0;

long time = 0;
long debounce = 1000;

void setup() {
  Serial.begin (9600); // Задаем скорость обмена com-порта 9600
  pinMode(pinX, INPUT);
  pinMode(pinY, INPUT);
  pinMode(buttonPowerPin, INPUT);
  digitalWrite(buttonPowerPin, HIGH); // включаем встроенный подтягивающий резистор
  digitalWrite(speedForwardPin, LOW);
  digitalWrite(speedBackwardPin, LOW);

  lcd.init();                      // Инициализация дисплея
  lcd.backlight();                 // Подключение подсветки
  lcdWrite();
}

void loop() {
  if (!powerCheck()) return;

  readJoystick();
}

void checkPins() {
  String result = String(digitalRead(buttonPowerPin)) + " " + String(analogRead(pinX)) + " " + String(analogRead(pinY));
  Serial.println(result);
}

// Определяем нажатие кнопки
bool powerCheck() {
  powerSwitchStateNew = digitalRead(buttonPowerPin);

  if (powerSwitchStateOld == powerSwitchStateNew) return powerState;
  if (powerSwitchStateNew != HIGH) return powerState;

  powerSwitchStateOld = powerSwitchStateNew;

  powerState = !powerState;

  lcdWrite();

  if (!powerState) stop();

  return powerState;
}

void lcdWrite() {
  String text;

  lcd.clear();

  if (powerState) {
    text = "Enabled";
  } else {
    text = "Disabled";
  }

  lcd.print(text);

  digitalWrite(ledPin, powerState); // включаем светодиод на пине 13
}

/**
   Set angle of Step motor

   @return bool true if angle is changed
*/
bool setAngle(int angleValue) {
  if (angleValuePrev == angleValue) return false;

  angleValuePrev = angleValue;

  analogWrite(anglePWNPin, abs(angleValue));

  return true;
}

/**
   Set speed of motor

   @return bool true if speed is changed
*/
bool setSpeed(int velocityValue) {
  if (0 && velocityValuePrev == velocityValue) return false;

  velocityValuePrev = velocityValue;

  if (velocityValue == 0) {
    stop ();

    return true;
  }

  if (velocityValue > 0) {
    digitalWrite(speedBackwardPin, LOW);
    digitalWrite(speedForwardPin, HIGH);
    analogWrite(speedPWMPin, abs(velocityValue));

    return true;
  }

  digitalWrite(speedForwardPin, LOW);
  digitalWrite(speedBackwardPin, HIGH);
  analogWrite(speedPWMPin, abs(velocityValue));

  return true;
}

void updateScreen(String str) {
  lcd.setCursor(0, 1);
  lcd.clear();
  lcd.print(str);
}

int getVelocity() {
  int Y = analogRead(pinY);

  Y = map(Y, 0, 1023, 255, -255);

  if (Y >= -20 && Y <= 20) Y = 0;

  return Y;
}

int getAngle() {
  int X = analogRead(pinX);

  X = map(X, 0, 1023, 180, 0);

  if (X >= 80 && X <= 100) X = 90;

  return X;
}

void readJoystick() {
  int angles = getAngle();
  int velocity = getVelocity();

  if (setAngle(angles)) updateScreen("Angle: " + String(angles));

  setSpeed(velocity);
}

void stop () {
  analogWrite(speedPWMPin, 0);
  digitalWrite(speedForwardPin, LOW);
  digitalWrite(speedBackwardPin, LOW);
  analogWrite(anglePWNPin, 0);
}
