// Msg: 每過一段時間會Lora 偵查， Raw value
// Msg2: Msg 中的 Data
// Msg3: Data後面的 Base64 碼
// Msg4: DataLength，資料長度
// Msg5: Msg 裡抓取 LoRa ID
//GetMsg from Master Node  

//**********************************************************
String decodingMsg(){
  //Serial.print("Msg3L_Len:"); Serial.println(Msg3.length());
  //Msg3.length()=20，要放進去char array 要多一個空間，因此才會+1
  //Msg3_Len = 21
  int Msg3_Len = Decode_Msg.length()+1;
  char input[Msg3_Len];
  //字串轉成char array
  Decode_Msg.toCharArray(input,Msg3_Len);
  //int inputLen = sizeof(input)-1;
  //Serial.println("inputLen:");Serial.println(inputLen);
  
  int decodedLen = base64_dec_len(input, Msg3_Len);
  char decoded[decodedLen];
  base64_decode(decoded, input, Msg3_Len);  
  Decode_Msg="";
  return decoded;
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
