import requests

URL = "http://192.168.41.165:5000/"

response = requests.get(URL + "rfid/0008340664")

print(response.json())