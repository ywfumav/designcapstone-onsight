#include <ESP32Servo.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
//#include <BLE2902.h>

//BLECharacteristic *pCharacteristic;
bool deviceConnected = false;
//int txValue = 0;

Servo myservo1;
Servo myservo2;
Servo myservo3;
//Servo myservo4;
int posFalse = 0;
int posTrue = 90;
int servoPin1 = 12;
int servoPin2 = 25;
int servoPin3 = 27;
//int servoPin4 = 32;

#define SERVICE_UUID           "6E400001-B5A3-F393-E0A9-E50E24DCCA9E" //UART SERVICE UUID
#define CHARACTERISTIC_UUID_TX "6E400003-B5A3-F393-E0A9-E50E24DCCA9E"
#define CHARACTERISTIC_UUID_RX "6E400002-B5A3-F393-E0A9-E50E24DCCA9E"

class MyServerCallbacks: public BLEServerCallbacks {
  void onConnect(BLEServer *pServer) {
    deviceConnected = true;
    //Serial.println("Connected");
  }

  void onDisconnect(BLEServer *pServer) {
    deviceConnected = false;
    //Serial.println("Disconnected");
    //delay(500);
    //pServer->getAdvertising()->start();
    //Serial.println("Reconnecting");
  }
};

class MyCallbacks: public BLECharacteristicCallbacks {
  void onWrite(BLECharacteristic *pCharacteristic) {
    std:: string rxValue = pCharacteristic->getValue();

    if (rxValue.length() > 0) {
      if (rxValue.find(0x1)!= -1) {
        //Serial.println("North");
        myservo1.write(posTrue);
        delay(300);
        myservo2.write(posFalse);
        delay(300);
        myservo3.write(posFalse);
        delay(300);
      }

      else if (rxValue.find(0x2)!= -1) {
        //Serial.println("East");
        myservo2.write(posTrue);
        delay(300);
        myservo1.write(posFalse);
        delay(300);
        myservo3.write(posFalse);
        delay(300);
      }

      else if (rxValue.find(0x3)!= -1) {
        //Serial.println("West");
        myservo3.write(posTrue);
        delay(300);
        myservo2.write(posFalse);
        delay(300);
        myservo1.write(posFalse);
        delay(300);
      }

      else if (rxValue.find(0x5)!= -1) {
        myservo1.write(posTrue);
        delay(500);
        myservo1.write(posFalse);
      }

      else if (rxValue.find(0x6)!= -1) {
        myservo2.write(posTrue);
        delay(500);
        myservo2.write(posFalse);
      }

      else if (rxValue.find(0x7)!= -1) {
        myservo3.write(posTrue);
        delay(500);
        myservo3.write(posFalse);
      }

      else if (rxValue.find(0x8)!= -1) {
        //Serial.println("West");
        myservo1.write(posFalse);
        delay(300);
        myservo2.write(posFalse);
        delay(300);
        myservo3.write(posFalse);
        delay(300);
      }

      else if (rxValue.find(0x9)!= -1) {
        myservo1.write(posTrue);
        delay(300);
        myservo2.write(posTrue);
        delay(300);
        myservo3.write(posTrue);
        delay(300);
      }

      else {
        delay(200);
        //Serial.println("Nothing");
      }
    }
    
  }
};

void setup() {
  //SERVO
  ESP32PWM::allocateTimer(0);
  ESP32PWM::allocateTimer(1);
  ESP32PWM::allocateTimer(2);
  ESP32PWM::allocateTimer(3);
  myservo1.setPeriodHertz(50);    // standard 50 hz servo
  myservo2.setPeriodHertz(50);    // standard 50 hz servo
  myservo3.setPeriodHertz(50);    // standard 50 hz servo
  //myservo4.setPeriodHertz(50);    // standard 50 hz servo
  myservo1.attach(servoPin1, 500, 2400); // attaches the servo on pin 17 to the servo object
  myservo2.attach(servoPin2, 500, 2400); // attaches the servo on pin 18 to the servo object
  myservo3.attach(servoPin3, 500, 2400); // attaches the servo on pin 25 to the servo object
  //myservo4.attach(servoPin4, 500, 2400); // attaches the servo on pin 27 to the servo object
  
  //UART
  //Serial.begin(115200);
  
  //Initialise BLE Device to be discovered
  BLEDevice::init("MyESP32");
  
  //Create the BLE Server
  BLEServer *pServer = BLEDevice::createServer();
  BLEService *pService = pServer->createService(SERVICE_UUID);
  pServer->setCallbacks(new MyServerCallbacks());

  //Create BLE Characteristic
  //BLECharacteristic *pCharacteristic;
  /*pCharacteristic = pService->createCharacteristic(
                     CHARACTERISTIC_UUID_TX,
                     BLECharacteristic::PROPERTY_NOTIFY
                   );*/

  //BLE2902 Notify
  //pCharacteristic->addDescriptor(new BLE2902());

  //BLE Characteristic for Receiving End
  BLECharacteristic *pCharacteristic = pService->createCharacteristic(
                                         CHARACTERISTIC_UUID_RX,
                                         BLECharacteristic::PROPERTY_WRITE
                                       );
  pCharacteristic->setCallbacks(new MyCallbacks());

  //Start service
  pService->start();

  //Start advertising
  pServer->getAdvertising()->start();
  //Serial.println("Waiting to be connected to a client...");
}

void loop() {
  if (deviceConnected) {
    /*txValue = random(0,20);
    char txString[8];
    dtostrf(txValue,1,2,txString);
    pCharacteristic->setValue(txString);
    pCharacteristic->notify();
    Serial.println("Sent: " + String(txString)); */
    delay(500);
  }
}
