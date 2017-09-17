void Clock_setup()
{
    Wire.begin();
    DS3231_init(DS3231_INTCN);
    memset(recv, 0, BUFF_MAX);
    //Serial.println("GET time");
//    parse_cmd("T002121113082017",16);   //設定初始時間
//    Serial.println("Set time OK");
}

void GetClock(int CaseT)
{
    struct ts t;
    static int fff = 0;
    static int f_t = 0;
    static long past = 0;
    String T;
    unsigned long nf,time_dur;
    // show time once in a while
        DS3231_get(&t);
//        Serial.print(t.mday); Serial.print("  "); Serial.print(t.hour); Serial.print(":");Serial.print(t.min); Serial.print(":");Serial.println(t.sec);
        if(CaseT == 0) {Initial_time[0] = t.mday;
                        Initial_time[1] = t.hour;
                        Initial_time[2] = t.min;
                        Initial_time[3] = t.sec;
                        timeString = String(t.hour) + ":" + String(t.min) + ":" + String(t.sec);}
        else if(CaseT == 1){
          nf = millis();
          time_dur = nf - ti;
          timeString =  String(t.hour) + ":" + String(t.min) + ":" + String(t.sec);
          
            if(t.hour == 0 && f_t == 0){
             T = String(t.year) + "/" + String(t.mon) + ":" + String(t.mday);
             flag_date = 1;
             f_t = 1;
              }
             else if(t.hour == 1){
               f_t = 0;
              }
              
            if((t.min % 10 == 0) && fff == 0){
              write_to_SD();
              lcd.clear();
              fff = 1;
             }     
           else if(t.min % 10 != 0){
            fff = 0;
          }
        }

  // Duration
  /*
  Duration = (t.mday - Initial_time[0])*24*60*60 
             +(t.hour - Initial_time[1])*60*60
             +(t.min - Initial_time[2])*60
             +(t.sec - Initial_time[3]);
            //Serial.print("      "); Serial.println(Duration);
  if(Duration > Testing_Time){ write_to_SD();
                               GetClock(0);
                               lcd.clear();}
  */ 
    
    if(millis() - past > Testing_Time && Temp_EC.var1_Temp != 0){
    Serial.print("Time:");Serial.print(timeString); Serial.print(",");
    Serial.print("Temp:");Serial.print(Temp_EC.var1_Temp); Serial.print(",");
    Serial.print("EC:");  Serial.print(Temp_EC.var2_EC);Serial.print(",");
    Serial.print("pH:"); Serial.print(PH_value);Serial.print(",");
    Serial.print("IR:"); Serial.print(Get_TSL.var1_IR);Serial.print(",");
    Serial.print("FULL:"); Serial.print(Get_TSL.var2_FULL);Serial.print(",");
    Serial.print("VISIBLE:"); Serial.print(Get_TSL.var3_VISIBLE);Serial.print(",");
    Serial.print("LUX:"); Serial.print(Get_TSL.var4_LUX);
    if(flag_date == 1){
      T = String(t.year) + "/" + String(t.mon) + "/" + String(t.mday);
      Serial.print(","); Serial.println(T); 
      flag_date = 0;
    }
    else{
      Serial.println(""); 
    }
    past = millis();
}                     
}

void parse_cmd(char *cmd, int cmdsize)
{
    uint8_t i;
    uint8_t reg_val;
    char buff[BUFF_MAX];
    struct ts t;

    //snprintf(buff, BUFF_MAX, "cmd was '%s' %d\n", cmd, cmdsize);
    //Serial.print(buff);

    // TssmmhhWDDMMYYYY aka set time
    if (cmd[0] == 84 && cmdsize == 16) {
        //T355720619112011
        t.sec = inp2toi(cmd, 1);
        t.min = inp2toi(cmd, 3);
        t.hour = inp2toi(cmd, 5);
        t.wday = cmd[7] - 48;
        t.mday = inp2toi(cmd, 8);
        t.mon = inp2toi(cmd, 10);
        t.year = inp2toi(cmd, 12) * 100 + inp2toi(cmd, 14);
        DS3231_set(t);
        Serial.println("OK");
    } else if (cmd[0] == 49 && cmdsize == 1) {  // "1" get alarm 1
        DS3231_get_a1(&buff[0], 59);
        Serial.println(buff);
    } else if (cmd[0] == 50 && cmdsize == 1) {  // "2" get alarm 1
        DS3231_get_a2(&buff[0], 59);
        Serial.println(buff);
    } else if (cmd[0] == 51 && cmdsize == 1) {  // "3" get aging register
        Serial.print("aging reg is ");
        Serial.println(DS3231_get_aging(), DEC);
    } else if (cmd[0] == 65 && cmdsize == 9) {  // "A" set alarm 1
        DS3231_set_creg(DS3231_INTCN | DS3231_A1IE);
        //ASSMMHHDD
        for (i = 0; i < 4; i++) {
            time[i] = (cmd[2 * i + 1] - 48) * 10 + cmd[2 * i + 2] - 48; // ss, mm, hh, dd
        }
        uint8_t flags[5] = { 0, 0, 0, 0, 0 };
        DS3231_set_a1(time[0], time[1], time[2], time[3], flags);
        DS3231_get_a1(&buff[0], 59);
        Serial.println(buff);
    } else if (cmd[0] == 66 && cmdsize == 7) {  // "B" Set Alarm 2
        DS3231_set_creg(DS3231_INTCN | DS3231_A2IE);
        //BMMHHDD
        for (i = 0; i < 4; i++) {
            time[i] = (cmd[2 * i + 1] - 48) * 10 + cmd[2 * i + 2] - 48; // mm, hh, dd
        }
        uint8_t flags[5] = { 0, 0, 0, 0 };
        DS3231_set_a2(time[0], time[1], time[2], flags);
        DS3231_get_a2(&buff[0], 59);
        Serial.println(buff);
    } else if (cmd[0] == 67 && cmdsize == 1) {  // "C" - get temperature register
        Serial.print("temperature reg is ");
        Serial.println(DS3231_get_treg(), DEC);
    } else if (cmd[0] == 68 && cmdsize == 1) {  // "D" - reset status register alarm flags
        reg_val = DS3231_get_sreg();
        reg_val &= B11111100;
        DS3231_set_sreg(reg_val);
    } else if (cmd[0] == 70 && cmdsize == 1) {  // "F" - custom fct
        reg_val = DS3231_get_addr(0x5);
        Serial.print("orig ");
        Serial.print(reg_val,DEC);
        Serial.print("month is ");
        Serial.println(bcdtodec(reg_val & 0x1F),DEC);
    } else if (cmd[0] == 71 && cmdsize == 1) {  // "G" - set aging status register
        DS3231_set_aging(0);
    } else if (cmd[0] == 83 && cmdsize == 1) {  // "S" - get status register
        Serial.print("status reg is ");
        Serial.println(DS3231_get_sreg(), DEC);
    } else {
        Serial.print("unknown command prefix ");
        Serial.println(cmd[0]);
        Serial.println(cmd[0], DEC);
    }
}
