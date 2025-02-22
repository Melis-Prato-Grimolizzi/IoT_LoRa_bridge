#include "LoRa_E220.h"

uint64_t readingMillis = 0;
const uint64_t deltaReading = 50;

const int startingDeltaTime = 1000; 

const uint8_t RXD = 2;
const uint8_t TXD = 3;

const uint8_t updateMessageSize = 3;

LoRa_E220 lora(RXD, TXD);

struct packet{
  byte header;
  uint8_t id;
  byte footer;
};

bool timer(const uint64_t start, const uint64_t delta){
  if((millis() - start) < delta){
    return false;
  }
  return true;
}

void setup() {

  Serial.begin(9600);
  while(!Serial){}

  const uint64_t startingTime = millis();
  while(!timer(startingTime, startingDeltaTime)){}
  delay(300); // nell'ultima versione del codice funzionante questo era presente, TODO: testare se funziona senza
  //Serial.println("Starting LoRa...");

  // LoRa Pin setup
  // pinMode(RXD, INPUT);
  // pinMode(TXD, OUTPUT);
  lora.begin();

  //Serial.println("Begin effettuato");
	ResponseStructContainer c;
	c = lora.getConfiguration();
  //Serial.println("Get configuration effettuato");
	Configuration configuration = *(Configuration*) c.data;
	//printParameters(configuration);


  // Serial.println("LoRa succesfully started!");
  // Serial.println("Ciao! Sono il ricevitore");
  readingMillis = millis();

}

void loop() {
  //Serial.write(test / 256);
  //Serial.write(test % 256);
  
  if((millis() - readingMillis) > deltaReading){
    readingMillis = millis();
    if(lora.available() > 0){
      //Serial.println("Ricevuto");

      ResponseStructContainer rsc = lora.receiveMessage(sizeof(packet)); 
      
      if (rsc.status.getResponseDescription() == "Success"){
        struct packet message = *(packet*) rsc.data;
        if (message.header == 0xFF && message.footer == 0xFE){
          // char str[30];
          // sprintf(str, "Ho ricevuto l'id %d", message.id);
          // Serial.println(str);
          Serial.write(message.id); // NOTA BENE: il write serve quando è up anche il bridge per mandare il dato
          
        }
      }
      
    }
  }

  ///////////////////////////////////////////////////////////////////////////////////////////
  // Questo codice è un test che ci fa vedere come scrivere un float sulla linea seriale
  /*
  if(Serial.available() > 0){
    delay(10);
    if(byte(Serial.read()) == 0xFF){
      int len = Serial.read();
      if(Serial.available() < len){
        while(Serial.read() != 0xFE){};
      }
      int zone = Serial.read();
      int id = Serial.read();
      Serial.write(zone);
      Serial.write(id);
      
      byte lat[4];
      Serial.readBytes(lat, 4);
      for(int i = 0; i < 4; i++){
        Serial.write(lat[i]);
      }

      byte lon[4];
      Serial.readBytes(lon, 4);
      for(int i = 0; i < 4; i++){
        Serial.write(lon[i]);
      }

      delay(10);
      int code = Serial.read();
      Serial.write(code);
    }
  }
  */
  ///////////////////////////////////////////////////////////////////////////////////////////
}


///////////////////////////////////////////////////////////////////////////////////////////
// Questo è il codice per la get configuration del modulo E220

/*
#include "Arduino.h"
#include "LoRa_E220.h"

// ---------- esp8266 pins --------------
//LoRa_E220 e220ttl(RX, TX, AUX, M0, M1);  // Arduino RX <-- e220 TX, Arduino TX --> e220 RX
//LoRa_E220 e220ttl(D3, D4, D5, D7, D6); // Arduino RX <-- e220 TX, Arduino TX --> e220 RX AUX M0 M1
LoRa_E220 e220ttl(2, 3); // Config without connect AUX and M0 M1

//#include <SoftwareSerial.h>
//SoftwareSerial mySerial(D2, D3); // Arduino RX <-- e220 TX, Arduino TX --> e220 RX
//LoRa_E220 e220ttl(&mySerial, D5, D7, D6); // AUX M0 M1
// -------------------------------------

// ---------- Arduino pins --------------
//LoRa_E220 e220ttl(4, 5, 3, 7, 6); // Arduino RX <-- e220 TX, Arduino TX --> e220 RX AUX M0 M1
//LoRa_E220 e220ttl(4, 5); // Config without connect AUX and M0 M1

//#include <SoftwareSerial.h>
//SoftwareSerial mySerial(4, 5); // Arduino RX <-- e220 TX, Arduino TX --> e220 RX
//LoRa_E220 e220ttl(&mySerial, 3, 7, 6); // AUX M0 M1
// -------------------------------------

// ------------- Arduino Nano 33 IoT -------------
// LoRa_E220 e220ttl(&Serial1, 2, 4, 6); //  RX AUX M0 M1
// -------------------------------------------------

// ------------- Arduino MKR WiFi 1010 -------------
// LoRa_E220 e220ttl(&Serial1, 0, 2, 4); //  RX AUX M0 M1
// -------------------------------------------------

// ---------- esp32 pins --------------
// LoRa_E220 e220ttl(&Serial2, 15, 21, 19); //  RX AUX M0 M1

//LoRa_E220 e220ttl(&Serial2, 22, 4, 18, 21, 19, UART_BPS_RATE_9600); //  esp32 RX <-- e220 TX, esp32 TX --> e220 RX AUX M0 M1
// -------------------------------------

// ---------- Raspberry PI Pico pins --------------
// LoRa_E220 e220ttl(&Serial2, 2, 10, 11); //  RX AUX M0 M1
// -------------------------------------

// ---------------- STM32 --------------------
//HardwareSerial Serial2(USART2);   // PA3  (RX)  PA2  (TX)
//LoRa_E220 e220ttl(&Serial2, PA0, PB0, PB10); //  RX AUX M0 M1
// -------------------------------------------------

void printParameters(struct Configuration configuration);
void printModuleInformation(struct ModuleInformation moduleInformation);

void setup() {
	Serial.begin(9600);
	while(!Serial){};
	delay(500);

	Serial.println();


	// Startup all pins and UART
	e220ttl.begin();
  Serial.println("Begin effettuato");
	ResponseStructContainer c;
	c = e220ttl.getConfiguration();
  Serial.println("Get configuration effettuato");
	// It's important get configuration pointer before all other operation
	Configuration configuration = *(Configuration*) c.data;
	Serial.println(c.status.getResponseDescription());
	Serial.println(c.status.code);

	printParameters(configuration);

	ResponseStructContainer cMi;
	cMi = e220ttl.getModuleInformation();
	// It's important get information pointer before all other operation
	ModuleInformation mi = *(ModuleInformation*)cMi.data;

	Serial.println(cMi.status.getResponseDescription());
	Serial.println(cMi.status.code);

	printModuleInformation(mi);

	c.close();
	cMi.close();
}

void loop() {

}
*/
/*
void printParameters(struct Configuration configuration) {
	Serial.println("----------------------------------------");

	Serial.print(F("HEAD : "));  Serial.print(configuration.COMMAND, HEX);Serial.print(" ");Serial.print(configuration.STARTING_ADDRESS, HEX);Serial.print(" ");Serial.println(configuration.LENGHT, HEX);
	Serial.println(F(" "));
	Serial.print(F("AddH : "));  Serial.println(configuration.ADDH, HEX);
	Serial.print(F("AddL : "));  Serial.println(configuration.ADDL, HEX);
	Serial.println(F(" "));
	Serial.print(F("Chan : "));  Serial.print(configuration.CHAN, DEC); Serial.print(" -> "); Serial.println(configuration.getChannelDescription());
	Serial.println(F(" "));
	Serial.print(F("SpeedParityBit     : "));  Serial.print(configuration.SPED.uartParity, BIN);Serial.print(" -> "); Serial.println(configuration.SPED.getUARTParityDescription());
	Serial.print(F("SpeedUARTDatte     : "));  Serial.print(configuration.SPED.uartBaudRate, BIN);Serial.print(" -> "); Serial.println(configuration.SPED.getUARTBaudRateDescription());
	Serial.print(F("SpeedAirDataRate   : "));  Serial.print(configuration.SPED.airDataRate, BIN);Serial.print(" -> "); Serial.println(configuration.SPED.getAirDataRateDescription());
	Serial.println(F(" "));
	Serial.print(F("OptionSubPacketSett: "));  Serial.print(configuration.OPTION.subPacketSetting, BIN);Serial.print(" -> "); Serial.println(configuration.OPTION.getSubPacketSetting());
	Serial.print(F("OptionTranPower    : "));  Serial.print(configuration.OPTION.transmissionPower, BIN);Serial.print(" -> "); Serial.println(configuration.OPTION.getTransmissionPowerDescription());
	Serial.print(F("OptionRSSIAmbientNo: "));  Serial.print(configuration.OPTION.RSSIAmbientNoise, BIN);Serial.print(" -> "); Serial.println(configuration.OPTION.getRSSIAmbientNoiseEnable());
	Serial.println(F(" "));
	Serial.print(F("TransModeWORPeriod : "));  Serial.print(configuration.TRANSMISSION_MODE.WORPeriod, BIN);Serial.print(" -> "); Serial.println(configuration.TRANSMISSION_MODE.getWORPeriodByParamsDescription());
	Serial.print(F("TransModeEnableLBT : "));  Serial.print(configuration.TRANSMISSION_MODE.enableLBT, BIN);Serial.print(" -> "); Serial.println(configuration.TRANSMISSION_MODE.getLBTEnableByteDescription());
	Serial.print(F("TransModeEnableRSSI: "));  Serial.print(configuration.TRANSMISSION_MODE.enableRSSI, BIN);Serial.print(" -> "); Serial.println(configuration.TRANSMISSION_MODE.getRSSIEnableByteDescription());
	Serial.print(F("TransModeFixedTrans: "));  Serial.print(configuration.TRANSMISSION_MODE.fixedTransmission, BIN);Serial.print(" -> "); Serial.println(configuration.TRANSMISSION_MODE.getFixedTransmissionDescription());


	Serial.println("----------------------------------------");
}
void printModuleInformation(struct ModuleInformation moduleInformation) {
	Serial.println("----------------------------------------");
	Serial.print(F("HEAD: "));  Serial.print(moduleInformation.COMMAND, HEX);Serial.print(" ");Serial.print(moduleInformation.STARTING_ADDRESS, HEX);Serial.print(" ");Serial.println(moduleInformation.LENGHT, DEC);

	Serial.print(F("Model no.: "));  Serial.println(moduleInformation.model, HEX);
	Serial.print(F("Version  : "));  Serial.println(moduleInformation.version, HEX);
	Serial.print(F("Features : "));  Serial.println(moduleInformation.features, HEX);
	Serial.println("----------------------------------------");
}
*/

///////////////////////////////////////////////////////////////////////////////////////////