# Bluetooth-and-WiFi-controlled-power-sockets  
ESP32 LED Control  
This project is designed to control an LED using both Bluetooth and WiFi. It allows the user to turn the LED on and off using a web server or a Bluetooth Low Energy (BLE) connection. The project uses an ESP32 board, the Arduino IDE, and various libraries.  

## Prerequisites  
* Arduino IDE installed on your computer  
* ESP32 board connected to your computer  
* WiFi network  
* Adafruit and Ubidots accounts for data publishing  
* Libraries:  
  * WiFi.h  
  * BLEDevice.h  
  * BLEUtils.h  
  * BLEServer.h  
  * UbidotsEsp32Mqtt.h  
  * Adafruit_MQTT.h  
  * Adafruit_MQTT_Client.h  

## Hardware Setup  
1. Connect an LED to the ESP32 board. Connect the cathode (-) of the LED to a GND pin on the board and the anode (+) to GPIO34 (pin A4).  
2. Connect a push button to GPIO19 and GND.  

## Schematic Diagram
![diagram](https://user-images.githubusercontent.com/130977093/232525375-7c35fa3e-8371-4689-a84b-0bdf661f3f38.png)

## Software Setup  
1. Create an account on the Adafruit website (https://www.adafruit.com/).  
2. Create a new dashboard on the Adafruit IO platform and add a new feed. Take note of the following values:  
&nbsp;  * AIO_USERNAME: your Adafruit username  
&nbsp;  * AIO_KEY: your Adafruit IO key  
&nbsp;  * ESP32_LED_Control: name of the feed  
3. Create an account on the Ubidots website (https://ubidots.com/).  
4. Create a new device on the Ubidots platform and add a new variable. Take note of the following values:  
&nbsp;  * UBIDOTS_TOKEN: your Ubidots token  
&nbsp;  * DEVICE_LABEL: name of the device  
&nbsp;  * VARIABLE_LABEL: name of the variable  
5. Open the Arduino IDE and install the required libraries.  
6. Copy and paste the code from code.ino into a new sketch.  
7. Replace the following values in the code with your own:  
&nbsp;  * AIO_USERNAME  
&nbsp;  * AIO_KEY  
&nbsp;  * UBIDOTS_TOKEN  
&nbsp;  * DEVICE_LABEL  
&nbsp;  * VARIABLE_LABEL  
&nbsp;  * ssid: your WiFi network name  
&nbsp;  * password: your WiFi network password  
8. Upload the code to the ESP32 board.  
9. Open the Serial Monitor to see the status of the board.  
10. Connect to the ESP32 board using Bluetooth and turn the LED on/off.  
## Usage  
### Turning the LED On/Off using Bluetooth  
1. On your mobile device, enable Bluetooth and scan for nearby devices.  
2. Connect to the device named "MyESP32".  
3. Find the characteristic with UUID beb5483e-36e1-4688-b7f5-ea07361b26a8.  
4. Write a value of 1 to turn the LED on, or 0 to turn it off.  
### Turning the LED On/Off using WiFi  
1. Connect your computer or mobile device to the WiFi network.  
2. Open a web browser and go to the IP address of the ESP32 board (which can be found in the Serial Monitor).  
3. To turn the LED on, go to http://<IP_ADDRESS>/on.  
4. To turn the LED off, go to http://<IP_ADDRESS>/off.  
5. To check the status of the LED, go to http://<IP_ADDRESS>/status.  
6. To check the amount of energy used by the LED, go to http://<IP_ADDRESS>/energy.  
### Publishing LED Status to Adafruit IO and Ubidots  
The LED status is published to both Adafruit IO and Ubidots platforms.  
To view the data published on the platforms, follow these steps:  

#### Adafruit IO  
1. Log in to your Adafruit IO account.  
2. Go to your dashboard and find the ESP32_LED_Control feed.  
3. The data published from the ESP32 board will be displayed on a graph.  
#### Ubidots  
1. Log in to your Ubidots account.  
2. Go to your device and find the LED status variable.  
3. The data published from the ESP32 board will be displayed on a graph.  
## Troubleshooting  
* If you have trouble connecting to the ESP32 board, check the serial output for any error messages.  
* If you have trouble connecting to the WiFi network, make sure the credentials are correct.  
* If you have trouble connecting to the Bluetooth device, make sure the device is discoverable and within range.  
## Conclusion  
This project demonstrates how to control an LED using both Bluetooth and WiFi. It also shows how to publish data to Adafruit IO and Ubidots platforms. With this knowledge, you can build your own IoT projects and monitor them from anywhere in the world.


# APP Design and Use
## Create an App using an Existing .aia File
1. Go to the MIT App Inventor website (https://appinventor.mit.edu/).
2. Click on the "Create Apps!" button to start the App Inventor.
3. Sign in to your App Inventor account or create a new one.
4. Click on "Projects" and then click on "Import project (.aia) from my computer".
5. Choose the .aia file you want to use and click on "OK". (An .aia file is already provided in this repository)
6. Wait for the project to load. You should see the blocks editor and the designer.
7. Save the changes to your app.

# App
<img src="https://github.com/notprameghuikey0913/Bluetooth-and-WiFi-controlled-power-sockets/blob/main/App/App%20Image.jpeg" alt="app" height="600" /> 

## Use an App created by App Inventor
To use an app created by MIT App Inventor, follow these steps:

1. Go to the MIT App Inventor website (https://appinventor.mit.edu/).
2. Click on "Projects" and then click on "My Projects".
3. Click on the name of the app you want to use.
4. Click on "Build" and then choose the platform you want to use (Android or iOS).
5. Follow the instructions provided by App Inventor to download and install the app on your device.
6. Open the app and use it as desired.
