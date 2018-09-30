#! /usr/bin/env python
# -*- coding: utf-8 -*-

####################################################
# 功能：接收一个群中指定人发送的消息，转发到另一个指定的群中
# 时间：2018-09-30
####################################################

import sys
import json
import itchat
from itchat.content import TEXT
from itchat.content import *

# 要监测的群名称A
from_group_name = '晨晨妈团购省钱帮'
# 监测A群中指定昵称的人B（不能是自己）
from_group_user = '我爱0投资创业'
# 要讲A群中的人B 转发的目的群
to_group_name = '测试转发群'


# 群的文本消息转发
# isGroupChat=True表示为群聊消息
@itchat.msg_register([TEXT, MAP, CARD, NOTE, SHARING], isGroupChat=True)
def group_resend_text(msg):
    if msg['FromUserName'] == from_group_id:
        print "接收到群中一条【文本】消息,来源：%s" % msg['ActualNickName']
        if from_group_user == msg['ActualNickName']:
            print "%s[%s]发送了消息:%s" % (msg['ActualNickName'], msg['ActualUserName'], msg['Content'])
            if msg['Type'] == TEXT:
                itchat.send('%s' % msg['Text'], to_group_id)
            elif msg['Type'] == SHARING:
                itchat.send('%s\n%s' % (msg['FileName'], msg['Url']),to_group_id)
            elif msg['Type'] == CARD:
                itchat.send('%s' % msg['Content'], to_group_id)
            elif msg['Type'] == NOTE:
                itchat.send('%s' % msg['Content'], to_group_id)
            elif msg['Type'] == MAP:
                itchat.send('%s' % msg['OriContent'], to_group_id)
            else:
                pass


# 群的媒体消息转发
# isGroupChat=True表示为群聊消息
@itchat.msg_register([PICTURE, ATTACHMENT, VIDEO, RECORDING], isGroupChat=True)
def group_resend_media(msg):
    if msg['FromUserName'] == from_group_id:
        print "接收到群中一条【媒体】消息,来源：%s" % msg['ActualNickName']
        if from_group_user == msg['ActualNickName']:
            print "%s[%s]发送了消息:%s" % (msg['ActualNickName'], msg['ActualUserName'], msg['Content'])
            msg['Text'](msg['FileName'])  # 下载文件
            itchat.send('@%s@%s' % ({'Picture': 'img', 'Video': 'vid'}.get(msg['Type'], 'fil'), msg['FileName']),
                        to_group_id)

# 当登录完成时的回调
def login_ready():
    print("Finish login!")

# 当退出完成时的回调
def exit_ready():
    print("Finish logout!")


if __name__ == '__main__':
    reload(sys)
    sys.setdefaultencoding('utf-8')

    itchat.auto_login(loginCallback=login_ready, exitCallback=exit_ready, hotReload=True)
    group = itchat.get_chatrooms(update=True)
    for g in group:
        if g['NickName'] == from_group_name:  # 从群中找到指定的群聊
            from_group_id = g['UserName']
        if g['NickName'] == to_group_name:    # 把消息发到这个群
            to_group_id = g['UserName']

    print "============================================"
    print "     监测的群：%s"%from_group_name
    print " ( ID:%s)"%from_group_id
    print "                             "
    print "     群中的人:%s"%from_group_user
    print "                 |"
    print "                 |"
    print "                 |"
    print "                 V"
    print "      转发到的群:%s"%to_group_name
    print " ( ID:%s)" % to_group_id
    print "============================================"
    itchat.run()
