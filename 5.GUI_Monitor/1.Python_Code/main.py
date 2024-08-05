import os 
import time 
import threading 
import tkinter as tk 
from app import WindowsForm as GUI
from paho.mqtt import client as mqtt_client 

host = 'iot-solar.nichietsuvn.com'
port = 1884
id = '00001' 
user = 'guest'
pwd = '123456a@'
subTopic = 'genio'  

receiveBuffer = '' 
startTime = 0 
__IDList = [] 
__templateList = [] 

def connectBroker(): 
    def on_connect(__client, userdata, flags, rc):
        if rc == 0:
            print('Connected to MQTT Broker')
        else:
            print('Connection falied!')

    __client = mqtt_client.Client(id)
    __client.username_pw_set(user, pwd)
    __client.on_connect = on_connect
    __client.connect(host, port)

    return __client

def on_message(client, userdata, msg):
    global receiveBuffer, startTime 

    receiveBuffer = str(msg.payload.decode()) 
    print('Receive data: ' + receiveBuffer) 

    for id in __IDList:
        print(id) 
        if id == str(msg.payload.decode())[0:5]: 
            __data = receiveBuffer.split('|') 

            __getIDVar = tk.StringVar()   
            __getIDVar.set(str(msg.payload.decode())[0:5]) 

            __getStepsVar = tk.StringVar() 
            __getStepsVar.set(__data[4])

            __getLongitudeVar = tk.StringVar()
            __getLongitudeVar.set(__data[5]) 

            __getLatitudeVar = tk.StringVar()
            __getLatitudeVar.set(__data[6]) 

            __getBatteryCapacityVar = tk.StringVar() 
            __getBatteryCapacityVar.set(__data[7])

            __getSpeedVar = tk.StringVar() 
            __getSpeedVar.set(__data[8]) 

            __getStatusUsingVar = tk.StringVar()   
            __getStatusUsingVar.set(__data[9]) 

            __getTimeActivity = tk.StringVar()
            __getTimeActivity.set(0) 

            startTime = time.time()
            __ls = [__getIDVar, __getStepsVar, __getLongitudeVar, __getLatitudeVar, __getBatteryCapacityVar, __getSpeedVar, __getStatusUsingVar, __getTimeActivity]  
            print(__ls) 

            win.drawTable(dataList=__ls)  
            __templateList.append(__ls) 

            __IDList.remove(id) 

    threading.Thread(target=autoUpdate).start() 

def autoUpdate(): 
    global receiveBuffer, startTime 

    for i in __templateList: 
        __data = receiveBuffer.split('|') 
        
        if i[0].get() == __data[0] and i[1].get() != __data[4]:
            i[1].set(__data[4])  

        if i[0].get() == __data[0] and i[2].get() != __data[5]: 
            i[2].set(__data[5])  

        if i[0].get() == __data[0] and i[3].get() != __data[6]: 
            i[3].set(__data[6])  

        if i[0].get() == __data[0] and i[4].get() != __data[7]:
            i[4].set(__data[7]) 

        if i[0].get() == __data[0] and i[5].get() != __data[8]:
            i[5].set(__data[8])

        if i[0].get() == __data[0] and i[6].get() != __data[9]:
            i[6].set(__data[9]) 

        endTime = time.time() - startTime 
        if endTime > 1 and i[0].get() == __data[0]: 
            i[7].set(0) 
            startTime = time.time() 

            print('Update: ' + __data[0] + ' ' + __data[4] + ' ' + __data[5] + ' ' + __data[6] + ' ' + __data[7] + ' ' + __data[8] + ' ' + __data[9])  
        elif endTime < 1 and i[0].get() == __data[0]: 
            i[7].set(int(endTime))  

def afterClickButtonGetID():     
    __nameID = 0 
    if win.getInputID.get() >= 1 and win.getInputID.get() < 10: 
        __nameID = str('G000') + str(win.getInputID.get()) 
        __IDList.append(__nameID) 
    elif win.getInputID.get() >= 10 and win.getInputID.get() < 100: 
        __nameID = str('G00') + str(win.getInputID.get()) 
        __IDList.append(__nameID) 
    elif win.getInputID.get() >= 100 and win.getInputID.get() < 1000:
        __nameID = str('G0') + str(win.getInputID.get()) 
        __IDList.append(__nameID) 
    elif win.getInputID.get() >= 1000 and win.getInputID.get() < 10000:
        __nameID = str('G') + str(win.getInputID.get()) 
        __IDList.append(__nameID) 

    win.textInput.delete(0, tk.END) 
    print('Read device list: ' + str(__IDList))  

def afterClickButtonStart(): 
    print('button') 

if __name__ == '__main__': 
    try:
        client = connectBroker()
        client.loop_start()
        client.subscribe(subTopic)
        client.on_message = on_message 

        win = GUI(BtnGetIDFunc=afterClickButtonGetID, BtnStartFunc=afterClickButtonStart)      
         
        win.mainloop() 

    except KeyboardInterrupt:
        client.loop_stop() 
        os._exit(0) 