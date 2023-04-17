#include <WiFi.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include "UbidotsEsp32Mqtt.h"

#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

#define AIO_SERVER      ""
#define AIO_SERVERPORT                    
#define AIO_USERNAME    ""
#define AIO_KEY         ""

/****************************************
 * Define Constants
 ****************************************/
const char *UBIDOTS_TOKEN = "";  // Put here your Ubidots TOKEN
//const char *WIFI_SSID = "";      // Put here your Wi-Fi SSID
//const char *WIFI_PASS = "";      // Put here your Wi-Fi password
const char *DEVICE_LABEL = "ledtest";//"642a7af5dffade000bfe04bc";   // Put here your Device label to which data  will be published
const char *VARIABLE_LABEL = "ledstatus"; // Put here your Variable label to which data  will be published

const int PUBLISH_FREQUENCY = 5000; // Update rate in milliseconds

unsigned long timer;
//uint8_t Pin = A4; // Pin used to read data from GPIO34 ADC_CH6.

Ubidots ubidots(UBIDOTS_TOKEN);

#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"

uint8_t ledPin = A4;
uint8_t buttonPin = 19;

int ledState = LOW;
int buttonState;
int lastButtonState = LOW;

unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 50;

unsigned long long duration = 0;
unsigned long long ledStartTime = 0;

class MyCallbacks: public BLECharacteristicCallbacks {
  void onWrite(BLECharacteristic *pCharacteristic) {
    std::string value = pCharacteristic->getValue();

    if (value.length() > 0) {
      for (int i = 0; i < value.length(); i++) {
        if (value[i] == '1') {
          ledState = HIGH;
          lastButtonState = HIGH;   
          digitalWrite(ledPin, ledState);
        } else if (value[i] == '0') {
          ledState = LOW;
          lastButtonState = LOW;
          digitalWrite(ledPin, ledState);
        }
      }
    }
  }
};

const char* ssid = "";
const char* password = "";

IPAddress staticIP(192, 168, 205, 4);
IPAddress gateway(192, 168, 205, 1);
IPAddress subnet(255, 255, 0, 0);

WiFiServer server(80);
WiFiClient client;

void callback(char *topic, byte *payload, unsigned int length)
{
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++)
  {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}

Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);        
Adafruit_MQTT_Subscribe ESP32_LED_Control = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/Esp32");
void MQTT_connect();
uint32_t x=0;

void MQTT_connect() {
  int8_t again;
  if (mqtt.connected()) {
    return;
  }
  Serial.print("Connecting to Adafruit IO");
  uint8_t retry = 5;
  while ((again = mqtt.connect()) != 0) { 
    Serial.println(mqtt.connectErrorString(again));
    Serial.println("Retrying Adafruit connection in 5 seconds...");
    mqtt.disconnect();
    delay(5000);  
    retry--;
    if (retry == 0) {
      while (1);
    }
  }
  Serial.println("");
  Serial.println("Adafruit IO is Connected!");
}

void setup() {
  Serial.begin(115200);
  //setupWiFi();
  ubidots.connectToWifi(ssid, password);
  ubidots.setCallback(callback);
  ubidots.setup();
  ubidots.reconnect();
  timer = millis();
  //ubidots.subscribeLastValue(DEVICE_LABEL, VARIABLE_LABEL); // Insert the dataSource and Variable's Labels
  setupLED();
  setupButton();
  setupBLE();
  setupServer();
  mqtt.subscribe(&ESP32_LED_Control);
}

void loop() {
  // energy
  if (ledState == HIGH && ledStartTime == 0) {
    ledStartTime = millis();
  } else if (ledState == LOW && ledStartTime != 0) {
    duration += millis() - ledStartTime;
    ledStartTime = 0;
  }
  
  int reading = digitalRead(buttonPin);

  if (reading != lastButtonState) {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (reading != buttonState) {
      buttonState = reading;
      if (buttonState == HIGH) {
        ledState = !ledState;
      }
    }
  }

  digitalWrite(ledPin, ledState);

  lastButtonState = reading;  

  client = server.available();

  if (client) {
    // send LED status to client    
    String request = getRequestFromClient(client);
    updateStatus(request);
  }

  MQTT_connect();
  Adafruit_MQTT_Subscribe *subscription;
  while ((subscription = mqtt.readSubscription(5000))) {
    if (subscription == &ESP32_LED_Control) {
      Serial.print(F("Got: "));
      Serial.println((char *)ESP32_LED_Control.lastread);
       if (!strcmp((char*) ESP32_LED_Control.lastread, "ON")) {
        ledState = HIGH;
        lastButtonState = HIGH;
        digitalWrite(ledPin, ledState);
      } else {
        ledState = LOW;
        lastButtonState = LOW;
        digitalWrite(ledPin, ledState);
      }
    }
  }

  if (!ubidots.connected())
  {
    ubidots.reconnect();
  }
  long temp = millis() - timer;
  if (abs(temp) > PUBLISH_FREQUENCY) // triggers the routine every 5 seconds
  {
    float value = digitalRead(ledPin);
    ubidots.add(VARIABLE_LABEL, value); // Insert your variable Labels and the value to be sent
    ubidots.publish(DEVICE_LABEL);
    timer = millis();
    //ubidots.subscribeLastValue(DEVICE_LABEL, VARIABLE_LABEL); // Insert the dataSource and Variable's Labels
  }
  ubidots.loop();
}

void setupLED() {
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, ledState);
}

void setupButton() {
  pinMode(buttonPin, INPUT);
}

void setupWiFi() {  
  if (!WiFi.config(staticIP, gateway, subnet)) {
    Serial.println("Error in configuration");
  }

  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("WiFi connected!");
  Serial.println();
}

void setupServer() {
  server.begin();
  Serial.println("Web server started!");
  Serial.print("Local IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();
}

void setupBLE() {
  BLEDevice::init("MyESP32");
  BLEServer *pServer = BLEDevice::createServer();

  BLEService *pService = pServer->createService(SERVICE_UUID);

  BLECharacteristic *pCharacteristic = pService->createCharacteristic(
                                         CHARACTERISTIC_UUID,
                                         BLECharacteristic::PROPERTY_READ |
                                         BLECharacteristic::PROPERTY_WRITE
                                       );

  pCharacteristic->setCallbacks(new MyCallbacks());

  pCharacteristic->setValue("Hello World");
  pService->start();

  BLEAdvertising *pAdvertising = pServer->getAdvertising();
  pAdvertising->start();

  Serial.println("Bluetooth setup complete"); 
}

String getRequestFromClient(WiFiClient client) {
  String request = client.readStringUntil('\n');
  Serial.println(request);
  request.trim();

  return request;
}

void updateStatus(String request) {
  if (request == "GET /on HTTP/1.1") {
    ledState = HIGH;
    lastButtonState = HIGH;
  } else if (request == "GET /off HTTP/1.1") {
    ledState = LOW;
    lastButtonState = LOW;
  } else if (request == "GET /status HTTP/1.1") {
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/plain");
    client.println("");
    client.println(ledState);
    
    client.flush();
    return;
  } else if (request == "GET /energy HTTP/1.1") {
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/plain");
    client.println("");
    client.println("energy");
    client.println(duration);
    
    client.flush();
    return;
  }

  digitalWrite(ledPin, ledState);
  
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/plain");
  client.println("");
  client.println("OK");
  
  client.flush();
}
