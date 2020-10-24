#include <OledDisplay.h>
#include "RGB_LED.h"

#include "AzureIotHub.h"
#include "DevKitMQTTClient.h"

RGB_LED rgbled;
const int DELAY_MS = 1000;

int executionNumber = 0;
bool executeBlink = false;
bool executeBlinkRgbLed = false;

void setup()
{
  Screen.init();

  Screen.print(0, "Initializing...");

  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(USER_BUTTON_A, INPUT);
  pinMode(USER_BUTTON_B, INPUT);

  rgbled.turnOff();
}

void loop()
{
  LogInfo("Execution #%d", ++executionNumber);
  Screen.print(0, "Press a button!");

  if (digitalRead(USER_BUTTON_A) == LOW || digitalRead(USER_BUTTON_B) == LOW)
  {
    Screen.clean();
  }

  if (digitalRead(USER_BUTTON_A) == LOW)
  {
    rgbled.turnOff();
    executeBlink = true;
    executeBlinkRgbLed = false;
  }

  if (digitalRead(USER_BUTTON_B) == LOW)
  {
    executeBlink = false;
    executeBlinkRgbLed = true;
  }

  if (digitalRead(USER_BUTTON_A) == LOW && digitalRead(USER_BUTTON_B) == LOW)
  {
    Screen.print(0, "Good night...");
    SystemStandby(60);
  }

  if (executeBlink)
  {
    blink();
  }
  else if (executeBlinkRgbLed)
  {
    blinkRgbLed();
  }

  delay(DELAY_MS);
}

void blinkRgbLed()
{
  const int *colors = getRgbValues();

  char msg[20];
  sprintf(msg, "RGB=%d,%d,%d", colors[0], colors[1], colors[2]);
  LogInfo(msg);

  rgbled.setColor(colors[0], colors[1], colors[2]);
  Screen.print(0, msg);
  delay(DELAY_MS);
}

void blink()
{
  digitalWrite(LED_BUILTIN, HIGH);
  Screen.print(0, "led is ON");
  delay(DELAY_MS);

  digitalWrite(LED_BUILTIN, LOW);
  Screen.print(0, "led is OFF");
  delay(DELAY_MS);
}

int *getRgbValues()
{
  const int COLOR_LOW = 0;
  const int COLOR_HIGH = 255;
  srand((unsigned)time(NULL));
  static int colors[3];

  for (int i = 0; i < 3; i++)
  {
    colors[i] = random(COLOR_LOW, COLOR_HIGH);
  }

  return colors;
}