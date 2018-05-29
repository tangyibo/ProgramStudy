#!/usr/bin/python
# -*- coding: UTF-8 -*-
import sys,os
import smtplib
from email import encoders
from email.mime.multipart import MIMEMultipart
from email.mime.base import MIMEBase
from email.mime.text import MIMEText
from email.header import Header


class MailSender:

    def __init__(self, host, user, password):
        self.mail_host = host
        self.mail_user = user
        self.mail_pass = password
        self.message = MIMEMultipart()
        self.message['From'] = user
        self.receivers_list=list()

    def set_subject(self, subject):
        self.message['Subject'] = Header(subject, 'utf-8')

    def set_content(self, text):
        self.message.attach(MIMEText(text, 'plain', 'utf-8'))

    def set_mail_to(self, mail_list):
        self.receivers_list=mail_list
        self.message['To'] = ';'.join(mail_list)

    def set_attach(self,filename,content_type):
        maintype, subtype = content_type.split('/', 1)
        basename = os.path.basename(filename)
        with open(filename, 'rb') as f:
            mime = MIMEBase(maintype, subtype)
            mime.add_header('Content-Disposition', 'attachment', filename=basename)
            mime.set_payload(f.read())
            encoders.encode_base64(mime)
            self.message.attach(mime)

    def send_mail(self):
        try:
            smtpObj = smtplib.SMTP()
            smtpObj.set_debuglevel(0)
            smtpObj.connect(self.mail_host,25)  # 25 为 SMTP端口号
            smtpObj.login(self.mail_user, self.mail_pass)
            smtpObj.sendmail(self.mail_user, self.receivers_list, self.message.as_string())
            smtpObj.quit()
            return True
        except smtplib.SMTPHeloError, e:
            print "SMTPHeloError:%s" % e.message
        except smtplib.SMTPRecipientsRefused, e:
            print "SMTPRecipientsRefused:%s" % e.message
        except smtplib.SMTPSenderRefused, e:
            print "SMTPSenderRefused:%s" % e.message
        except smtplib.SMTPDataError, e:
            print "SMTPDataError:%s" % e.message
        except smtplib.SMTPException, e:
            print "SMTPException: %s" % e.message
        return False


if __name__ == '__main__':
    reload(sys)
    sys.setdefaultencoding('utf-8')

    host = "smtp.sina.com"       # 设置服务器
    user = "xxxx@sina.com"       # 用户名
    passwd = "xxx"               # 口令
    receivers = ['xxx@126.com']  # 接收列表

    ms=MailSender(host,user,passwd)
    ms.set_subject("title test")
    ms.set_mail_to(receivers)
    ms.set_content("content test")
    ms.set_attach("E:\\HttpUtil.java",'application/text')
    ms.set_attach("E:\\password.txt", 'application/octet-stream')
    ret=ms.send_mail()
    if ret is True:
        print "邮件发送成功"
    else:
        print "邮件发送失败"
