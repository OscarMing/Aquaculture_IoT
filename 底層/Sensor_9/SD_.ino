/*
 * Arduino / Genuino Board MOSI  MISO  SCK SS  Level
  Mega1280 or Mega2560     51    50    52  53  5V
 */
//****************************************************************************************************************************
void Initial_SD(){
  //Serial.println("Initializing the SD Card...");
 if(!SD.begin(CS_Pin)){
  //Serial.println("Initialization Failed !!!");
 }
 else{
  //Serial.println("Initialization complete !!!");
 }
}
//****************************************************************************************************************************
void write_to_SD(){
  struct ts t;
  String File_;
  DS3231_get(&t);
 myFile = SD.open("aquarium.txt", FILE_WRITE);       // .csv :excel 檔
  if(myFile){
    if(flag_date == 1){
      String T = String(t.year) + "/" + String(t.mon) + "/" + String(t.mday);
      myFile.print("Date:"); myFile.println(T); 
      flag_date = 0;
    }
    myFile.print("Time:");  myFile.println(timeString); 
    myFile.print("Temp:");  myFile.println(Temp_EC.var1_Temp); 
    myFile.print("EC:"); myFile.println(Temp_EC.var2_EC);
    myFile.print("PH:"); myFile.println(PH_value);
    myFile.print("IR:"); myFile.println( Get_TSL.var1_IR);
    myFile.print("Full:"); myFile.println( Get_TSL.var2_FULL);
    myFile.print("Visible:"); myFile.println( Get_TSL.var3_VISIBLE);
    myFile.print("Lux:"); myFile.println( Get_TSL.var4_LUX);
    
    myFile.println("");
    //Serial.println("Writing Successful!!!");
    myFile.close();
     }
  else{
       lcd.clear();
        Serial.println("Error in opening the file !!!");
        lcd.setCursor(0,0);
        lcd.print("SD Write_Err");
      }
}
//****************************************************************************************************************************
