#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <SD.h>
#include <SPI.h>
#include "ds3231.h"
#include "TSL2561.h"
#include <OneWire.h>
#include <Streaming.h>    // e.g: Serial << "Example"
#include <Base64.h>      // Base64 編碼，Lora 傳送要用Base64，最多長度為20的資料
#include <avr/wdt.h>     // Watchdogs
#include <Keypad.h>     //按鍵keypad程式庫
#include <Servo.h>

int Testing_Time = 60000;   //seconds
unsigned long ti=millis();
int flag_date = 0;

// Water height define:****************************************************************************************************************
int Critical_height = 100;   // default height
int water_value;
const int Water_pin = A0;

// Lora define:****************************************************************************************************************
String Lora_raw_data0;
String Lora_raw_data1;
String Msg = "";          // Msg Master Send
String Ecd = "";          // Encoded Value
String MsgS = "";


// EC define:*******************************************************************************************************************
#define StartConvert 0
#define ReadTemperature 1
const byte numReadings = 20;     //the number of sample times
byte ECsensorPin = A9;  //EC Meter analog output,pin on analog 1
byte DS18B20_Pin = 2; //DS18B20 signal, pin on digital 2
unsigned int AnalogSampleInterval=25,printInterval=500,tempSampleInterval=500;  //analog sample interval;serial print interval;temperature sample interval
unsigned int readings[numReadings];      // the readings from the analog input
byte index = 0;                  // the index of the current reading
unsigned long AnalogValueTotal = 0;                  // the running total
float AnalogAverage = 0,averageVoltage=0;                // the average
unsigned long AnalogSampleTime,printTime,tempSampleTime;
float temperature,ECcurrent; 

struct Return_Temp_EC{
  float var1_Temp;
  float var2_EC;
};
Return_Temp_EC Temp_EC;

 
//Temperature chip i/o
OneWire ds(DS18B20_Pin);  // on digital pin 2
//*****************************************************************************************************************************
// PH define:*******************************************************************************************************************
#define SensorPin A8            //pH meter Analog output to Arduino Analog Input 2
#define Offset -0.54            //deviation compensate
#define samplingInterval 20
#define ArrayLenth  40    //times of collection
float PH_value;
float voltage;
int pHArray[ArrayLenth];   //Store the average value of the sensor feedback
int pHArrayIndex=0;    
//*****************************************************************************************************************************
// TSL2561 define:*******************************************************************************************************************
TSL2561 tsl(TSL2561_ADDR_FLOAT);  

struct Return_TSL2561{
  int var1_IR;
  int var2_FULL;
  int var3_VISIBLE;
  int var4_LUX;
};
Return_TSL2561 Get_TSL;

//*****************************************************************************************************************************
// Clock define:*******************************************************************************************************************
#define BUFF_MAX 128

uint8_t time[8];
char recv[BUFF_MAX];
unsigned int recv_size = 0;
unsigned long prev, interval = 5000;
void parse_cmd(char *cmd, int cmdsize);

int Initial_time[3];
String timeString;
unsigned long Duration;
//*****************************************************************************************************************************
LiquidCrystal_I2C lcd(0x27, 16, 2);
File myFile;

int CS_Pin = 4;    //Chip select
//float humidity, temperature;

int C = -1;
//Keypad define*******************************************************************************************************************
  int flag_failure = false;
  const byte ROWS=4;    //定義4行
  const byte COLS=4;    //定義4列
  char keys[ROWS][COLS]={
    {'7','4','1','0'},
    {'8','5','2','A'},
    {'9','6','3','B'},
    {'C','D','E','F'} 
  };
  byte rowPins[ROWS] = {42,43,44,45};
  byte colPins[COLS] = {46,47,48,49};
 Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS);

 Servo servo_base;
 Servo servo_shoulder;
 Servo servo_forearm;
 int base, shoulder, forearm; 
 boolean flag_servo = false;
 int Servo_control[3] = {500,2500,500};

//主程式碼************************************************************************************************************************
void setup() {
  Serial.begin(38400);
  lcd.begin();    // initialize the LCD
  lcd.backlight();   // Turn on the blacklight and print a message
  pinMode(53, OUTPUT);
  pinMode(Water_pin,INPUT);
  SD.begin(CS_Pin);
  tsl_setup();
  Initial_SD();
  Clock_setup();
  EC_setup();
  servo_setup();
  Lora_Slave_setup();
  wdt_enable(WDTO_8S);
  lcd.clear();
  lcd.print("Welcome");
  delay(1000);
  lcd.clear();
  Serial.println("Setup Ok");
}
//主程式碼************************************************************************************************************************
void loop() {
  GetClock(1);
  Water_loop();
  Get_tsl();
  Get_PH();
  Get_EC();
  lcd.setCursor(0,0); lcd.print("T:"); lcd.print(Temp_EC.var1_Temp,1); 
  lcd.setCursor(8,0); lcd.print("EC:"); lcd.print(Temp_EC.var2_EC,3); 
  lcd.setCursor(0,1); lcd.print(timeString); lcd.print(" ");
  lcd.setCursor(9,1); lcd.print("pH:"); lcd.print(PH_value);
  Lora_Send(); 
  wdt_reset();

}
//Water height define************************************************************************************************************************
void Water_loop(){
  water_value = analogRead(Water_pin);
  water_value = water_value;    // 判斷是否需要抽水

}



