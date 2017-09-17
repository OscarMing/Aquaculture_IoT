void Feed_Setup() {
 servo_feeder.attach(8,500,2500);    // Set servo to digital pin 8, min, max
 digitalWrite(Actuator_pin[6],LOW);
 feeder=servo_feeder.readMicroseconds();
 for(int i=feeder;i>500;i-=10){
    servo_feeder.writeMicroseconds(i);
    delay(10); 
  }
  digitalWrite(Actuator_pin[6],HIGH);
}

void Feed_mode(String Case) {                       // Loop through motion tests
  if(Case == "1111"){
         Serial.println(" Start a mode for giving a lot of fish food. "); // need test how much time 
         Start_a_lot_of_Feeder();
         digitalWrite(Actuator_pin[6],LOW);  //open 7th Rlay  Actuator_pin[6]
         delay(5000);   
         Close_a_lot_of_Feeder();
         digitalWrite(Actuator_pin[6],HIGH);
  }
  else if(Case == "2222"){
         Serial.println(" Start a mode for giving sufficient fish food. "); // need test how much time 
         Start_sufficient_Feeder();
         digitalWrite(Actuator_pin[6],LOW);  //open 7th Rlay  Actuator_pin[6]
         delay(5000);
         Close_sufficient_Feeder();
         digitalWrite(Actuator_pin[6],HIGH);
  }
  else if(Case == "3333"){
         Serial.println(" Start a mode for giving a little of fish food. "); // need test how much time 
         Start_a_little_of_Feeder();
         digitalWrite(Actuator_pin[6],LOW);  //open 7th Rlay  Actuator_pin[6]
         delay(5000);
         Close_a_little_of_Feeder();
         digitalWrite(Actuator_pin[6],HIGH);
  }
  else if(Case == "0000"){
         Serial.println("Initial Position ..."); 
         feeder=servo_feeder.readMicroseconds();
           for(int i=feeder;i>500;i-=10){
              servo_feeder.writeMicroseconds(i);
              delay(10); 
            }
         digitalWrite(Actuator_pin[6],HIGH);
  }
  else{
    Serial.println("No Feed Command Found");  
  }
}

void Start_a_lot_of_Feeder() {
    
    for(int i = 500; i <= 1700; i+=10){
    servo_feeder.writeMicroseconds(i);     // 直接以脈衝寬度控制 
    feeder=servo_feeder.readMicroseconds();
//    Serial.println(feeder);
    delay(10);
  } 
   
}

void Close_a_lot_of_Feeder() {
  
    for(int i = 1700; i >= 500; i-=10){
    servo_feeder.writeMicroseconds(i);     // 直接以脈衝寬度控制 
    feeder=servo_feeder.readMicroseconds();
//    Serial.println(feeder);
    delay(10);
  }
  
}

void Start_sufficient_Feeder() {
    
    for(int i = 500; i <= 1100; i+=10){
    servo_feeder.writeMicroseconds(i);      // 直接以脈衝寬度控制 
    feeder=servo_feeder.readMicroseconds();
//    Serial.println(feeder);
    delay(10);
  } 
   
}

void Close_sufficient_Feeder() {
  
    for(int i = 1100; i >= 500; i-=10){
    servo_feeder.writeMicroseconds(i);      // 直接以脈衝寬度控制 
    feeder=servo_feeder.readMicroseconds();
//    Serial.println(feeder);
    delay(10);
  }
  
}
void Start_a_little_of_Feeder() {
    
    for(int i = 500; i <= 800; i+=10){
    servo_feeder.writeMicroseconds(i);      // 直接以脈衝寬度控制 
    feeder=servo_feeder.readMicroseconds();
//    Serial.println(feeder);
    delay(10);
  } 
   
}

void Close_a_little_of_Feeder() {
  
    for(int i = 800; i >= 500; i-=10){
    servo_feeder.writeMicroseconds(i);      // 直接以脈衝寬度控制 
    feeder=servo_feeder.readMicroseconds();
//    Serial.println(feeder);
    delay(10);
  }
  
}

