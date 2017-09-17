# -*- coding: utf-8 -*-
class Analyze_data():
    def __init__(self):
        self.Temp = 0
        self.EC = 0
        self.pH = 0
        self.Lux = 0
        self.Water1 = 0
        self.Water2 = 0
        self.relay = []
        self.servo = []
        self.feed = 500
        self.F_mode = 0
        self.sensor_flag = False
        self.relay_flag = False
        self.servo_flag = False
        self.feed_flag = False
        self.F_mode_flag =False
    def Analyze_data(self,data):
        self.__init__()
        title = ''
        try:
            d = data.split(',')
            title, f = d[0].split(':')
            print('Data in --> ', data)
        except:
            print("Data can't Split --> ",data)
        num = 0
        if title == "b'Tp":
            for x in d:
                try:
                    d1, d2 = x.split(':')
                    d2 = float(d2)
                    if 'Tp' in d1:
                        self.Temp = d2
                        num += 1
                    elif d1 == 'E':
                        self.EC = d2
                        num += 1
                    elif d1 == 'S':
                        self.Water1 = d2
                        num += 1
                    elif d1 == 'pH':
                        self.pH = d2
                        num += 1
                    elif d1 == 'Lx':
                        self.Lux = d2
                        num += 1
                    elif d1 == 'T':
                        self.Water2 = d2
                        num += 1
                except:
                    pass
            if num == 6:
                self.sensor_flag = True
            else:
                print('Sensor data number error...', d)
                
        elif title == "b'Relay":
            if self.check_relay(d[1][0:8]):
                self.relay = d[1][0:8]
            if len(self.relay) == 8:
                self.relay_flag = True
            else:
                print('Relay data number error --> ',d)

        elif title == "b'Servo":
            for i in range(1,len(d)):
                if self.check_servo(d[i]):
                    self.servo.append(d[i])
            if len(self.servo) == 3:
                self.servo_flag = True
            else:
                print('Servo data number error --> ',d)

        elif title == "b'Feed":
            if ((int(d[1])>=500) and (int(d[1])<=2500)):
                self.feed = d[1]
                self.feed_flag = True
            else:
                print('Feed servo data error --> ', d)

        elif title == "b'Fmode":
            if ((d[1]=='1') or (d[1]=='2') or (d[1]=='3')):
                self.F_mode = d[1]
                self.F_mode_flag = True
            else:
                print('Feed mode data error --> ', d)

        else:
            print("Data can't analyze")
            
            
    def check_servo(self,numIn):
        try:
            if (int(numIn)>=500) and (int(numIn)<=2500):
                return True
            else:
                return False
        except:
            return False
        
    def check_relay(self,numIn):
        try:
            for i in numIn:
                if i != '0' and i !='1':
                    return False
                else:
                    pass
            return True
        except:
            return False

    def Return_Sensor_Data(self):
        return self.Temp, self.EC, self.pH, self.Lux, self.Water1, self.Water2

    def Return_Servo_Data(self):
        return self.servo    # data type list

    def Return_Relay_Data(self):
        return self.relay    # data type string
    
    def Return_Feed_Data(self):
        return self.feed     # data type string
    
    def Return_Fmode_Data(self):
        return self.F_mode  # data type string

'''
a = 'Tp:12.2,E:12,S:12.3,pH:12.3,Lx:123,T:234'
b = "b'Relay:,10001111"
c = "b'Servo:,524,887,1222,1223"
d = "b'Fmode:,2,\r\n'"

# example

anay = Analyze_data()

anay.Analyze_data(d)
print(anay.F_mode_flag)
print(anay.Return_Fmode_Data())
'''
