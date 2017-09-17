import sys
import os
sys.path.append(os.path.abspath("/home/pi/Desktop/templates"))
import netifaces as ni
import serial
import time
import datetime
from Send_email import Send_Email    # Send_Email(Title, Text)
from Get_Serial_Data import *   # class
from MySQLdb import Send_MySQL     #Send_MySQL(Temperature,EC_value,PH_value,Lux,Water1,Water2)
from MQTT_NAS import *    # class

Topic_Sensor = 'Sensor_Test'
Topic_Relay = 'Relay_Test'
Topic_Servo = 'Servo_Test'
Topic_Feed = 'Servo4_Test'
Topic_Fmode = 'Fmode_Test'
Topic_Up_Down = 'UpDown_Test'
Topic_Control = 'Control_Test'
inet_IP = ni.ifaddresses('wlan0')[2][0]['addr']
# Get IP address 192.168.xxx.xxx
port = 1883
Broker = inet_IP
print('Broker: ',Broker)
print()
DB_Subject = 'Data'
html_serial_problem ='''
                        <html>
                        <head></head>
                        <body>
                        <p>Send Serial Fail...<br>
                        </p>
                        </body>
                        </html>
                     '''

# Main Code here

arduino_Serial = serial.Serial('/dev/ttyACM0',38400)   # Connect to Serial

mqtt_Nas = MQTT_NAS(Broker, port, Topic_Control)
mqtt_Nas.MQTT_Setup()

Serial_data = Analyze_data()

while True:
    Command = {}
    print('Ready to get data ... ')
    while arduino_Serial.inWaiting():
        pass
    Command = mqtt_Nas.Get_message()
    if Command != False:
        print('Command dictionary --> ', Command)
        Command, en_command = mqtt_Nas.MQTT_Data_Analyze(Command)
        if(en_command):
            Command = bytes(Command,'utf-8')    # conver to bytes data type
            print('Command Send --> ', Command)
            arduino_Serial.write(Command)    # Command need bytes type
                
    data = str(arduino_Serial.readline())
    Serial_data.Analyze_data(data)

    if Serial_data.sensor_flag:
        Temperature,EC_value,PH_value,Lux,Water1,Water2 = Serial_data.Return_Sensor_Data()
        Send_MySQL(Temperature,EC_value,PH_value,Lux,Water1,Water2)
        mqtt_Nas.MQTT_Sensor(Topic_Sensor,Temperature,EC_value,PH_value,Lux,Water1,Water2)
        a = '<p>Temperature: %.2f (Celsius)' % Temperature + '<br>'
        b = '<p>EC: %.2f (ms/cm)' % EC_value + '<br>'
        c = '<p>pH: %.2f' % PH_value + '<br>'
        d = '<p>Lux: %.2f' % Lux + '<br>'
        html_DB_txt='''
                    <html>
                    <head></head>
                    <body>
                    <p>Data<br>5
                    '''+a+b+c+d+'''
                    </p>
                    </body>
                    </html>
                    '''
        #Send_Email('Sensor_Data',html_DB_txt)
        
    elif Serial_data.relay_flag:
        relay_str = Serial_data.Return_Relay_Data()
        mqtt_Nas.MQTT_Relay(Topic_Relay, relay_str)

    elif Serial_data.servo_flag:
        servo_list = Serial_data.Return_Servo_Data()
        mqtt_Nas.MQTT_Servo(Topic_Servo, servo_list)

    elif Serial_data.feed_flag:
        feed = Serial_data.Return_Feed_Data()
        mqtt_Nas.MQTT_Feed(Topic_Feed, feed)
        
    elif Serial_data.F_mode_flag:
        Fmode = Serial_data.Return_Fmode_Data()
        mqtt_Nas.MQTT_Fmode(Topic_Fmode, Fmode)

    arduino_Serial.flushInput()
    arduino_Serial.flushOutput()
    

