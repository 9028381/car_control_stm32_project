import _thread
import time
import os, machine
import gc
import network as n
import socket
import time
from machine import UART
import uos

gc.collect()
uart = UART(2, baudrate=460800, timeout=0, rxbuf = 4096)

ap = n.WLAN(n.AP_IF)
ap.active(True)
ap.config(essid="esp32", password="12345678") #设置wifi名称和密码

wlan=None
listenSocket=None
port = 32

wlan = n.WLAN(n.AP_IF)

ip=wlan.ifconfig()[0]
#建立Socket连接
listenSocket = socket.socket()
#绑定IP地址
try:
    listenSocket.bind((ip,port))
except:
    listenSocket.close()
    listenSocket = socket.socket()
    listenSocket.bind((ip,port))
print("TCP SERVER IP:",ip,":",port)
#Socket属性
listenSocket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 4096)
#侦听
listenSocket.listen(1)

def wifi2uart(lock, core):
    while True:
        try:
            data_net = conn.recv(4096)
            uart.write(data_net)
        except:
            pass
def uart2wifi(lock, core):
    while True:
        data_uart = uart.read(4096)
        if data_uart is not None:
            try:
                conn.send(data_uart)
            except:
                machine.reset()

# 主线程保持运行
while True:
    #获取客户端信息
    conn,addr = listenSocket.accept()
    conn.settimeout(0)
    while True:
        # 创建两个锁对象
        lock1 = _thread.allocate_lock()
        lock2 = _thread.allocate_lock()
        # 创建第一个线程并锁定到核心0
        _thread.start_new_thread(wifi2uart, (lock1, 0))
        # 创建第二个线程并锁定到核心1
        _thread.start_new_thread(uart2wifi, (lock2, 1))
        while True:
            pass
        