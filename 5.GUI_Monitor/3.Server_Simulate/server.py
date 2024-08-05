import time 
import random 
from paho.mqtt import client as mqtt_client

host = 'iot-solar.nichietsuvn.com'
port = 1884
id = 'test1' 
user = 'guest'
pwd = '123456a@'
pubTopic = 'server'

def connectBroker():
    def on_connect(__client, userdata, flags, rc):
        if rc == 0:
            print('Connected to MQTT Broker')
        else:
            print('Connection failed!')

    __client = mqtt_client.Client(id)
    __client.username_pw_set(user, pwd) 
    __client.on_connect = on_connect
    __client.connect(host, port, 300) 

    return __client 

def publish(client):
    while True:
        time.sleep(1) 
        randomID = random.randint(1, 10)
        if randomID >= 1 and randomID < 10:
            __deviceID = 'G000' + str(randomID)  
        elif randomID >= 10 and randomID < 100:
            __deviceID = 'G00' + str(randomID) 

        contents = str(__deviceID) + '|00|01|01|' + str(random.randint(1, 9999)) + '|' + str(float('{:.4f}'.format(random.uniform(1000.0000, 9999.9999)))) + '|' + str(float('{:.4f}'.format(random.uniform(10000.0000, 99999.9999)))) + '|' + str(random.randint(1, 100)) + '|' + str(float('{:.2f}'.format(random.uniform(0.00, 9.99)))) + '|1' 
        #@@contents = 'G0002' + '|00|01|01|' + str(random.randint(1, 9999)) + '|' + str(float('{:.4f}'.format(random.uniform(1000.0000, 9999.9999)))) + '|' + str(float('{:.4f}'.format(random.uniform(10000.0000, 99999.9999)))) + '|' + str(random.randint(1, 100)) + '|' + str(float('{:.2f}'.format(random.uniform(0.00, 9.99)))) + '|1' 
        result = client.publish(pubTopic, contents) 
        
        status = result[0] 
        if status == 0:
            print(f"Send `{contents}` to topic `{pubTopic}`")
        else:
            print(f"Failed to send message to topic {pubTopic}")

if __name__ == '__main__': 
    client = connectBroker() 
    client.loop_start()  
    publish(client) 