// base --> Servo 1
// shoulder --> Servo 2
// forearm --> Servo 3
void servo_setup(){
servo_base.attach(9,500,2500);         // Set servo to digital pin 9, min, max
servo_shoulder.attach(10,500,2500);    // Set servo to digital pin 10, min, max
servo_forearm.attach(11,500,2500);     // Set servo to digital pin 11, min, max           
servo_base.writeMicroseconds(500);      
servo_shoulder.writeMicroseconds(2500);
servo_forearm.writeMicroseconds(500) ;     
delay(1000);   
}
void Start() {
  Serial.println("Start .....");
  int j = 1450;
  for(int i = 1550; i <= 2500; i+=10){
    servo_shoulder.writeMicroseconds(i); // 直接以脈衝寬度控制 
    j -= 10;
    servo_forearm.writeMicroseconds(j);
//    Serial.println(shoulder);
    delay(10);
  }

  flag_servo = false;
}

void IntoTheWater() {  
  Serial.println("In to the water .....");
  int j = 500;
   for(int i = 2500; i >= 1550; i-=10){
    servo_shoulder.writeMicroseconds(i); // 直接以脈衝寬度控制 
    j += 10;
    servo_forearm.writeMicroseconds(j);
//    Serial.println(shoulder);
    delay(10);
   }
  flag_servo = true;
}
//*****************************************************************************************
void Str2Mat_Servo(String Angle_in) {     // String to int, to get servo data
  int String_Len, order=0, num=0;
  int angle[4];
  String da, out;
  String da_mat[3]; 
  boolean flag = true;
  String_Len = Angle_in.length();
  for(int i=0;i<String_Len;i++){
    da = Angle_in.substring(i,i+1);
    //Serial.println(da);
    if(da != ","){
        da_mat[order] = da;
        order += 1;
    }
    else if(da == ","){
      for(int j=0;j<order;j++){     // 將數字的 String連結在一起
          out += da_mat[j];
          da_mat[j] = "0";  // 清除陣列資料
      }
      //Serial.println(out);
      angle[num] = out.toInt();      // 將數字的 String轉成 int
      //Serial.println(angle[num]);
      if(angle[num] > 180 || angle[num] < 0){
        flag = false;
        Serial.println("Exceed Integer Range Error ...");
      }
      num += 1;
      out = "";
      order = 0;
    }
  }
  if(num != 4){
    flag = false;
    Serial.println("Number Angle Error ...");
  }
  if(flag == true){
    for(int i;i<4;i++){
      Servo_control[i] = angle[i];
    }
  }
  else{
    Serial.println("String to Integer Error ...");
  }
}


