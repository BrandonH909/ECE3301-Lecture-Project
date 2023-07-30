#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <BlynkSimpleEsp8266.h>
#include <Adafruit_MLX90614.h>

#define BLYNK_TEMPLATE_ID "TMPL2wT4lMKPc"
#define BLYNK_TEMPLATE_NAME "MLX90614 Temperature Sensor"
#define BLYNK_AUTH_TOKEN "en42OI6r8PjcN6hVEwzri1ENa5cpzo1u"

char auth[] = "en42OI6r8PjcN6hVEwzri1ENa5cpzo1u";
char ssid[] = "Your Network ID";
char pass[] = "Your Wifi Password";

#define SDA_PIN 12
#define SCL_PIN 14
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET    -1

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
Adafruit_MLX90614 mlx;

void setup()
{
  Serial.begin(115200);
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

   // Initialize the I2C communication
 Wire.begin(SDA_PIN, SCL_PIN);
 

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  delay(2000);
  display.clearDisplay();
  display.setTextColor(WHITE);
  

  if (!mlx.begin(0x5B)) { // Initialize MLX90614 sensor
    display.setTextSize(1);
    display.setCursor(0, 0);
    display.println("MLX90614 Failed");
    display.setCursor(0, 10);
    display.println("Check wiring!");
    display.display();
    while (1); // Stop here if the sensor initialization fails
  }

  Serial.println("Begin ok!");
}

void loop()
{
  Blynk.run();

  // Read temperature values from the sensor
  float ambientTemp = mlx.readAmbientTempC();
  float objectTemp = mlx.readObjectTempC();

  // Print measured data in Celsius and Fahrenheit
  Serial.print("Ambient Celsius: "); Serial.print(ambientTemp); Serial.println(" C");
  Serial.print("Object Celsius: ");  Serial.print(objectTemp);  Serial.println(" C");
  Serial.print("Ambient Fahrenheit: "); Serial.print(ambientTemp * 9 / 5 + 32); Serial.println(" F");
  float ftemp = objectTemp * 9 / 5 + 32;
  Serial.print("Object Fahrenheit: ");  Serial.print(ftemp);  Serial.println(" F");

  // Display temperature on OLED screen
  display.clearDisplay();
  display.setCursor(5, 0);
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.println("Temperature");
  display.setTextSize(2);
  display.setCursor(0, 20);
  display.print(objectTemp, 1);
  display.print((char)247);
  display.print("C");

  display.setCursor(0, 45);
  display.print(ftemp, 1);
  display.print((char)247);
  display.print("F");

  display.display();

  // Send temperature data to Blynk app
  Blynk.virtualWrite(V2, objectTemp);
  Blynk.virtualWrite(V1, ftemp);

  delay(1000);
}
