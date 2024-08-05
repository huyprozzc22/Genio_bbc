//mosquitto_sub -h "iot-solar.nichietsuvn.com" -p 1884 -u "guest" -P "123456a@" -t "genio"
//mosquitto_sub -h "healthcare-broker.nichietsuvn.com" -p 1886 -u "babycare" -P "123qaz" -t "topic_heart_rate"

#include <ESP8266WiFi.h>
#include <PubSubClient.h>

char data[100];
char buff [150];
volatile byte indx;
int LED =5; //LED active HIGHT
//int LED =14; //LED active HIGHT
//const int BUTTON_PIN =12 ; // the number of the pushbutton pin
const int BUTTON_PIN = 4; // the number of the pushbutton pin
//const int LONG_PRESS_TIME  = 3000; // 1000 milliseconds
bool interruptFlag=false;
// Variables will change:
unsigned long pressedTime  = 0;
unsigned long releasedTime = 0;

#define wifi_ssid "nichietsu"
#define wifi_password "Aa@123456"

#define GENIO 
#ifdef GENIO
  #define mqtt_server "iot-solar.nichietsuvn.com"
  #define mqtt_port 1884
  #define mqtt_user "guest"
  #define mqtt_password "123456a@"
  #define pub_topic "genio"
  #define mqtt_client "ID15"
#else 
  #define mqtt_server "healthcare-broker.nichietsuvn.com"
  #define mqtt_port 1886
  #define mqtt_user "babycare"
  #define mqtt_password "123qaz"
  #define pub_topic "topic_heart_rate"
  #define mqtt_client "ID00020"
#endif 

// Kobell 
//#define mqtt_server "iot-solar.nichietsuvn.com"
//#define mqtt_port 1884

//#define mqtt_server "10.0.0.37"
//#define mqtt_port 1883
//#define mqtt_user "guest"
//#define mqtt_password "123456a@"
//#define pub_topic "topic_genio"
//#define mqtt_client "ID00008"

WiFiClient espClient;
PubSubClient client;

ICACHE_RAM_ATTR void INTERRUPT_HANDLE() {
  Serial.println("INTERRUPT DETECTED!!!");
  interruptFlag= true;
}
void setup() {
  Serial.begin(115200);
  pinMode(LED,OUTPUT);
  digitalWrite(LED,LOW);//Tắt LED
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), INTERRUPT_HANDLE, FALLING);
  setup_wifi();
  client.setClient(espClient);
  client.setServer(mqtt_server, mqtt_port);
}

void printWifiStatus(){
  Serial.println("\nWiFi Connected.");
  Serial.printf("SSID: %s\n", WiFi.SSID().c_str());
  //Serial.printf("Password: %s\n", WiFi.psk().c_str());
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
}
void smartConfig()
{
  WiFi.mode(WIFI_STA);
  WiFi.beginSmartConfig();    //Set up SmartConfig
  Serial.println("Start SmartConfig."); 
  Serial.println("Waiting for SmartConfig. Launch Mobile App to progress SmartConfig.");
  while (!WiFi.smartConfigDone()) {
    digitalWrite(LED,1);
    delay(100);
    digitalWrite(LED,0);
    delay(400);
    Serial.print(".");
    //status_NoConnect();
  }
  Serial.println("");
  Serial.println("SmartConfig done.");
  
  /* Wait for WiFi to connect to AP*/ 
  Serial.print("WiFi Connecting.");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    
  }

  printWifiStatus();

  //WiFi.setAutoConnect(true);

}
void setup_wifi() {
  /*
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(wifi_ssid);

  WiFi.begin(wifi_ssid, wifi_password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  digitalWrite(2,LOW);
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  */
  WiFi.begin();
  //int wifi_count_connect =30;
  while (WiFi.status() != WL_CONNECTED&& interruptFlag == false ) { //chờ đến khi wifi kết nối
    // status_NoConnect();
     Serial.println("noconnect....!");
     delay(500);
         
  }
}

void reconnect() {
 
  while (!client.connected()&& interruptFlag == false) {   
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    // If you do not want to use a username and password, change next line to
    // if (client.connect(mqtt_client)) {
    if (client.connect(mqtt_client, mqtt_user, mqtt_password)) {
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
      
    }
   // count_connect--;
  }
}


void clear_mem()
{
  for (char i =0; i <150; i++)
  {
    buff[i] = '\0';
  }
}

void loop() {
    if(interruptFlag== true){
    delay(5000);
    if(digitalRead(BUTTON_PIN)==LOW ){
        interruptFlag = false;
        WiFi.disconnect(true);
        ESP.eraseConfig();
        delay(500); 
        smartConfig();
        
     }
    else interruptFlag= false;
    
  }
 if(WiFi.isConnected())
 {digitalWrite(LED,1);}
 else{digitalWrite(LED,0);}
 
  
  if (!client.connected()) {
    digitalWrite(LED,0);//OFF LED
    reconnect();
  }
  client.loop();
  
  if (Serial.available() > 0) {
    byte c = Serial.read();
    
    if (indx < sizeof buff) {
      buff [indx++] = c; // save data in the next index in the array buff
      if (c == 10) { //check for the end of the word
        Serial.println(buff);
        
        client.publish(pub_topic, buff, false);
        indx= 0; //reset button to zero
        clear_mem();
      }
    }
  }
  
}
