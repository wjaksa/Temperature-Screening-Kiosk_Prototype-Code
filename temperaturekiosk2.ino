// This #include statement was automatically added by the Particle IDE.
#include <Adafruit_MLX90614.h>

// This #include statement was automatically added by the Particle IDE.
#include <LiquidCrystal.h>


Adafruit_MLX90614 mlx = Adafruit_MLX90614();

// code for LCD display
#include "LiquidCrystal/LiquidCrystal.h"
LiquidCrystal lcd(2, 3, 4, 5, 6, 7); // Creates an LCD object. Parameters: (rs, enable, d4, d5, d6, d7) 

//Ultrasonic Sensor
const int triggerPin = D9;
const int echoPin = D10;
const int buzzer  = D11;
float pulse_width, distance;

const pin_t MY_LED1 = D12;  // RED ALARM LED
const pin_t MY_LED2 = D13;  // GREEN ACCEPTED LED
int TIME1 = 500; // RED LED ON/OFF Time Delay
int TIME2 = 1000; // GREEN LED ON/OFF Time Delay

float MaxTemp = 37.5; //Max Allowed Temperature in Celcius


void setup() 
{
// for Infrared sensor
    mlx.begin();
    
//for LCD Display
    lcd.begin(16,2); // Initializes the interface to the LCD screen, and specifies the dimensions (width and height) of the display } 
    
//Ultrasonic Sensor
    pinMode(triggerPin, OUTPUT);
    pinMode(echoPin, INPUT);
    digitalWrite(triggerPin, LOW);
    delayMicroseconds(2);
    
//Buzzer
    pinMode(buzzer, OUTPUT);
 
//LEDs   
	pinMode(MY_LED1, OUTPUT); // RED LED
	pinMode(MY_LED2, OUTPUT); // GREEN LED
    
}



void loop() 
{
    if(distance<=5) //Required minimum gap between Sensor and forehead
    {

    delay(500);
    float amb = mlx.readAmbientTempC();
    float obj1 = mlx.readObjectTempC(); // Reading the Person's temperature
    digitalWrite(buzzer,HIGH);
    delay(250);
    digitalWrite(buzzer,LOW);
    delay(250);
    float obj2 = mlx.readObjectTempC();
    digitalWrite(buzzer,HIGH);
    delay(250);
    digitalWrite(buzzer,LOW);
    delay(250);
    float obj3 = mlx.readObjectTempC();
    digitalWrite(buzzer,HIGH);
    delay(250);
    digitalWrite(buzzer,LOW);
    delay(250);
    float obj4 = mlx.readObjectTempC();
    digitalWrite(buzzer,HIGH);
    delay(1000);
    float obj = ((obj1+obj2+obj3+obj4)/4)+3; // 3°C is added to the average to compensate the error in measurement after testing the device
    digitalWrite(buzzer,LOW);
    Particle.publish("AmbientTemp", String(amb), PRIVATE); // Publishing on Particle Events for information and records
    Particle.publish("PersonalTemp", String(obj), PRIVATE); // Publishing on Particle Events for information and records
    
            lcd.clear(); // Clears the LCD screen
            lcd.setCursor(0,0);
            lcd.print("Temp :       \337C"); // Prints "TEMP : Wording and degree (° = \337C) symbol
            lcd.setCursor(7,0);
            lcd.print(obj); // Prints person's measured average temperature on the LCD 
            
 
        if(obj>MaxTemp) // Acceptable maximum temperature
        {
            lcd.setCursor(0,1);
            lcd.print("TEMP IS TOO HIGH");

            //ALARM RED LED
            for (int j=1; j<=30; j=j+1) // j is the No of blinks of the LED
            {
            digitalWrite(MY_LED1,HIGH); //Turn red LED on
            delay(TIME1);             //Leave on for TIME 1
            digitalWrite(MY_LED1,LOW);  //Turn red LED off
            delay(TIME1);            //Leave off TIME 2
            }

            lcd.clear(); // Clears the LCD screen
        
        }
        else
        {
            
            digitalWrite(MY_LED2, HIGH); // GREEN LED is ON if the temperature is accepeted
            delay(10000); // Display and LED  ON delay
            
            digitalWrite(MY_LED2, LOW); // GREEN LED is OFF
            lcd.clear(); // Clears the LCD screen 
            
        }
    
 
    }
    
    else
    
    {
    Particle.publish("Come Closer", PRIVATE); //Display message if the person is far from the device.
    
    lcd.setCursor(0,0);
    lcd.print("PLEASE"); // Prints "Come Closer" on the LCD 
    lcd.setCursor(5,1);
    lcd.print("COME CLOSER"); // Prints "Come Closer" on the LCD
    
    //digitalWrite(buzzer,LOW);
    }
    

//For Ultrasonic Sensor
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin, LOW);
  pulse_width = pulseIn(echoPin, HIGH);
  distance = (pulse_width*.343)/20;
  Particle.publish("Distance", String(distance)); //Publish the distance on Particle event for referance.
  delay(500);
    
}

