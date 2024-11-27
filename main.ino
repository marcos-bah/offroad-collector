// SD Card
const int _MISO = 4;  // AKA SPI RX
const int _MOSI = 7;  // AKA SPI TX
const int _CS = 5;
const int _SCK = 6;

#include <SPI.h>
#include <SD.h>

File dataFile;

void createColunmData() {
  dataFile = SD.open("data.csv", FILE_WRITE);
  if (dataFile) {
    Serial.println("Writing column to data.csv...");
    dataFile.println("ax,ay,az,gx,gy,gz,label");
    dataFile.close();
  }else{
    Serial.println("error opening data.csv");
  }
}

void insertRowData(String rowData) {
  dataFile = SD.open("data.csv", FILE_WRITE);
  if (dataFile) {
    Serial.println("Inserting row into data.csv...");
    dataFile.println(rowData); 
    dataFile.close();
    Serial.println("Row inserted successfully.");
  } else {
    Serial.println("Error opening data.csv for writing.");
  }
}

void readData() {
  dataFile = SD.open("data.csv");
  Serial.println("Reading Data: ");
  while (dataFile.available()) {
      Serial.write(dataFile.read());
  }
  dataFile.close();
}

int initSDCard() {
  Serial.println("\nInitializing SD card...");

  bool sdInitialized = false;

  if (_MISO == 0 || _MISO == 4 || _MISO == 16) {
    SPI.setRX(_MISO);
    SPI.setTX(_MOSI);
    SPI.setSCK(_SCK);
    sdInitialized = SD.begin(_CS);
  } else if (_MISO == 8 || _MISO == 12) {
    SPI1.setRX(_MISO);
    SPI1.setTX(_MOSI);
    SPI1.setSCK(_SCK);
    sdInitialized = SD.begin(_CS, SPI1);
  } else {
    Serial.println(F("ERROR: Unknown SPI Configuration"));
    return 0;
  }

  if (!sdInitialized) {
    Serial.println("initialization failed. Things to check:");
    Serial.println("* is a card inserted?");
    Serial.println("* is your wiring correct?");
    Serial.println("* did you change the chipSelect pin to match your shield or module?");
    return 0;
  } else {
    Serial.println("Wiring is correct and a card is present.");
    return 1;
  }
}

//end SD Card

// MPU 
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>

Adafruit_MPU6050 mpu;
TwoWire CustomI2C0(i2c0, 0, 1);

const int numSamples = 50;             
int samplesRead = 0;  
const int inputLength = 300;

int initMPU() {
  if (!mpu.begin(0x68,&CustomI2C0,0)) {
    Serial.println("Failed to find MPU6050 chip");
    return 0;
  }

  mpu.setAccelerometerRange(MPU6050_RANGE_16_G);
  mpu.setGyroRange(MPU6050_RANGE_250_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
  delay(100);

  return 1;
}

float aX, aY, aZ, gX, gY, gZ;
String label;

void readMPU() {
  digitalWrite(LED_BUILTIN, HIGH);  
  samplesRead = 0;

  while (samplesRead < numSamples) {
    sensors_event_t a, g, temp;
    mpu.getEvent(&a, &g, &temp);

    aX = a.acceleration.x;
    aY = a.acceleration.y;
    aZ = a.acceleration.z;
    gX = g.gyro.x;
    gY = g.gyro.y;
    gZ = g.gyro.z;

    samplesRead++;

    String row = String(aX) + "," + String(aY) + "," + String(aZ) + "," +
                 String(gX) + "," + String(gY) + "," + String(gZ) + "," + label;

    insertRowData(row);

    Serial.println(row);

    delay(80);
  }

  digitalWrite(LED_BUILTIN, LOW);  
}

//end MPU

// Buttons for states
const int buttonIdle = 28; 
const int buttonMovement = 27;
const int buttonFall = 26;

const int ledIdle = 20;
const int ledMovement = 19;
const int ledFall = 18;

int buttonStateIdle = 0;
int buttonStateMovement = 0;
int buttonStateFall = 0;

void initButtonLed() {
  pinMode(ledIdle, OUTPUT);
  pinMode(ledMovement, OUTPUT);
  pinMode(ledFall, OUTPUT);

  pinMode(LED_BUILTIN, OUTPUT);

  pinMode(buttonIdle, INPUT_PULLDOWN);
  pinMode(buttonMovement, INPUT_PULLDOWN);
  pinMode(buttonFall, INPUT_PULLDOWN);
}

void collect() {
  buttonStateMovement = digitalRead(buttonMovement);
  buttonStateFall = digitalRead(buttonFall);
  buttonStateIdle = digitalRead(buttonIdle);

  // Serial.print("Idle: ");
  // Serial.print(buttonStateIdle);
  // Serial.print(", Movement: ");
  // Serial.print(buttonStateMovement);
  // Serial.print(", Fall: ");
  // Serial.println(buttonStateFall);

  if (buttonStateIdle == HIGH) {
    digitalWrite(ledFall, LOW); 
    digitalWrite(ledMovement, LOW); 
    digitalWrite(ledIdle, HIGH); 
    label = "idle";
    buttonStateFall = 0;
    buttonStateMovement = 0;
    readMPU();

  }else if(buttonStateMovement == HIGH) {
    digitalWrite(ledFall, LOW); 
    digitalWrite(ledIdle, LOW); 
    digitalWrite(ledMovement, HIGH); 
    label = "movement";
    buttonStateFall = 0;
    buttonStateIdle = 0;
    readMPU();

  }else if(buttonStateFall == HIGH) {
    digitalWrite(ledMovement, LOW); 
    digitalWrite(ledIdle, LOW); 
    digitalWrite(ledFall, HIGH); 
    label = "fall";
    buttonStateMovement = 0;
    buttonStateIdle = 0;
    readMPU();

  }

  digitalWrite(ledFall, LOW); 
  digitalWrite(ledIdle, LOW); 
  digitalWrite(ledMovement, LOW); 
}

int write = 0;

void setup() {
  Serial.begin(115200);

  if(initSDCard() == 1) {
    Serial.println("Works SD Card!");
  }else{
    return;
  }

  if(initMPU() == 1) {
    Serial.println("Works MPU!");
  }else{
    return;
  }

  initButtonLed();

  write = 1;

  if(write == 1){
    createColunmData();
  }else{
    readData();
  }
}

void loop() {
  if(write == 1){
    collect();
  }
}
