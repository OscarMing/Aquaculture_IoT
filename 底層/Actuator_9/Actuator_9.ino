#include <Streaming.h>    // e.g: Serial << "Example"
#include <Base64.h>      // Base64 編碼，Lora 傳送要用Base64，最多長度為20的資料
#include <Servo.h>
#include <avr/wdt.h>

// Feed Servo define:****************************************************************************************************************
Servo servo_feeder;

int feeder; 
String mode = "";

// Lora define:****************************************************************************************************************
String Msg = "";          // Msg Master Send
String Ecd = "";          // Encoded Value
String Dcd = "";
String MsgS = "";
String Decode_Msg="";

// All Control Relay define:****************************************************************************************************************
String Relay_String = "00000000";

//const int relay110_1 = 22;
//const int relay110_2 = 23;
//const int relay110_3 = 24;
//const int relay110_4 = 25;    // 電磁閥的電
//const int pump_relay1 = 26;
//const int pump_relay2 = 27;
//const int Relay_7 = 28;
//const int Relay_8 = 29;

const int Actuator_pin[8] = {22,23,24,25,26,27,28,29};

boolean flag_go = false;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(38400);
  Lora_Slave_setup();
  Actuator_Setup();
  Feed_Setup();
  wdt_enable(WDTO_8S);
//  Actuator_update(Relay_String);
//  Serial.println(Relay_String);
//  delay(1000);
}

void loop() {
  // put your main code here, to run repeatedly:
  getMsg();
  wdt_reset();
}

void Lora_Slave_setup() {
  Serial3.begin(38400); //LoRa use 
  Serial.println("Lora Slave test!!!");
  Serial3 << "LoraMode" << " " << "SLAVE" << "\r";   //initial the module

}

//GetMsg from Master Node  
void getMsg(){
  static unsigned long TT = millis();
  char c;
  String s = "", Msg3;
  String Servo_String;
  while(Serial3.available()){
    c = Serial3.read();
        s += c;
    delay(3);
//    Serial.write(c);
    }
  if(s.length()>10){
    Msg = s;
    s="";
//    Serial.print("S.length: "); Serial.println(s.length());
//     Serial.print("s: ");Serial.println(s);
//     Serial.print("Msg: ");  Serial.println(Msg);
    String Msg2 = Msg.substring(39,43);
//   Serial.print("Msg2: "); Serial.println(Msg2);
    if(Msg2 == "Data"){
      Serial.println("OK");
      int Msg_Len = Msg.length();
      Msg3 = Msg.substring(44,48);    // Data message
      Decode_Msg = Msg.substring(48,Msg_Len-2);
       //Serial.print("Msg.length: "); Serial.println(Msg_Len-2);
//       Serial.print("Msg3: "); Serial.println(Msg3);
      }
     Msg2 = "";
     if(Msg3 == "ConR"){
        if(Check_Control(Msg.substring(48,56))){
            Relay_String = Msg.substring(48,56);
            Serial.print("Relay_String   --->"); Serial.println(Relay_String);
            Actuator_update(Relay_String);
        }
        else{
          Serial.println("Command String Error...");
        }
     }
     else if(Msg3 == "ConF"){
        Feed_mode(Msg.substring(48,52));
     }
     else if(Msg3 == "ConW"){
        Serial.println("Ready to Water In and Out !");
        digitalWrite(Actuator_pin[4],LOW);
        delay(5000);
        digitalWrite(Actuator_pin[4],HIGH);
     }
     else if(Msg3 == "MsgR"){
        Serial.println("Ready to send relay data");
        DataSend_Lora0();
     }
     else if(Msg3 == "MsgS"){
        Serial.println("Ready to send servo data");
        DataSend_Lora1();
     }
     else if(Msg3 == "RSTN"){
        Serial.println("Reset...");
        wdt_enable(WDTO_1S);
        delay(1100);
     }
  }
}
/* Value measure:
Temp_EC.var1_Temp
Temp_EC.var2_EC
PH_value
Get_TSL.var4_LUX
*/
//***********************************************************************
void DataSend_Lora0(){      // Relay Data
  String Lora_raw_data0;
  Lora_raw_data0 = "Data" + Relay_String;
  Serial.print("Uncode: "); Serial.print(Lora_raw_data0 );
  Lora_raw_data0 = encodingMsg(Lora_raw_data0);
  Serial.print(" , decode: "); Serial.println(Lora_raw_data0); 
  Serial3 << "LoraNodeData "<< Lora_raw_data0 <<"\r" ;   // Send Data
}
//***********************************************************************
void DataSend_Lora1(){       // Servo Data
  String Lora_raw_data1;
  feeder=servo_feeder.readMicroseconds();
  Lora_raw_data1 = "Acvo" + String(feeder);
  Serial.print("Uncode: "); Serial.print(Lora_raw_data1 );
  Lora_raw_data1 = encodingMsg(Lora_raw_data1);
  Serial.print(" , decode: "); Serial.println(Lora_raw_data1); 
  Serial3 << "LoraNodeData "<< Lora_raw_data1 <<"\r" ;   // Send Data

}
//***********************************************************************
