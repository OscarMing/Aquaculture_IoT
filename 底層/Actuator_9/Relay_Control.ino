// All Control Relay define:****************************************************************************************************************
//const int relay110_1 = 22;
//const int relay110_2 = 23;
//const int relay110_3 = 24;
//const int relay110_4 = 25;    // 電磁閥的電
//const int pump_relay1 = 26;
//const int pump_relay2 = 27;
//const int Relay_7 = 28;
//const int Relay_8 = 30;

void Actuator_Setup(){     // All off
  for(int i=0; i<8; i++){
    pinMode(Actuator_pin[i], OUTPUT);
  }
  Actuator_update(Relay_String);
}

void Actuator_update(String Act){
  String R;
  for(int i=0;i<8;i++){
    R = Act.substring(i,i+1);
//    Serial.println(R);
    if(R == "0"){
      if((i==4) || (i==5) || (i==6) || (i==7)){    // Relay 5~8 低準位觸發
        digitalWrite(Actuator_pin[i], HIGH);
      }
       else{
        digitalWrite(Actuator_pin[i], LOW);
       }
    }
    else if(R == "1"){
      if((i==4) || (i==5) || (i==6) || (i==7)){
        digitalWrite(Actuator_pin[i], LOW);
      }
      else{
        digitalWrite(Actuator_pin[i], HIGH);
      }
    }
  }
}

boolean Check_Control(String check){
  String R;
  for(int i=0;i<8;i++){
    R = check.substring(i,i+1);
    if((R!="0") && (R!="1")){
      return false;
    }
  }
  return true;
}



