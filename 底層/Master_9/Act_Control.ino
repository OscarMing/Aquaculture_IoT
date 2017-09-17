void Str2Mat_Sensor(String Data_in) {
  int String_Len,order=0, order2=0, num=0;
  String da, out, out2;
  String da_mat[3];    // 整數
  String da_mat2[2];    // 小數點
  float Data_S[6];
  boolean flag = true;   // 更新資料旗標
  boolean flag_float = false;   // 小數點旗標
  String_Len = Data_in.length();
  for(int i=0;i<String_Len;i++){
    da = Data_in.substring(i,i+1);
    //Serial.println(da);
    if(da == "."){
       flag_float = true;
    }
    else if(da != ","){
        if(flag_float != true){
            if(isNum(da)){    // 整數部分
                da_mat[order] = da;
                order += 1;
            }
        }
        else{
            if(isNum(da)){    // 小數部分
                da_mat2[order2] = da;
                order2 += 1;
            }
        }
    }
    
    else if(da == ","){
      for(int j=0;j<order;j++){     // 將數字的 String連結在一起
          out += da_mat[j];
          da_mat[j] = "0";  // 清除陣列資料
      }
      for(int t=0;t<order2;t++){
          out2 += da_mat2[t];
          da_mat2[t] = "0";
      }
      //Serial.println(out);
      Data_S[num] = out.toInt() + 0.01*(out2.toInt());      // 將數字的 String轉成 int
      num += 1;
      out = "";
      out2 = "";
      order = 0;
      order2 = 0;
      flag_float = false;
    }
  }
   if(num != 6){
       flag = false;
       Serial.println("Sensor Data Length Error...");
    }
  if(flag == true){
      for(int j=0;j<num;j++){
        data_sensor[j] = Data_S[j];
//        Serial.println(data_sensor[j]);
      }
  }
}
//****************************************************************************************
void Str2Mat_Relay(String Data_in) {
  int String_Len;
  String da;
  boolean out[8];
  boolean flag = true;   // 更新資料旗標
  String_Len = Data_in.length();
  if(String_Len != 8){
    flag = false;
    Serial.println("Relay Data Length Error...");
  }
  for(int i=0;i<String_Len;i++){
    da = Data_in.substring(i,i+1);
    //Serial.println(da);
    if(da=="0"){
      out[i] = 0;
    }
    else if(da=="1"){
      out[i] = 1;
    }
    else{
      flag = false;
      Serial.println("Relay Data Error...");
    }
  }

  if(flag == true){
    for(int i=0;i<8;i++){
      R_control[i] = out[i];
//      Serial.println(R_control[i]);
    }
  }
}
//****************************************************************************************
void Str2Mat_Servo(String Angle_in) {
  int String_Len, order=0, num=0;
  int angle[3];
  String da, out;
  String da_mat[4]; 
  boolean flag = true;
  String_Len = Angle_in.length();
  for(int i=0;i<String_Len;i++){
    da = Angle_in.substring(i,i+1);
//    Serial.println(da);
    if((da != ",") && (da != ":")){
      if(isNum(da)){
        da_mat[order] = da;
        order += 1;
      }
    }
    else if(da == "," && (order>=3)){
      for(int j=0;j<order;j++){     // 將數字的 String連結在一起
          out += da_mat[j];
          da_mat[j] = "0";  // 清除陣列資料
      }
      //Serial.println(out);
      angle[num] = out.toInt();      // 將數字的 String轉成 int
//      Serial.println(angle[num]);
      if(angle[num] > 2500 || angle[num] < 500){
        flag = false;
        Serial.println("Exceed Integer Range Error ...");
      }
      num += 1;
      out = "";
      order = 0;
    }
  }
  if(num != 3){
    flag = false;
    Serial.println("Servo Data Length Error...");
  }
  if(flag == true){
    for(int i;i<num;i++){
      Servo_control[i] = angle[i];
//      Serial.println(Servo_control[i]);
    }
  }
}
//****************************************************************************************
void Str2Int_Feed(String DataIn){
  int String_Len, order=0;
  String da,out;
  String da_mat[4]; 
  String_Len = DataIn.length();
  //Serial.print("DataIn:  "); Serial.println(DataIn);
  for(int i=0;i<String_Len;i++){
    da = DataIn.substring(i,i+1);
    //Serial.println(da);
    if((da != ",") && (da != ":")){
      if(isNum(da)){
        da_mat[order] = da;
        //Serial.println(da_mat[order]);
        order += 1;
      }
    }
   }
      for(int j=0;j<order;j++){
         out += da_mat[j];
         da_mat[j] = "0";    
      }

      if((out.toInt()>=500) && (out.toInt()<=2500)){
         Feed = out.toInt();
      }
      else{
         Serial.println("Feed Servo Data Error...");
      }
}
//****************************************************************************************
boolean isNum(String n){
  if((n=="0")||(n=="1")||(n=="2")||(n=="3")||(n=="4")||(n=="5")||(n=="6")||(n=="7")||(n=="8")||(n=="9")){
    return true;
  }
  else{
    return false;
  }
}
//****************************************************************************************
void show_data_lcd(){
  if(flag_sensor_mode == true){
     lcd.clear();
     lcd.setCursor(0,0); lcd.print("Tp:"); lcd.print(data_sensor[0]);
     lcd.setCursor(9,0); lcd.print("EC:"); lcd.print(data_sensor[1]);
     lcd.setCursor(0,1); lcd.print("pH:"); lcd.print(data_sensor[3]);
     lcd.setCursor(9,1); lcd.print("Lx:"); lcd.print(data_sensor[4]);

  }
  else if(flag_water_mode == true){
      lcd.clear();
      lcd.setCursor(0,0); lcd.print("Water1: "); lcd.print(data_sensor[2]);
      lcd.setCursor(0,1); lcd.print("Water2: "); lcd.print(data_sensor[5]);
  }
  else if(flag_relay_mode == true){
      lcd.clear();
      lcd.setCursor(0,0); lcd.print("R1~4 "); lcd.print(R_control[0]); lcd.print(R_control[1]); lcd.print(R_control[2]); lcd.print(R_control[3]);
      lcd.setCursor(0,1); lcd.print("R5~8 "); lcd.print(R_control[4]); lcd.print(R_control[5]); lcd.print(R_control[6]); lcd.print(R_control[7]);
  }
  else if(flag_servo_mode == true){
      lcd.clear();
      lcd.setCursor(0,0); lcd.print("S1:"); lcd.print(Servo_control[0]);
      lcd.setCursor(8,0); lcd.print("S2:"); lcd.print(Servo_control[1]);
      lcd.setCursor(0,1); lcd.print("S3:"); lcd.print(Servo_control[2]);
  }
  else if(flag_feed_mode == true){
      lcd.clear();
      lcd.setCursor(0,0); lcd.print("Fmode: "); lcd.print(feed_mode);
      lcd.setCursor(0,1); lcd.print("FServo: "); lcd.print(Feed);
  }
}
//****************************************************************************************
void Water_Judge(float dataIn){
  if(dataIn >= 800){
    delay(1000);
    sendCmd(12);
  }
}
//****************************************************************************************
