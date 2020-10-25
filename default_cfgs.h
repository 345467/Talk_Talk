#pragma once

string default_common_cfg=R""(
#日志输出级别
log.level=INFO

#===============================

#标签:ip
gui.label.ip.x=10
gui.label.ip.y=10

#标签:msg
gui.label.msg.x=10
gui.label.msg.y=35

#文本框:ip
gui.textfield.ip.x=150
gui.textfield.ip.y=10
gui.textfield.ip.w=310
gui.textfield.ip.h=20

#文本域:msg
gui.textarea.msg.x=150
gui.textarea.msg.y=35
gui.textarea.msg.w=300
gui.textarea.msg.h=200

#文本域:recvs
gui.textarea.recvs.x=10
gui.textarea.recvs.y=105
gui.textarea.recvs.w=100
gui.textarea.recvs.h=200

#按钮:send
gui.button.send.x=10
gui.button.send.y=55
gui.button.send.w=100
gui.button.send.h=20

#按钮:exit
gui.button.exit.x=10
gui.button.exit.y=80
gui.button.exit.w=100
gui.button.exit.h=20

#===============================

#语言文件
lang.zh_cn.label.ip=输入IP地址
lang.zh_cn.label.msg=输入要发送的消息
lang.zh_cn.textfield.ip=127.0.0.1
lang.zh_cn.textarea.msg=在此输入要发送的消息
lang.zh_cn.textarea.recvs=收到消息:空
lang.zh_cn.button.send=发送消息
lang.zh_cn.button.restart=重启 
)"";
