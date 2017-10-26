//MASTER NODE v7.5
#include <Timer.h>
#include <Base64.h>
#include <LiquidCrystal_I2C.h>
//#include <SoftwareSerial.h>
#include <Streaming.h>
#include <Keypad.h>     //按鍵keypad程式庫
#include <avr/wdt.h>
// Msg: 每過一段時間會Lora 偵查， Raw value
// Msg2: Msg 中的 Data
// Msg3: Data後面的 Base64 碼
// Msg4: DataLength，資料長度
// Msg5: Msg 裡抓取 LoRa ID

int Critical_height=800;

//Keypad define*******************************************************************************************************************
  boolean flag_failure = false;
  const byte ROWS=4;    //定義4行
  const byte COLS=4;    //定義4列
  char keys[ROWS][COLS]={
    {'1','2','3','A'},    
    {'4','5','6','B'},
    {'7','8','9','C'},
    {'*','0','#','D'},
  };
  byte rowPins[ROWS] = {22,23,24,25};
  byte colPins[COLS] = {26,27,28,29};
 Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS);

  unsigned long feed_time = 120000; // ms
  unsigned long feed_last_time = millis();
  byte feed_mode = 2;    // sufficient feed mode

// Relay Control Pin:
boolean R_control[8] = {0,0,0,0,0,0,0,0};
int Servo_control[3] = {800,1000,2000};
int Feed = 500;
/*
 * R0: 110 AC 插座-1
 * R1: 110 AC 插座-2
 * R2: 110 AC 插座-3
 * R3: 110 AC 插座-4   --> 電磁閥
 * R4: 12 DC -1        --> 抽出水馬達
 * R5: 12 DC -2        --> 抽入水馬達
 * R6: NC
 * R7: NC
 */

//char* nodeID[] = {"4d0058","370059","440058","6d0059"};
char* nodeID[] = {"6d0059","4d0058"};    // 0: Sensor 端 , 1: Actuator 端
uint8_t charSize = sizeof(nodeID);
unsigned long lastTime = 0;
int debounceDelay=100;
int flag_send = 0;
boolean flag_on = 0;

//Define Timer
Timer sendCmd_clock;

//Define SoftwareSerial
//SoftwareSerial mySerial(10, 11);

//DLefine Variable
String Msg = "";
String Msg3 = "";
String Dcd = "";
String Sensor_Data="";

LiquidCrystal_I2C lcd(0x27, 16, 2);

float data_sensor[6];
boolean flag_sensor_mode = true;  // Initial Mode
boolean flag_water_mode = false;
boolean flag_relay_mode = false;
boolean flag_servo_mode = false;
boolean flag_feed_mode = false;

boolean manual_flag = false;    // 手動 / 自動模式切換


void setup() {
  Serial.begin(38400);
  Serial2.begin(38400);
  lcd.begin();    // initialize the LCD
  lcd.backlight();   // Turn on the blacklight and print a message
  initCMD();
  sendCmd_clock.every(20000,Send_flag);    // 每60秒命令Slave 送一筆資料
  //Serial.println(charSize);
  wdt_enable(WDTO_8S);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Welcome !!!");
  delay(1000);
  lcd.clear();
  lcd.print("Waiting Data...");
  pinMode(13,OUTPUT);
}
// 主程式碼:****************************************************************************************
void loop() {
  char key;
  byte show_mode;
  int func;
  int control_mode;
  float D;
  String Command = "";
  char a;
// Raspberry Pi Send Command ...
  while(Serial.available()){
    a = Serial.read();
    Command += a;
    delay(3);
  }
  if(Command.length() >= 5){
//    Serial.println(Command.length());
     Control_Commnand_Send(Command);      // Send command to actuator or sensor
  }
  
  if(manual_flag == false){   // manual mode or not
    sendCmd_clock.update();
      if(millis() - feed_last_time > feed_time){   // time for feed fish
        Feed_Fish(feed_mode);  // feed fish once
        feed_last_time = millis();
      }
  }
  getMsg();
  wdt_reset();
  if(flag_on==1){
    if(flag_send==0){
      sendCmd(1);
    }
     else if(flag_send==1){
       sendCmd(2);
    }
     else if(flag_send==2){
       sendCmd(3);
     }
     else if(flag_send==3){
       sendCmd(5);
     }
     else if(flag_send==4){
       sendCmd(6);
     }
     else if(flag_send==5){
       Serial.println(("Fmode:," + String(feed_mode)) + ",");
     }
    flag_on = 0;
  }

flag_failure = false;
key = keypad.getKey();
switch(key){
  case '0':
        lcd.clear(); lcd.print("Reset ...");
        wdt_enable(WDTO_1S);
        delay(1100);
  break;
  case '1':
        lcd.clear(); 
        lcd.setCursor(0,0); lcd.print("Get Servo Data"); 
        lcd.setCursor(0,1); lcd.print("Please Wait...");
        sendCmd(1);
  break;
  case '2':
        lcd.clear(); 
        lcd.setCursor(0,0); lcd.print("Water In..."); 
        lcd.setCursor(0,1); lcd.print("Please Wait...");
        sendCmd(2);
  break;
  case '3':
        lcd.clear(); 
        lcd.setCursor(0,0); lcd.print("Get Sensor Data"); 
        lcd.setCursor(0,1); lcd.print("Please Wait...");
        sendCmd(3);
  break;
  case '4':
        lcd.clear(); 
        lcd.setCursor(0,0); lcd.print("Water Out..."); 
        lcd.setCursor(0,1); lcd.print("Please Wait...");
        sendCmd(4);
  break;
  case '5':
        lcd.clear(); 
        lcd.setCursor(0,0); lcd.print("Get Relay Data"); 
        lcd.setCursor(0,1); lcd.print("Please Wait...");
        sendCmd(5);
  break;
  case '6':
        lcd.clear(); 
        lcd.setCursor(0,0); lcd.print("Get Feed Data"); 
        lcd.setCursor(0,1); lcd.print("Please Wait...");
        sendCmd(6);
  break;
  case '7':

  break;
  case '8':

  break;
  case '9':
        if(manual_flag == false){
          lcd.clear(); 
          lcd.setCursor(0,0); lcd.print("Manual Mode...");
          delay(1000);
          lcd.clear();
          manual_flag = true;
        }
        else{
          lcd.clear(); 
          lcd.setCursor(0,0); lcd.print("Auto Mode...");
          delay(1000);
          lcd.clear();
          manual_flag = false;
        }
  break;
  case 'A':

  break;
  case 'B':
        lcd.clear();  lcd.print("Enter key.....");
        lcd.setCursor(0,1);
        D = Enterkeypad(); 
        lcd.clear(); lcd.print("Enter key= "); lcd.print(D);
        delay(1000); lcd.clear();
  break;
  case 'C':

  break;
  case 'D':
        lcd.clear(); 
        lcd.setCursor(0,0); lcd.print("Show Mode...");
        lcd.setCursor(0,1); lcd.print("Enter Num 1~5");
        lcd.setCursor(14,1);
        show_mode = Enterkeypad();
        switch(show_mode){
          case 1:
                flag_sensor_mode = true;
                flag_water_mode = false;
                flag_relay_mode = false;
                flag_servo_mode = false;
                flag_feed_mode = false;
          break;
          case 2:
                flag_sensor_mode = false;
                flag_water_mode = true;
                flag_relay_mode = false;
                flag_servo_mode = false;
                flag_feed_mode = false;
          break;
          case 3:
                flag_sensor_mode = false;
                flag_water_mode = false;
                flag_relay_mode = true;
                flag_servo_mode = false;
                flag_feed_mode = false;
          break;
          case 4:
                flag_sensor_mode = false;
                flag_water_mode = false;
                flag_relay_mode = false;
                flag_servo_mode = true;
                flag_feed_mode = false;
          break;
          case 5:
                flag_sensor_mode = false;
                flag_water_mode = false;
                flag_relay_mode = false;
                flag_servo_mode = false;
                flag_feed_mode = true;
          break;
        }
        lcd.clear();
  break;
  case '*':
        lcd.clear(); 
        lcd.setCursor(0,0); lcd.print("Send Command...");
        lcd.setCursor(0,1); lcd.print("Num 1~11");
        lcd.setCursor(12,1);
        control_mode = Enterkeypad();
        lcd.clear();
        lcd.setCursor(0,0); lcd.print("Command "); lcd.print(control_mode);
        sendCmd(control_mode);
        lcd.setCursor(0,1); lcd.print("Send ...");
        delay(1000);
        lcd.clear();
  break;
  case '#':         // Setting Mode
        lcd.clear(); 
        lcd.setCursor(0,0); lcd.print("Setting");
        lcd.setCursor(0,1); lcd.print("Num 0~3");
        lcd.setCursor(12,1);
        control_mode = Enterkeypad();
        switch(control_mode){
          case 0:
                lcd.clear(); lcd.setCursor(0,0); lcd.print("1 --> Reset");
                lcd.setCursor(0,1);
                D = Enterkeypad();
                if(D == 1){
                  lcd.setCursor(8,1); lcd.print("OK!!!");
                  sendCmd(0);
                  delay(1000);
                }
                else{
                  lcd.clear();
                  lcd.setCursor(0,0); lcd.print("Error...");
                  delay(1000);
                }
                lcd.clear();
                
          break;
          case 1:
                lcd.clear(); lcd.setCursor(0,0); lcd.print("Feed per hour?");
                lcd.setCursor(0,1);
                D = Enterkeypad();
                feed_time = D*60*60*1000;
                lcd.setCursor(8,1); lcd.print("OK!!!");
                delay(1000);
                lcd.clear();
          break;
          case 2:
                lcd.clear(); lcd.setCursor(0,0); lcd.print("Feed Mode 0~3 ?");
                lcd.setCursor(0,1);
                func = Enterkeypad();
                if((func==0)||(func==1)||(func==2)||(func==3)){
                   feed_mode = func;
                   lcd.setCursor(8,1); lcd.print("OK!!!");
                   delay(1000);
                }
                else{
                  lcd.clear(); lcd.setCursor(0,0); lcd.print("FMode error...");
                  lcd.setCursor(0,1); lcd.print("Not found...");
                  delay(1000);
                }
                
                lcd.clear();
          break;
          case 3:
                lcd.clear(); 
                lcd.setCursor(0,0); lcd.print("Choose Relay ?"); 
                lcd.setCursor(0,1); lcd.print("1~8 ?");
                lcd.setCursor(8,1);
                control_mode = Enterkeypad();
                lcd.clear(); lcd.setCursor(0,0); lcd.print("On~1 , Off~0");
                lcd.setCursor(0,1);
                func = Enterkeypad();
                if((control_mode <=8) && (control_mode>=1) && ((func==1) || (func==0))){
                    R_control[(control_mode-1)] = func;
                    lcd.setCursor(5,1); lcd.print(" GO");
                    sendCmd(7);
                    delay(1000);
                }       
                else{
                  lcd.clear(); lcd.setCursor(0,0); lcd.print("Enter error...");
                  delay(1000);
                }
                lcd.clear();
          break;
          default:
                lcd.clear(); lcd.setCursor(0,0); lcd.print("Not found...");
                delay(1000);
                lcd.clear();
          break;
        }
  break;
}
}
//****************************************************************************************
//Initial Lora Network
//宣告為Gateway，同時將所需的節點加入群組
void initCMD(){
  Serial2 << "LoraMode" << " " << "MASTER" <<"\r" ;
  for(byte i=0; i<(charSize-2); i++){
    Serial2 << "LoraJoinNode" << " " << nodeID[i] <<"\r" ;
  }
  }
//****************************************************************************************
void Send_flag(){
  if(flag_send == 0){
    flag_send = 1;
    flag_on = 1;
  }
  else if(flag_send == 1){
    flag_send = 2;
    flag_on = 1;
  }
  else if(flag_send == 2){
    flag_send = 3;
    flag_on = 1;
  }
  else if(flag_send == 3){
    flag_send = 4;
    flag_on = 1;
  }
  else if(flag_send == 4){
    flag_send = 5;
    flag_on = 1;
  }  
  else if(flag_send == 5){
    flag_send = 0;
    flag_on = 1;
  }  
}
//****************************************************************************************
void sendCmd(int C){
  int i;
  int k = 0;
  String Actuator_Send;
//  Serial.print("Send Command"); Serial.println(C);
  switch(C){
    case 0:
            if(timeDelay()){
                Serial2 << "LoraNodeData" << " " << nodeID[0] << " " << "ResetNow" << "\r";      // Reset Sensor
              }
             Serial2.flush();
            if(timeDelay()){
                Serial2 << "LoraNodeData" << " " << nodeID[1] << " " << "RSTN0000XXXX" << "\r";    // Reset Actuator
              }
             Serial2.flush();
    break;
    case 1:
            if(timeDelay()){
                Serial2 << "LoraNodeData" << " " << nodeID[0] << " " << "ServoMsg" << "\r";
              }
             Serial2.flush();
    break;
    case 2:
            Actuator_Send = "Servo111" + Actuator_Send;      // Control Servo into the water
            if(timeDelay()){
                Serial2 << "LoraNodeData" << " " << nodeID[0] << " " << Actuator_Send << "\r";
              }
             Serial2.flush();
    break;
    case 3:
           while(k < 3){      // 最後多送一筆，讓Slave端能及時送資料回來
              if(timeDelay()){
                Serial2 << "LoraNodeData" << " " << nodeID[0] << " " << "DataMsg" << k << "\r";   // 0: Sensor 端
                k++;
              }
            }
          k=0;
          Serial2.flush();
    break;
    case 4:
            Actuator_Send = "Servo000";      // out of water
            if(timeDelay()){
                Serial2 << "LoraNodeData" << " " << nodeID[0] << " " << Actuator_Send << "\r";
              }
             Serial2.flush();
    break;
    case 5:
            if(timeDelay()){
                Serial2 << "LoraNodeData" << " " << nodeID[1] << " " << "MsgR12345678" << "\r";
              }
             Serial2.flush();
    break;
    case 6:
            if(timeDelay()){
                Serial2 << "LoraNodeData" << " " << nodeID[1] << " " << "MsgS12345678" << "\r";
              }
            if(timeDelay()){
                Serial2 << "LoraNodeData" << " " << nodeID[1] << " " << "HelloWorld!" << "\r";
              }
             Serial2.flush();
    break;
    case 7:
            for(i=0;i<8;i++){
              Actuator_Send += String(R_control[i]);
            }
            //Actuator_Send =  encodingMsg(Actuator_Send);
            Actuator_Send = "ConR" + Actuator_Send;
            if(timeDelay()){
                Serial2 << "LoraNodeData" << " " << nodeID[1] << " " << Actuator_Send << "\r";
              }
             Serial2.flush();
    break;
    case 8:
            if(timeDelay()){
                Serial2 << "LoraNodeData" << " " << nodeID[1] << " " << "ConF1111XXXX" << "\r";
              }
             Serial2.flush();
    break;
    case 9:
            if(timeDelay()){
                Serial2 << "LoraNodeData" << " " << nodeID[1] << " " << "ConF2222XXXX" << "\r";
              }
             Serial2.flush();
    break;
    case 10:
            if(timeDelay()){
                Serial2 << "LoraNodeData" << " " << nodeID[1] << " " << "ConF3333XXXX" << "\r";
              }
             Serial2.flush();
    break;
    case 11:
            if(timeDelay()){
                Serial2 << "LoraNodeData" << " " << nodeID[1] << " " << "ConF0000XXXX" << "\r";
              }
             Serial2.flush();
    break;
    case 12:
            if(timeDelay()){
                Serial2 << "LoraNodeData" << " " << nodeID[1] << " " << "ConW1111XXXX" << "\r";
              }
             Serial2.flush();
    break;
  }
}
//****************************************************************************************
boolean timeDelay(){
  if((millis()-lastTime) > debounceDelay){
    lastTime = millis();
    return true;
    }else{
      return false;
      }
  }
//****************************************************************************************
void getMsg(){
  //Serial.flush();
  char c;
  String s = "";
  while(Serial2.available()){
    c = Serial2.read();
    s += c;
    delay(2);
//   Serial.write(c);
    }
  Serial.flush();
  if(s.length()>9){
    Msg = s;
    //Serial.print(s.length());
    //Serial.println(s);
//    Serial.print("Msg ");
//    Serial.println(Msg);
    s="";
    if(timeDelay()){  
    strMsg2();
    }
    }
   else if(s.length()<9){
      s="";
      Serial.flush();
      }
  }
//****************************************************************************************
void strMsg2(){
  String Msg2 = Msg.substring(28,32);
//  Serial.println("Msg");
//  Serial.println(Msg);
//  Serial.println("Msg2");
//  Serial.println(Msg2);
  if(Msg2 == "Data"){

    int Msg_Len = Msg.length();
    Msg3 = Msg.substring(33,Msg_Len-2);//"-2"=>消除/r/n
//    Serial.println("Msg3");
//    Serial.println(Msg3);
    }
  String Msg4 = Msg.substring(24,26);

//  Serial.println(Msg.length());
//  Serial.println(Msg);
//  Serial.println(Msg3);
//  Serial << "Count1=" << count1 << "\n" ;
//  Serial << "Count2=" << count2 << "\n";
//  Serial << "Count3=" << count3 << "\n";
//  Serial << "Count4=" << count4 << "\n";
//  Serial.println(Msg4);
  Msg2 = "";
  decodingMsg();
//  Serial.println(Dcd);
//  String Msg5 = Msg.substring(5,11);
////  Serial.print("Msg5: ");Serial.println(Msg5);
//  if(Msg4=="16"){
//    count1++; 
//    }else if(Msg4=="20"&& Msg5=="370059"){
//      count2++;
//      }else if(Msg4=="20"&& Msg5=="440058"){
//        count3++;
//        }else if(Msg4=="20"&& Msg5=="6d0059"){
//          count4++;
//        }
  Dcd = "";
  }
//**********************************************************
void decodingMsg(){
  String Relay_Str, Servo_Str;
  //Serial.println("Decoding....");
  //strMsg2();
  //Serial.print("Msg3:"); Serial.println(Msg3);
  //Serial.print("Msg3L_Len:"); Serial.println(Msg3.length());
  //Msg3.length()=20，要放進去char array 要多一個空間，因此才會+1
  //Msg3_Len = 21
  int Msg3_Len = Msg3.length()+1;
  char input[Msg3_Len];
  //字串轉成char array
  Msg3.toCharArray(input,Msg3_Len);
  //int inputLen = sizeof(input)-1;
  //Serial.println("inputLen:");Serial.println(inputLen);
  
  int decodedLen = base64_dec_len(input, Msg3_Len);
  char decoded[decodedLen];
  base64_decode(decoded, input, Msg3_Len);  
  Dcd = decoded;
//  Serial.print("Dcd:  ");
//  Serial.println(Dcd);
  
  if(Dcd.substring(0,2) == "Tp"){
    Sensor_Data = Dcd;
  }
  else if(Dcd.substring(0,2) == "pH"){
    Sensor_Data = Sensor_Data + "," + Dcd + ",";
    Str2Mat_Sensor(Sensor_Data);
    Water_Judge(data_sensor[2]);   // Water height data
    Serial.println(Sensor_Data);
    Sensor_Data = "";
  }
  else if(Dcd.substring(0,4) == "Data"){
    Str2Mat_Relay(Dcd.substring(4,12));   // 將String 轉成byte的形式
    Relay_Str = "Relay:," + Dcd.substring(4,12) + ",";
    Serial.println(Relay_Str);
  }
  else if(Dcd.substring(0,4) == "Serv"){
    Str2Mat_Servo(Dcd.substring(4,Dcd.length()));
    Servo_Str = "Servo:," + Dcd.substring(4,Dcd.length());
    Serial.println(Servo_Str);
  }
  else if(Dcd.substring(0,4) == "Acvo"){
    Str2Int_Feed(Dcd.substring(4,Dcd.length()));
    Serial.println("Feed:," + Dcd.substring(4,Dcd.length()) + ",");
    Serial.println(Feed);
  }
  show_data_lcd();
  Msg3="";
  Serial2.flush();
}
//***********************************************************************
//Data Encoding
String encodingMsg(String data_to_base64){
  int NN_Len = data_to_base64.length()+1;
  char input[NN_Len];
  data_to_base64.toCharArray(input,NN_Len);   //String To Char
  int inputLen = sizeof(input)-1; 
  int encodedLen = base64_enc_len(inputLen);
  char encoded[encodedLen];
  base64_encode(encoded, input, inputLen); 
  //Serial.println(encoded);
  return encoded;
  }

//**********************************************************
float Enterkeypad(){                                      //利用while迴圈和按鍵，輸入一碼或二碼或三碼，以得到input
  int i=0, c = 0, K[3], neg = 1;
  float n = 0, s = 1;
  K[0]  = 0;  K[1]  = 0;  K[2]  = 0;  K[3]  = 0;
  while(i<100){
  wdt_reset();    // WatchDog 
  K[i]  = keypad.getKey();
  if(K[i] ==  49) {K[i] = 1; lcd.print(K[i]);i++;}  
  else if(K[i] ==  50) {K[i] = 2; lcd.print(K[i]); i++;}      
  else if(K[i] ==  51) {K[i] = 3; lcd.print(K[i]); i++;}      
  else if(K[i] ==  52) {K[i] = 4; lcd.print(K[i]); i++;}      
  else if(K[i] ==  53) {K[i] = 5; lcd.print(K[i]); i++;}      
  else if(K[i] ==  54) {K[i] = 6; lcd.print(K[i]); i++;}      
  else if(K[i] ==  55) {K[i] = 7; lcd.print(K[i]); i++;}      
  else if(K[i] ==  56) {K[i] = 8; lcd.print(K[i]); i++;}      
  else if(K[i] ==  57) {K[i] = 9; lcd.print(K[i]); i++;}      
  else if(K[i] ==  48) {K[i] = 0; lcd.print(K[i]); i++;}
  else if(K[i] ==  35) {lcd.clear(); lcd.print("Restart"); delay(1000); lcd.clear(); i=0; n = 0, s = 1; c = 0; neg = 1; lcd.setCursor(0,1);}      //  # : Restart
  else if(K[i] ==  66) {lcd.clear(); lcd.setCursor(0,1); for(int j=0;j<i-1;j++) {lcd.print(K[j]);}i--; if(i<0) {i=0;}}                 //  B : Delete
  else if(K[i] ==  67) {lcd.clear(); for(int j=0;j<i;j++) {K[j] = 0;} break;}                             //  C : Break; 直接跳離迴圈
  else if(K[i] ==  68) {neg = -1; lcd.print("-");}                            //  D : 負號
  else if(K[i] ==  42) {if(i == 1) {c = 1;} else if(i == 2) {c = 2;} lcd.print(".");}                            //  * : .(小數點)
  else if(K[i] ==  65 && i == 0) {lcd.clear(); lcd.print("error...");  lcd.setCursor(0,1); lcd.print("Restart"); delay(1000); lcd.clear(); i=0; lcd.setCursor(0,1);}    
  else if(K[i] ==  65 && i == 1) {K[2] = K[0]; K[1] = 0; K[0] = 0; break;}             //輸入一碼
  else if(K[i] ==  65 && i == 2) {K[2] = K[1]; K[1] = K[0]; K[0] = 0; if(c == 1) {s = 0.1;} break;}          //輸入二碼
  else if(K[i] ==  65 && i == 3) {lcd.clear(); if(c == 1) {s = 0.01;} else if(c == 2) {s =0.1;} break;}        // A  : Enter            //輸入三碼
  else{K[i]  = 0;}          
  //Serial.println(i);     
  } 
  n = neg * (100*K[0]  + 10*K[1] + 1*K[2]) * s;
  //Serial.println(n);
  return n;
}
//**********************************************************
void emergency(){                 //緊急按鈕
  char key;
  int F;
  key = keypad.getKey();
  if(key == '#') {
                  lcd.clear(); lcd.setCursor(0,0); lcd.print("1-->break"); lcd.setCursor(0,15);
                               lcd.setCursor(0,1); lcd.print("0-->continue.");
                  F = Enterkeypad();
                   switch(F){
                    case 1:
                     flag_failure = true; 
                     lcd.clear();  
                    break;                 
                    case 0:
                      lcd.clear();
                    break;
                    default:
                       lcd.clear(); lcd.setCursor(0,0); lcd.print("Error......."); delay(1000);
                       lcd.clear();
                    break;
                    }
  }
}
//**********************************************************
void Feed_Fish(byte mode){
  if(mode == 1){
    sendCmd(8);
    Serial.println("Fmode:,1,");
  }
  else if(mode == 2){
    Serial.println("Fmode:,2,");
    sendCmd(9);
  }
  else if(mode == 3){
    Serial.println("Fmode:,3,");
    sendCmd(10);
  }
  else{
    Serial.println("Feed Fish mode Error");
  }
}
//****************************************************************************************
