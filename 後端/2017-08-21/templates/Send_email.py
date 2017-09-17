#-*-Coding:utf-8-*-

import smtplib
from email.mime.text import MIMEText
from email.mime.multipart import MIMEMultipart
#email Setting
email_host = 'smtp.gmail.com'
email_port = 587
email_username = 'zxpay73945788@gmail.com'
email_password = 'asdf739411'
from_email = email_username
to_list = ['zxpay73945788@gmail.com','auroral.13king518@gmail.com']


def Send_Email(Subject,html_txt):
       try:
              email_conn = smtplib.SMTP(email_host,email_port)
              email_conn.ehlo()
              email_conn.starttls() #Put the SMTP connection in TLS (Transport Layer Security) mode.
                                           #All SMTP commands that follow will be encrypted.
                                          #You should then call ehlo() again.

              the_msg = MIMEMultipart('alternative')
              the_msg['Subject'] = Subject
              the_msg['From'] = from_email
              the_msg['To'] = 'auroral.13king518@gmail.com'

              plain_txt = 'Testing the message'

              part_1 = MIMEText(plain_txt,'plain')
              part_2 = MIMEText(html_txt,'html')

              the_msg.attach(part_1)
              the_msg.attach(part_2)

              email_conn.login(email_username,email_password)
              email_conn.sendmail(from_email, to_list,the_msg.as_string())
              email_conn.quit()

       except smtplib.SMTPAuthenticationError:
              print('Login Fail !!!')
       except:
              print('Send email error...')       







