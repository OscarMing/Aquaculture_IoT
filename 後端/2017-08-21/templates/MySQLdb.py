import mysql.connector
import time
import numpy
import datetime
from Send_email import Send_Email


date = ''
timex = ''
unix = time.time()
def dynamic_data_entry(Temperature, EC_value, PH_value,Lux,Water1,Water2):
       import time
       unix = time.time()
       global date,timex
       date= str(datetime.datetime.fromtimestamp(unix).strftime('%Y/%m/%d'))
       timex = str(datetime.datetime.fromtimestamp(unix).strftime('%H:%M:%S'))

       c.execute("INSERT INTO Test (Date,Time,Temperature,EC,pH,Lux,Water1,Water2) VALUES (%s,%s,%s,%s,%s,%s,%s,%s)",
                 (date,timex,Temperature, EC_value, PH_value,Lux,Water1,Water2))
       conn.commit()


def Send_MySQL(Temperature,EC_value,PH_value,Lux,Water1,Water2):
       Json_text = ""
       try:
               global conn,c
               conn = mysql.connector.connect('127.0.0.1',user='root',password='raspberry',database='Aquarium')
               c = conn.cursor()
               dynamic_data_entry(Temperature, EC_value, PH_value,Lux,Water1,Water2)
               print('Write DB Successful')
       except:
              DB_Subject = 'Database_Problem'
              html_DB_txt = '''
              <html>
              <head></head>
              <body>
              <p>Write database Error!!!<br>
                     Please Go to check...
              </p>
              </body>
              </html>
              '''
              Send_Email(DB_Subject,html_DB_txt)
              print('Write DB Error')

       '''
       try:
              with open('aquarium_data.json','w') as f:
                     for data_j in JJ:
                            json.dump(data_j, f)
                     print('Write Json Successful')
       except:
              print('Write Json Error')
       '''
       
       c.close()
       conn.close()





