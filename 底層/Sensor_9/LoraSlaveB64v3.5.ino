void Lora_Slave_setup() {
  Serial2.begin(38400); //LoRa use 
  Serial.println("Lora Slave test!!!");
  Serial2 << "LoraMode" << " " << "SLAVE" << "\r";   //initial the module
}

void Lora_Send() {
  getMsg();
}

//GetMsg from Master Node  
void getMsg(){
  static unsigned long TT = millis();
  char c;
  String s = "";
  while(Serial2.available()){
    c = Serial2.read();
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
    String Msg3="";
    String Msg2 = Msg.substring(38,42);
//  Serial.print("Msg2: "); Serial.println(Msg2);
    if(Msg2 == "Data"){
      Serial.println("OK");
      int Msg_Len = Msg.length();
      Msg3 = Msg.substring(43,Msg_Len-2);    // Data message
//       Serial.print("Msg.length: "); Serial.println(Msg_Len);
//       Serial.print("Msg3: "); Serial.println(Msg3);
      }
     Msg2 = "";
     if(Msg3 == "DataMsg0"){
      if(flag_servo == true){
          Serial.println("Ready to Send DataMsg0 !!!");
          DataSend_Lora0();
          Serial2.flush();
      }
      }
     else if(Msg3 == "DataMsg1"){
      if(flag_servo == true){
          Serial.println("Ready to Send DataMsg1 !!!");
          DataSend_Lora1();
          Serial2.flush();
          Start();
      }
      }
      else if(Msg3 == "Servo111"){
        Serial.println("Into the water !!!");
        IntoTheWater();
      }
      else if(Msg3 == "Servo000"){
        Serial.println("Out of the water");
        Start();
      }
      else if(Msg3 == "ServoMsg"){
        Serial.println("Servo Message to Send...");
        DataSend_Lora2();
      }
      else if(Msg3 == "ResetNow"){
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
void DataSend_Lora0(){
  Lora_raw_data0 = "Tp:" + String(Temp_EC.var1_Temp) + "," 
                + "E:" + String(Temp_EC.var2_EC) + ",S:" + String(water_value);
  Ecd = encodingMsg(Lora_raw_data0);
  Serial.print("Uncode: "); Serial.print(Lora_raw_data0);
  Serial.print(" , decode: "); Serial.println(Ecd); 
  Serial2 << "LoraNodeData "<< Ecd <<"\r" ;   // Send Data
  
  Ecd = "";
  Lora_raw_data0 = "";
}
//***********************************************************************
void DataSend_Lora1(){
  Lora_raw_data1 = "pH:" + String(PH_value) + "," 
                + "Lx:" + String(Get_TSL.var4_LUX) + ",T:" + "456" ;
                
  Ecd = encodingMsg(Lora_raw_data1);
  Serial.print("Uncode: "); Serial.print(Lora_raw_data1);
  Serial.print(" , decode: "); Serial.println(Ecd); 
  Serial2 << "LoraNodeData "<< Ecd <<"\r" ;   // Send Data
  Ecd = "";
  Lora_raw_data1 = "";
}
//***********************************************************************
void DataSend_Lora2(){
  String Lora_raw_data2;
  for(int i=0;i<3;i++){
    Lora_raw_data2 += String(Servo_control[i]) + ",";
  }
  Lora_raw_data2 = "Serv" + Lora_raw_data2;
  Ecd = encodingMsg(Lora_raw_data2);
  Serial.print("Uncode: "); Serial.print(Lora_raw_data2);
  Serial.print(" , decode: "); Serial.println(Ecd); 
  Serial2 << "LoraNodeData "<< Ecd <<"\r" ;   // Send Data

  Ecd = "";
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
//***********************************************************************
