# Publisher.py
import paho.mqtt.client as mqtt
import time
import sys
import json
import ast
#先import
import logging

class MQTT_NAS:
    def __init__(self, Broker, Port, topic):
        self.broker = Broker
        self.port = Port
        self.return_dict = {}
        self.flag = False
        self.sub_topic = topic
        self.pub_topic = 'Control_Test'
        
    def MQTT_Setup(self):
        self.client = mqtt.Client()
        self.client.on_connect = self.on_connect
        self.client.on_message = self.on_message
        self.client.connect(self.broker,self.port)
        self.client.loop_start()

    def MQTT_Publish(self, Topic, Msg):
        self.pub_topic = Topic
        self.client.on_publish = self.on_publish
        self.client.publish(self.pub_topic, Msg)
        
    def on_connect(self,client, userdata, flags, rc):
        print("Connected with result code "+str(rc))
        self.client.subscribe(self.sub_topic)
     
    def on_message(self,client, userdata, msg):
        self.return_dict = msg.payload.decode()  # class bytes to string
        self.return_dict = ast.literal_eval(self.return_dict)   # string to dictionary
        self.flag = True
        print(msg.topic+" "+str(msg.payload))

    def Get_message(self):
        if self.flag == True:
            self.flag = False
            return self.return_dict
        else:
            return False


    def on_publish(self,client, userdata, mid):
        print ("Message Publish...    mid:"+ str(mid))      

    def MQTT_Sensor(self, topic, Temperature, EC_value,PH_value, Lux, Water1, Water2):
        #決定此log的名稱
        logger = logging.getLogger('first_logging')
        #決定層級
        logging.basicConfig(level=logging.DEBUG)
        # Produce formater first
        formatter = logging.Formatter('%(asctime)s')
         
        # Setup Handler
        console = logging.StreamHandler()
        console.setLevel(logging.DEBUG)
        console.setFormatter(formatter)
         
        # Setup Logger
        logger.addHandler(console)
        logger.setLevel(logging.DEBUG)

        MQTT_MSG =[]

        Json_text = {
                        'Temp': Temperature,
                        'Ph': PH_value,
                        'EC': EC_value,
                        'Lux': Lux,
                        'Water1': Water1,
                        'Water2': Water2
                }

        MQTT_MSG = json.dumps(Json_text)
        
        self.MQTT_Publish(topic,MQTT_MSG)


    def MQTT_Relay(self, topic, relay_str):
        #決定此log的名稱
        logger = logging.getLogger('first_logging')
        #決定層級
        logging.basicConfig(level=logging.DEBUG)
        # Produce formater first
        formatter = logging.Formatter('%(asctime)s')
         
        # Setup Handler
        console = logging.StreamHandler()
        console.setLevel(logging.DEBUG)
        console.setFormatter(formatter)
         
        # Setup Logger
        logger.addHandler(console)
        logger.setLevel(logging.DEBUG)

        MQTT_MSG = []

        Json_text = {
                        'Relay1': relay_str[0],
                        'Relay2': relay_str[1],
                        'Relay3': relay_str[2],
                        'Relay4': relay_str[3],
                        'Relay5': relay_str[4],
                        'Relay6': relay_str[5],
                        'Relay7': relay_str[6],
                        'Relay8': relay_str[7],
                }

        MQTT_MSG = json.dumps(Json_text)
        
        self.MQTT_Publish(topic,MQTT_MSG)

    def MQTT_Servo(self, topic, servo_list):
        #決定此log的名稱
        logger = logging.getLogger('first_logging')
        #決定層級
        logging.basicConfig(level=logging.DEBUG)
        # Produce formater first
        formatter = logging.Formatter('%(asctime)s')
         
        # Setup Handler
        console = logging.StreamHandler()
        console.setLevel(logging.DEBUG)
        console.setFormatter(formatter)
         
        # Setup Logger
        logger.addHandler(console)
        logger.setLevel(logging.DEBUG)

        JMQTT_MSG = []
        Json_text = {
                        'Servo1':str(servo_list[0]),
                        'Servo2':str(servo_list[1]),
                        'Servo3':str(servo_list[2]),
                }

        MQTT_MSG = json.dumps(Json_text)
        
        self.MQTT_Publish(topic,MQTT_MSG)


    def MQTT_Feed(self, topic, feed):
        #決定此log的名稱
        logger = logging.getLogger('first_logging')
        #決定層級
        logging.basicConfig(level=logging.DEBUG)
        # Produce formater first
        formatter = logging.Formatter('%(asctime)s')
         
        # Setup Handler
        console = logging.StreamHandler()
        console.setLevel(logging.DEBUG)
        console.setFormatter(formatter)
         
        # Setup Logger
        logger.addHandler(console)
        logger.setLevel(logging.DEBUG)

        JMQTT_MSG = []

        Json_text = {
                        'Servo4':str(feed),
                }

        MQTT_MSG = json.dumps(Json_text)
        
        self.MQTT_Publish(topic,MQTT_MSG)
        

    def MQTT_Fmode(self, topic, Fmode):
        #決定此log的名稱
        logger = logging.getLogger('first_logging')
        #決定層級
        logging.basicConfig(level=logging.DEBUG)
        # Produce formater first
        formatter = logging.Formatter('%(asctime)s')
         
        # Setup Handler
        console = logging.StreamHandler()
        console.setLevel(logging.DEBUG)
        console.setFormatter(formatter)
         
        # Setup Logger
        logger.addHandler(console)
        logger.setLevel(logging.DEBUG)

        JMQTT_MSG = []

        Json_text = {
                        'Fmode':str(Fmode),
                }

        MQTT_MSG = json.dumps(Json_text)
        
        self.MQTT_Publish(topic,MQTT_MSG)



        

    def MQTT_Data_Analyze(self,dataIn):    # dictionary data in
        # Test whether dataIn is dictionary...
        try:
            dataIn.keys()
        except:
            print('Command is not the dictionary type ...')
            return 0,0
        
        # Feed servo to control
        if 'Feed_Mode' in dataIn.keys():  # value: 0,1,2,3
            return 'Fd,%s\n\r' % str(dataIn['Feed']), 1

        # Relay command to control
        elif 'Relay1' in dataIn.keys():
            return 'R1,%s\n\r' % str(dataIn['Relay1']), 1
        elif 'Relay2' in dataIn.keys():
            return 'R2,%s\n\r' % str(dataIn['Relay2']), 1
        elif 'Relay3' in dataIn.keys():
            return 'R3,%s\n\r' % str(dataIn['Relay3']), 1
        elif 'Relay4' in dataIn.keys():
            return 'R4,%s\n\r' % str(dataIn['Relay4']), 1 
        elif 'Relay5' in dataIn.keys():
            return 'R5,%s\n\r' % str(dataIn['Relay5']), 1
        elif 'Relay6' in dataIn.keys():
            return 'R6,%s\n\r' % str(dataIn['Relay6']), 1
        elif 'Relay7' in dataIn.keys():
            return 'R7,%s\n\r' % str(dataIn['Relay7']), 1
        elif 'Relay8' in dataIn.keys():
            return 'R8,%s\n\r' % str(dataIn['Relay8']), 1
        elif 'Relay9' in dataIn.keys(): # 1 --> all on  , 0 --> all off
            return 'RR,%s\n\r' % str(dataIn['Relay9']), 1 

        # Servo command to control
        elif 'Servo_WaterIO' in dataIn.keys():  # water in : 1
            return 'S1,%s\n\r' % str(dataIn['Servo1']), 1

        # Warning ...
        elif 'Servo1_Warning' in dataIn.keys():
            print('Servo1 data error, please go to check')
            return 0, 0
        elif 'Servo2_Warning' in dataIn.keys():
            print('Servo2 data error, please go to check')
            return 0, 0
        elif 'Servo3_Warning' in dataIn.keys():
            print('Servo3 data error, please go to check')
            return 0, 0
        elif 'Servo4_Warning' in dataIn.keys():
            print('Servo4 data error, please go to check')
            return 0, 0
                  
        else:
            return 0, 0


# Sensor_Test  Motor1, Motor2: 1 , 0
# Relay_Test   Relay1-9 : 1 , 0
# Fmode_Test   Feed_Mode : "1" "2" "3" "0"
# UpDown_Test  Servo_down ; Servo_up :1 ,0




## example
'''
mqtt_Nas = MQTT_NAS('192.168.1.200',1883,'Control_Test')
mqtt_Nas.MQTT_Setup()
mqtt_Nas.MQTT_Feed('Servo_Test',1233)

while True:
    Command = mqtt_Nas.Get_message()
    if Command != False:
        print('Command dictionary --> ', Command)
        Command, en_command = mqtt_Nas.MQTT_Data_Analyze(Command)
        if(en_command):
            Command = bytes(Command,'utf-8')
            print('Command Send --> ', Command)
    time.sleep(1)
'''
  


