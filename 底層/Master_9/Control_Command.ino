void Control_Commnand_Send(String command){
  // command 格式 --> L,1
  String head = command.substring(0,2);
  String check = command.substring(2,3);
  String formation = command.substring(3,4);
  //Serial.print(head); Serial.print("    "); Serial.println(formation);
    // 看head 和 formation下指令
    if(check == ","){
          if(head == "L1"){        // LED 燈 pin 13  test
                if(formation == "1"){
                  digitalWrite(13,HIGH);
                }
                else if(formation == "0"){
                  digitalWrite(13,LOW);
                }
          }
          else if(head == "R1"){
              if(formation == "1"){
                 R_control[0] = 1;
                 sendCmd(7);
              }
              else if(formation == "0"){
                 R_control[0] = 0;
                 sendCmd(7);
              }
          }
          else if(head == "R2"){
              if(formation == "1"){
                 R_control[1] = 1;
                 sendCmd(7);
              }
              else if(formation == "0"){
                 R_control[1] = 0;
                 sendCmd(7);
              }
          }
          else if(head == "R3"){
              if(formation == "1"){
                 R_control[2] = 1;
                 sendCmd(7);
              }
              else if(formation == "0"){
                 R_control[2] = 0;
                 sendCmd(7);
              }
          }
      
          else if(head == "R4"){
              if(formation == "1"){
                 R_control[3] = 1;
                 sendCmd(7);
              }
              else if(formation == "0"){
                 R_control[3] = 0;
                 sendCmd(7);
              }
          }
          else if(head == "R5"){
              if(formation == "1"){
                 R_control[4] = 1;
                 sendCmd(7);
              }
              else if(formation == "0"){
                 R_control[4] = 0;
                 sendCmd(7);
              }
          }
          else if(head == "R6"){
              if(formation == "1"){
                 R_control[5] = 1;
                 sendCmd(7);
              }
              else if(formation == "0"){
                 R_control[5] = 0;
                 sendCmd(7);
              }
          }
          else if(head == "R7"){
              if(formation == "1"){
                 R_control[6] = 1;
                 sendCmd(7);
              }
              else if(formation == "0"){
                 R_control[6] = 0;
                 sendCmd(7);
              }
          }
          else if(head == "R8"){
              if(formation == "1"){
                 R_control[7] = 1;
                 sendCmd(7);
              }
              else if(formation == "0"){
                 R_control[7] = 0;
                 sendCmd(7);
              }
          }

          else if(head == "RR"){   // All on or All off
              if(formation == "1"){
                for(int i=0;i<8;i++){
                  R_control[i] = 1;
                }
                 sendCmd(7);
              }
              else if(formation == "0"){
                for(int i=0;i<8;i++){
                  R_control[i] = 0;
                }
                 sendCmd(7);
              }
          }
      
          else if(head == "S1"){
              if(formation == "1"){   // into the water
                 sendCmd(2);
              }
              else if(formation == "0"){  // out of water
                 sendCmd(4);
              }
          }
      
          else if(head == "Fd"){    /// Change Feed mode
              if(formation == "0"){   // mode 2
                 sendCmd(11);
              }
              else if(formation == "1"){   // mode 1
                 feed_mode = 1;
                 sendCmd(8);
              }
              else if(formation == "2"){   // mode 2
                 feed_mode = 2;
                 sendCmd(9);
              }
              else if(formation == "3"){   // mode 2
                 feed_mode = 3;
                 sendCmd(10);
              }
              
          }
          
          else{
              Serial.println("No Command Found ...");
          }
    }
}

