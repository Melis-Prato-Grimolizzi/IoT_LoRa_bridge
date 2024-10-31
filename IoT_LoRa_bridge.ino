# define MINPACKETSIZE 3

struct Slot{
  uint8_t zone_; // parking zone
  uint8_t id_; // parking id
  float lat_; // parking latitude
  float lon_; // parking longitude

  /**
  * @brief Constructor for a slot.
  *
  * @param zone The zone that the slot belongs to.
  * @param id The unique id wrt zone.
  * @param lat Latitude of parking slot.
  * @param lon Longitude of parking slot.
  * 
  *
  */
  Slot(uint8_t zone, uint8_t id, double lat, double lon) : 
    zone_(zone), id_(id), lat_(lat), lon_(lon) {}

  // empty constructor
  Slot() : zone_(0), id_(0), lat_(0), lon_(0) {}
};

void setup() {
  Serial.begin(38400);

}

void loop() {
  if(Serial.available() > MINPACKETSIZE){
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
}
